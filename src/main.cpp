////    Includes    ////
#include "helpers.h"

////    Initilization setup    ////
void setup(void)
{
    SPI.begin();
    Wire.begin();
    Serial.begin(SERIAL_MONITOR_BAUD);

    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);
    
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);

    setParts();
    Serial.printf("Setup Done\n");
}

////    Main loop    ////
void loop(void)
{
    static uint32_t timestamp = 0;
    
    start = millis(); 

    setParts();

    // harvest data; check data quality with partstates

    if (partsStates.baro)
    {
        if (baro.getTempPress(&temp, &pres))
        {
            Serial.printf("baro read failed\n");
            status |= DATA_BARO;
        }
        else{
            altitude =  44330.0f *(1.0f - powf(pres / pad_pres,0.190295f));
            altitude*=  3.28084f; //ft
            status &= ~DATA_BARO;
        }
    }

    if (partsStates.gps)
    {
        if (gps.read_RMC(&lon, &lat, 4000))
        {
            Serial.printf("gps RMC read failed\n");
            status |= DATA_GPS;
        }
        else{
            status &= ~DATA_GPS;
        }
    }

    if (partsStates.ina)
    {
        if (volt_battery = ina260.readBusVoltage(); volt_battery >100)
        {
            Serial.printf("ina read failed\n");
            status |= DATA_INA;
        }
        else{
            status &= ~DATA_INA;
        }
    }

    if (partsStates.mpu)
    {
        if (mpu.get_acc(acc_range, &imu_acc) or mpu.get_gyro(gyro_range, &imu_gyro))
        {
            Serial.printf("mpu read failed\n");
            status |= DATA_IMU;
        }
        else{
            status &= ~DATA_IMU;
        }
    }

    sprintf(
        sensor_data, outputFormat,
        timestamp++, volt_battery, imu_acc.XAxis, imu_acc.YAxis, imu_acc.ZAxis, imu_gyro.XAxis, imu_gyro.YAxis, imu_gyro.ZAxis, temp, pres, altitude, lat, lon);

    Serial.printf("%s", sensor_data);

    if (partsStates.sdcard)
    {
        File dataFile = SD.open(logFileName.c_str(), FILE_WRITE);

        if (dataFile)
        {
            dataFile.println(sensor_data);
            status &= ~DATA_SD;
            dataFile.close();
        }
        else
        {
            Serial.printf("error opening %s\n", logFileName.c_str());
            partsStates.sdcard = false;
            status |= DATA_SD;
        }
    }

    snprintf(packet, sizeof(packet),
    "%s>%s:"
    "PKT=%d,"
    "BAT=%.1fV,"
    "ACC X=%.2fg,"
    "ACC Y=%.2fg,"
    "ACC Z=%.2fg,"
    "GYRO X=%.2frad/s,"
    "GYRO Y=%.2frad/s,"
    "GYRO Z=%.2frad/s,"
    "TEMP=%.2fC,"
    "PRES=%.2fmbar,"
    "ALT=%.2fft",
    "LAT=%lf,"
    "LON=%lf",
    "STAT=%08X",
    CALLSIGN,
    GROUND,
    packetnum++,
    volt_battery,
    imu_acc.XAxis,
    imu_acc.YAxis,
    imu_acc.ZAxis,
    imu_gyro.XAxis,
    imu_gyro.YAxis,
    imu_gyro.ZAxis,
    temp,
    pres,
    altitude,
    lat,
    lon,
    (unsigned long)status);
 
    if (partsStates.lora)
    {
        rf96.send((uint8_t*)packet, strlen(packet));
        if (rf96.waitPacketSent())
        {
            status &= ~DATA_LORA;
            Serial.println("Packet was sent");
        }
        else
        {
            status |= DATA_LORA;
            Serial.println("Packet Error");
        }
    }
    delay(2000);
}

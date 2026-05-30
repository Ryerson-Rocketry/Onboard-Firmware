////    Includes    ////
#include "helpers.h"
#include <Arduino.h>
#include <math.h>
#include <Wire.h>
#include <SPI.h> 
#include <RH_RF95.h> 

////    Initilization setup    ////
void setup(void)
{
    SPI.begin();
    Wire.begin();
    Serial.begin(SERIAL_MONITOR_BAUD);
    
    setParts();

    Serial.printf("Setup Done\n");
}

////    Main loop    ////
void loop(void)
{
    static uint32_t timestamp = 0;
    char string[256] = {0};
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
        string, outputFormat,
        timestamp++, volt_battery, imu_acc.XAxis, imu_acc.YAxis, imu_acc.ZAxis, imu_gyro.XAxis, imu_gyro.YAxis, imu_gyro.ZAxis, temp, pres, lat, lon);

    Serial.printf("%s", string);

    if (partsStates.sdcard)
    {
        File dataFile = SD.open(logFileName.c_str(), FILE_WRITE);

        if (dataFile)
        {
            dataFile.println(string);
            status |= DATA_SD;
            dataFile.close();
        }
        else
        {
            Serial.printf("error opening %s\n", logFileName.c_str());
            partsStates.sdcard = false;
            status &= ~DATA_SD;
        }
    }
}
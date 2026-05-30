#ifndef __RRC_HELPER_FUNCS__
#define __RRC_HELPER_FUNCS__


////    Includes    ////
#include "rrc_encoder/src/rrc_encoder.h"
#include <Arduino.h>
#include <SPI.h> 
#include <gps.h>
#include <ms5611.h>
#include <MPU6050.h> 
#include <Adafruit_INA260.h>
#include <SD.h>
#include <RH_RF95.h> 


////    Defines    ////

#define GPS_SERIAL              Serial7
#define BARO_WIRE               Wire     
#define AD0_VAL                 0
#define SERIAL_MONITOR_BAUD     115200
#define PIN_RED                 4
#define PIN_GREEN               3
#define PIN_BLUE                2       
#define SD_CS_PIN               10
#define RFM96_CS                9
#define RFM96_GPIO              5
#define RF96_FREQ               433.0
#define BUZZER                  PIN_A6
#define BUZZER_ENABLE           PIN_A7


////    Constants    ////

long freq               = 1000;
bool led_debug          = false;
bool built_in_SD_card   = true;
int  acc_range          = 1; 
int  gyro_range         = 1; 
String logFileName      = "log.txt";
double                     temp;
double                     pres;
double                     lon;
double                     lat;
double                     volt_battery;
float                      x;
float                      y;
float                      z;
float                      r;
AStruct                    imu_acc;
GStruct                    imu_gyro;
uint32_t                   start;
uint8_t                    counter = 0;


/*

const char outputFormat[] =
    R"""(
timestamp:   %lu
voltage battery: %lf V
x = %lf g    y = %lf g   z = %lf g   total = %lf g
T = %lf C    P = %lf mbar
Location:    %lf, %lf

)""";
*/

//Update May 20: Added spaces cause it was AIDS to read
const char outputFormat[] =
    R"""(%lu, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf
)""";


////    Objects    ////

Ms5611              baro;
GPS                 gps;
RH_RF95             rf96(RFM96_CS, RFM96_GPIO);
MPU                 mpu;
Adafruit_INA260     ina260;
SDClass             sd;

struct
{
    bool    baro   = false;
    bool    gps    = false;
    bool    sdcard = false;
    bool    mpu    = false;
    bool    ina    = false;
    bool    lora   = false; 
} partsStates;

enum initStatus
{
    FAULT_BARO = (1 << 0),
    FAULT_GPS  = (1 << 1),
    FAULT_SD   = (1 << 2),
    FAULT_INA  = (1 << 3),
    FAULT_IMU  = (1 << 4),

    DATA_BARO =  (1 << 5),
    DATA_GPS  =  (1 << 6),
    DATA_SD   =  (1 << 7),
    DATA_INA  =  (1 << 8),
    DATA_IMU  =  (1 << 9),
};

uint32_t status = 0;


////    Functions declarations    ////
void buzzFor    (unsigned int time_ms, unsigned int after = 0);
void setParts   (void);


////    Functions definitions    ////

void buzzFor(unsigned int time_ms, unsigned int after)
{
    if(digitalRead(BUZZER_ENABLE))
    {
        digitalWrite(BUZZER, HIGH);
        delay(time_ms);
        digitalWrite(BUZZER, LOW);
        delay(after);
        led_debug = true; 
    }
}


void setParts(void)
{

    // init MS5611
    if(!partsStates.baro)
    {        
        if(baro.init(&BARO_WIRE)){
            partsStates.baro = false;
            Serial.println("MS5611 init error");
            status |= FAULT_BARO;
        }
        else
        {
            partsStates.baro = true;
            status &= ~FAULT_BARO;
            Serial.println("MS5611 init OK");
        }
    }

    // init GPS
    if(!partsStates.gps)
    {
        if(gps.init(&GPS_SERIAL, 9600)){
            partsStates.gps = false;
            Serial.println("GPS init error");
            status |= FAULT_GPS;
        }
        else{
            partsStates.gps = true;
            status &= ~FAULT_GPS;
            Serial.println("GPS init OK");
        }    
    }

    // init SD card
    if(!partsStates.sdcard)
    {
        if (!sd.begin(SD_CS_PIN))
        {
            partsStates.sdcard = false;
            Serial.println("SD Card init error");
            status |= FAULT_SD;
        }
        else
        {
            partsStates.sdcard = true;
            status &= ~FAULT_SD;
            Serial.println("SD Card init OK");
        }
    }

    // init INA
    if(!partsStates.ina)
    {        
        if(!ina260.begin()){
            partsStates.ina = false;
            Serial.println("INA260 init error");
            status |= FAULT_INA;
        }
        else
        {
            partsStates.ina = true;
            status &= ~FAULT_INA;
            Serial.println("INA260 init OK");
        }
    }

    // init MPU
    if(!partsStates.mpu)
    {        
        if(!mpu.init(gyro_range,acc_range)){
            partsStates.mpu = false;
            Serial.println("MPU6050 init error");
            status |= FAULT_IMU;
        }
        else
        {
            partsStates.mpu = true;
            status &= ~FAULT_IMU;
            Serial.println("MPU6050 init OK");
        }
    }
}

#endif
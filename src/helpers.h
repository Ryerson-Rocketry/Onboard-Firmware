#ifndef __RRC_HELPER_FUNCS__
#define __RRC_HELPER_FUNCS__

////    Includes    ////
#include <Arduino.h>
#include <gps.h>
#include <ms5611.h>
#include <MPU6050.h>
#include <SD.h>
#include "rrc_encoder/src/rrc_encoder.h"
// #include "ICM_20948.h"

////    Defines    ////
#define RFD_BAUD 115200
#define RFD_SERIAL Serial2
#define GPS_SERIAL Serial7
#define BARO_WIRE Wire2
#define BUZZER PIN_A12
#define BUZZER_ENABLE PIN_A13 // always apply
#define IMU_WIRE Wire1
#define AD0_VAL 0
#define SERIAL_MONITOR_BAUD 115200
#define PIN_RED 4
#define PIN_GREEN 3
#define PIN_BLUE 2

long freq = 4000;
bool led_debug = false;

////    Constants    ////
String logFileName = "log.txt";
const char outputFormat[] =
    R"""(
timestamp:   %lu
V = %lf
x = %lf g    y = %lf g   z = %lf g   
T = %lf C    P = %lf mbar
Location:    %lf, %lf

)""";

////    Objects    ////
Ms5611 baro;
GPS gps;
struct
{
    bool baro = false;
    bool gps = false;
    bool sdcard = false;
    bool rfd = false;
} partsStates;
// ICM_20948_I2C myICM;

////    Functions declairations    ////
void transmit(double data, uint8_t header, uint32_t time);
void debug(void);
void buzzFor(unsigned int time_ms, unsigned int after = 0);
void setParts(void);
// void getScaledAGMT(ICM_20948_I2C *sensor);

////    Functions definitions    ////
void transmit(double data, uint8_t header, uint32_t time)
{
    uint8_t package[10] = {0};
    encode(data, header, time, package);
    RFD_SERIAL.write(package, 10);
}

void debug(void)
{
    static int count = 0;
    Serial.printf("debug    %d\n", count++);
}

void buzzFor(unsigned int time_ms, unsigned int after)
{
    if (digitalRead(BUZZER_ENABLE))
    {
        digitalWrite(BUZZER, HIGH);
        delay(time_ms);
        digitalWrite(BUZZER, LOW);
        delay(after);
        led_debug = true;
    }
}

void errorLED(int errorcode)
{
    unsigned long time = 0;
    unsigned long delta = 0;
    time = millis();

    switch (errorcode)
    {
    case 0:
        analogWrite(PIN_RED, 0);
        analogWrite(PIN_GREEN, 0);
        analogWrite(PIN_BLUE, 0);
        break;
    case 1:
        analogWrite(PIN_RED, 255);
        analogWrite(PIN_GREEN, 0);
        analogWrite(PIN_BLUE, 0);
        break;

    case 2:

        analogWrite(PIN_RED, 0);
        analogWrite(PIN_GREEN, 255);
        analogWrite(PIN_BLUE, 0);
        break;

    case 3:

        analogWrite(PIN_RED, 0);
        analogWrite(PIN_GREEN, 0);
        analogWrite(PIN_BLUE, 255);
        break;

    case 4:

        analogWrite(PIN_RED, 0);
        analogWrite(PIN_GREEN, 200);
        analogWrite(PIN_BLUE, 255);

        break;

    case 5:

        analogWrite(PIN_RED, 200);
        analogWrite(PIN_GREEN, 12);
        analogWrite(PIN_BLUE, 0);

        break;
    case 6:

        analogWrite(PIN_RED, 50);
        analogWrite(PIN_GREEN, 50);
        analogWrite(PIN_BLUE, 50);
        break;
    }
}

void setParts(void)
{
    // init MS5611
    if (!partsStates.baro)
    {
        if (baro.init(&BARO_WIRE))
        {

            partsStates.baro = false;
            Serial.println("MS5611 init error");

            buzzFor(150, 100);
            errorLED(2);
            errorLED(0);
            delay(100);
            errorLED(2);
            errorLED(0);
            delay(100);
            errorLED(2);
        }
        else
        {
            partsStates.baro = true;
            Serial.println("MS5611 init OK");
        }
    }

    // init GPS
    if (!partsStates.gps)
    {
        if (gps.init(&GPS_SERIAL, 9600))
        {
            partsStates.gps = true;
            Serial.println("GPS init OK");
        }
        else
        {
            buzzFor(100, 100);

            errorLED(3);
            errorLED(0);
            delay(100);
            errorLED(3);
            errorLED(0);
            delay(100);
            errorLED(3);
            Serial.println("gps init error");
        }
    }

    // init RFD
    if (!partsStates.rfd)
    {
        RFD_SERIAL.begin(RFD_BAUD);
        if (RFD_SERIAL)

        {
            partsStates.rfd = true;
            Serial.println("RFD init OK");
        }
        else
        {
            buzzFor(100, 100);

            errorLED(4);
            errorLED(0);
            delay(100);
            errorLED(4);
            errorLED(0);
            delay(100);
            errorLED(4);
        }
    }

    // init SD card
    if (!partsStates.sdcard)
    {
        if (!SD.begin(BUILTIN_SDCARD))
        {
            partsStates.sdcard = false;
            Serial.println("SD Card init error");

            buzzFor(100, 100);

            errorLED(5);
            errorLED(0);
            delay(100);
            errorLED(5);
            errorLED(0);
            delay(100);
            errorLED(5);
        }
        else
        {
            partsStates.sdcard = true;
            Serial.println("SD Card init OK");
        }
    }
}

#endif //  #ifndef __RRC_HELPER_FUNCS__

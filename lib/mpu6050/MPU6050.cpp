#include <Wire.h>
#include <stdint.h>
#include "MPU6050.h"

uint8_t mpuaddr=0x68;
void MPU::pwr_setup(){//power management registers setup
<<<<<<< Updated upstream
    Wire2.beginTransmission(ADDR);
    Wire2.write(PWR_MGMT_1);
    Wire2.write(0x01);
    Wire2.endTransmission(true);
}

void MPU::gyro_setup(int range){//gyroscope registers setup
    Wire2.beginTransmission(ADDR);
    Wire2.write(GYRO_CONFIG);
   if(range==0){
         Wire2.write(0x00);
    }
    else if(range==1){
         Wire2.write(0x08);
    }
    else if(range==2){
         Wire2.write(0x10);
    }
    else if(range==3){
         Wire2.write(0x18);
    }
    Wire2.endTransmission(true);
}

void MPU::acc_setup(int range){//accelerometer registers setup
    Wire2.beginTransmission(ADDR);
    Wire2.write(ACC_CONFIG);
    if(range==0){
         Wire2.write(0x00);
    }
    else if(range==1){
         Wire2.write(0x8);
    }
    else if(range==2){
         Wire2.write(0x10);
    }
    else if(range==3){
         Wire2.write(0x18);
    }

    Wire2.endTransmission(true);
}

void MPU::get_acc(int Anum, struct AStruct *acc){
    Wire2.beginTransmission(ADDR);
    Wire2.write(ACCEL_XOUT_H);
    Wire2.endTransmission(false);
    Wire2.requestFrom(mpuaddr,(size_t)6,true);  
    
    int16_t xdata=Wire2.read()<<8|Wire2.read();
    int16_t ydata=Wire2.read()<<8|Wire2.read();
    int16_t zdata=Wire2.read()<<8|Wire2.read();
=======
    Wire1.beginTransmission(ADDR);
    Wire1.write(PWR_MGMT_1);
    Wire1.write(0x01);
    Wire1.endTransmission(true);
}

void MPU::gyro_setup(int range){//gyroscope registers setup
    Wire1.beginTransmission(ADDR);
    Wire1.write(GYRO_CONFIG);
   if(range==0){
         Wire1.write(0x00);
    }
    else if(range==1){
         Wire1.write(0x08);
    }
    else if(range==2){
         Wire1.write(0x10);
    }
    else if(range==3){
         Wire1.write(0x18);
    }
    Wire1.endTransmission(true);
}

void MPU::acc_setup(int range){//accelerometer registers setup
    Wire1.beginTransmission(ADDR);
    Wire1.write(ACC_CONFIG);
    if(range==0){
         Wire1.write(0x00);
    }
    else if(range==1){
         Wire1.write(0x8);
    }
    else if(range==2){
         Wire1.write(0x10);
    }
    else if(range==3){
         Wire1.write(0x18);
    }

    Wire1.endTransmission(true);
}

void MPU::get_acc(int Anum, struct AStruct *acc){
    Wire1.beginTransmission(ADDR);
    Wire1.write(ACCEL_XOUT_H);
    Wire1.endTransmission(false);
    Wire1.requestFrom(mpuaddr,(size_t)6,true);  
    
    int16_t xdata=Wire1.read()<<8|Wire1.read();
    int16_t ydata=Wire1.read()<<8|Wire1.read();
    int16_t zdata=Wire1.read()<<8|Wire1.read();
>>>>>>> Stashed changes

    acc->XAxis=(float)xdata/AccelRange[Anum];
    acc->YAxis=(float)ydata/AccelRange[Anum];
    acc->ZAxis=(float)zdata/AccelRange[Anum];

<<<<<<< Updated upstream
    Wire2.endTransmission(true);
}

void MPU::get_temp(struct TStruct *temp){
    Wire2.beginTransmission(ADDR);
    Wire2.write(TEMP_OUT_H);
    Wire2.endTransmission(false);
    Wire2.requestFrom(mpuaddr,(size_t)2,true);  
    
    int16_t tdata=Wire2.read()<<8|Wire2.read();

    temp->TempC=float(tdata)/340 +36.53;

    Wire2.endTransmission(true);
}

void MPU::get_gyro(int Gnum,struct GStruct *gyro){
    Wire2.beginTransmission(ADDR);
    Wire2.write(GYRO_XOUT_H);
    Wire2.endTransmission(true);
    Wire2.requestFrom(mpuaddr,(size_t)6,true);  
    
    int16_t xdata=Wire2.read()<<8|Wire2.read();
    int16_t ydata=Wire2.read()<<8|Wire2.read();
    int16_t zdata=Wire2.read()<<8|Wire2.read();
=======
    Wire1.endTransmission(true);
}

void MPU::get_temp(struct TStruct *temp){
    Wire1.beginTransmission(ADDR);
    Wire1.write(TEMP_OUT_H);
    Wire1.endTransmission(false);
    Wire1.requestFrom(mpuaddr,(size_t)2,true);  
    
    int16_t tdata=Wire1.read()<<8|Wire1.read();

    temp->TempC=float(tdata)/340 +36.53;

    Wire1.endTransmission(true);
}

void MPU::get_gyro(int Gnum,struct GStruct *gyro){
    Wire1.beginTransmission(ADDR);
    Wire1.write(GYRO_XOUT_H);
    Wire1.endTransmission(true);
    Wire1.requestFrom(mpuaddr,(size_t)6,true);  
    
    int16_t xdata=Wire1.read()<<8|Wire1.read();
    int16_t ydata=Wire1.read()<<8|Wire1.read();
    int16_t zdata=Wire1.read()<<8|Wire1.read();
>>>>>>> Stashed changes
    
    gyro->XAxis=(float)xdata/GyroRange[Gnum];
    gyro->YAxis=(float)ydata/GyroRange[Gnum];
    gyro->ZAxis=(float)zdata/GyroRange[Gnum];

<<<<<<< Updated upstream
    Wire2.endTransmission(true);
=======
    Wire1.endTransmission(true);
>>>>>>> Stashed changes
}
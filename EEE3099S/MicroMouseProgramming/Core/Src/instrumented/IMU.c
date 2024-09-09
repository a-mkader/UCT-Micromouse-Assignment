//********************************************************************
//*                      Micro Mouse Board                           *
//*                          IMU Library                             *
//*==================================================================*
//* @author:    Jesse Jabez Arendse                                  *
//* @date:      24-10-2023                                           *
//*==================================================================*

#include "main.h"
#include "IMU.h"

uint8_t checking = 0;

float accel_x = 0;
float accel_y = 0;
float accel_z = 0;
float gyro_x  = 0;
float gyro_y  = 0;
float gyro_z  = 0;

uint16_t accelMem[3];
uint16_t gyroMem[3];

short IMUCounter = 0;


extern I2C_HandleTypeDef hi2c2;
extern float IMU_Accel[3];
extern float IMU_Gyro[3];
uint8_t checkIMU;


int16_t signNumber16(uint16_t unsignedValue){
	int16_t signedValue;

	if (unsignedValue <= INT16_MAX) {
        signedValue = (int16_t)unsignedValue; // No change needed, it fits in the signed range.
    } else {
        signedValue = -((int16_t)(UINT16_MAX - unsignedValue + 1));
    }

	return signedValue;
}


static void WriteMem(uint8_t devAddress, uint8_t RegisterAddress, uint16_t Value)
{
	uint8_t addr[2];
	addr[0] = (Value >> 8) & 0xff;  // upper byte
	addr[1] = (Value >> 0) & 0xff; // lower byte
	HAL_I2C_Mem_Write(&hi2c2, (devAddress<<1)+0, RegisterAddress, 1, (uint8_t*)addr, 2, HAL_MAX_DELAY);
}

static uint16_t ReadMem(uint8_t devAddress, uint8_t RegisterAddress)
{
	uint8_t Value[2];

	HAL_I2C_Mem_Read(&hi2c2, (devAddress<<1)+1, RegisterAddress, 1, &Value, 2, HAL_MAX_DELAY);

	return ((Value[0] << 8) | Value[1]);
}

float readAccel(short axis){
    accel_x =  signNumber16(ReadMem(IMU_ADDRESS,IMU_ACCEL_DATA_ADDRESS  ))*10/16384.0f; //because full range sensitivity, converts to m/s^2
    accel_y =  signNumber16(ReadMem(IMU_ADDRESS,IMU_ACCEL_DATA_ADDRESS+2))*10/16384.0f;
    accel_z =  signNumber16(ReadMem(IMU_ADDRESS,IMU_ACCEL_DATA_ADDRESS+4))*10/16384.0f;

    if (axis == IMU_X_AXIS){
        return accel_x;
    }
    if (axis == IMU_Y_AXIS){
        return accel_y;
    }
    if (axis == IMU_Z_AXIS){
        return accel_z;
    }
}

float readGyro(short axis){
    gyro_x = signNumber16(ReadMem(IMU_ADDRESS,IMU_GYRO_DATA_ADDRESS  ))/131.0f; //because full range sensitivity
    gyro_y = signNumber16(ReadMem(IMU_ADDRESS,IMU_GYRO_DATA_ADDRESS+2))/131.0f;
    gyro_z = signNumber16(ReadMem(IMU_ADDRESS,IMU_GYRO_DATA_ADDRESS+4))/131.0f;

    if (axis == IMU_X_AXIS){
        return gyro_x;
    }
    if (axis == IMU_Y_AXIS){
        return gyro_y;
    }
    if (axis == IMU_Z_AXIS){
        return gyro_z;
    }
}


void refreshIMUValues(){  // this is an interrupt, makes board run slow, dividing the interrupts task to make board more responsive
    IMU_Accel[0] =  signNumber16(ReadMem(IMU_ADDRESS,IMU_ACCEL_DATA_ADDRESS  ))*10/16384.0f; //because full range sensitivity, converts to m/s^2
    IMU_Accel[1] =  signNumber16(ReadMem(IMU_ADDRESS,IMU_ACCEL_DATA_ADDRESS+2))*10/16384.0f;
    IMU_Accel[2] =  signNumber16(ReadMem(IMU_ADDRESS,IMU_ACCEL_DATA_ADDRESS+4))*10/16384.0f;

    IMU_Gyro[0] = signNumber16(ReadMem(IMU_ADDRESS,IMU_GYRO_DATA_ADDRESS  ))/131.0f; 
    IMU_Gyro[1] = signNumber16(ReadMem(IMU_ADDRESS,IMU_GYRO_DATA_ADDRESS+2))/131.0f;
    IMU_Gyro[2] = signNumber16(ReadMem(IMU_ADDRESS,IMU_GYRO_DATA_ADDRESS+4))/131.0f;
}


// uint8_t checkIMUVal;

void initIMU(){
    uint8_t wake = 0;
    HAL_I2C_Mem_Write(&hi2c2, (IMU_ADDRESS<<1)+0, IMU_WAKE_ADDRESS, 1, &wake , 1                     , HAL_MAX_DELAY);
    HAL_I2C_Mem_Write(&hi2c2, (IMU_ADDRESS<<1)+0, IMU_OUTPUT_RATE_ADDRESS, 1, (uint8_t*) IMU_OUTPUT_RATE, 1 , HAL_MAX_DELAY); // sets data output rate to 1 kHz
    HAL_I2C_Mem_Write(&hi2c2, (IMU_ADDRESS<<1)+0, IMU_ACCEL_CONFIG_ADDRESS, 1,(uint8_t*) 0x0, 1             , HAL_MAX_DELAY); // full range: +-2g
    HAL_I2C_Mem_Write(&hi2c2, (IMU_ADDRESS<<1)+0, IMU_GYRO_CONFIG_ADDRESS, 1,(uint8_t*) 0x0, 1              , HAL_MAX_DELAY); // full range: +-250 deg/s 
    
    HAL_I2C_Mem_Read(&hi2c2, (IMU_ADDRESS<<1)+1, IMU_DEVICE_CHECK, 1, &checkIMU, 1, HAL_MAX_DELAY);     
    // HAL_I2C_Mem_Read(&hi2c2, (IMU_ADDRESS<<1)+1, IMU_WAKE_ADDRESS, 1, &checkIMUVal, 1, HAL_MAX_DELAY);                     
}

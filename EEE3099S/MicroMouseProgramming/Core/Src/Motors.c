//********************************************************************
//*                      Micro Mouse Board                           *
//*                          Motor Library                           *
//*==================================================================*
//* @author:    Jesse Jabez Arendse                                  *
//* @date:      04/07/2024                                           *
//*==================================================================*

#include "main.h"
#include "Motors.h"

extern TIM_HandleTypeDef htim3;
extern uint16_t MOT_L[2];
extern uint16_t MOT_R[2];

void initMotors(){
    TIM3->CCR1 = 0;
    TIM3->CCR2 = 0;
    TIM3->CCR3 = 0;
    TIM3->CCR4 = 0;

    HAL_TIM_PWM_Start(&htim3, RIGHT_FWD);
    HAL_TIM_PWM_Start(&htim3, RIGHT_BWD);
    HAL_TIM_PWM_Start(&htim3, LEFT_FWD);
    HAL_TIM_PWM_Start(&htim3, LEFT_BWD);
}

void updateSpeeds(){
    TIM3->CCR1 = MOT_R[0];
    TIM3->CCR2 = MOT_R[1];
    TIM3->CCR3 = MOT_L[0];
    TIM3->CCR4 = MOT_L[1];
}
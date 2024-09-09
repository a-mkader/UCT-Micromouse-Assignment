//********************************************************************
//*                      Micro Mouse Board                           *
//*                          Motor Library                           *
//*==================================================================*
//* @author:    Jesse Jabez Arendse                                  *
//* @date:      04/07/2024                                           *
//*==================================================================*


#define RIGHT_FWD TIM_CHANNEL_1
#define RIGHT_BWD TIM_CHANNEL_2
#define LEFT_FWD TIM_CHANNEL_3
#define LEFT_BWD TIM_CHANNEL_4

void initMotors();
void updateSpeeds();
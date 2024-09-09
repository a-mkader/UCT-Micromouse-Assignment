#include "main.h"
#include "CustomWhile.h"
#include "usb_device.h"

#include "math.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define USB_VBUS_PIN GPIO_PIN_9
#define USB_VBUS_PORT GPIOA
#define IS_USB_CONNECTED() (HAL_GPIO_ReadPin(USB_VBUS_PORT, USB_VBUS_PIN) == GPIO_PIN_SET)

bool diag_ProSW[2];
bool diag_ProLED[3]; 
extern uint8_t IMUCheck;
extern float IMU_Accel[3]; 
extern float IMU_Gyro[3]; 
bool diag_IR_LEDs[8];
uint32_t IR_LED_TIMER; 
extern uint16_t ADC1s[9];
extern uint16_t ADC2s[9];
extern uint16_t ADC_H[9];
extern uint16_t ADC_L[9];
extern uint32_t currTicksLS;
extern uint32_t currTicksRS;
uint16_t diag_MOT_L[2] = {0,0};
uint16_t diag_MOT_R[2] = {0,0};
extern bool Detections[8];
extern uint16_t Thresholds[8];

void UnInterruptable(void){
    uint32_t irq;
    // Loop through all possible IRQ numbers
    for (irq = 0; irq < 240; irq++)
    {
        // Skip USB FS
        if (irq == OTG_FS_IRQn || irq == RCC_IRQn)
        {
            continue;
        }
        // Disable the interrupt
        HAL_NVIC_DisableIRQ((IRQn_Type)irq);
    }
}

void Interruptable(void){
    uint32_t irq;
    // Loop through all possible IRQ numbers
    for (irq = 0; irq < 240; irq++){
        HAL_NVIC_EnableIRQ((IRQn_Type)irq);
    }
}



void CustomWhile(void){
    




    // SIMULINK BYTE UNPACKING AND DEMUXING MUST MATCH THIS

    diag_ProSW[0] = HAL_GPIO_ReadPin(SW1_GPIO_Port,SW1_Pin);
    diag_ProSW[1] = HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin);

    diag_ProLED[0] = HAL_GPIO_ReadPin(LED0_GPIO_Port,LED0_Pin);
    diag_ProLED[1] = HAL_GPIO_ReadPin(LED1_GPIO_Port,LED1_Pin);
    diag_ProLED[2] = HAL_GPIO_ReadPin(LED2_GPIO_Port,LED2_Pin);

    diag_IR_LEDs[0] = HAL_GPIO_ReadPin(LED_MOT_LS_GPIO_Port,LED_MOT_LS_Pin);
    diag_IR_LEDs[1] = HAL_GPIO_ReadPin(LED_FWD_LS_GPIO_Port,LED_FWD_LS_Pin);
    diag_IR_LEDs[2] = HAL_GPIO_ReadPin(LED_DOWN_LS_GPIO_Port,LED_DOWN_LS_Pin);
    diag_IR_LEDs[3] = HAL_GPIO_ReadPin(LED_LS_GPIO_Port,LED_LS_Pin);
    diag_IR_LEDs[4] = HAL_GPIO_ReadPin(LED_RS_GPIO_Port,LED_RS_Pin);
    diag_IR_LEDs[5] = HAL_GPIO_ReadPin(LED_DOWN_RS_GPIO_Port,LED_DOWN_RS_Pin);
    diag_IR_LEDs[6] = HAL_GPIO_ReadPin(LED_FWD_RS_GPIO_Port,LED_FWD_RS_Pin);
    diag_IR_LEDs[7] = HAL_GPIO_ReadPin(LED_MOT_RS_GPIO_Port,LED_MOT_RS_Pin);
  
    diag_MOT_R[0] = TIM3 -> CCR1;
    diag_MOT_R[1] = TIM3 -> CCR2;
    diag_MOT_L[0] = TIM3 -> CCR3;
    diag_MOT_L[1] = TIM3 -> CCR4;

        UnInterruptable();

        // very important: using a variable for header and terminator seemed to be optimized out with simulinks settings, hardcode them rather

        while( CDC_Transmit_FS("JJA",3)            != USBD_OK){if (!IS_USB_CONNECTED()) {break;}} 
        while( CDC_Transmit_FS(&diag_ProSW ,1*2)   != USBD_OK){if (!IS_USB_CONNECTED()) {break;}}
        while( CDC_Transmit_FS(&diag_ProLED ,1*3)  != USBD_OK){if (!IS_USB_CONNECTED()) {break;}}
        while( CDC_Transmit_FS(&IMU_Accel, 4*3)    != USBD_OK){if (!IS_USB_CONNECTED()) {break;}}
        while( CDC_Transmit_FS(&IMU_Gyro , 4*3)    != USBD_OK){if (!IS_USB_CONNECTED()) {break;}}
        while( CDC_Transmit_FS(&ADC1s, 2*9)        != USBD_OK){if (!IS_USB_CONNECTED()) {break;}}
        while( CDC_Transmit_FS(&ADC_H, 2*9)        != USBD_OK){if (!IS_USB_CONNECTED()) {break;}}
        while( CDC_Transmit_FS(&ADC_L, 2*9)        != USBD_OK){if (!IS_USB_CONNECTED()) {break;}}
        while( CDC_Transmit_FS(&diag_IR_LEDs, 8)   != USBD_OK){if (!IS_USB_CONNECTED()) {break;}}
        while( CDC_Transmit_FS(&IR_LED_TIMER, 4)   != USBD_OK){if (!IS_USB_CONNECTED()) {break;}}
        while( CDC_Transmit_FS(&diag_MOT_L, 2*2)   != USBD_OK){if (!IS_USB_CONNECTED()) {break;}}
        while( CDC_Transmit_FS(&diag_MOT_R, 2*2)   != USBD_OK){if (!IS_USB_CONNECTED()) {break;}}
        while( CDC_Transmit_FS(&Detections, 1*8)   != USBD_OK){if (!IS_USB_CONNECTED()) {break;}}
        while( CDC_Transmit_FS(&Thresholds, 2*8)   != USBD_OK){if (!IS_USB_CONNECTED()) {break;}}
        while( CDC_Transmit_FS(&currTicksLS, 1*4)   != USBD_OK){if (!IS_USB_CONNECTED()) {break;}}
        while( CDC_Transmit_FS(&currTicksRS, 1*4)   != USBD_OK){if (!IS_USB_CONNECTED()) {break;}}
        while( CDC_Transmit_FS("3099",4)           != USBD_OK){if (!IS_USB_CONNECTED()) {break;}}


        Interruptable();
    
} 
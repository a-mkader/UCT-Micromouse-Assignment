/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MicroMouseTemplate.c
 *
 * Code generated for Simulink model 'MicroMouseTemplate'.
 *
 * Model version                  : 1.264
 * Simulink Coder version         : 9.9 (R2023a) 19-Nov-2022
 * C/C++ source code generated on : Thu Sep  5 14:32:16 2024
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "MicroMouseTemplate.h"
#include "rtwtypes.h"
#include "MicroMouseTemplate_types.h"
#include "MicroMouseTemplate_private.h"
#include <string.h>
#include <math.h>
#include "stm_timer_ll.h"
#include "stm_adc_ll.h"

/* Named constants for Chart: '<Root>/HelloMicroMouse!' */
#define MicroMouseTemp_IN_ButtonPressed ((uint8_T)1U)
#define MicroMouseTemplate_IN_Left     ((uint8_T)1U)
#define MicroMouseTemplate_IN_Right    ((uint8_T)2U)
#define MicroMouseTemplate_IN_Stop     ((uint8_T)2U)
#define Micro_IN_WaitUntilButtonPressed ((uint8_T)3U)

/* user code (top of source file) */
/* System '<Root>' */
extern I2C_HandleTypeDef hi2c2;

/* Exported block states */
real32_T IMU_Accel[3];                 /* '<S7>/Data Store Memory' */
real32_T IMU_Gyro[3];                  /* '<S7>/Data Store Memory1' */
int32_T currTicksRS;                   /* '<S1>/Data Store Memory2' */
int32_T currTicksLS;                   /* '<S1>/Data Store Memory4' */
uint16_T Thresholds[8];                /* '<S1>/Data Store Memory1' */
uint16_T ADC1s[9];                     /* '<S6>/Data Store Memory' */
uint16_T ADC_H[9];                     /* '<S6>/Data Store Memory1' */
uint16_T ADC_L[9];                     /* '<S6>/Data Store Memory2' */
boolean_T Detections[8];               /* '<S1>/Data Store Memory' */

/* Block signals (default storage) */
B_MicroMouseTemplate_T MicroMouseTemplate_B;

/* Block states (default storage) */
DW_MicroMouseTemplate_T MicroMouseTemplate_DW;

/* Real-time model */
static RT_MODEL_MicroMouseTemplate_T MicroMouseTemplate_M_;
RT_MODEL_MicroMouseTemplate_T *const MicroMouseTemplate_M =
  &MicroMouseTemplate_M_;

/* Forward declaration for local functions */
static void MicroMouseT_SystemCore_setup_dx(stm32cube_blocks_AnalogInput__T *obj);
static void MicroMouseTemp_SystemCore_setup(stm32cube_blocks_PWMOutput_Mi_T *obj);
static void MicroMouseTe_SystemCore_setup_d(stm32cube_blocks_PWMOutput_Mi_T *obj);

/* System initialize for atomic system: */
void MicroMouseTe_MATLABSystem1_Init(DW_MATLABSystem1_MicroMouseTe_T *localDW,
  P_MATLABSystem1_MicroMouseTem_T *localP)
{
  int32_T i;

  /* Start for MATLABSystem: '<S47>/MATLAB System1' */
  localDW->objisempty = true;
  localDW->obj.InitialCondition = localP->MATLABSystem1_InitialCondition;
  localDW->obj.isInitialized = 1;

  /*  Number of inputs */
  /*  Initialize the buffer during the first call */
  for (i = 0; i < 25; i++) {
    localDW->obj.Buffer[i] = localDW->obj.InitialCondition;
  }

  /* End of Start for MATLABSystem: '<S47>/MATLAB System1' */
  /*  Reset the buffer to initial condition */
}

/* Output and update for atomic system: */
void MicroMouseTemplat_MATLABSystem1(real_T rtu_0,
  B_MATLABSystem1_MicroMouseTem_T *localB, DW_MATLABSystem1_MicroMouseTe_T
  *localDW, P_MATLABSystem1_MicroMouseTem_T *localP)
{
  /* MATLABSystem: '<S47>/MATLAB System1' */
  if (localDW->obj.InitialCondition != localP->MATLABSystem1_InitialCondition) {
    localDW->obj.InitialCondition = localP->MATLABSystem1_InitialCondition;
  }

  /*  Number of inputs */
  /*  Number of outputs */
  /*  Output the current state of the buffer */
  memcpy(&localB->MATLABSystem1[0], &localDW->obj.Buffer[0], 25U * sizeof(real_T));

  /*  Update the buffer */
  localB->rtu_0[0] = rtu_0;
  memcpy(&localB->rtu_0[1], &localDW->obj.Buffer[0], 24U * sizeof(real_T));
  memcpy(&localDW->obj.Buffer[0], &localB->rtu_0[0], 25U * sizeof(real_T));

  /* End of MATLABSystem: '<S47>/MATLAB System1' */
}

/* System initialize for atomic system: */
void MicroMouseTe_MATLABSystem3_Init(DW_MATLABSystem3_MicroMouseTe_T *localDW,
  P_MATLABSystem3_MicroMouseTem_T *localP)
{
  int32_T i;

  /* Start for MATLABSystem: '<S47>/MATLAB System3' */
  localDW->objisempty = true;
  localDW->obj.InitialCondition = localP->MATLABSystem3_InitialCondition;
  localDW->obj.isInitialized = 1;

  /*  Number of inputs */
  /*  Initialize the buffer during the first call */
  for (i = 0; i < 10; i++) {
    localDW->obj.Buffer[i] = localDW->obj.InitialCondition;
  }

  /* End of Start for MATLABSystem: '<S47>/MATLAB System3' */
  /*  Reset the buffer to initial condition */
}

/* Output and update for atomic system: */
void MicroMouseTemplat_MATLABSystem3(real_T rtu_0,
  B_MATLABSystem3_MicroMouseTem_T *localB, DW_MATLABSystem3_MicroMouseTe_T
  *localDW, P_MATLABSystem3_MicroMouseTem_T *localP)
{
  /* MATLABSystem: '<S47>/MATLAB System3' */
  if (localDW->obj.InitialCondition != localP->MATLABSystem3_InitialCondition) {
    localDW->obj.InitialCondition = localP->MATLABSystem3_InitialCondition;
  }

  /*  Number of inputs */
  /*  Number of outputs */
  /*  Output the current state of the buffer */
  memcpy(&localB->MATLABSystem3[0], &localDW->obj.Buffer[0], 10U * sizeof(real_T));

  /*  Update the buffer */
  localB->rtu_0[0] = rtu_0;
  memcpy(&localB->rtu_0[1], &localDW->obj.Buffer[0], 9U * sizeof(real_T));
  memcpy(&localDW->obj.Buffer[0], &localB->rtu_0[0], 10U * sizeof(real_T));

  /* End of MATLABSystem: '<S47>/MATLAB System3' */
}

static void MicroMouseT_SystemCore_setup_dx(stm32cube_blocks_AnalogInput__T *obj)
{
  uint16_T module;
  uint16_T triggerType;
  uint32_T noConv;
  ADC_Type_T adcStructLoc;
  obj->isSetupComplete = false;
  obj->isInitialized = 1;
  adcStructLoc.dmastream = LL_DMA_CHANNEL_2;
  adcStructLoc.dmaPeripheralPtr = DMA1;
  adcStructLoc.InternalBufferPtr = (void*)(&obj->ADCInternalBuffer[0]);
  adcStructLoc.peripheralPtr = ADC2;
  adcStructLoc.InjectedNoOfConversion = 0U;
  adcStructLoc.InternalBufferSize = 9U;
  module = ADC_DMA_INTERRUPT_MODE;
  triggerType = ADC_TRIGGER_AND_READ;
  noConv = LL_ADC_REG_SEQ_SCAN_ENABLE_9RANKS;
  adcStructLoc.RegularNoOfConversion = 9U;
  obj->ADCHandle = ADC_Handle_Init(&adcStructLoc, module, 1, triggerType, noConv);
  enableADCAutomaticCalibrationOffset(obj->ADCHandle, 2);
  enableADC(obj->ADCHandle);
  startADCConversionForExternalTrigger(obj->ADCHandle, 1);
  obj->isSetupComplete = true;
}

static void MicroMouseTemp_SystemCore_setup(stm32cube_blocks_PWMOutput_Mi_T *obj)
{
  uint8_T ChannelInfo;
  TIM_Type_T b;
  boolean_T isSlaveModeTriggerEnabled;
  obj->isInitialized = 1;
  b.PeripheralPtr = TIM3;
  b.isCenterAlignedMode = false;
  b.repetitionCounter = 0U;
  obj->TimerHandle = Timer_Handle_Init(&b);
  enableTimerInterrupts(obj->TimerHandle, 0);
  ChannelInfo = ENABLE_CH;
  enableTimerChannel3(obj->TimerHandle, ChannelInfo);
  ChannelInfo = ENABLE_CH;
  enableTimerChannel4(obj->TimerHandle, ChannelInfo);
  isSlaveModeTriggerEnabled = isSlaveTriggerModeEnabled(obj->TimerHandle);
  if (!isSlaveModeTriggerEnabled) {
    enableCounter(obj->TimerHandle, false);
  }

  obj->isSetupComplete = true;
}

static void MicroMouseTe_SystemCore_setup_d(stm32cube_blocks_PWMOutput_Mi_T *obj)
{
  uint8_T ChannelInfo;
  TIM_Type_T b;
  boolean_T isSlaveModeTriggerEnabled;
  obj->isInitialized = 1;
  b.PeripheralPtr = TIM3;
  b.isCenterAlignedMode = false;
  b.repetitionCounter = 0U;
  obj->TimerHandle = Timer_Handle_Init(&b);
  enableTimerInterrupts(obj->TimerHandle, 0);
  ChannelInfo = ENABLE_CH;
  enableTimerChannel1(obj->TimerHandle, ChannelInfo);
  ChannelInfo = ENABLE_CH;
  enableTimerChannel2(obj->TimerHandle, ChannelInfo);
  isSlaveModeTriggerEnabled = isSlaveTriggerModeEnabled(obj->TimerHandle);
  if (!isSlaveModeTriggerEnabled) {
    enableCounter(obj->TimerHandle, false);
  }

  obj->isSetupComplete = true;
}

/* Model step function */
void MicroMouseTemplate_step(void)
{
  /* local block i/o variables */
  real_T rtb_MaxofElements1;
  real_T rtb_MaxofElements6;
  GPIO_TypeDef * portNameLoc;
  uint16_T triggerType;
  real_T maxV;
  real_T minV;
  real_T u1;
  int32_T i;
  uint32_T C3_DC;
  for (i = 0; i < 8; i++) {
    /* DataStoreWrite: '<S1>/Data Store Write' */
    Detections[i] = false;

    /* DataStoreWrite: '<S1>/Data Store Write1' */
    Thresholds[i] = 0U;
  }

  /* DataStoreWrite: '<S1>/Data Store Write2' */
  currTicksRS = 0;

  /* DataStoreWrite: '<S1>/Data Store Write3' */
  currTicksLS = 0;

  /* MATLABSystem: '<S46>/Analog to Digital Converter' */
  triggerType = ADC_TRIGGER_AND_READ;
  regularReadADCDMA(MicroMouseTemplate_DW.obj.ADCHandle, triggerType, &ADC1s[0]);

  /* DSPFlip: '<S6>/Flip' incorporates:
   *  MATLABSystem: '<S46>/Analog to Digital Converter'
   */
  MicroMouseTemplate_B.Flip[0] = ADC1s[7];
  MicroMouseTemplate_B.Flip[7] = ADC1s[0];
  MicroMouseTemplate_B.Flip[1] = ADC1s[6];
  MicroMouseTemplate_B.Flip[6] = ADC1s[1];
  MicroMouseTemplate_B.Flip[2] = ADC1s[5];
  MicroMouseTemplate_B.Flip[5] = ADC1s[2];
  MicroMouseTemplate_B.Flip[3] = ADC1s[4];
  MicroMouseTemplate_B.Flip[4] = ADC1s[3];
  for (i = 0; i < 8; i++) {
    /* DataTypeConversion: '<S49>/Cast To Double' */
    MicroMouseTemplate_B.CastToDouble[i] = MicroMouseTemplate_B.Flip[i];
  }

  MicroMouseTemplat_MATLABSystem1(MicroMouseTemplate_B.CastToDouble[0],
    &MicroMouseTemplate_B.MATLABSystem2_ci,
    &MicroMouseTemplate_DW.MATLABSystem2_ci,
    &MicroMouseTemplate_P.MATLABSystem2_ci);

  /* MinMax: '<S47>/Max of Elements' incorporates:
   *  MATLABSystem: '<S49>/MATLAB System2'
   */
  MicroMouseTemplate_B.maxV =
    MicroMouseTemplate_B.MATLABSystem2_ci.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem2_ci.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.maxV < u1) {
      MicroMouseTemplate_B.maxV = u1;
    }
  }

  MicroMouseTemplat_MATLABSystem3(MicroMouseTemplate_B.CastToDouble[1],
    &MicroMouseTemplate_B.MATLABSystem4, &MicroMouseTemplate_DW.MATLABSystem4,
    &MicroMouseTemplate_P.MATLABSystem4);

  /* MinMax: '<S47>/Max of Elements1' incorporates:
   *  MATLABSystem: '<S47>/MATLAB System4'
   */
  rtb_MaxofElements1 = MicroMouseTemplate_B.MATLABSystem4.MATLABSystem3[0];
  for (i = 0; i < 9; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem4.MATLABSystem3[i + 1];
    if (rtb_MaxofElements1 < u1) {
      rtb_MaxofElements1 = u1;
    }
  }

  /* End of MinMax: '<S47>/Max of Elements1' */
  MicroMouseTemplat_MATLABSystem1(rtb_MaxofElements1,
    &MicroMouseTemplate_B.MATLABSystem1, &MicroMouseTemplate_DW.MATLABSystem1,
    &MicroMouseTemplate_P.MATLABSystem1);

  /* MinMax: '<S47>/Min' incorporates:
   *  MATLABSystem: '<S47>/MATLAB System1'
   */
  MicroMouseTemplate_B.minV = MicroMouseTemplate_B.MATLABSystem1.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem1.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.minV > u1) {
      MicroMouseTemplate_B.minV = u1;
    }
  }

  MicroMouseTemplat_MATLABSystem1(MicroMouseTemplate_B.CastToDouble[2],
    &MicroMouseTemplate_B.MATLABSystem1_ci,
    &MicroMouseTemplate_DW.MATLABSystem1_ci,
    &MicroMouseTemplate_P.MATLABSystem1_ci);

  /* MinMax: '<S47>/Max of Elements2' incorporates:
   *  MATLABSystem: '<S49>/MATLAB System1'
   */
  MicroMouseTemplate_B.maxV_m =
    MicroMouseTemplate_B.MATLABSystem1_ci.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem1_ci.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.maxV_m < u1) {
      MicroMouseTemplate_B.maxV_m = u1;
    }
  }

  MicroMouseTemplat_MATLABSystem1(MicroMouseTemplate_B.CastToDouble[3],
    &MicroMouseTemplate_B.MATLABSystem3_c,
    &MicroMouseTemplate_DW.MATLABSystem3_c,
    &MicroMouseTemplate_P.MATLABSystem3_c);

  /* MinMax: '<S47>/Max of Elements3' incorporates:
   *  MATLABSystem: '<S49>/MATLAB System3'
   */
  MicroMouseTemplate_B.maxV_c =
    MicroMouseTemplate_B.MATLABSystem3_c.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem3_c.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.maxV_c < u1) {
      MicroMouseTemplate_B.maxV_c = u1;
    }
  }

  MicroMouseTemplat_MATLABSystem1(MicroMouseTemplate_B.CastToDouble[4],
    &MicroMouseTemplate_B.MATLABSystem4_c,
    &MicroMouseTemplate_DW.MATLABSystem4_c,
    &MicroMouseTemplate_P.MATLABSystem4_c);

  /* MinMax: '<S47>/Max of Elements4' incorporates:
   *  MATLABSystem: '<S49>/MATLAB System4'
   */
  MicroMouseTemplate_B.maxV_k =
    MicroMouseTemplate_B.MATLABSystem4_c.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem4_c.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.maxV_k < u1) {
      MicroMouseTemplate_B.maxV_k = u1;
    }
  }

  MicroMouseTemplat_MATLABSystem1(MicroMouseTemplate_B.CastToDouble[5],
    &MicroMouseTemplate_B.MATLABSystem5, &MicroMouseTemplate_DW.MATLABSystem5,
    &MicroMouseTemplate_P.MATLABSystem5);

  /* MinMax: '<S47>/Max of Elements5' incorporates:
   *  MATLABSystem: '<S49>/MATLAB System5'
   */
  MicroMouseTemplate_B.maxV_cx =
    MicroMouseTemplate_B.MATLABSystem5.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem5.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.maxV_cx < u1) {
      MicroMouseTemplate_B.maxV_cx = u1;
    }
  }

  MicroMouseTemplat_MATLABSystem3(MicroMouseTemplate_B.CastToDouble[6],
    &MicroMouseTemplate_B.MATLABSystem3, &MicroMouseTemplate_DW.MATLABSystem3,
    &MicroMouseTemplate_P.MATLABSystem3);

  /* MinMax: '<S47>/Max of Elements6' incorporates:
   *  MATLABSystem: '<S47>/MATLAB System3'
   */
  rtb_MaxofElements6 = MicroMouseTemplate_B.MATLABSystem3.MATLABSystem3[0];
  for (i = 0; i < 9; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem3.MATLABSystem3[i + 1];
    if (rtb_MaxofElements6 < u1) {
      rtb_MaxofElements6 = u1;
    }
  }

  /* End of MinMax: '<S47>/Max of Elements6' */
  MicroMouseTemplat_MATLABSystem1(rtb_MaxofElements6,
    &MicroMouseTemplate_B.MATLABSystem2, &MicroMouseTemplate_DW.MATLABSystem2,
    &MicroMouseTemplate_P.MATLABSystem2);

  /* MinMax: '<S47>/Min1' incorporates:
   *  MATLABSystem: '<S47>/MATLAB System2'
   */
  minV = MicroMouseTemplate_B.MATLABSystem2.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem2.MATLABSystem1[i + 1];
    if (minV > u1) {
      minV = u1;
    }
  }

  MicroMouseTemplat_MATLABSystem1(MicroMouseTemplate_B.CastToDouble[7],
    &MicroMouseTemplate_B.MATLABSystem6, &MicroMouseTemplate_DW.MATLABSystem6,
    &MicroMouseTemplate_P.MATLABSystem6);

  /* MinMax: '<S47>/Max of Elements7' incorporates:
   *  MATLABSystem: '<S49>/MATLAB System6'
   */
  maxV = MicroMouseTemplate_B.MATLABSystem6.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem6.MATLABSystem1[i + 1];
    if (maxV < u1) {
      maxV = u1;
    }
  }

  /* MinMax: '<S47>/Max of Elements' */
  u1 = fmod(floor(MicroMouseTemplate_B.maxV), 65536.0);

  /* DataStoreWrite: '<S44>/ADC_H Write' incorporates:
   *  MinMax: '<S47>/Max of Elements'
   */
  ADC_H[0] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S47>/Min' */
  u1 = fmod(floor(MicroMouseTemplate_B.minV), 65536.0);

  /* DataStoreWrite: '<S44>/ADC_H Write' incorporates:
   *  MinMax: '<S47>/Min'
   */
  ADC_H[1] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S47>/Max of Elements2' */
  u1 = fmod(floor(MicroMouseTemplate_B.maxV_m), 65536.0);

  /* DataStoreWrite: '<S44>/ADC_H Write' incorporates:
   *  MinMax: '<S47>/Max of Elements2'
   */
  ADC_H[2] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S47>/Max of Elements3' */
  u1 = fmod(floor(MicroMouseTemplate_B.maxV_c), 65536.0);

  /* DataStoreWrite: '<S44>/ADC_H Write' incorporates:
   *  MinMax: '<S47>/Max of Elements3'
   */
  ADC_H[3] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S47>/Max of Elements4' */
  u1 = fmod(floor(MicroMouseTemplate_B.maxV_k), 65536.0);

  /* DataStoreWrite: '<S44>/ADC_H Write' incorporates:
   *  MinMax: '<S47>/Max of Elements4'
   */
  ADC_H[4] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S47>/Max of Elements5' */
  u1 = fmod(floor(MicroMouseTemplate_B.maxV_cx), 65536.0);

  /* DataStoreWrite: '<S44>/ADC_H Write' incorporates:
   *  MinMax: '<S47>/Max of Elements5'
   */
  ADC_H[5] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S47>/Min1' */
  u1 = fmod(floor(minV), 65536.0);

  /* DataStoreWrite: '<S44>/ADC_H Write' incorporates:
   *  MinMax: '<S47>/Min1'
   */
  ADC_H[6] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S47>/Max of Elements7' */
  u1 = fmod(floor(maxV), 65536.0);

  /* DataStoreWrite: '<S44>/ADC_H Write' incorporates:
   *  Constant: '<S44>/Constant'
   *  MinMax: '<S47>/Max of Elements7'
   */
  ADC_H[7] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);
  ADC_H[8] = MicroMouseTemplate_P.Constant_Value_b;

  /* MinMax: '<S48>/Max of Elements8' incorporates:
   *  MATLABSystem: '<S49>/MATLAB System2'
   */
  MicroMouseTemplate_B.minV =
    MicroMouseTemplate_B.MATLABSystem2_ci.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem2_ci.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.minV > u1) {
      MicroMouseTemplate_B.minV = u1;
    }
  }

  MicroMouseTemplat_MATLABSystem1(MicroMouseTemplate_B.CastToDouble[1],
    &MicroMouseTemplate_B.MATLABSystem1_c,
    &MicroMouseTemplate_DW.MATLABSystem1_c,
    &MicroMouseTemplate_P.MATLABSystem1_c);

  /* MinMax: '<S48>/Max of Elements9' incorporates:
   *  MATLABSystem: '<S48>/MATLAB System1'
   */
  minV = MicroMouseTemplate_B.MATLABSystem1_c.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem1_c.MATLABSystem1[i + 1];
    if (minV > u1) {
      minV = u1;
    }
  }

  /* MinMax: '<S48>/Max of Elements10' incorporates:
   *  MATLABSystem: '<S49>/MATLAB System1'
   */
  MicroMouseTemplate_B.maxV_m =
    MicroMouseTemplate_B.MATLABSystem1_ci.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem1_ci.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.maxV_m > u1) {
      MicroMouseTemplate_B.maxV_m = u1;
    }
  }

  /* MinMax: '<S48>/Max of Elements11' incorporates:
   *  MATLABSystem: '<S49>/MATLAB System3'
   */
  MicroMouseTemplate_B.maxV_c =
    MicroMouseTemplate_B.MATLABSystem3_c.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem3_c.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.maxV_c > u1) {
      MicroMouseTemplate_B.maxV_c = u1;
    }
  }

  /* MinMax: '<S48>/Max of Elements12' incorporates:
   *  MATLABSystem: '<S49>/MATLAB System4'
   */
  MicroMouseTemplate_B.maxV_k =
    MicroMouseTemplate_B.MATLABSystem4_c.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem4_c.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.maxV_k > u1) {
      MicroMouseTemplate_B.maxV_k = u1;
    }
  }

  /* MinMax: '<S48>/Max of Elements13' incorporates:
   *  MATLABSystem: '<S49>/MATLAB System5'
   */
  MicroMouseTemplate_B.maxV_cx =
    MicroMouseTemplate_B.MATLABSystem5.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem5.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.maxV_cx > u1) {
      MicroMouseTemplate_B.maxV_cx = u1;
    }
  }

  MicroMouseTemplat_MATLABSystem1(MicroMouseTemplate_B.CastToDouble[6],
    &MicroMouseTemplate_B.MATLABSystem2_c,
    &MicroMouseTemplate_DW.MATLABSystem2_c,
    &MicroMouseTemplate_P.MATLABSystem2_c);

  /* MinMax: '<S48>/Max of Elements14' incorporates:
   *  MATLABSystem: '<S48>/MATLAB System2'
   */
  maxV = MicroMouseTemplate_B.MATLABSystem2_c.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem2_c.MATLABSystem1[i + 1];
    if (maxV > u1) {
      maxV = u1;
    }
  }

  /* MinMax: '<S48>/Max of Elements15' incorporates:
   *  MATLABSystem: '<S49>/MATLAB System6'
   */
  MicroMouseTemplate_B.maxV = MicroMouseTemplate_B.MATLABSystem6.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem6.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.maxV > u1) {
      MicroMouseTemplate_B.maxV = u1;
    }
  }

  /* MinMax: '<S48>/Max of Elements8' */
  u1 = fmod(floor(MicroMouseTemplate_B.minV), 65536.0);

  /* DataStoreWrite: '<S44>/ADC_L Write' incorporates:
   *  MinMax: '<S48>/Max of Elements8'
   */
  ADC_L[0] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S48>/Max of Elements9' */
  u1 = fmod(floor(minV), 65536.0);

  /* DataStoreWrite: '<S44>/ADC_L Write' incorporates:
   *  MinMax: '<S48>/Max of Elements9'
   */
  ADC_L[1] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S48>/Max of Elements10' */
  u1 = fmod(floor(MicroMouseTemplate_B.maxV_m), 65536.0);

  /* DataStoreWrite: '<S44>/ADC_L Write' incorporates:
   *  MinMax: '<S48>/Max of Elements10'
   */
  ADC_L[2] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S48>/Max of Elements11' */
  u1 = fmod(floor(MicroMouseTemplate_B.maxV_c), 65536.0);

  /* DataStoreWrite: '<S44>/ADC_L Write' incorporates:
   *  MinMax: '<S48>/Max of Elements11'
   */
  ADC_L[3] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S48>/Max of Elements12' */
  u1 = fmod(floor(MicroMouseTemplate_B.maxV_k), 65536.0);

  /* DataStoreWrite: '<S44>/ADC_L Write' incorporates:
   *  MinMax: '<S48>/Max of Elements12'
   */
  ADC_L[4] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S48>/Max of Elements13' */
  u1 = fmod(floor(MicroMouseTemplate_B.maxV_cx), 65536.0);

  /* DataStoreWrite: '<S44>/ADC_L Write' incorporates:
   *  MinMax: '<S48>/Max of Elements13'
   */
  ADC_L[5] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S48>/Max of Elements14' */
  u1 = fmod(floor(maxV), 65536.0);

  /* DataStoreWrite: '<S44>/ADC_L Write' incorporates:
   *  MinMax: '<S48>/Max of Elements14'
   */
  ADC_L[6] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S48>/Max of Elements15' */
  u1 = fmod(floor(MicroMouseTemplate_B.maxV), 65536.0);

  /* DataStoreWrite: '<S44>/ADC_L Write' incorporates:
   *  Constant: '<S44>/Constant1'
   *  MinMax: '<S48>/Max of Elements15'
   */
  ADC_L[7] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);
  ADC_L[8] = MicroMouseTemplate_P.Constant1_Value;

  /* Outputs for Atomic SubSystem: '<Root>/GPIO for IR LEDs' */
  /* MATLABSystem: '<S19>/Digital Port Write' */
  portNameLoc = GPIOE;
  LL_GPIO_SetOutputPin(portNameLoc, 0U);
  LL_GPIO_ResetOutputPin(portNameLoc, 512U);

  /* MATLABSystem: '<S21>/Digital Port Write' */
  portNameLoc = GPIOE;
  LL_GPIO_SetOutputPin(portNameLoc, 0U);
  LL_GPIO_ResetOutputPin(portNameLoc, 16384U);

  /* MATLABSystem: '<S23>/Digital Port Write' */
  portNameLoc = GPIOE;
  LL_GPIO_SetOutputPin(portNameLoc, 0U);
  LL_GPIO_ResetOutputPin(portNameLoc, 256U);

  /* MATLABSystem: '<S25>/Digital Port Write' */
  portNameLoc = GPIOE;
  LL_GPIO_SetOutputPin(portNameLoc, 0U);
  LL_GPIO_ResetOutputPin(portNameLoc, 32768U);

  /* MATLABSystem: '<S27>/Digital Port Write' */
  portNameLoc = GPIOE;
  LL_GPIO_SetOutputPin(portNameLoc, 0U);
  LL_GPIO_ResetOutputPin(portNameLoc, 4096U);

  /* MATLABSystem: '<S29>/Digital Port Write' */
  portNameLoc = GPIOB;
  LL_GPIO_SetOutputPin(portNameLoc, 0U);
  LL_GPIO_ResetOutputPin(portNameLoc, 4096U);

  /* MATLABSystem: '<S31>/Digital Port Write' */
  portNameLoc = GPIOE;
  LL_GPIO_SetOutputPin(portNameLoc, 0U);
  LL_GPIO_ResetOutputPin(portNameLoc, 8192U);

  /* MATLABSystem: '<S33>/Digital Port Write' */
  portNameLoc = GPIOE;
  LL_GPIO_SetOutputPin(portNameLoc, 0U);
  LL_GPIO_ResetOutputPin(portNameLoc, 2048U);

  /* End of Outputs for SubSystem: '<Root>/GPIO for IR LEDs' */

  /* Chart: '<Root>/HelloMicroMouse!' */
  if (MicroMouseTemplate_DW.temporalCounter_i1 < 7U) {
    MicroMouseTemplate_DW.temporalCounter_i1++;
  }

  if (MicroMouseTemplate_DW.bitsForTID0.is_active_c2_MicroMouseTemplate == 0U) {
    MicroMouseTemplate_DW.bitsForTID0.is_active_c2_MicroMouseTemplate = 1U;
    MicroMouseTemplate_DW.bitsForTID0.is_c2_MicroMouseTemplate =
      Micro_IN_WaitUntilButtonPressed;
  } else {
    switch (MicroMouseTemplate_DW.bitsForTID0.is_c2_MicroMouseTemplate) {
     case MicroMouseTemp_IN_ButtonPressed:
      if (MicroMouseTemplate_DW.bitsForTID0.is_ButtonPressed ==
          MicroMouseTemplate_IN_Left) {
        if (MicroMouseTemplate_DW.temporalCounter_i1 >= 5U) {
          MicroMouseTemplate_DW.bitsForTID0.is_ButtonPressed =
            MicroMouseTemplate_IN_Right;
          MicroMouseTemplate_DW.temporalCounter_i1 = 0U;
        }

        /* case IN_Right: */
      } else if (MicroMouseTemplate_DW.temporalCounter_i1 >= 5U) {
        MicroMouseTemplate_DW.bitsForTID0.is_ButtonPressed =
          MicroMouseTemplate_IN_Left;
        MicroMouseTemplate_DW.temporalCounter_i1 = 0U;
      }
      break;

     case MicroMouseTemplate_IN_Stop:
      break;

     default:
      /* case IN_WaitUntilButtonPressed: */
      MicroMouseTemplate_B.LED0 = !MicroMouseTemplate_B.LED0;
      break;
    }
  }

  /* End of Chart: '<Root>/HelloMicroMouse!' */

  /* MATLABSystem: '<S38>/Digital Port Write' incorporates:
   *  Constant: '<S5>/Constant'
   */
  portNameLoc = GPIOD;
  if (MicroMouseTemplate_P.Constant_Value != 0.0) {
    i = 128;
  } else {
    i = 0;
  }

  LL_GPIO_SetOutputPin(portNameLoc, (uint32_T)i);
  LL_GPIO_ResetOutputPin(portNameLoc, ~(uint32_T)i & 128U);

  /* End of MATLABSystem: '<S38>/Digital Port Write' */

  /* MATLABSystem: '<S40>/PWM Output' */
  C3_DC = checkFrequencyAndDutyCycleLimits
    (MicroMouseTemplate_DW.obj_n.TimerHandle, 0U);
  setDutyCycleChannel3(MicroMouseTemplate_DW.obj_n.TimerHandle, C3_DC);
  C3_DC = checkFrequencyAndDutyCycleLimits
    (MicroMouseTemplate_DW.obj_n.TimerHandle, 0U);
  setDutyCycleChannel4(MicroMouseTemplate_DW.obj_n.TimerHandle, C3_DC);

  /* MATLABSystem: '<S42>/PWM Output' */
  C3_DC = checkFrequencyAndDutyCycleLimits
    (MicroMouseTemplate_DW.obj_g.TimerHandle, 0U);
  setDutyCycleChannel1(MicroMouseTemplate_DW.obj_g.TimerHandle, C3_DC);
  C3_DC = checkFrequencyAndDutyCycleLimits
    (MicroMouseTemplate_DW.obj_g.TimerHandle, 0U);
  setDutyCycleChannel2(MicroMouseTemplate_DW.obj_g.TimerHandle, C3_DC);

  /* MATLABSystem: '<S57>/Digital Port Write' */
  portNameLoc = GPIOC;
  LL_GPIO_SetOutputPin(portNameLoc, 0U);
  LL_GPIO_ResetOutputPin(portNameLoc, 8192U);

  /* MATLABSystem: '<S59>/Digital Port Write' */
  portNameLoc = GPIOC;
  LL_GPIO_SetOutputPin(portNameLoc, 0U);
  LL_GPIO_ResetOutputPin(portNameLoc, 16384U);

  /* MATLABSystem: '<S61>/Digital Port Write' */
  portNameLoc = GPIOC;
  LL_GPIO_SetOutputPin(portNameLoc, 0U);
  LL_GPIO_ResetOutputPin(portNameLoc, 32768U);

  /* MATLABSystem: '<S55>/Digital Port Write' incorporates:
   *  Constant: '<S8>/Constant'
   */
  portNameLoc = GPIOB;
  if (MicroMouseTemplate_P.Constant_Value_i != 0.0) {
    i = 8;
  } else {
    i = 0;
  }

  LL_GPIO_SetOutputPin(portNameLoc, (uint32_T)i);
  LL_GPIO_ResetOutputPin(portNameLoc, ~(uint32_T)i & 8U);

  /* End of MATLABSystem: '<S55>/Digital Port Write' */

  /* MATLABSystem: '<S65>/Digital Port Read' */
  portNameLoc = GPIOE;
  LL_GPIO_ReadInputPort(portNameLoc);

  /* MATLABSystem: '<S67>/Digital Port Read' */
  portNameLoc = GPIOB;
  LL_GPIO_ReadInputPort(portNameLoc);

  /* user code (Update function Body) */

  /* System '<Root>' */
  refreshIMUValues();

  /* System '<Root>' */
  CustomWhile();
}

/* Model initialize function */
void MicroMouseTemplate_initialize(void)
{
  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      /* Start for DataStoreMemory: '<S1>/Data Store Memory' */
      Detections[i] = MicroMouseTemplate_P.DataStoreMemory_InitialValue_p4;

      /* Start for DataStoreMemory: '<S1>/Data Store Memory1' */
      Thresholds[i] = MicroMouseTemplate_P.DataStoreMemory1_InitialValue_j;
    }

    /* Start for DataStoreMemory: '<S1>/Data Store Memory2' */
    currTicksRS = MicroMouseTemplate_P.DataStoreMemory2_InitialValue;

    /* Start for DataStoreMemory: '<S1>/Data Store Memory4' */
    currTicksLS = MicroMouseTemplate_P.DataStoreMemory4_InitialValue;
    for (i = 0; i < 9; i++) {
      /* Start for DataStoreMemory: '<S6>/Data Store Memory' */
      ADC1s[i] = MicroMouseTemplate_P.DataStoreMemory_InitialValue_p;

      /* Start for DataStoreMemory: '<S6>/Data Store Memory1' */
      ADC_H[i] = MicroMouseTemplate_P.DataStoreMemory1_InitialValue_p;

      /* Start for DataStoreMemory: '<S6>/Data Store Memory2' */
      ADC_L[i] = MicroMouseTemplate_P.DataStoreMemory2_InitialValue_p;
    }

    /* Start for DataStoreMemory: '<S7>/Data Store Memory' */
    IMU_Accel[0] = MicroMouseTemplate_P.DataStoreMemory_InitialValue;

    /* Start for DataStoreMemory: '<S7>/Data Store Memory1' */
    IMU_Gyro[0] = MicroMouseTemplate_P.DataStoreMemory1_InitialValue;

    /* Start for DataStoreMemory: '<S7>/Data Store Memory' */
    IMU_Accel[1] = MicroMouseTemplate_P.DataStoreMemory_InitialValue;

    /* Start for DataStoreMemory: '<S7>/Data Store Memory1' */
    IMU_Gyro[1] = MicroMouseTemplate_P.DataStoreMemory1_InitialValue;

    /* Start for DataStoreMemory: '<S7>/Data Store Memory' */
    IMU_Accel[2] = MicroMouseTemplate_P.DataStoreMemory_InitialValue;

    /* Start for DataStoreMemory: '<S7>/Data Store Memory1' */
    IMU_Gyro[2] = MicroMouseTemplate_P.DataStoreMemory1_InitialValue;

    /* user code (Initialize function Body) */

    /* System '<Root>' */
    initIMU();

    /* Start for MATLABSystem: '<S46>/Analog to Digital Converter' */
    MicroMouseTemplate_DW.obj.isInitialized = 0;
    MicroMouseTemplate_DW.obj.matlabCodegenIsDeleted = false;
    MicroMouseT_SystemCore_setup_dx(&MicroMouseTemplate_DW.obj);
    MicroMouseTe_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem2_ci,
      &MicroMouseTemplate_P.MATLABSystem2_ci);
    MicroMouseTe_MATLABSystem3_Init(&MicroMouseTemplate_DW.MATLABSystem4,
      &MicroMouseTemplate_P.MATLABSystem4);
    MicroMouseTe_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem1,
      &MicroMouseTemplate_P.MATLABSystem1);
    MicroMouseTe_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem1_ci,
      &MicroMouseTemplate_P.MATLABSystem1_ci);
    MicroMouseTe_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem3_c,
      &MicroMouseTemplate_P.MATLABSystem3_c);
    MicroMouseTe_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem4_c,
      &MicroMouseTemplate_P.MATLABSystem4_c);
    MicroMouseTe_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem5,
      &MicroMouseTemplate_P.MATLABSystem5);
    MicroMouseTe_MATLABSystem3_Init(&MicroMouseTemplate_DW.MATLABSystem3,
      &MicroMouseTemplate_P.MATLABSystem3);
    MicroMouseTe_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem2,
      &MicroMouseTemplate_P.MATLABSystem2);
    MicroMouseTe_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem6,
      &MicroMouseTemplate_P.MATLABSystem6);
    MicroMouseTe_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem1_c,
      &MicroMouseTemplate_P.MATLABSystem1_c);
    MicroMouseTe_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem2_c,
      &MicroMouseTemplate_P.MATLABSystem2_c);

    /* Start for MATLABSystem: '<S40>/PWM Output' */
    MicroMouseTemplate_DW.obj_n.isInitialized = 0;
    MicroMouseTemplate_DW.obj_n.matlabCodegenIsDeleted = false;
    MicroMouseTemp_SystemCore_setup(&MicroMouseTemplate_DW.obj_n);

    /* Start for MATLABSystem: '<S42>/PWM Output' */
    MicroMouseTemplate_DW.obj_g.isInitialized = 0;
    MicroMouseTemplate_DW.obj_g.matlabCodegenIsDeleted = false;
    MicroMouseTe_SystemCore_setup_d(&MicroMouseTemplate_DW.obj_g);
  }
}

/* Model terminate function */
void MicroMouseTemplate_terminate(void)
{
  uint16_T mode;
  uint8_T ChannelInfo;

  /* Terminate for MATLABSystem: '<S46>/Analog to Digital Converter' */
  if (!MicroMouseTemplate_DW.obj.matlabCodegenIsDeleted) {
    MicroMouseTemplate_DW.obj.matlabCodegenIsDeleted = true;
    if ((MicroMouseTemplate_DW.obj.isInitialized == 1) &&
        MicroMouseTemplate_DW.obj.isSetupComplete) {
      mode = ADC_DMA_INTERRUPT_MODE;
      ADC_Handle_Deinit(MicroMouseTemplate_DW.obj.ADCHandle, mode, 1);
    }
  }

  /* End of Terminate for MATLABSystem: '<S46>/Analog to Digital Converter' */

  /* Terminate for MATLABSystem: '<S40>/PWM Output' */
  if (!MicroMouseTemplate_DW.obj_n.matlabCodegenIsDeleted) {
    MicroMouseTemplate_DW.obj_n.matlabCodegenIsDeleted = true;
    if ((MicroMouseTemplate_DW.obj_n.isInitialized == 1) &&
        MicroMouseTemplate_DW.obj_n.isSetupComplete) {
      disableCounter(MicroMouseTemplate_DW.obj_n.TimerHandle);
      disableTimerInterrupts(MicroMouseTemplate_DW.obj_n.TimerHandle, 0);
      ChannelInfo = ENABLE_CH;
      disableTimerChannel3(MicroMouseTemplate_DW.obj_n.TimerHandle, ChannelInfo);
      ChannelInfo = ENABLE_CH;
      disableTimerChannel4(MicroMouseTemplate_DW.obj_n.TimerHandle, ChannelInfo);
    }
  }

  /* End of Terminate for MATLABSystem: '<S40>/PWM Output' */

  /* Terminate for MATLABSystem: '<S42>/PWM Output' */
  if (!MicroMouseTemplate_DW.obj_g.matlabCodegenIsDeleted) {
    MicroMouseTemplate_DW.obj_g.matlabCodegenIsDeleted = true;
    if ((MicroMouseTemplate_DW.obj_g.isInitialized == 1) &&
        MicroMouseTemplate_DW.obj_g.isSetupComplete) {
      disableCounter(MicroMouseTemplate_DW.obj_g.TimerHandle);
      disableTimerInterrupts(MicroMouseTemplate_DW.obj_g.TimerHandle, 0);
      ChannelInfo = ENABLE_CH;
      disableTimerChannel1(MicroMouseTemplate_DW.obj_g.TimerHandle, ChannelInfo);
      ChannelInfo = ENABLE_CH;
      disableTimerChannel2(MicroMouseTemplate_DW.obj_g.TimerHandle, ChannelInfo);
    }
  }

  /* End of Terminate for MATLABSystem: '<S42>/PWM Output' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
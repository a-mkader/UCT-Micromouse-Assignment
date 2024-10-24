/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MicroMouseTemplate.c
 *
 * Code generated for Simulink model 'MicroMouseTemplate'.
 *
 * Model version                  : 3.20
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Thu Oct 17 12:52:41 2024
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "MicroMouseTemplate.h"
#include <float.h>
#include <math.h>
#include "rtwtypes.h"
#include "MicroMouseTemplate_types.h"
#include "MicroMouseTemplate_private.h"
#include <string.h>
#include "stm_timer_ll.h"
#include "stm_adc_ll.h"

/* Named constants for Chart: '<Root>/HelloMicroMouse!' */
#define MicroMouseTe_IN_FollowBlackLine ((uint8_T)3U)
#define MicroMouseTe_IN_NO_ACTIVE_CHILD ((uint8_T)0U)
#define MicroMouseTempla_IN_Calibration ((uint8_T)2U)
#define MicroMouseTemplat_IN_Calibrated ((uint8_T)1U)
#define MicroMouseTemplat_IN_FourthSide ((uint8_T)3U)
#define MicroMouseTemplat_IN_SecondSide ((uint8_T)4U)
#define MicroMouseTemplate_IN_Average  ((uint8_T)1U)
#define MicroMouseTemplate_IN_FirstSide ((uint8_T)2U)
#define MicroMouseTemplate_IN_Forward  ((uint8_T)1U)
#define MicroMouseTemplate_IN_Stop     ((uint8_T)2U)
#define MicroMouseTemplate_IN_ThirdSide ((uint8_T)5U)
#define MicroMouseTemplate_IN_Turn1    ((uint8_T)6U)
#define MicroMouseTemplate_IN_Turn2    ((uint8_T)7U)
#define MicroMouseTemplate_IN_Turn3    ((uint8_T)8U)
#define MicroMouseTemplate_IN_TurnLeft ((uint8_T)3U)
#define MicroMouseTemplate_IN_TurnRight ((uint8_T)4U)
#define MicroMouseTemplate_IN_WadeLeft ((uint8_T)5U)
#define MicroMouseTemplate_IN_WadeRight ((uint8_T)6U)
#define Micro_IN_WaitUntilButtonPressed ((uint8_T)4U)

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
static void MicroMouseTempl_FollowBlackLine(const boolean_T *NOT);
static void MicroMouseTemp_SystemCore_setup(stm32cube_blocks_AnalogInput__T *obj);
static void MicroMous_PWMOutput_setupImpl_d(stm32cube_blocks_PWMOutput_Mi_T *obj);
static void MicroMouseT_PWMOutput_setupImpl(stm32cube_blocks_PWMOutput_Mi_T *obj);
static void rate_monotonic_scheduler(void);

/*
 * Time delay interpolation routine
 *
 * The linear interpolation is performed using the formula:
 *
 * (t2 - tMinusDelay)         (tMinusDelay - t1)
 * u(t)  =  ----------------- * u1  +  ------------------- * u2
 * (t2 - t1)                  (t2 - t1)
 */
real_T rt_TDelayInterpolate(
  real_T tMinusDelay,                  /* tMinusDelay = currentSimTime - delay */
  real_T tStart,
  real_T *uBuf,
  int_T bufSz,
  int_T *lastIdx,
  int_T oldestIdx,
  int_T newIdx,
  real_T initOutput,
  boolean_T discrete,
  boolean_T minorStepAndTAtLastMajorOutput)
{
  int_T i;
  real_T yout, t1, t2, u1, u2;
  real_T* tBuf = uBuf + bufSz;

  /*
   * If there is only one data point in the buffer, this data point must be
   * the t= 0 and tMinusDelay > t0, it ask for something unknown. The best
   * guess if initial output as well
   */
  if ((newIdx == 0) && (oldestIdx ==0 ) && (tMinusDelay > tStart))
    return initOutput;

  /*
   * If tMinusDelay is less than zero, should output initial value
   */
  if (tMinusDelay <= tStart)
    return initOutput;

  /* For fixed buffer extrapolation:
   * if tMinusDelay is small than the time at oldestIdx, if discrete, output
   * tailptr value,  else use tailptr and tailptr+1 value to extrapolate
   * It is also for fixed buffer. Note: The same condition can happen for transport delay block where
   * use tStart and and t[tail] other than using t[tail] and t[tail+1].
   * See below
   */
  if ((tMinusDelay <= tBuf[oldestIdx] ) ) {
    if (discrete) {
      return(uBuf[oldestIdx]);
    } else {
      int_T tempIdx= oldestIdx + 1;
      if (oldestIdx == bufSz-1)
        tempIdx = 0;
      t1= tBuf[oldestIdx];
      t2= tBuf[tempIdx];
      u1= uBuf[oldestIdx];
      u2= uBuf[tempIdx];
      if (t2 == t1) {
        if (tMinusDelay >= t2) {
          yout = u2;
        } else {
          yout = u1;
        }
      } else {
        real_T f1 = (t2-tMinusDelay) / (t2-t1);
        real_T f2 = 1.0 - f1;

        /*
         * Use Lagrange's interpolation formula.  Exact outputs at t1, t2.
         */
        yout = f1*u1 + f2*u2;
      }

      return yout;
    }
  }

  /*
   * When block does not have direct feedthrough, we use the table of
   * values to extrapolate off the end of the table for delays that are less
   * than 0 (less then step size).  This is not completely accurate.  The
   * chain of events is as follows for a given time t.  Major output - look
   * in table.  Update - add entry to table.  Now, if we call the output at
   * time t again, there is a new entry in the table. For very small delays,
   * this means that we will have a different answer from the previous call
   * to the output fcn at the same time t.  The following code prevents this
   * from happening.
   */
  if (minorStepAndTAtLastMajorOutput) {
    /* pretend that the new entry has not been added to table */
    if (newIdx != 0) {
      if (*lastIdx == newIdx) {
        (*lastIdx)--;
      }

      newIdx--;
    } else {
      if (*lastIdx == newIdx) {
        *lastIdx = bufSz-1;
      }

      newIdx = bufSz - 1;
    }
  }

  i = *lastIdx;
  if (tBuf[i] < tMinusDelay) {
    /* Look forward starting at last index */
    while (tBuf[i] < tMinusDelay) {
      /* May occur if the delay is less than step-size - extrapolate */
      if (i == newIdx)
        break;
      i = ( i < (bufSz-1) ) ? (i+1) : 0;/* move through buffer */
    }
  } else {
    /*
     * Look backwards starting at last index which can happen when the
     * delay time increases.
     */
    while (tBuf[i] >= tMinusDelay) {
      /*
       * Due to the entry condition at top of function, we
       * should never hit the end.
       */
      i = (i > 0) ? i-1 : (bufSz-1);   /* move through buffer */
    }

    i = ( i < (bufSz-1) ) ? (i+1) : 0;
  }

  *lastIdx = i;
  if (discrete) {
    /*
     * tempEps = 128 * eps;
     * localEps = max(tempEps, tempEps*fabs(tBuf[i]))/2;
     */
    double tempEps = (DBL_EPSILON) * 128.0;
    double localEps = tempEps * fabs(tBuf[i]);
    if (tempEps > localEps) {
      localEps = tempEps;
    }

    localEps = localEps / 2.0;
    if (tMinusDelay >= (tBuf[i] - localEps)) {
      yout = uBuf[i];
    } else {
      if (i == 0) {
        yout = uBuf[bufSz-1];
      } else {
        yout = uBuf[i-1];
      }
    }
  } else {
    if (i == 0) {
      t1 = tBuf[bufSz-1];
      u1 = uBuf[bufSz-1];
    } else {
      t1 = tBuf[i-1];
      u1 = uBuf[i-1];
    }

    t2 = tBuf[i];
    u2 = uBuf[i];
    if (t2 == t1) {
      if (tMinusDelay >= t2) {
        yout = u2;
      } else {
        yout = u1;
      }
    } else {
      real_T f1 = (t2-tMinusDelay) / (t2-t1);
      real_T f2 = 1.0 - f1;

      /*
       * Use Lagrange's interpolation formula.  Exact outputs at t1, t2.
       */
      yout = f1*u1 + f2*u2;
    }
  }

  return(yout);
}

/*
 * Set which subrates need to run this base step (base rate always runs).
 * This function must be called prior to calling the model step function
 * in order to remember which rates need to run this base step.  The
 * buffering of events allows for overlapping preemption.
 */
void MicroMouseTemplate_SetEventsForThisBaseStep(boolean_T *eventFlags)
{
  /* Task runs when its counter is zero, computed via rtmStepTask macro */
  eventFlags[2] = ((boolean_T)rtmStepTask(MicroMouseTemplate_M, 2));
}

/*
 *         This function updates active task flag for each subrate
 *         and rate transition flags for tasks that exchange data.
 *         The function assumes rate-monotonic multitasking scheduler.
 *         The function must be called at model base rate so that
 *         the generated code self-manages all its subrates and rate
 *         transition flags.
 */
static void rate_monotonic_scheduler(void)
{
  /* Compute which subrates run during the next base time step.  Subrates
   * are an integer multiple of the base rate counter.  Therefore, the subtask
   * counter is reset when it reaches its limit (zero means run).
   */
  (MicroMouseTemplate_M->Timing.TaskCounters.TID[2])++;
  if ((MicroMouseTemplate_M->Timing.TaskCounters.TID[2]) > 3) {/* Sample time: [0.1s, 0.0s] */
    MicroMouseTemplate_M->Timing.TaskCounters.TID[2] = 0;
  }
}

/* System initialize for atomic system: */
void MicroMou_MATLABSystem1_Init(DW_MATLABSystem1_MicroMouseTe_T *localDW,
  P_MATLABSystem1_MicroMouseTem_T *localP)
{
  int32_T i;

  /* Start for MATLABSystem: '<S52>/MATLAB System1' */
  localDW->objisempty = true;
  localDW->obj.InitialCondition = localP->MATLABSystem1_InitialCondition;
  localDW->obj.isInitialized = 1;

  /*  Number of inputs */
  /*  Initialize the buffer during the first call */
  for (i = 0; i < 25; i++) {
    localDW->obj.Buffer[i] = localDW->obj.InitialCondition;
  }

  /* End of Start for MATLABSystem: '<S52>/MATLAB System1' */
}

/* Output and update for atomic system: */
void MicroMouseTem_MATLABSystem1(real_T rtu_0, B_MATLABSystem1_MicroMouseTem_T
  *localB, DW_MATLABSystem1_MicroMouseTe_T *localDW,
  P_MATLABSystem1_MicroMouseTem_T *localP)
{
  /* MATLABSystem: '<S52>/MATLAB System1' */
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

  /* End of MATLABSystem: '<S52>/MATLAB System1' */
}

/* System initialize for atomic system: */
void MicroMou_MATLABSystem3_Init(DW_MATLABSystem3_MicroMouseTe_T *localDW,
  P_MATLABSystem3_MicroMouseTem_T *localP)
{
  int32_T i;

  /* Start for MATLABSystem: '<S52>/MATLAB System3' */
  localDW->objisempty = true;
  localDW->obj.InitialCondition = localP->MATLABSystem3_InitialCondition;
  localDW->obj.isInitialized = 1;

  /*  Number of inputs */
  /*  Initialize the buffer during the first call */
  for (i = 0; i < 10; i++) {
    localDW->obj.Buffer[i] = localDW->obj.InitialCondition;
  }

  /* End of Start for MATLABSystem: '<S52>/MATLAB System3' */
}

/* Output and update for atomic system: */
void MicroMouseTem_MATLABSystem3(real_T rtu_0, B_MATLABSystem3_MicroMouseTem_T
  *localB, DW_MATLABSystem3_MicroMouseTe_T *localDW,
  P_MATLABSystem3_MicroMouseTem_T *localP)
{
  /* MATLABSystem: '<S52>/MATLAB System3' */
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

  /* End of MATLABSystem: '<S52>/MATLAB System3' */
}

real_T rt_roundd(real_T u)
{
  real_T y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

/* Function for Chart: '<Root>/HelloMicroMouse!' */
static void MicroMouseTempl_FollowBlackLine(const boolean_T *NOT)
{
  int32_T tmp;
  uint32_T tmp_0;
  uint32_T tmp_1;
  boolean_T guard1;
  boolean_T guard2;
  if (*NOT) {
    switch (MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine) {
     case MicroMouseTemplate_IN_Forward:
      MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
        MicroMouseTe_IN_NO_ACTIVE_CHILD;
      break;

     case MicroMouseTemplate_IN_Stop:
      MicroMouseTemplate_B.LED0 = false;
      MicroMouseTemplate_B.LED2 = false;
      MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
        MicroMouseTe_IN_NO_ACTIVE_CHILD;
      break;

     case MicroMouseTemplate_IN_TurnLeft:
      MicroMouseTemplate_B.LED0 = false;
      MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
        MicroMouseTe_IN_NO_ACTIVE_CHILD;
      break;

     case MicroMouseTemplate_IN_TurnRight:
      MicroMouseTemplate_B.LED2 = false;
      MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
        MicroMouseTe_IN_NO_ACTIVE_CHILD;
      break;

     case MicroMouseTemplate_IN_WadeLeft:
      MicroMouseTemplate_B.LED0 = false;
      MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
        MicroMouseTe_IN_NO_ACTIVE_CHILD;
      break;

     case MicroMouseTemplate_IN_WadeRight:
      MicroMouseTemplate_B.LED2 = false;
      MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
        MicroMouseTe_IN_NO_ACTIVE_CHILD;
      break;
    }

    MicroMouseTemplate_B.rightWheel_m = 50;
    MicroMouseTemplate_B.leftWheel_ds = 50;
    MicroMouseTemplate_DW.bitsForTID2.is_c2_MicroMouseTemplate =
      Micro_IN_WaitUntilButtonPressed;
  } else {
    guard1 = false;
    guard2 = false;
    switch (MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine) {
     case MicroMouseTemplate_IN_Forward:
      tmp_1 = MicroMouseTemplate_DW.DownLSThresh + 200U;
      if (MicroMouseTemplate_DW.DownLSThresh + 200U > 65535U) {
        tmp_1 = 65535U;
      }

      if (MicroMouseTemplate_B.Flip[5] > (int32_T)tmp_1) {
        MicroMouseTemplate_B.rightWheel_m = 90;
        MicroMouseTemplate_B.leftWheel_ds = 90;
        MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
          MicroMouseTemplate_IN_WadeRight;
      } else {
        tmp_1 = MicroMouseTemplate_DW.MOTRSThresh + 200U;
        if (MicroMouseTemplate_DW.MOTRSThresh + 200U > 65535U) {
          tmp_1 = 65535U;
        }

        tmp_0 = MicroMouseTemplate_DW.MOTLSThresh + 200U;
        if (MicroMouseTemplate_DW.MOTLSThresh + 200U > 65535U) {
          tmp_0 = 65535U;
        }

        if ((MicroMouseTemplate_B.Flip[0] < (int32_T)tmp_1) ||
            (MicroMouseTemplate_B.Flip[7] < (int32_T)tmp_0)) {
          MicroMouseTemplate_B.rightWheel_m = 90;
          MicroMouseTemplate_B.leftWheel_ds = 90;
          MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
            MicroMouseTemplate_IN_Stop;
        } else {
          tmp_1 = MicroMouseTemplate_DW.DownRSThresh + 200U;
          if (MicroMouseTemplate_DW.DownRSThresh + 200U > 65535U) {
            tmp_1 = 65535U;
          }

          if (MicroMouseTemplate_B.Flip[2] > (int32_T)tmp_1) {
            MicroMouseTemplate_B.rightWheel_m = 90;
            MicroMouseTemplate_B.leftWheel_ds = 90;
            MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
              MicroMouseTemplate_IN_WadeLeft;
          } else {
            MicroMouseTemplate_B.rightWheel_m = 90;
            MicroMouseTemplate_B.leftWheel_ds = 90;
          }
        }
      }
      break;

     case MicroMouseTemplate_IN_Stop:
      if (MicroMouseTemplate_B.Flip[4] < MicroMouseTemplate_DW.LSThresh) {
        MicroMouseTemplate_B.LED0 = false;
        MicroMouseTemplate_B.LED2 = false;
        MicroMouseTemplate_DW.temporalCounter_i1 = 0U;
        MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
          MicroMouseTemplate_IN_TurnLeft;
      } else if ((MicroMouseTemplate_B.Flip[1] <
                  MicroMouseTemplate_DW.FWDRSThresh) ||
                 (MicroMouseTemplate_B.Flip[6] <
                  MicroMouseTemplate_DW.FWDLSThresh)) {
        MicroMouseTemplate_B.LED0 = false;
        MicroMouseTemplate_B.LED2 = false;
        MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
          MicroMouseTemplate_IN_Forward;
      } else if (MicroMouseTemplate_B.Flip[3] < MicroMouseTemplate_DW.RSThresh)
      {
        MicroMouseTemplate_B.LED0 = false;
        MicroMouseTemplate_B.LED2 = false;
        MicroMouseTemplate_DW.temporalCounter_i1 = 0U;
        MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
          MicroMouseTemplate_IN_TurnRight;
      } else {
        MicroMouseTemplate_B.LED0 = true;
        MicroMouseTemplate_B.LED2 = true;
        MicroMouseTemplate_B.rightWheel_m = 40;
        MicroMouseTemplate_B.leftWheel_ds = 40;
      }
      break;

     case MicroMouseTemplate_IN_TurnLeft:
      if (MicroMouseTemplate_DW.temporalCounter_i1 >= 6U) {
        if ((MicroMouseTemplate_B.Flip[1] < MicroMouseTemplate_DW.FWDRSThresh) ||
            (MicroMouseTemplate_B.Flip[6] < MicroMouseTemplate_DW.FWDLSThresh))
        {
          MicroMouseTemplate_B.LED0 = false;
          MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
            MicroMouseTemplate_IN_Forward;
        } else {
          tmp_1 = MicroMouseTemplate_DW.FWDRSThresh + 200U;
          if (MicroMouseTemplate_DW.FWDRSThresh + 200U > 65535U) {
            tmp_1 = 65535U;
          }

          tmp_0 = MicroMouseTemplate_DW.FWDLSThresh + 200U;
          if (MicroMouseTemplate_DW.FWDLSThresh + 200U > 65535U) {
            tmp_0 = 65535U;
          }

          if ((MicroMouseTemplate_B.Flip[1] > (int32_T)tmp_1) &&
              (MicroMouseTemplate_B.Flip[6] < (int32_T)tmp_0)) {
            MicroMouseTemplate_B.LED0 = false;
            MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
              MicroMouseTemplate_IN_Stop;
          } else {
            guard1 = true;
          }
        }
      } else {
        guard1 = true;
      }
      break;

     case MicroMouseTemplate_IN_TurnRight:
      if (MicroMouseTemplate_DW.temporalCounter_i1 >= 6U) {
        if ((MicroMouseTemplate_B.Flip[1] < MicroMouseTemplate_DW.FWDRSThresh) ||
            (MicroMouseTemplate_B.Flip[6] < MicroMouseTemplate_DW.FWDLSThresh))
        {
          MicroMouseTemplate_B.LED2 = false;
          MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
            MicroMouseTemplate_IN_Forward;
        } else {
          tmp_1 = MicroMouseTemplate_DW.FWDRSThresh + 200U;
          if (MicroMouseTemplate_DW.FWDRSThresh + 200U > 65535U) {
            tmp_1 = 65535U;
          }

          tmp_0 = MicroMouseTemplate_DW.FWDLSThresh + 200U;
          if (MicroMouseTemplate_DW.FWDLSThresh + 200U > 65535U) {
            tmp_0 = 65535U;
          }

          if ((MicroMouseTemplate_B.Flip[1] > (int32_T)tmp_1) &&
              (MicroMouseTemplate_B.Flip[6] < (int32_T)tmp_0)) {
            MicroMouseTemplate_B.LED2 = false;
            MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
              MicroMouseTemplate_IN_Stop;
          } else {
            guard2 = true;
          }
        }
      } else {
        guard2 = true;
      }
      break;

     case MicroMouseTemplate_IN_WadeLeft:
      if ((MicroMouseTemplate_B.Flip[2] < MicroMouseTemplate_DW.DownRSThresh) &&
          (MicroMouseTemplate_B.Flip[5] < MicroMouseTemplate_DW.DownLSThresh)) {
        MicroMouseTemplate_B.LED0 = false;
        MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
          MicroMouseTemplate_IN_Forward;
      } else if (MicroMouseTemplate_B.Flip[5] >
                 MicroMouseTemplate_DW.DownLSThresh) {
        MicroMouseTemplate_B.LED0 = false;
        MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
          MicroMouseTemplate_IN_WadeRight;
      } else {
        MicroMouseTemplate_B.LED0 = true;
        tmp = MicroMouseTemplate_B.rightWheel_m + 2;
        if (MicroMouseTemplate_B.rightWheel_m + 2 > 127) {
          tmp = 127;
        }

        MicroMouseTemplate_B.rightWheel_m = (int8_T)tmp;
        tmp = MicroMouseTemplate_B.leftWheel_ds + 3;
        if (MicroMouseTemplate_B.leftWheel_ds + 3 > 127) {
          tmp = 127;
        }

        MicroMouseTemplate_B.leftWheel_ds = (int8_T)tmp;
      }
      break;

     default:
      /* case IN_WadeRight: */
      if ((MicroMouseTemplate_B.Flip[5] < MicroMouseTemplate_DW.DownLSThresh) &&
          (MicroMouseTemplate_B.Flip[2] < MicroMouseTemplate_DW.DownRSThresh)) {
        MicroMouseTemplate_B.LED2 = false;
        MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
          MicroMouseTemplate_IN_Forward;
      } else if (MicroMouseTemplate_B.Flip[2] >
                 MicroMouseTemplate_DW.DownRSThresh) {
        MicroMouseTemplate_B.LED2 = false;
        MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
          MicroMouseTemplate_IN_WadeLeft;
      } else {
        MicroMouseTemplate_B.LED2 = true;
        tmp = MicroMouseTemplate_B.rightWheel_m + 3;
        if (MicroMouseTemplate_B.rightWheel_m + 3 > 127) {
          tmp = 127;
        }

        MicroMouseTemplate_B.rightWheel_m = (int8_T)tmp;
        tmp = MicroMouseTemplate_B.leftWheel_ds + 2;
        if (MicroMouseTemplate_B.leftWheel_ds + 2 > 127) {
          tmp = 127;
        }

        MicroMouseTemplate_B.leftWheel_ds = (int8_T)tmp;
      }
      break;
    }

    if (guard2) {
      MicroMouseTemplate_B.rightWheel_m = -85;
      MicroMouseTemplate_B.leftWheel_ds = 85;
      MicroMouseTemplate_B.LED2 = !MicroMouseTemplate_B.LED2;
    }

    if (guard1) {
      MicroMouseTemplate_B.rightWheel_m = 85;
      MicroMouseTemplate_B.leftWheel_ds = -85;
      MicroMouseTemplate_B.LED0 = !MicroMouseTemplate_B.LED0;
    }
  }
}

static void MicroMouseTemp_SystemCore_setup(stm32cube_blocks_AnalogInput__T *obj)
{
  ADC_Type_T adcStructLoc;
  obj->isSetupComplete = false;

  /* Start for MATLABSystem: '<S51>/Analog to Digital Converter' */
  obj->isInitialized = 1;
  adcStructLoc.InternalBufferPtr = (void*)(&obj->ADCInternalBuffer[0]);
  adcStructLoc.InjectedNoOfConversion = 0U;
  adcStructLoc.peripheralPtr = ADC2;
  adcStructLoc.dmaPeripheralPtr = DMA1;
  adcStructLoc.dmastream = LL_DMA_CHANNEL_2;
  adcStructLoc.DataTransferMode = ADC_DR_TRANSFER;
  adcStructLoc.DmaTransferMode = ADC_DMA_TRANSFER_UNLIMITED;
  adcStructLoc.InternalBufferSize = 9U;
  adcStructLoc.RegularNoOfConversion = 9U;
  obj->ADCHandle = ADC_Handle_Init(&adcStructLoc, ADC_DMA_INTERRUPT_MODE, 1,
    ADC_TRIGGER_AND_READ, LL_ADC_REG_SEQ_SCAN_ENABLE_9RANKS);
  enableADCAutomaticCalibrationOffset(obj->ADCHandle, 2);
  enableADC(obj->ADCHandle);
  startADCConversionForExternalTrigger(obj->ADCHandle, 1);
  obj->isSetupComplete = true;
}

static void MicroMous_PWMOutput_setupImpl_d(stm32cube_blocks_PWMOutput_Mi_T *obj)
{
  TIM_Type_T b;
  boolean_T isSlaveModeTriggerEnabled;

  /* Start for MATLABSystem: '<S47>/PWM Output' */
  b.PeripheralPtr = TIM3;
  b.isCenterAlignedMode = false;

  /* Start for MATLABSystem: '<S47>/PWM Output' */
  b.repetitionCounter = 0U;
  obj->TimerHandle = Timer_Handle_Init(&b);
  enableTimerInterrupts(obj->TimerHandle, 0);
  enableTimerChannel1(obj->TimerHandle, ENABLE_CH);
  enableTimerChannel2(obj->TimerHandle, ENABLE_CH);
  isSlaveModeTriggerEnabled = isSlaveTriggerModeEnabled(obj->TimerHandle);
  if (!isSlaveModeTriggerEnabled) {
    /* Start for MATLABSystem: '<S47>/PWM Output' */
    enableCounter(obj->TimerHandle, false);
  }
}

static void MicroMouseT_PWMOutput_setupImpl(stm32cube_blocks_PWMOutput_Mi_T *obj)
{
  TIM_Type_T b;
  boolean_T isSlaveModeTriggerEnabled;

  /* Start for MATLABSystem: '<S45>/PWM Output' */
  b.PeripheralPtr = TIM3;
  b.isCenterAlignedMode = false;

  /* Start for MATLABSystem: '<S45>/PWM Output' */
  b.repetitionCounter = 0U;
  obj->TimerHandle = Timer_Handle_Init(&b);
  enableTimerInterrupts(obj->TimerHandle, 0);
  enableTimerChannel3(obj->TimerHandle, ENABLE_CH);
  enableTimerChannel4(obj->TimerHandle, ENABLE_CH);
  isSlaveModeTriggerEnabled = isSlaveTriggerModeEnabled(obj->TimerHandle);
  if (!isSlaveModeTriggerEnabled) {
    /* Start for MATLABSystem: '<S45>/PWM Output' */
    enableCounter(obj->TimerHandle, false);
  }
}

/* Model step function for TID0 */
void MicroMouseTemplate_step0(void)    /* Sample time: [0.0s, 0.0s] */
{
  /* local block i/o variables */
  real_T rtb_TransportDelay1;
  GPIO_TypeDef * portNameLoc;
  int32_T i;

  {                                    /* Sample time: [0.0s, 0.0s] */
    rate_monotonic_scheduler();
  }

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

  /* TransportDelay: '<S4>/Transport Delay1' */
  {
    real_T **uBuffer = (real_T**)
      &MicroMouseTemplate_DW.TransportDelay1_PWORK.TUbufferPtrs[0];
    real_T simTime = MicroMouseTemplate_M->Timing.t[0];
    real_T tMinusDelay = simTime - (0.5 * MicroMouseTemplate_P.IR_LED_PERIOD);
    rtb_TransportDelay1 = rt_TDelayInterpolate(
      tMinusDelay,
      0.0,
      *uBuffer,
      MicroMouseTemplate_DW.TransportDelay1_IWORK.CircularBufSize,
      &MicroMouseTemplate_DW.TransportDelay1_IWORK.Last,
      MicroMouseTemplate_DW.TransportDelay1_IWORK.Tail,
      MicroMouseTemplate_DW.TransportDelay1_IWORK.Head,
      MicroMouseTemplate_P.TransportDelay1_InitOutput,
      1,
      0);
  }

  /* Sum: '<S34>/Sum' incorporates:
   *  Constant: '<S34>/Constant'
   *  UnitDelay: '<S34>/Unit Delay'
   */
  MicroMouseTemplate_B.Sum = MicroMouseTemplate_P.Constant_Value_i -
    MicroMouseTemplate_DW.UnitDelay_DSTATE;

  /* Outputs for Atomic SubSystem: '<Root>/GPIO for IR LEDs' */
  /* MATLABSystem: '<S19>/Digital Port Write' incorporates:
   *  Constant: '<S4>/Constant'
   */
  portNameLoc = GPIOE;
  if (MicroMouseTemplate_P.Constant_Value != 0.0) {
    i = 512;
  } else {
    i = 0;
  }

  LL_GPIO_SetOutputPin(portNameLoc, (uint32_T)i);
  LL_GPIO_ResetOutputPin(portNameLoc, ~(uint32_T)i & 512U);

  /* End of MATLABSystem: '<S19>/Digital Port Write' */

  /* MATLABSystem: '<S21>/Digital Port Write' incorporates:
   *  Constant: '<S4>/Constant'
   */
  portNameLoc = GPIOE;
  if (MicroMouseTemplate_P.Constant_Value != 0.0) {
    i = 16384;
  } else {
    i = 0;
  }

  LL_GPIO_SetOutputPin(portNameLoc, (uint32_T)i);
  LL_GPIO_ResetOutputPin(portNameLoc, ~(uint32_T)i & 16384U);

  /* End of MATLABSystem: '<S21>/Digital Port Write' */

  /* MATLABSystem: '<S23>/Digital Port Write' */
  portNameLoc = GPIOE;
  if (rtb_TransportDelay1 != 0.0) {
    i = 256;
  } else {
    i = 0;
  }

  LL_GPIO_SetOutputPin(portNameLoc, (uint32_T)i);
  LL_GPIO_ResetOutputPin(portNameLoc, ~(uint32_T)i & 256U);

  /* End of MATLABSystem: '<S23>/Digital Port Write' */

  /* MATLABSystem: '<S25>/Digital Port Write' */
  portNameLoc = GPIOE;
  if (rtb_TransportDelay1 != 0.0) {
    i = 32768;
  } else {
    i = 0;
  }

  LL_GPIO_SetOutputPin(portNameLoc, (uint32_T)i);
  LL_GPIO_ResetOutputPin(portNameLoc, ~(uint32_T)i & 32768U);

  /* End of MATLABSystem: '<S25>/Digital Port Write' */

  /* MATLABSystem: '<S27>/Digital Port Write' */
  portNameLoc = GPIOE;
  if (MicroMouseTemplate_B.Sum != 0.0) {
    i = 4096;
  } else {
    i = 0;
  }

  LL_GPIO_SetOutputPin(portNameLoc, (uint32_T)i);
  LL_GPIO_ResetOutputPin(portNameLoc, ~(uint32_T)i & 4096U);

  /* End of MATLABSystem: '<S27>/Digital Port Write' */

  /* MATLABSystem: '<S29>/Digital Port Write' incorporates:
   *  Constant: '<S4>/Constant'
   */
  portNameLoc = GPIOB;
  if (MicroMouseTemplate_P.Constant_Value != 0.0) {
    i = 4096;
  } else {
    i = 0;
  }

  LL_GPIO_SetOutputPin(portNameLoc, (uint32_T)i);
  LL_GPIO_ResetOutputPin(portNameLoc, ~(uint32_T)i & 4096U);

  /* End of MATLABSystem: '<S29>/Digital Port Write' */

  /* MATLABSystem: '<S31>/Digital Port Write' incorporates:
   *  Constant: '<S4>/Constant'
   */
  portNameLoc = GPIOE;
  if (MicroMouseTemplate_P.Constant_Value != 0.0) {
    i = 8192;
  } else {
    i = 0;
  }

  LL_GPIO_SetOutputPin(portNameLoc, (uint32_T)i);
  LL_GPIO_ResetOutputPin(portNameLoc, ~(uint32_T)i & 8192U);

  /* End of MATLABSystem: '<S31>/Digital Port Write' */

  /* MATLABSystem: '<S33>/Digital Port Write' */
  portNameLoc = GPIOE;
  if (MicroMouseTemplate_B.Sum != 0.0) {
    i = 2048;
  } else {
    i = 0;
  }

  LL_GPIO_SetOutputPin(portNameLoc, (uint32_T)i);
  LL_GPIO_ResetOutputPin(portNameLoc, ~(uint32_T)i & 2048U);

  /* End of MATLABSystem: '<S33>/Digital Port Write' */
  /* End of Outputs for SubSystem: '<Root>/GPIO for IR LEDs' */

  /* MATLABSystem: '<S43>/Digital Port Write' incorporates:
   *  Constant: '<S5>/Constant'
   */
  portNameLoc = GPIOD;
  if (MicroMouseTemplate_P.Constant_Value_l != 0.0) {
    i = 128;
  } else {
    i = 0;
  }

  LL_GPIO_SetOutputPin(portNameLoc, (uint32_T)i);
  LL_GPIO_ResetOutputPin(portNameLoc, ~(uint32_T)i & 128U);

  /* End of MATLABSystem: '<S43>/Digital Port Write' */

  /* MATLABSystem: '<S60>/Digital Port Write' incorporates:
   *  Constant: '<S8>/Constant'
   */
  portNameLoc = GPIOB;
  if (MicroMouseTemplate_P.Constant_Value_in != 0.0) {
    i = 8;
  } else {
    i = 0;
  }

  LL_GPIO_SetOutputPin(portNameLoc, (uint32_T)i);
  LL_GPIO_ResetOutputPin(portNameLoc, ~(uint32_T)i & 8U);

  /* End of MATLABSystem: '<S60>/Digital Port Write' */

  /* user code (Update function Body for TID0) */

  /* System '<Root>' */
  refreshIMUValues();

  /* System '<Root>' */
  CustomWhile();

  /* Update for TransportDelay: '<S4>/Transport Delay1' */
  {
    real_T **uBuffer = (real_T**)
      &MicroMouseTemplate_DW.TransportDelay1_PWORK.TUbufferPtrs[0];
    real_T simTime = MicroMouseTemplate_M->Timing.t[0];
    MicroMouseTemplate_DW.TransportDelay1_IWORK.Head =
      ((MicroMouseTemplate_DW.TransportDelay1_IWORK.Head <
        (MicroMouseTemplate_DW.TransportDelay1_IWORK.CircularBufSize-1)) ?
       (MicroMouseTemplate_DW.TransportDelay1_IWORK.Head+1) : 0);
    if (MicroMouseTemplate_DW.TransportDelay1_IWORK.Head ==
        MicroMouseTemplate_DW.TransportDelay1_IWORK.Tail) {
      MicroMouseTemplate_DW.TransportDelay1_IWORK.Tail =
        ((MicroMouseTemplate_DW.TransportDelay1_IWORK.Tail <
          (MicroMouseTemplate_DW.TransportDelay1_IWORK.CircularBufSize-1)) ?
         (MicroMouseTemplate_DW.TransportDelay1_IWORK.Tail+1) : 0);
    }

    (*uBuffer + MicroMouseTemplate_DW.TransportDelay1_IWORK.CircularBufSize)
      [MicroMouseTemplate_DW.TransportDelay1_IWORK.Head] = simTime;
    (*uBuffer)[MicroMouseTemplate_DW.TransportDelay1_IWORK.Head] =
      MicroMouseTemplate_B.Sum;
  }

  /* Update for UnitDelay: '<S34>/Unit Delay' */
  MicroMouseTemplate_DW.UnitDelay_DSTATE = MicroMouseTemplate_B.Sum;

  /* Update absolute time */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  MicroMouseTemplate_M->Timing.t[0] =
    ((time_T)(++MicroMouseTemplate_M->Timing.clockTick0)) *
    MicroMouseTemplate_M->Timing.stepSize0;

  /* Update absolute time */
  /* The "clockTick1" counts the number of times the code of this task has
   * been executed. The resolution of this integer timer is 0.025, which is the step size
   * of the task. Size of "clockTick1" ensures timer will not overflow during the
   * application lifespan selected.
   */
  MicroMouseTemplate_M->Timing.clockTick1++;
}

/* Model step function for TID2 */
void MicroMouseTemplate_step2(void)    /* Sample time: [0.1s, 0.0s] */
{
  real_T maxV;
  real_T maxV_0;
  real_T maxV_1;
  real_T minV;
  real_T u1;
  int32_T i;
  uint32_T pinReadLoc;
  int8_T rtPrevAction;
  boolean_T NOT;
  boolean_T NOT1;

  /* MATLABSystem: '<S51>/Analog to Digital Converter' */
  regularReadADCDMA(MicroMouseTemplate_DW.obj.ADCHandle, ADC_TRIGGER_AND_READ,
                    &ADC1s[0]);

  /* DSPFlip: '<S6>/Flip' incorporates:
   *  MATLABSystem: '<S51>/Analog to Digital Converter'
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
    /* DataTypeConversion: '<S54>/Cast To Double' */
    MicroMouseTemplate_B.rtb_CastToDouble_m[i] = MicroMouseTemplate_B.Flip[i];
  }

  MicroMouseTem_MATLABSystem1((real_T)MicroMouseTemplate_B.rtb_CastToDouble_m[0],
    &MicroMouseTemplate_B.MATLABSystem2_ci,
    &MicroMouseTemplate_DW.MATLABSystem2_ci,
    &MicroMouseTemplate_P.MATLABSystem2_ci);

  /* MinMax: '<S52>/Max of Elements' incorporates:
   *  MATLABSystem: '<S54>/MATLAB System2'
   */
  MicroMouseTemplate_B.maxV =
    MicroMouseTemplate_B.MATLABSystem2_ci.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem2_ci.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.maxV < u1) {
      MicroMouseTemplate_B.maxV = u1;
    }
  }

  MicroMouseTem_MATLABSystem3((real_T)MicroMouseTemplate_B.rtb_CastToDouble_m[1],
    &MicroMouseTemplate_B.MATLABSystem4, &MicroMouseTemplate_DW.MATLABSystem4,
    &MicroMouseTemplate_P.MATLABSystem4);

  /* MinMax: '<S52>/Max of Elements1' incorporates:
   *  MATLABSystem: '<S52>/MATLAB System4'
   */
  MicroMouseTemplate_B.maxV_c =
    MicroMouseTemplate_B.MATLABSystem4.MATLABSystem3[0];
  for (i = 0; i < 9; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem4.MATLABSystem3[i + 1];
    if (MicroMouseTemplate_B.maxV_c < u1) {
      MicroMouseTemplate_B.maxV_c = u1;
    }
  }

  MicroMouseTem_MATLABSystem1(MicroMouseTemplate_B.maxV_c,
    &MicroMouseTemplate_B.MATLABSystem1, &MicroMouseTemplate_DW.MATLABSystem1,
    &MicroMouseTemplate_P.MATLABSystem1);

  /* End of MinMax: '<S52>/Max of Elements1' */

  /* MinMax: '<S52>/Min' incorporates:
   *  MATLABSystem: '<S52>/MATLAB System1'
   */
  MicroMouseTemplate_B.minV = MicroMouseTemplate_B.MATLABSystem1.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem1.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.minV > u1) {
      MicroMouseTemplate_B.minV = u1;
    }
  }

  MicroMouseTem_MATLABSystem1((real_T)MicroMouseTemplate_B.rtb_CastToDouble_m[2],
    &MicroMouseTemplate_B.MATLABSystem1_ci,
    &MicroMouseTemplate_DW.MATLABSystem1_ci,
    &MicroMouseTemplate_P.MATLABSystem1_ci);

  /* MinMax: '<S52>/Max of Elements2' incorporates:
   *  MATLABSystem: '<S54>/MATLAB System1'
   */
  MicroMouseTemplate_B.maxV_c =
    MicroMouseTemplate_B.MATLABSystem1_ci.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem1_ci.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.maxV_c < u1) {
      MicroMouseTemplate_B.maxV_c = u1;
    }
  }

  MicroMouseTem_MATLABSystem1((real_T)MicroMouseTemplate_B.rtb_CastToDouble_m[3],
    &MicroMouseTemplate_B.MATLABSystem3_c,
    &MicroMouseTemplate_DW.MATLABSystem3_c,
    &MicroMouseTemplate_P.MATLABSystem3_c);

  /* MinMax: '<S52>/Max of Elements3' incorporates:
   *  MATLABSystem: '<S54>/MATLAB System3'
   */
  MicroMouseTemplate_B.maxV_k =
    MicroMouseTemplate_B.MATLABSystem3_c.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem3_c.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.maxV_k < u1) {
      MicroMouseTemplate_B.maxV_k = u1;
    }
  }

  MicroMouseTem_MATLABSystem1((real_T)MicroMouseTemplate_B.rtb_CastToDouble_m[4],
    &MicroMouseTemplate_B.MATLABSystem4_c,
    &MicroMouseTemplate_DW.MATLABSystem4_c,
    &MicroMouseTemplate_P.MATLABSystem4_c);

  /* MinMax: '<S52>/Max of Elements4' incorporates:
   *  MATLABSystem: '<S54>/MATLAB System4'
   */
  maxV = MicroMouseTemplate_B.MATLABSystem4_c.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem4_c.MATLABSystem1[i + 1];
    if (maxV < u1) {
      maxV = u1;
    }
  }

  MicroMouseTem_MATLABSystem1((real_T)MicroMouseTemplate_B.rtb_CastToDouble_m[5],
    &MicroMouseTemplate_B.MATLABSystem5, &MicroMouseTemplate_DW.MATLABSystem5,
    &MicroMouseTemplate_P.MATLABSystem5);

  /* MinMax: '<S52>/Max of Elements5' incorporates:
   *  MATLABSystem: '<S54>/MATLAB System5'
   */
  maxV_0 = MicroMouseTemplate_B.MATLABSystem5.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem5.MATLABSystem1[i + 1];
    if (maxV_0 < u1) {
      maxV_0 = u1;
    }
  }

  MicroMouseTem_MATLABSystem3((real_T)MicroMouseTemplate_B.rtb_CastToDouble_m[6],
    &MicroMouseTemplate_B.MATLABSystem3, &MicroMouseTemplate_DW.MATLABSystem3,
    &MicroMouseTemplate_P.MATLABSystem3);

  /* MinMax: '<S52>/Max of Elements6' incorporates:
   *  MATLABSystem: '<S52>/MATLAB System3'
   */
  maxV_1 = MicroMouseTemplate_B.MATLABSystem3.MATLABSystem3[0];
  for (i = 0; i < 9; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem3.MATLABSystem3[i + 1];
    if (maxV_1 < u1) {
      maxV_1 = u1;
    }
  }

  MicroMouseTem_MATLABSystem1(maxV_1, &MicroMouseTemplate_B.MATLABSystem2,
    &MicroMouseTemplate_DW.MATLABSystem2, &MicroMouseTemplate_P.MATLABSystem2);

  /* End of MinMax: '<S52>/Max of Elements6' */

  /* MinMax: '<S52>/Min1' incorporates:
   *  MATLABSystem: '<S52>/MATLAB System2'
   */
  minV = MicroMouseTemplate_B.MATLABSystem2.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem2.MATLABSystem1[i + 1];
    if (minV > u1) {
      minV = u1;
    }
  }

  MicroMouseTem_MATLABSystem1((real_T)MicroMouseTemplate_B.rtb_CastToDouble_m[7],
    &MicroMouseTemplate_B.MATLABSystem6, &MicroMouseTemplate_DW.MATLABSystem6,
    &MicroMouseTemplate_P.MATLABSystem6);

  /* MinMax: '<S52>/Max of Elements7' incorporates:
   *  MATLABSystem: '<S54>/MATLAB System6'
   */
  maxV_1 = MicroMouseTemplate_B.MATLABSystem6.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem6.MATLABSystem1[i + 1];
    if (maxV_1 < u1) {
      maxV_1 = u1;
    }
  }

  /* MinMax: '<S52>/Max of Elements' */
  u1 = fmod(floor(MicroMouseTemplate_B.maxV), 65536.0);

  /* DataStoreWrite: '<S49>/ADC_H Write' incorporates:
   *  MinMax: '<S52>/Max of Elements'
   */
  ADC_H[0] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S52>/Min' */
  u1 = fmod(floor(MicroMouseTemplate_B.minV), 65536.0);

  /* DataStoreWrite: '<S49>/ADC_H Write' incorporates:
   *  MinMax: '<S52>/Min'
   */
  ADC_H[1] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S52>/Max of Elements2' */
  u1 = fmod(floor(MicroMouseTemplate_B.maxV_c), 65536.0);

  /* DataStoreWrite: '<S49>/ADC_H Write' incorporates:
   *  MinMax: '<S52>/Max of Elements2'
   */
  ADC_H[2] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S52>/Max of Elements3' */
  u1 = fmod(floor(MicroMouseTemplate_B.maxV_k), 65536.0);

  /* DataStoreWrite: '<S49>/ADC_H Write' incorporates:
   *  MinMax: '<S52>/Max of Elements3'
   */
  ADC_H[3] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S52>/Max of Elements4' */
  u1 = fmod(floor(maxV), 65536.0);

  /* DataStoreWrite: '<S49>/ADC_H Write' incorporates:
   *  MinMax: '<S52>/Max of Elements4'
   */
  ADC_H[4] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S52>/Max of Elements5' */
  u1 = fmod(floor(maxV_0), 65536.0);

  /* DataStoreWrite: '<S49>/ADC_H Write' incorporates:
   *  MinMax: '<S52>/Max of Elements5'
   */
  ADC_H[5] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S52>/Min1' */
  u1 = fmod(floor(minV), 65536.0);

  /* DataStoreWrite: '<S49>/ADC_H Write' incorporates:
   *  MinMax: '<S52>/Min1'
   */
  ADC_H[6] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S52>/Max of Elements7' */
  u1 = fmod(floor(maxV_1), 65536.0);

  /* DataStoreWrite: '<S49>/ADC_H Write' incorporates:
   *  Constant: '<S49>/Constant'
   *  MinMax: '<S52>/Max of Elements7'
   */
  ADC_H[7] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);
  ADC_H[8] = MicroMouseTemplate_P.Constant_Value_b;

  /* MinMax: '<S53>/Max of Elements8' incorporates:
   *  MATLABSystem: '<S54>/MATLAB System2'
   */
  MicroMouseTemplate_B.minV =
    MicroMouseTemplate_B.MATLABSystem2_ci.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem2_ci.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.minV > u1) {
      MicroMouseTemplate_B.minV = u1;
    }
  }

  MicroMouseTem_MATLABSystem1((real_T)MicroMouseTemplate_B.rtb_CastToDouble_m[1],
    &MicroMouseTemplate_B.MATLABSystem1_c,
    &MicroMouseTemplate_DW.MATLABSystem1_c,
    &MicroMouseTemplate_P.MATLABSystem1_c);

  /* MinMax: '<S53>/Max of Elements9' incorporates:
   *  MATLABSystem: '<S53>/MATLAB System1'
   */
  minV = MicroMouseTemplate_B.MATLABSystem1_c.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem1_c.MATLABSystem1[i + 1];
    if (minV > u1) {
      minV = u1;
    }
  }

  /* MinMax: '<S53>/Max of Elements10' incorporates:
   *  MATLABSystem: '<S54>/MATLAB System1'
   */
  MicroMouseTemplate_B.maxV_c =
    MicroMouseTemplate_B.MATLABSystem1_ci.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem1_ci.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.maxV_c > u1) {
      MicroMouseTemplate_B.maxV_c = u1;
    }
  }

  /* MinMax: '<S53>/Max of Elements11' incorporates:
   *  MATLABSystem: '<S54>/MATLAB System3'
   */
  MicroMouseTemplate_B.maxV_k =
    MicroMouseTemplate_B.MATLABSystem3_c.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem3_c.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.maxV_k > u1) {
      MicroMouseTemplate_B.maxV_k = u1;
    }
  }

  /* MinMax: '<S53>/Max of Elements12' incorporates:
   *  MATLABSystem: '<S54>/MATLAB System4'
   */
  maxV = MicroMouseTemplate_B.MATLABSystem4_c.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem4_c.MATLABSystem1[i + 1];
    if (maxV > u1) {
      maxV = u1;
    }
  }

  /* MinMax: '<S53>/Max of Elements13' incorporates:
   *  MATLABSystem: '<S54>/MATLAB System5'
   */
  maxV_0 = MicroMouseTemplate_B.MATLABSystem5.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem5.MATLABSystem1[i + 1];
    if (maxV_0 > u1) {
      maxV_0 = u1;
    }
  }

  MicroMouseTem_MATLABSystem1((real_T)MicroMouseTemplate_B.rtb_CastToDouble_m[6],
    &MicroMouseTemplate_B.MATLABSystem2_c,
    &MicroMouseTemplate_DW.MATLABSystem2_c,
    &MicroMouseTemplate_P.MATLABSystem2_c);

  /* MinMax: '<S53>/Max of Elements14' incorporates:
   *  MATLABSystem: '<S53>/MATLAB System2'
   */
  maxV_1 = MicroMouseTemplate_B.MATLABSystem2_c.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem2_c.MATLABSystem1[i + 1];
    if (maxV_1 > u1) {
      maxV_1 = u1;
    }
  }

  /* MinMax: '<S53>/Max of Elements15' incorporates:
   *  MATLABSystem: '<S54>/MATLAB System6'
   */
  MicroMouseTemplate_B.maxV = MicroMouseTemplate_B.MATLABSystem6.MATLABSystem1[0];
  for (i = 0; i < 24; i++) {
    u1 = MicroMouseTemplate_B.MATLABSystem6.MATLABSystem1[i + 1];
    if (MicroMouseTemplate_B.maxV > u1) {
      MicroMouseTemplate_B.maxV = u1;
    }
  }

  /* MinMax: '<S53>/Max of Elements8' */
  u1 = fmod(floor(MicroMouseTemplate_B.minV), 65536.0);

  /* DataStoreWrite: '<S49>/ADC_L Write' incorporates:
   *  MinMax: '<S53>/Max of Elements8'
   */
  ADC_L[0] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S53>/Max of Elements9' */
  u1 = fmod(floor(minV), 65536.0);

  /* DataStoreWrite: '<S49>/ADC_L Write' incorporates:
   *  MinMax: '<S53>/Max of Elements9'
   */
  ADC_L[1] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S53>/Max of Elements10' */
  u1 = fmod(floor(MicroMouseTemplate_B.maxV_c), 65536.0);

  /* DataStoreWrite: '<S49>/ADC_L Write' incorporates:
   *  MinMax: '<S53>/Max of Elements10'
   */
  ADC_L[2] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S53>/Max of Elements11' */
  u1 = fmod(floor(MicroMouseTemplate_B.maxV_k), 65536.0);

  /* DataStoreWrite: '<S49>/ADC_L Write' incorporates:
   *  MinMax: '<S53>/Max of Elements11'
   */
  ADC_L[3] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S53>/Max of Elements12' */
  u1 = fmod(floor(maxV), 65536.0);

  /* DataStoreWrite: '<S49>/ADC_L Write' incorporates:
   *  MinMax: '<S53>/Max of Elements12'
   */
  ADC_L[4] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S53>/Max of Elements13' */
  u1 = fmod(floor(maxV_0), 65536.0);

  /* DataStoreWrite: '<S49>/ADC_L Write' incorporates:
   *  MinMax: '<S53>/Max of Elements13'
   */
  ADC_L[5] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S53>/Max of Elements14' */
  u1 = fmod(floor(maxV_1), 65536.0);

  /* DataStoreWrite: '<S49>/ADC_L Write' incorporates:
   *  MinMax: '<S53>/Max of Elements14'
   */
  ADC_L[6] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);

  /* MinMax: '<S53>/Max of Elements15' */
  u1 = fmod(floor(MicroMouseTemplate_B.maxV), 65536.0);

  /* DataStoreWrite: '<S49>/ADC_L Write' incorporates:
   *  Constant: '<S49>/Constant1'
   *  MinMax: '<S53>/Max of Elements15'
   */
  ADC_L[7] = (uint16_T)(u1 < 0.0 ? (int32_T)(uint16_T)-(int16_T)(uint16_T)-u1 :
                        (int32_T)(uint16_T)u1);
  ADC_L[8] = MicroMouseTemplate_P.Constant1_Value;

  /* MATLABSystem: '<S70>/Digital Port Read' */
  pinReadLoc = LL_GPIO_ReadInputPort(GPIOE);

  /* Logic: '<S9>/NOT' incorporates:
   *  MATLABSystem: '<S70>/Digital Port Read'
   * */
  NOT = ((pinReadLoc & 64U) == 0U);

  /* MATLABSystem: '<S72>/Digital Port Read' */
  pinReadLoc = LL_GPIO_ReadInputPort(GPIOB);

  /* Logic: '<S9>/NOT1' incorporates:
   *  MATLABSystem: '<S72>/Digital Port Read'
   * */
  NOT1 = ((pinReadLoc & 4U) == 0U);

  /* Chart: '<Root>/HelloMicroMouse!' */
  if (MicroMouseTemplate_DW.temporalCounter_i1 < 63U) {
    MicroMouseTemplate_DW.temporalCounter_i1++;
  }

  if (MicroMouseTemplate_DW.bitsForTID2.is_active_c2_MicroMouseTemplate == 0U) {
    MicroMouseTemplate_DW.bitsForTID2.is_active_c2_MicroMouseTemplate = 1U;
    MicroMouseTemplate_DW.bitsForTID2.is_c2_MicroMouseTemplate =
      Micro_IN_WaitUntilButtonPressed;
  } else {
    switch (MicroMouseTemplate_DW.bitsForTID2.is_c2_MicroMouseTemplate) {
     case MicroMouseTemplat_IN_Calibrated:
      if (NOT1) {
        switch (MicroMouseTemplate_DW.bitsForTID2.is_Calibrated) {
         case MicroMouseTemplate_IN_Average:
          pinReadLoc = (uint32_T)MicroMouseTemplate_DW.FWDLSThresh1 +
            MicroMouseTemplate_DW.FWDLSThresh2;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          pinReadLoc += MicroMouseTemplate_DW.FWDLSThresh3;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          pinReadLoc += MicroMouseTemplate_DW.FWDLSThresh4;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          MicroMouseTemplate_DW.FWDLSThresh = (uint16_T)rt_roundd((real_T)
            pinReadLoc * 0.25);
          pinReadLoc = (uint32_T)MicroMouseTemplate_DW.FWDRSThresh1 +
            MicroMouseTemplate_DW.FWDRSThresh2;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          pinReadLoc += MicroMouseTemplate_DW.FWDRSThresh3;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          pinReadLoc += MicroMouseTemplate_DW.FWDRSThresh4;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          MicroMouseTemplate_DW.FWDRSThresh = (uint16_T)rt_roundd((real_T)
            pinReadLoc * 0.25);
          pinReadLoc = (uint32_T)MicroMouseTemplate_DW.LSThresh1 +
            MicroMouseTemplate_DW.LSThresh2;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          pinReadLoc += MicroMouseTemplate_DW.LSThresh3;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          pinReadLoc += MicroMouseTemplate_DW.LSThresh4;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          MicroMouseTemplate_DW.LSThresh = (uint16_T)rt_roundd((real_T)
            pinReadLoc * 0.25);
          pinReadLoc = (uint32_T)MicroMouseTemplate_DW.RSThresh1 +
            MicroMouseTemplate_DW.RSThresh2;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          pinReadLoc += MicroMouseTemplate_DW.RSThresh3;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          pinReadLoc += MicroMouseTemplate_DW.RSThresh4;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          MicroMouseTemplate_DW.RSThresh = (uint16_T)rt_roundd((real_T)
            pinReadLoc * 0.25);
          MicroMouseTemplate_DW.bitsForTID2.is_Calibrated =
            MicroMouseTe_IN_NO_ACTIVE_CHILD;
          break;

         case MicroMouseTemplate_IN_Turn1:
          MicroMouseTemplate_B.rightWheel_m = 40;
          MicroMouseTemplate_B.leftWheel_ds = 40;
          MicroMouseTemplate_DW.bitsForTID2.is_Calibrated =
            MicroMouseTe_IN_NO_ACTIVE_CHILD;
          break;

         case MicroMouseTemplate_IN_Turn2:
          MicroMouseTemplate_B.rightWheel_m = 40;
          MicroMouseTemplate_B.leftWheel_ds = 40;
          MicroMouseTemplate_DW.bitsForTID2.is_Calibrated =
            MicroMouseTe_IN_NO_ACTIVE_CHILD;
          break;

         case MicroMouseTemplate_IN_Turn3:
          MicroMouseTemplate_B.rightWheel_m = 40;
          MicroMouseTemplate_B.leftWheel_ds = 40;
          MicroMouseTemplate_DW.bitsForTID2.is_Calibrated =
            MicroMouseTe_IN_NO_ACTIVE_CHILD;
          break;

         default:
          MicroMouseTemplate_DW.bitsForTID2.is_Calibrated =
            MicroMouseTe_IN_NO_ACTIVE_CHILD;
          break;
        }

        MicroMouseTemplate_DW.bitsForTID2.is_c2_MicroMouseTemplate =
          Micro_IN_WaitUntilButtonPressed;
      } else {
        switch (MicroMouseTemplate_DW.bitsForTID2.is_Calibrated) {
         case MicroMouseTemplate_IN_Average:
          pinReadLoc = (uint32_T)MicroMouseTemplate_DW.FWDLSThresh1 +
            MicroMouseTemplate_DW.FWDLSThresh2;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          pinReadLoc += MicroMouseTemplate_DW.FWDLSThresh3;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          pinReadLoc += MicroMouseTemplate_DW.FWDLSThresh4;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          MicroMouseTemplate_DW.FWDLSThresh = (uint16_T)rt_roundd((real_T)
            pinReadLoc * 0.25);
          pinReadLoc = (uint32_T)MicroMouseTemplate_DW.FWDRSThresh1 +
            MicroMouseTemplate_DW.FWDRSThresh2;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          pinReadLoc += MicroMouseTemplate_DW.FWDRSThresh3;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          pinReadLoc += MicroMouseTemplate_DW.FWDRSThresh4;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          MicroMouseTemplate_DW.FWDRSThresh = (uint16_T)rt_roundd((real_T)
            pinReadLoc * 0.25);
          pinReadLoc = (uint32_T)MicroMouseTemplate_DW.LSThresh1 +
            MicroMouseTemplate_DW.LSThresh2;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          pinReadLoc += MicroMouseTemplate_DW.LSThresh3;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          pinReadLoc += MicroMouseTemplate_DW.LSThresh4;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          MicroMouseTemplate_DW.LSThresh = (uint16_T)rt_roundd((real_T)
            pinReadLoc * 0.25);
          pinReadLoc = (uint32_T)MicroMouseTemplate_DW.RSThresh1 +
            MicroMouseTemplate_DW.RSThresh2;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          pinReadLoc += MicroMouseTemplate_DW.RSThresh3;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          pinReadLoc += MicroMouseTemplate_DW.RSThresh4;
          if (pinReadLoc > 65535U) {
            pinReadLoc = 65535U;
          }

          MicroMouseTemplate_DW.RSThresh = (uint16_T)rt_roundd((real_T)
            pinReadLoc * 0.25);
          break;

         case MicroMouseTemplate_IN_FirstSide:
          if (MicroMouseTemplate_DW.temporalCounter_i1 >= 50U) {
            MicroMouseTemplate_DW.temporalCounter_i1 = 0U;
            MicroMouseTemplate_DW.bitsForTID2.is_Calibrated =
              MicroMouseTemplate_IN_Turn1;
            MicroMouseTemplate_B.rightWheel_m = 85;
            MicroMouseTemplate_B.leftWheel_ds = -85;
          }
          break;

         case MicroMouseTemplat_IN_FourthSide:
          if (MicroMouseTemplate_DW.temporalCounter_i1 >= 20U) {
            MicroMouseTemplate_DW.bitsForTID2.is_Calibrated =
              MicroMouseTemplate_IN_Average;
          }
          break;

         case MicroMouseTemplat_IN_SecondSide:
          if (MicroMouseTemplate_DW.temporalCounter_i1 >= 20U) {
            MicroMouseTemplate_DW.temporalCounter_i1 = 0U;
            MicroMouseTemplate_DW.bitsForTID2.is_Calibrated =
              MicroMouseTemplate_IN_Turn2;
            MicroMouseTemplate_B.rightWheel_m = 85;
            MicroMouseTemplate_B.leftWheel_ds = -85;
          }
          break;

         case MicroMouseTemplate_IN_ThirdSide:
          if (MicroMouseTemplate_DW.temporalCounter_i1 >= 20U) {
            MicroMouseTemplate_DW.temporalCounter_i1 = 0U;
            MicroMouseTemplate_DW.bitsForTID2.is_Calibrated =
              MicroMouseTemplate_IN_Turn3;
            MicroMouseTemplate_B.rightWheel_m = 85;
            MicroMouseTemplate_B.leftWheel_ds = -85;
          }
          break;

         case MicroMouseTemplate_IN_Turn1:
          if (MicroMouseTemplate_DW.temporalCounter_i1 >= 6U) {
            MicroMouseTemplate_B.rightWheel_m = 40;
            MicroMouseTemplate_B.leftWheel_ds = 40;
            MicroMouseTemplate_DW.temporalCounter_i1 = 0U;
            MicroMouseTemplate_DW.bitsForTID2.is_Calibrated =
              MicroMouseTemplat_IN_SecondSide;
            MicroMouseTemplate_DW.FWDLSThresh2 = MicroMouseTemplate_B.Flip[6];
            MicroMouseTemplate_DW.FWDRSThresh2 = MicroMouseTemplate_B.Flip[1];
            MicroMouseTemplate_DW.LSThresh2 = MicroMouseTemplate_B.Flip[4];
            MicroMouseTemplate_DW.RSThresh2 = MicroMouseTemplate_B.Flip[3];
          }
          break;

         case MicroMouseTemplate_IN_Turn2:
          if (MicroMouseTemplate_DW.temporalCounter_i1 >= 6U) {
            MicroMouseTemplate_B.rightWheel_m = 40;
            MicroMouseTemplate_B.leftWheel_ds = 40;
            MicroMouseTemplate_DW.temporalCounter_i1 = 0U;
            MicroMouseTemplate_DW.bitsForTID2.is_Calibrated =
              MicroMouseTemplate_IN_ThirdSide;
            MicroMouseTemplate_DW.FWDLSThresh3 = MicroMouseTemplate_B.Flip[6];
            MicroMouseTemplate_DW.FWDRSThresh3 = MicroMouseTemplate_B.Flip[1];
            MicroMouseTemplate_DW.LSThresh3 = MicroMouseTemplate_B.Flip[4];
            MicroMouseTemplate_DW.RSThresh3 = MicroMouseTemplate_B.Flip[3];
          }
          break;

         default:
          /* case IN_Turn3: */
          if (MicroMouseTemplate_DW.temporalCounter_i1 >= 6U) {
            MicroMouseTemplate_B.rightWheel_m = 40;
            MicroMouseTemplate_B.leftWheel_ds = 40;
            MicroMouseTemplate_DW.temporalCounter_i1 = 0U;
            MicroMouseTemplate_DW.bitsForTID2.is_Calibrated =
              MicroMouseTemplat_IN_FourthSide;
            MicroMouseTemplate_DW.FWDLSThresh4 = MicroMouseTemplate_B.Flip[6];
            MicroMouseTemplate_DW.FWDRSThresh4 = MicroMouseTemplate_B.Flip[1];
            MicroMouseTemplate_DW.LSThresh4 = MicroMouseTemplate_B.Flip[4];
            MicroMouseTemplate_DW.RSThresh4 = MicroMouseTemplate_B.Flip[3];
          }
          break;
        }
      }
      break;

     case MicroMouseTempla_IN_Calibration:
      if (MicroMouseTemplate_DW.temporalCounter_i1 >= 50U) {
        MicroMouseTemplate_DW.bitsForTID2.is_c2_MicroMouseTemplate =
          MicroMouseTemplat_IN_Calibrated;
        MicroMouseTemplate_B.rightWheel_m = 50;
        MicroMouseTemplate_B.leftWheel_ds = 50;
        MicroMouseTemplate_B.LED0 = true;
        MicroMouseTemplate_B.LED1 = true;
        MicroMouseTemplate_B.LED2 = true;
        MicroMouseTemplate_DW.temporalCounter_i1 = 0U;
        MicroMouseTemplate_DW.bitsForTID2.is_Calibrated =
          MicroMouseTemplate_IN_FirstSide;
        MicroMouseTemplate_DW.FWDLSThresh1 = MicroMouseTemplate_B.Flip[6];
        MicroMouseTemplate_DW.FWDRSThresh1 = MicroMouseTemplate_B.Flip[1];
        MicroMouseTemplate_DW.LSThresh1 = MicroMouseTemplate_B.Flip[4];
        MicroMouseTemplate_DW.RSThresh1 = MicroMouseTemplate_B.Flip[3];
      } else {
        if (MicroMouseTemplate_DW.FWDLSThreshMin > MicroMouseTemplate_B.Flip[6])
        {
          MicroMouseTemplate_DW.FWDLSThreshMin = MicroMouseTemplate_B.Flip[6];
        }

        if (MicroMouseTemplate_DW.FWDRSThreshMin > MicroMouseTemplate_B.Flip[1])
        {
          MicroMouseTemplate_DW.FWDRSThreshMin = MicroMouseTemplate_B.Flip[1];
        }

        if (MicroMouseTemplate_DW.LSThreshMin > MicroMouseTemplate_B.Flip[4]) {
          MicroMouseTemplate_DW.LSThreshMin = MicroMouseTemplate_B.Flip[4];
        }

        if (MicroMouseTemplate_DW.RSThreshMin > MicroMouseTemplate_B.Flip[3]) {
          MicroMouseTemplate_DW.RSThreshMin = MicroMouseTemplate_B.Flip[3];
        }

        if (MicroMouseTemplate_DW.FWDLSThreshMax < MicroMouseTemplate_B.Flip[6])
        {
          MicroMouseTemplate_DW.FWDLSThreshMax = MicroMouseTemplate_B.Flip[6];
        }

        if (MicroMouseTemplate_DW.FWDRSThreshMax < MicroMouseTemplate_B.Flip[1])
        {
          MicroMouseTemplate_DW.FWDRSThreshMax = MicroMouseTemplate_B.Flip[1];
        }

        if (MicroMouseTemplate_DW.LSThreshMax < MicroMouseTemplate_B.Flip[4]) {
          MicroMouseTemplate_DW.LSThreshMax = MicroMouseTemplate_B.Flip[4];
        }

        if (MicroMouseTemplate_DW.RSThreshMax < MicroMouseTemplate_B.Flip[3]) {
          MicroMouseTemplate_DW.RSThreshMax = MicroMouseTemplate_B.Flip[3];
        }
      }
      break;

     case MicroMouseTe_IN_FollowBlackLine:
      MicroMouseTempl_FollowBlackLine(&NOT);
      break;

     default:
      /* case IN_WaitUntilButtonPressed: */
      if (NOT) {
        MicroMouseTemplate_DW.temporalCounter_i1 = 0U;
        MicroMouseTemplate_DW.bitsForTID2.is_c2_MicroMouseTemplate =
          MicroMouseTempla_IN_Calibration;
        MicroMouseTemplate_B.LED1 = true;
        MicroMouseTemplate_DW.DownLSThresh = MicroMouseTemplate_B.Flip[5];
        MicroMouseTemplate_DW.DownRSThresh = MicroMouseTemplate_B.Flip[2];
        MicroMouseTemplate_DW.FWDLSThreshMin = MicroMouseTemplate_B.Flip[6];
        MicroMouseTemplate_DW.FWDRSThreshMin = MicroMouseTemplate_B.Flip[1];
        MicroMouseTemplate_DW.LSThreshMin = MicroMouseTemplate_B.Flip[4];
        MicroMouseTemplate_DW.RSThreshMin = MicroMouseTemplate_B.Flip[3];
        MicroMouseTemplate_DW.MOTLSThresh = MicroMouseTemplate_B.Flip[7];
        MicroMouseTemplate_DW.MOTRSThresh = MicroMouseTemplate_B.Flip[0];
        MicroMouseTemplate_DW.FWDLSThreshMax = MicroMouseTemplate_B.Flip[6];
        MicroMouseTemplate_DW.FWDRSThreshMax = MicroMouseTemplate_B.Flip[1];
        MicroMouseTemplate_DW.LSThreshMax = MicroMouseTemplate_B.Flip[4];
        MicroMouseTemplate_DW.RSThreshMax = MicroMouseTemplate_B.Flip[3];
      } else if (NOT1) {
        MicroMouseTemplate_DW.bitsForTID2.is_c2_MicroMouseTemplate =
          MicroMouseTe_IN_FollowBlackLine;
        MicroMouseTemplate_B.LED0 = false;
        MicroMouseTemplate_B.LED1 = true;
        MicroMouseTemplate_B.LED2 = false;
        MicroMouseTemplate_DW.bitsForTID2.is_FollowBlackLine =
          MicroMouseTemplate_IN_Forward;
      } else {
        MicroMouseTemplate_B.LED0 = false;
        MicroMouseTemplate_B.LED1 = !MicroMouseTemplate_B.LED1;
        MicroMouseTemplate_B.LED2 = false;
        MicroMouseTemplate_B.rightWheel_m = 0;
        MicroMouseTemplate_B.leftWheel_ds = 0;
      }
      break;
    }
  }

  /* End of Chart: '<Root>/HelloMicroMouse!' */

  /* If: '<S5>/If2' incorporates:
   *  Abs: '<S5>/Abs'
   */
  if (MicroMouseTemplate_B.rightWheel_m > 0) {
    /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem' incorporates:
     *  ActionPort: '<S35>/Action Port'
     */
    /* SignalConversion generated from: '<S35>/rightWheel' */
    MicroMouseTemplate_B.rightWheel_c = MicroMouseTemplate_B.rightWheel_m;

    /* End of Outputs for SubSystem: '<S5>/If Action Subsystem' */
  } else if (MicroMouseTemplate_B.rightWheel_m < 0) {
    /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S37>/Action Port'
     */
    /* Abs: '<S5>/Abs' incorporates:
     *  SignalConversion generated from: '<S37>/rightWheel'
     */
    MicroMouseTemplate_B.rightWheel = (int8_T)-MicroMouseTemplate_B.rightWheel_m;

    /* End of Outputs for SubSystem: '<S5>/If Action Subsystem2' */
  } else {
    /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S37>/Action Port'
     */
    /* SignalConversion generated from: '<S37>/rightWheel' incorporates:
     *  Abs: '<S5>/Abs'
     */
    MicroMouseTemplate_B.rightWheel = 0;

    /* End of Outputs for SubSystem: '<S5>/If Action Subsystem2' */
  }

  /* End of If: '<S5>/If2' */

  /* MATLABSystem: '<S47>/PWM Output' */
  setDutyCycleInPercentageChannel1(MicroMouseTemplate_DW.obj_g.TimerHandle,
    MicroMouseTemplate_B.rightWheel_c);
  setDutyCycleInPercentageChannel2(MicroMouseTemplate_DW.obj_g.TimerHandle,
    MicroMouseTemplate_B.rightWheel);

  /* If: '<S5>/If1' incorporates:
   *  Abs: '<S5>/Abs1'
   */
  rtPrevAction = MicroMouseTemplate_DW.If1_ActiveSubsystem;
  if (MicroMouseTemplate_B.leftWheel_ds > 0) {
    MicroMouseTemplate_DW.If1_ActiveSubsystem = 0;
  } else {
    MicroMouseTemplate_DW.If1_ActiveSubsystem = 1;
  }

  if (rtPrevAction != MicroMouseTemplate_DW.If1_ActiveSubsystem) {
    switch (rtPrevAction) {
     case 0:
      /* Disable for SignalConversion generated from: '<S36>/leftWheel' incorporates:
       *  Outport: '<S36>/Out1'
       * */
      MicroMouseTemplate_B.leftWheel_d = MicroMouseTemplate_P.Out1_Y0;
      break;

     case 1:
      /* Disable for SignalConversion generated from: '<S38>/leftWheel' incorporates:
       *  Outport: '<S38>/Out1'
       * */
      MicroMouseTemplate_B.leftWheel = MicroMouseTemplate_P.Out1_Y0_l;
      break;
    }
  }

  if (MicroMouseTemplate_DW.If1_ActiveSubsystem == 0) {
    /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S36>/Action Port'
     */
    /* SignalConversion generated from: '<S36>/leftWheel' */
    MicroMouseTemplate_B.leftWheel_d = MicroMouseTemplate_B.leftWheel_ds;

    /* End of Outputs for SubSystem: '<S5>/If Action Subsystem1' */
  } else if (MicroMouseTemplate_B.leftWheel_ds < 0) {
    /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem4' incorporates:
     *  ActionPort: '<S38>/Action Port'
     */
    /* Abs: '<S5>/Abs1' incorporates:
     *  SignalConversion generated from: '<S38>/leftWheel'
     */
    MicroMouseTemplate_B.leftWheel = (int8_T)-MicroMouseTemplate_B.leftWheel_ds;

    /* End of Outputs for SubSystem: '<S5>/If Action Subsystem4' */
  } else {
    /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem4' incorporates:
     *  ActionPort: '<S38>/Action Port'
     */
    /* SignalConversion generated from: '<S38>/leftWheel' incorporates:
     *  Abs: '<S5>/Abs1'
     */
    MicroMouseTemplate_B.leftWheel = MicroMouseTemplate_B.leftWheel_ds;

    /* End of Outputs for SubSystem: '<S5>/If Action Subsystem4' */
  }

  /* End of If: '<S5>/If1' */

  /* MATLABSystem: '<S45>/PWM Output' */
  setDutyCycleInPercentageChannel3(MicroMouseTemplate_DW.obj_n.TimerHandle,
    MicroMouseTemplate_B.leftWheel_d);
  setDutyCycleInPercentageChannel4(MicroMouseTemplate_DW.obj_n.TimerHandle,
    MicroMouseTemplate_B.leftWheel);

  /* MATLABSystem: '<S62>/Digital Port Write' */
  MicroMouseTemplate_B.portNameLoc = GPIOC;
  if (MicroMouseTemplate_B.LED0) {
    i = 8192;
  } else {
    i = 0;
  }

  LL_GPIO_SetOutputPin(MicroMouseTemplate_B.portNameLoc, (uint32_T)i);
  LL_GPIO_ResetOutputPin(MicroMouseTemplate_B.portNameLoc, ~(uint32_T)i & 8192U);

  /* End of MATLABSystem: '<S62>/Digital Port Write' */

  /* MATLABSystem: '<S64>/Digital Port Write' */
  MicroMouseTemplate_B.portNameLoc = GPIOC;
  if (MicroMouseTemplate_B.LED1) {
    i = 16384;
  } else {
    i = 0;
  }

  LL_GPIO_SetOutputPin(MicroMouseTemplate_B.portNameLoc, (uint32_T)i);
  LL_GPIO_ResetOutputPin(MicroMouseTemplate_B.portNameLoc, ~(uint32_T)i & 16384U);

  /* End of MATLABSystem: '<S64>/Digital Port Write' */

  /* MATLABSystem: '<S66>/Digital Port Write' */
  MicroMouseTemplate_B.portNameLoc = GPIOC;
  if (MicroMouseTemplate_B.LED2) {
    i = 32768;
  } else {
    i = 0;
  }

  LL_GPIO_SetOutputPin(MicroMouseTemplate_B.portNameLoc, (uint32_T)i);
  LL_GPIO_ResetOutputPin(MicroMouseTemplate_B.portNameLoc, ~(uint32_T)i & 32768U);

  /* End of MATLABSystem: '<S66>/Digital Port Write' */

  /* user code (Update function Body for TID2) */

  /* System '<Root>' */
  refreshIMUValues();

  /* System '<Root>' */
  CustomWhile();
}

/* Model initialize function */
void MicroMouseTemplate_initialize(void)
{
  /* Registration code */
  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&MicroMouseTemplate_M->solverInfo,
                          &MicroMouseTemplate_M->Timing.simTimeStep);
    rtsiSetTPtr(&MicroMouseTemplate_M->solverInfo, &rtmGetTPtr
                (MicroMouseTemplate_M));
    rtsiSetStepSizePtr(&MicroMouseTemplate_M->solverInfo,
                       &MicroMouseTemplate_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&MicroMouseTemplate_M->solverInfo, (&rtmGetErrorStatus
      (MicroMouseTemplate_M)));
    rtsiSetRTModelPtr(&MicroMouseTemplate_M->solverInfo, MicroMouseTemplate_M);
  }

  rtsiSetSimTimeStep(&MicroMouseTemplate_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetIsMinorTimeStepWithModeChange(&MicroMouseTemplate_M->solverInfo, false);
  rtsiSetIsContModeFrozen(&MicroMouseTemplate_M->solverInfo, false);
  rtsiSetSolverName(&MicroMouseTemplate_M->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr(MicroMouseTemplate_M, &MicroMouseTemplate_M->Timing.tArray[0]);
  MicroMouseTemplate_M->Timing.stepSize0 = 0.025;

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

    /* Start for TransportDelay: '<S4>/Transport Delay1' */
    {
      real_T *pBuffer =
        &MicroMouseTemplate_DW.TransportDelay1_RWORK.TUbufferArea[0];
      MicroMouseTemplate_DW.TransportDelay1_IWORK.Tail = 0;
      MicroMouseTemplate_DW.TransportDelay1_IWORK.Head = 0;
      MicroMouseTemplate_DW.TransportDelay1_IWORK.Last = 0;
      MicroMouseTemplate_DW.TransportDelay1_IWORK.CircularBufSize = 1024;
      pBuffer[0] = MicroMouseTemplate_P.TransportDelay1_InitOutput;
      pBuffer[1024] = MicroMouseTemplate_M->Timing.t[0];
      MicroMouseTemplate_DW.TransportDelay1_PWORK.TUbufferPtrs[0] = (void *)
        &pBuffer[0];
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
    for (i = 0; i < 9; i++) {
      /* Start for DataStoreMemory: '<S6>/Data Store Memory' */
      ADC1s[i] = MicroMouseTemplate_P.DataStoreMemory_InitialValue_p;

      /* Start for DataStoreMemory: '<S6>/Data Store Memory1' */
      ADC_H[i] = MicroMouseTemplate_P.DataStoreMemory1_InitialValue_p;

      /* Start for DataStoreMemory: '<S6>/Data Store Memory2' */
      ADC_L[i] = MicroMouseTemplate_P.DataStoreMemory2_InitialValue_p;
    }

    /* Start for If: '<S5>/If1' */
    MicroMouseTemplate_DW.If1_ActiveSubsystem = -1;

    /* user code (Initialize function Body) */

    /* System '<Root>' */
    initIMU();

    /* InitializeConditions for UnitDelay: '<S34>/Unit Delay' */
    MicroMouseTemplate_DW.UnitDelay_DSTATE =
      MicroMouseTemplate_P.UnitDelay_InitialCondition;

    /* SystemInitialize for IfAction SubSystem: '<S5>/If Action Subsystem' */
    /* SystemInitialize for SignalConversion generated from: '<S35>/rightWheel' incorporates:
     *  Outport: '<S35>/Out1'
     */
    MicroMouseTemplate_B.rightWheel_c = MicroMouseTemplate_P.Out1_Y0_o;

    /* End of SystemInitialize for SubSystem: '<S5>/If Action Subsystem' */

    /* SystemInitialize for IfAction SubSystem: '<S5>/If Action Subsystem2' */
    /* SystemInitialize for SignalConversion generated from: '<S37>/rightWheel' incorporates:
     *  Outport: '<S37>/Out1'
     */
    MicroMouseTemplate_B.rightWheel = MicroMouseTemplate_P.Out1_Y0_m;

    /* End of SystemInitialize for SubSystem: '<S5>/If Action Subsystem2' */

    /* SystemInitialize for IfAction SubSystem: '<S5>/If Action Subsystem1' */
    /* SystemInitialize for SignalConversion generated from: '<S36>/leftWheel' incorporates:
     *  Outport: '<S36>/Out1'
     */
    MicroMouseTemplate_B.leftWheel_d = MicroMouseTemplate_P.Out1_Y0;

    /* End of SystemInitialize for SubSystem: '<S5>/If Action Subsystem1' */

    /* SystemInitialize for IfAction SubSystem: '<S5>/If Action Subsystem4' */
    /* SystemInitialize for SignalConversion generated from: '<S38>/leftWheel' incorporates:
     *  Outport: '<S38>/Out1'
     */
    MicroMouseTemplate_B.leftWheel = MicroMouseTemplate_P.Out1_Y0_l;

    /* End of SystemInitialize for SubSystem: '<S5>/If Action Subsystem4' */

    /* Start for MATLABSystem: '<S51>/Analog to Digital Converter' */
    MicroMouseTemplate_DW.obj.isInitialized = 0;
    MicroMouseTemplate_DW.obj.matlabCodegenIsDeleted = false;
    MicroMouseTemp_SystemCore_setup(&MicroMouseTemplate_DW.obj);
    MicroMou_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem2_ci,
      &MicroMouseTemplate_P.MATLABSystem2_ci);
    MicroMou_MATLABSystem3_Init(&MicroMouseTemplate_DW.MATLABSystem4,
      &MicroMouseTemplate_P.MATLABSystem4);
    MicroMou_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem1,
      &MicroMouseTemplate_P.MATLABSystem1);
    MicroMou_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem1_ci,
      &MicroMouseTemplate_P.MATLABSystem1_ci);
    MicroMou_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem3_c,
      &MicroMouseTemplate_P.MATLABSystem3_c);
    MicroMou_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem4_c,
      &MicroMouseTemplate_P.MATLABSystem4_c);
    MicroMou_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem5,
      &MicroMouseTemplate_P.MATLABSystem5);
    MicroMou_MATLABSystem3_Init(&MicroMouseTemplate_DW.MATLABSystem3,
      &MicroMouseTemplate_P.MATLABSystem3);
    MicroMou_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem2,
      &MicroMouseTemplate_P.MATLABSystem2);
    MicroMou_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem6,
      &MicroMouseTemplate_P.MATLABSystem6);
    MicroMou_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem1_c,
      &MicroMouseTemplate_P.MATLABSystem1_c);
    MicroMou_MATLABSystem1_Init(&MicroMouseTemplate_DW.MATLABSystem2_c,
      &MicroMouseTemplate_P.MATLABSystem2_c);

    /* Start for MATLABSystem: '<S47>/PWM Output' */
    MicroMouseTemplate_DW.obj_g.matlabCodegenIsDeleted = false;
    MicroMouseTemplate_DW.obj_g.isSetupComplete = false;
    MicroMouseTemplate_DW.obj_g.isInitialized = 1;
    MicroMous_PWMOutput_setupImpl_d(&MicroMouseTemplate_DW.obj_g);
    MicroMouseTemplate_DW.obj_g.isSetupComplete = true;

    /* Start for MATLABSystem: '<S45>/PWM Output' */
    MicroMouseTemplate_DW.obj_n.matlabCodegenIsDeleted = false;
    MicroMouseTemplate_DW.obj_n.isSetupComplete = false;
    MicroMouseTemplate_DW.obj_n.isInitialized = 1;
    MicroMouseT_PWMOutput_setupImpl(&MicroMouseTemplate_DW.obj_n);
    MicroMouseTemplate_DW.obj_n.isSetupComplete = true;
  }
}

/* Model terminate function */
void MicroMouseTemplate_terminate(void)
{
  /* Terminate for MATLABSystem: '<S51>/Analog to Digital Converter' */
  if (!MicroMouseTemplate_DW.obj.matlabCodegenIsDeleted) {
    MicroMouseTemplate_DW.obj.matlabCodegenIsDeleted = true;
    if ((MicroMouseTemplate_DW.obj.isInitialized == 1) &&
        MicroMouseTemplate_DW.obj.isSetupComplete) {
      ADC_Handle_Deinit(MicroMouseTemplate_DW.obj.ADCHandle,
                        ADC_DMA_INTERRUPT_MODE, 1);
    }
  }

  /* End of Terminate for MATLABSystem: '<S51>/Analog to Digital Converter' */

  /* Terminate for MATLABSystem: '<S47>/PWM Output' */
  if (!MicroMouseTemplate_DW.obj_g.matlabCodegenIsDeleted) {
    MicroMouseTemplate_DW.obj_g.matlabCodegenIsDeleted = true;
    if ((MicroMouseTemplate_DW.obj_g.isInitialized == 1) &&
        MicroMouseTemplate_DW.obj_g.isSetupComplete) {
      disableCounter(MicroMouseTemplate_DW.obj_g.TimerHandle);
      disableTimerInterrupts(MicroMouseTemplate_DW.obj_g.TimerHandle, 0);
      disableTimerChannel1(MicroMouseTemplate_DW.obj_g.TimerHandle, ENABLE_CH);
      disableTimerChannel2(MicroMouseTemplate_DW.obj_g.TimerHandle, ENABLE_CH);
    }
  }

  /* End of Terminate for MATLABSystem: '<S47>/PWM Output' */

  /* Terminate for MATLABSystem: '<S45>/PWM Output' */
  if (!MicroMouseTemplate_DW.obj_n.matlabCodegenIsDeleted) {
    MicroMouseTemplate_DW.obj_n.matlabCodegenIsDeleted = true;
    if ((MicroMouseTemplate_DW.obj_n.isInitialized == 1) &&
        MicroMouseTemplate_DW.obj_n.isSetupComplete) {
      disableCounter(MicroMouseTemplate_DW.obj_n.TimerHandle);
      disableTimerInterrupts(MicroMouseTemplate_DW.obj_n.TimerHandle, 0);
      disableTimerChannel3(MicroMouseTemplate_DW.obj_n.TimerHandle, ENABLE_CH);
      disableTimerChannel4(MicroMouseTemplate_DW.obj_n.TimerHandle, ENABLE_CH);
    }
  }

  /* End of Terminate for MATLABSystem: '<S45>/PWM Output' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

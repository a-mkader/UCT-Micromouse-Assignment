/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MicroMouseTemplate_types.h
 *
 * Code generated for Simulink model 'MicroMouseTemplate'.
 *
 * Model version                  : 3.15
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
<<<<<<< HEAD
<<<<<<< HEAD
 * C/C++ source code generated on : Wed Sep 18 21:07:26 2024
=======
 * C/C++ source code generated on : Wed Sep 18 20:58:09 2024
>>>>>>> 4f20edd72ec7e95f0ca68e4761f28d084f7309f4
=======
 * C/C++ source code generated on : Thu Sep 19 13:59:04 2024
>>>>>>> 6baeb643d52f0db44e8a661bbdf1c9b481646043
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef MicroMouseTemplate_types_h_
#define MicroMouseTemplate_types_h_
#include "rtwtypes.h"
#include "stm_timer_ll.h"
#include "stm_adc_ll.h"
#ifndef struct_tag_TL74IR3Rn74hKhfKckCIBC
#define struct_tag_TL74IR3Rn74hKhfKckCIBC

struct tag_TL74IR3Rn74hKhfKckCIBC
{
  int32_T isInitialized;
  real_T InitialCondition;
  real_T Buffer[25];
};

#endif                                 /* struct_tag_TL74IR3Rn74hKhfKckCIBC */

#ifndef typedef_CircularBuffer_MicroMouseTemp_T
#define typedef_CircularBuffer_MicroMouseTemp_T

typedef struct tag_TL74IR3Rn74hKhfKckCIBC CircularBuffer_MicroMouseTemp_T;

#endif                             /* typedef_CircularBuffer_MicroMouseTemp_T */

#ifndef struct_tag_v2DAHo7klwgURvzbuzh8NC
#define struct_tag_v2DAHo7klwgURvzbuzh8NC

struct tag_v2DAHo7klwgURvzbuzh8NC
{
  int32_T isInitialized;
  real_T InitialCondition;
  real_T Buffer[10];
};

#endif                                 /* struct_tag_v2DAHo7klwgURvzbuzh8NC */

#ifndef typedef_CircularBuffer_MicroMouseTe_p_T
#define typedef_CircularBuffer_MicroMouseTe_p_T

typedef struct tag_v2DAHo7klwgURvzbuzh8NC CircularBuffer_MicroMouseTe_p_T;

#endif                             /* typedef_CircularBuffer_MicroMouseTe_p_T */

#ifndef struct_tag_S5MMpcz8vT0FlmhmEl4q5C
#define struct_tag_S5MMpcz8vT0FlmhmEl4q5C

struct tag_S5MMpcz8vT0FlmhmEl4q5C
{
  int32_T isInitialized;
};

#endif                                 /* struct_tag_S5MMpcz8vT0FlmhmEl4q5C */

#ifndef typedef_stm32cube_blocks_DigitalPortW_T
#define typedef_stm32cube_blocks_DigitalPortW_T

typedef struct tag_S5MMpcz8vT0FlmhmEl4q5C stm32cube_blocks_DigitalPortW_T;

#endif                             /* typedef_stm32cube_blocks_DigitalPortW_T */

/* Custom Type definition for MATLABSystem: '<S47>/PWM Output' */
#include "stm_timer_ll.h"
#include "stm_timer_ll.h"
#ifndef struct_tag_C3ug75osl8w3UN1OXa3ipB
#define struct_tag_C3ug75osl8w3UN1OXa3ipB

struct tag_C3ug75osl8w3UN1OXa3ipB
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  TIM_Type_T* TimerHandle;
};

#endif                                 /* struct_tag_C3ug75osl8w3UN1OXa3ipB */

#ifndef typedef_stm32cube_blocks_PWMOutput_Mi_T
#define typedef_stm32cube_blocks_PWMOutput_Mi_T

typedef struct tag_C3ug75osl8w3UN1OXa3ipB stm32cube_blocks_PWMOutput_Mi_T;

#endif                             /* typedef_stm32cube_blocks_PWMOutput_Mi_T */

/* Custom Type definition for MATLABSystem: '<S51>/Analog to Digital Converter' */
#include "stm_adc_ll.h"
#include "stm_adc_ll.h"
#ifndef struct_tag_wiywjhRn0JhvylthmapQgB
#define struct_tag_wiywjhRn0JhvylthmapQgB

struct tag_wiywjhRn0JhvylthmapQgB
{
  int32_T isInitialized;
};

#endif                                 /* struct_tag_wiywjhRn0JhvylthmapQgB */

#ifndef typedef_stm32cube_blocks_DigitalPortR_T
#define typedef_stm32cube_blocks_DigitalPortR_T

typedef struct tag_wiywjhRn0JhvylthmapQgB stm32cube_blocks_DigitalPortR_T;

#endif                             /* typedef_stm32cube_blocks_DigitalPortR_T */

#ifndef struct_tag_6dsyvroagB27OCKFoJuL7
#define struct_tag_6dsyvroagB27OCKFoJuL7

struct tag_6dsyvroagB27OCKFoJuL7
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  ADC_Type_T* ADCHandle;
  uint16_T ADCInternalBuffer[18];
};

#endif                                 /* struct_tag_6dsyvroagB27OCKFoJuL7 */

#ifndef typedef_stm32cube_blocks_AnalogInput__T
#define typedef_stm32cube_blocks_AnalogInput__T

typedef struct tag_6dsyvroagB27OCKFoJuL7 stm32cube_blocks_AnalogInput__T;

#endif                             /* typedef_stm32cube_blocks_AnalogInput__T */

/* Parameters for system: '<S52>/MATLAB System1' */
typedef struct P_MATLABSystem1_MicroMouseTem_T_ P_MATLABSystem1_MicroMouseTem_T;

/* Parameters for system: '<S52>/MATLAB System3' */
typedef struct P_MATLABSystem3_MicroMouseTem_T_ P_MATLABSystem3_MicroMouseTem_T;

/* Parameters (default storage) */
typedef struct P_MicroMouseTemplate_T_ P_MicroMouseTemplate_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_MicroMouseTemplate_T RT_MODEL_MicroMouseTemplate_T;

#endif                                 /* MicroMouseTemplate_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

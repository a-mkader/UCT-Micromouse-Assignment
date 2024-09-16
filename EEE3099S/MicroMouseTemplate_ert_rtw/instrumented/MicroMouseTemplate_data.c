/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MicroMouseTemplate_data.c
 *
 * Code generated for Simulink model 'MicroMouseTemplate'.
 *
<<<<<<< HEAD
 * Model version                  : 1.264
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Fri Sep 13 12:11:45 2024
=======
 * Model version                  : 3.5
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Sat Sep 14 20:35:13 2024
>>>>>>> a7a15a19f1076b326b6a0f18cbd9f81c566186bc
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "MicroMouseTemplate.h"

/* Block parameters (default storage) */
P_MicroMouseTemplate_T MicroMouseTemplate_P = {
  /* Expression: 1
   * Referenced by: '<S5>/Constant'
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S8>/Constant'
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<Root>/Pulse Generator'
   */
  1.0,

  /* Computed Parameter: PulseGenerator_Period
   * Referenced by: '<Root>/Pulse Generator'
   */
  2.0,

  /* Computed Parameter: PulseGenerator_Duty
   * Referenced by: '<Root>/Pulse Generator'
   */
  1.0,

  /* Expression: 0
   * Referenced by: '<Root>/Pulse Generator'
   */
  0.0,

  /* Expression: 1
   * Referenced by: '<Root>/Pulse Generator2'
   */
  1.0,

  /* Computed Parameter: PulseGenerator2_Period
   * Referenced by: '<Root>/Pulse Generator2'
   */
  2.0,

  /* Computed Parameter: PulseGenerator2_Duty
   * Referenced by: '<Root>/Pulse Generator2'
   */
  1.0,

  /* Expression: 0
   * Referenced by: '<Root>/Pulse Generator2'
   */
  0.0,

  /* Expression: 1
   * Referenced by: '<Root>/Pulse Generator1'
   */
  1.0,

  /* Computed Parameter: PulseGenerator1_Period
   * Referenced by: '<Root>/Pulse Generator1'
   */
  2.0,

  /* Computed Parameter: PulseGenerator1_Duty
   * Referenced by: '<Root>/Pulse Generator1'
   */
  1.0,

  /* Expression: 0
   * Referenced by: '<Root>/Pulse Generator1'
   */
  0.0,

  /* Computed Parameter: DataStoreMemory2_InitialValue
   * Referenced by: '<S1>/Data Store Memory2'
   */
  0,

  /* Computed Parameter: DataStoreMemory4_InitialValue
   * Referenced by: '<S1>/Data Store Memory4'
   */
  0,

  /* Computed Parameter: DataStoreMemory_InitialValue
   * Referenced by: '<S7>/Data Store Memory'
   */
  0.0F,

  /* Computed Parameter: DataStoreMemory1_InitialValue
   * Referenced by: '<S7>/Data Store Memory1'
   */
  0.0F,

  /* Computed Parameter: DataStoreMemory1_InitialValue_j
   * Referenced by: '<S1>/Data Store Memory1'
   */
  0U,

  /* Computed Parameter: Constant_Value_b
   * Referenced by: '<S48>/Constant'
   */
  0U,

  /* Computed Parameter: Constant1_Value
   * Referenced by: '<S48>/Constant1'
   */
  0U,

  /* Computed Parameter: DataStoreMemory_InitialValue_p
   * Referenced by: '<S6>/Data Store Memory'
   */
  0U,

  /* Computed Parameter: DataStoreMemory1_InitialValue_p
   * Referenced by: '<S6>/Data Store Memory1'
   */
  0U,

  /* Computed Parameter: DataStoreMemory2_InitialValue_p
   * Referenced by: '<S6>/Data Store Memory2'
   */
  0U,

  /* Computed Parameter: DataStoreMemory_InitialValue_p4
   * Referenced by: '<S1>/Data Store Memory'
   */
  false,

  /* Computed Parameter: Out1_Y0
   * Referenced by: '<S35>/Out1'
   */
  0,

  /* Computed Parameter: Out1_Y0_l
   * Referenced by: '<S37>/Out1'
   */
  0,

  /* Computed Parameter: Out1_Y0_o
   * Referenced by: '<S34>/Out1'
   */
  0,

  /* Computed Parameter: Out1_Y0_m
   * Referenced by: '<S36>/Out1'
   */
  0,

  /* Start of '<S53>/MATLAB System6' */
  {
    /* Expression: 0
     * Referenced by: '<S53>/MATLAB System6'
     */
    0.0
  }
  ,

  /* End of '<S53>/MATLAB System6' */

  /* Start of '<S53>/MATLAB System5' */
  {
    /* Expression: 0
     * Referenced by: '<S53>/MATLAB System5'
     */
    0.0
  }
  ,

  /* End of '<S53>/MATLAB System5' */

  /* Start of '<S53>/MATLAB System4' */
  {
    /* Expression: 0
     * Referenced by: '<S53>/MATLAB System4'
     */
    0.0
  }
  ,

  /* End of '<S53>/MATLAB System4' */

  /* Start of '<S53>/MATLAB System3' */
  {
    /* Expression: 0
     * Referenced by: '<S53>/MATLAB System3'
     */
    0.0
  }
  ,

  /* End of '<S53>/MATLAB System3' */

  /* Start of '<S53>/MATLAB System2' */
  {
    /* Expression: 0
     * Referenced by: '<S53>/MATLAB System2'
     */
    0.0
  }
  ,

  /* End of '<S53>/MATLAB System2' */

  /* Start of '<S53>/MATLAB System1' */
  {
    /* Expression: 0
     * Referenced by: '<S53>/MATLAB System1'
     */
    0.0
  }
  ,

  /* End of '<S53>/MATLAB System1' */

  /* Start of '<S52>/MATLAB System2' */
  {
    /* Expression: 0
     * Referenced by: '<S52>/MATLAB System2'
     */
    0.0
  }
  ,

  /* End of '<S52>/MATLAB System2' */

  /* Start of '<S52>/MATLAB System1' */
  {
    /* Expression: 0
     * Referenced by: '<S52>/MATLAB System1'
     */
    0.0
  }
  ,

  /* End of '<S52>/MATLAB System1' */

  /* Start of '<S51>/MATLAB System4' */
  {
    /* Expression: 0
     * Referenced by: '<S51>/MATLAB System4'
     */
    0.0
  }
  ,

  /* End of '<S51>/MATLAB System4' */

  /* Start of '<S51>/MATLAB System3' */
  {
    /* Expression: 0
     * Referenced by: '<S51>/MATLAB System3'
     */
    0.0
  }
  ,

  /* End of '<S51>/MATLAB System3' */

  /* Start of '<S51>/MATLAB System2' */
  {
    /* Expression: 0
     * Referenced by: '<S51>/MATLAB System2'
     */
    0.0
  }
  ,

  /* End of '<S51>/MATLAB System2' */

  /* Start of '<S51>/MATLAB System1' */
  {
    /* Expression: 0
     * Referenced by: '<S51>/MATLAB System1'
     */
    0.0
  }
  /* End of '<S51>/MATLAB System1' */
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: survey_dml_core_4_initialize.c
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 06-Jun-2020 22:16:48
 */

/* Include Files */
#include "survey_dml_core_4_initialize.h"
#include "eml_rand_mt19937ar_stateful.h"
#include "rt_nonfinite.h"
#include "survey_dml_core_4.h"
#include "survey_dml_core_4_data.h"

/* Function Definitions */

/*
 * Arguments    : void
 * Return Type  : void
 */
void survey_dml_core_4_initialize(void)
{
  rt_InitInfAndNaN();
  c_eml_rand_mt19937ar_stateful_i();
  isInitialized_survey_dml_core_4 = true;
}

/*
 * File trailer for survey_dml_core_4_initialize.c
 *
 * [EOF]
 */

/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: survey_dml_core_2.h
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 05-Jun-2020 14:34:57
 */

#ifndef SURVEY_DML_CORE_2_H
#define SURVEY_DML_CORE_2_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "survey_dml_core_2_types.h"

/* Function Declarations */
extern void survey_dml_core_2(double NUMB_RND, double NUMB_SMP, double INDX_FIG,
  const emxArray_real_T *DATA_DIS_ANT, const emxArray_real_T *DATA_COE_WIN,
  const double DATA_ANG_OBJ[2], const double DATA_POW_OBJ[2], double DATA_SNR,
  double DATA_DLT_ANG_TST, double DATA_RNG_ANG_TST, emxArray_real_T *idxAngRnd);

#endif

/*
 * File trailer for survey_dml_core_2.h
 *
 * [EOF]
 */

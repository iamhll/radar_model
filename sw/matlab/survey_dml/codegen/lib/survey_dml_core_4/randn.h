/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: randn.h
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 06-Jun-2020 22:16:48
 */

#ifndef RANDN_H
#define RANDN_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "survey_dml_core_4_types.h"

/* Function Declarations */
extern void genrand_uint32_vector(unsigned int mt[625], unsigned int u[2]);
extern void randn(const double varargin_1[2], emxArray_real_T *r);

#endif

/*
 * File trailer for randn.h
 *
 * [EOF]
 */

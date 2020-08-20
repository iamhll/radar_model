/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: dbscanMine2_emxAPI.h
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 20-Aug-2020 15:51:06
 */

#ifndef DBSCANMINE2_EMXAPI_H
#define DBSCANMINE2_EMXAPI_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "dbscanMine2_types.h"

/* Function Declarations */
extern emxArray_real_T *emxCreateND_real_T(int numDimensions, const int *size);
extern emxArray_real_T *emxCreateWrapperND_real_T(double *data, int
  numDimensions, const int *size);
extern emxArray_real_T *emxCreateWrapper_real_T(double *data, int rows, int cols);
extern emxArray_real_T *emxCreate_real_T(int rows, int cols);
extern void emxDestroyArray_real_T(emxArray_real_T *emxArray);
extern void emxInitArray_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#endif

/*
 * File trailer for dbscanMine2_emxAPI.h
 *
 * [EOF]
 */

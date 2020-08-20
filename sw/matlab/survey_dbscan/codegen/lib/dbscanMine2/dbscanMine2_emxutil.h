/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: dbscanMine2_emxutil.h
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 20-Aug-2020 15:51:06
 */

#ifndef DBSCANMINE2_EMXUTIL_H
#define DBSCANMINE2_EMXUTIL_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "dbscanMine2_types.h"

/* Function Declarations */
extern void emxEnsureCapacity_boolean_T(emxArray_boolean_T *emxArray, int
  oldNumel);
extern void emxEnsureCapacity_real_T(emxArray_real_T *emxArray, int oldNumel);
extern void emxEnsureCapacity_uint32_T(emxArray_uint32_T *emxArray, int oldNumel);
extern void emxFree_boolean_T(emxArray_boolean_T **pEmxArray);
extern void emxFree_real_T(emxArray_real_T **pEmxArray);
extern void emxFree_uint32_T(emxArray_uint32_T **pEmxArray);
extern void emxInit_boolean_T(emxArray_boolean_T **pEmxArray, int numDimensions);
extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);
extern void emxInit_uint32_T(emxArray_uint32_T **pEmxArray, int numDimensions);

#endif

/*
 * File trailer for dbscanMine2_emxutil.h
 *
 * [EOF]
 */

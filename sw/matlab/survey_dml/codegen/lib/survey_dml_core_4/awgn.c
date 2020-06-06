/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: awgn.c
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 06-Jun-2020 22:16:48
 */

/* Include Files */
#include "awgn.h"
#include "randn.h"
#include "rt_nonfinite.h"
#include "survey_dml_core_4.h"
#include "survey_dml_core_4_emxutil.h"
#include "survey_dml_core_4_rtwutil.h"
#include <math.h>

/* Function Definitions */

/*
 * Arguments    : const emxArray_creal_T *varargin_1
 *                double varargin_2
 *                emxArray_creal_T *y
 * Return Type  : void
 */
void awgn(const emxArray_creal_T *varargin_1, double varargin_2,
          emxArray_creal_T *y)
{
  emxArray_real_T *b_y;
  int nx;
  int k;
  emxArray_real_T *c_y;
  double d_y;
  emxArray_creal_T *noise;
  emxArray_real_T *r;
  emxArray_real_T *b;
  double b_varargin_1[2];
  emxInit_real_T(&b_y, 1);
  nx = varargin_1->size[0];
  k = b_y->size[0];
  b_y->size[0] = varargin_1->size[0];
  emxEnsureCapacity_real_T(b_y, k);
  for (k = 0; k < nx; k++) {
    b_y->data[k] = rt_hypotd_snf(varargin_1->data[k].re, varargin_1->data[k].im);
  }

  emxInit_real_T(&c_y, 1);
  k = c_y->size[0];
  c_y->size[0] = b_y->size[0];
  emxEnsureCapacity_real_T(c_y, k);
  nx = b_y->size[0];
  for (k = 0; k < nx; k++) {
    c_y->data[k] = rt_powd_snf(b_y->data[k], 2.0);
  }

  emxFree_real_T(&b_y);
  nx = c_y->size[0];
  if (c_y->size[0] == 0) {
    d_y = 0.0;
  } else {
    d_y = c_y->data[0];
    for (k = 2; k <= nx; k++) {
      d_y += c_y->data[k - 1];
    }
  }

  emxFree_real_T(&c_y);
  emxInit_creal_T(&noise, 2);
  emxInit_real_T(&r, 2);
  emxInit_real_T(&b, 2);
  b_varargin_1[0] = varargin_1->size[0];
  b_varargin_1[1] = 1.0;
  randn(b_varargin_1, r);
  b_varargin_1[0] = varargin_1->size[0];
  b_varargin_1[1] = 1.0;
  randn(b_varargin_1, b);
  d_y = sqrt(d_y / (double)varargin_1->size[0] / rt_powd_snf(10.0, varargin_2 /
              10.0) / 2.0);
  k = noise->size[0] * noise->size[1];
  noise->size[0] = r->size[0];
  noise->size[1] = 1;
  emxEnsureCapacity_creal_T(noise, k);
  nx = r->size[0] * r->size[1];
  for (k = 0; k < nx; k++) {
    noise->data[k].re = d_y * (r->data[k] + 0.0 * b->data[k]);
    noise->data[k].im = d_y * b->data[k];
  }

  emxFree_real_T(&b);
  emxFree_real_T(&r);
  k = y->size[0];
  y->size[0] = varargin_1->size[0];
  emxEnsureCapacity_creal_T(y, k);
  nx = varargin_1->size[0];
  for (k = 0; k < nx; k++) {
    y->data[k].re = varargin_1->data[k].re + noise->data[k].re;
    y->data[k].im = varargin_1->data[k].im + noise->data[k].im;
  }

  emxFree_creal_T(&noise);
}

/*
 * File trailer for awgn.c
 *
 * [EOF]
 */

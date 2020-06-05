/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: survey_dml_core_2.c
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 05-Jun-2020 14:34:57
 */

/* Include Files */
#include "survey_dml_core_2.h"
#include "colon.h"
#include "rand.h"
#include "randn.h"
#include "rt_nonfinite.h"
#include "survey_dml_core_2_data.h"
#include "survey_dml_core_2_emxutil.h"
#include "survey_dml_core_2_initialize.h"
#include <math.h>

/* Function Declarations */
static double rt_hypotd_snf(double u0, double u1);
static double rt_powd_snf(double u0, double u1);

/* Function Definitions */

/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_hypotd_snf(double u0, double u1)
{
  double y;
  double a;
  a = fabs(u0);
  y = fabs(u1);
  if (a < y) {
    a /= y;
    y *= sqrt(a * a + 1.0);
  } else if (a > y) {
    y /= a;
    y = a * sqrt(y * y + 1.0);
  } else {
    if (!rtIsNaN(y)) {
      y = a * 1.4142135623730951;
    }
  }

  return y;
}

/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_powd_snf(double u0, double u1)
{
  double y;
  double d;
  double d1;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else {
    d = fabs(u0);
    d1 = fabs(u1);
    if (rtIsInf(u1)) {
      if (d == 1.0) {
        y = 1.0;
      } else if (d > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d1 == 0.0) {
      y = 1.0;
    } else if (d1 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = rtNaN;
    } else {
      y = pow(u0, u1);
    }
  }

  return y;
}

/*
 * function idxAngRnd = survey_dml_core_2(NUMB_RND, NUMB_SMP, INDX_FIG, DATA_DIS_ANT, DATA_COE_WIN, DATA_ANG_OBJ, DATA_POW_OBJ, DATA_SNR, DATA_DLT_ANG_TST, DATA_RNG_ANG_TST)
 * *** DERIVED PARAMETER *********************************************************
 * Arguments    : double NUMB_RND
 *                double NUMB_SMP
 *                double INDX_FIG
 *                const emxArray_real_T *DATA_DIS_ANT
 *                const emxArray_real_T *DATA_COE_WIN
 *                const double DATA_ANG_OBJ[2]
 *                const double DATA_POW_OBJ[2]
 *                double DATA_SNR
 *                double DATA_DLT_ANG_TST
 *                double DATA_RNG_ANG_TST
 *                emxArray_real_T *idxAngRnd
 * Return Type  : void
 */
void survey_dml_core_2(double NUMB_RND, double NUMB_SMP, double INDX_FIG, const
  emxArray_real_T *DATA_DIS_ANT, const emxArray_real_T *DATA_COE_WIN, const
  double DATA_ANG_OBJ[2], const double DATA_POW_OBJ[2], double DATA_SNR, double
  DATA_DLT_ANG_TST, double DATA_RNG_ANG_TST, emxArray_real_T *idxAngRnd)
{
  emxArray_real_T *DATA_ANG_TST;
  int i;
  emxArray_creal_T *datPTst;
  int loop_ub;
  int NUMB_ANT;
  int NUMB_ANG_TST;
  emxArray_creal_T *datA;
  emxArray_creal_T *datP;
  emxArray_creal_T *datSmpSum;
  emxArray_creal_T *y;
  emxArray_creal_T *b_y;
  emxArray_creal_T *a;
  int idxAng0;
  int i1;
  int idxAng1;
  unsigned int b_idxAng1;
  double t_im;
  int i2;
  emxArray_creal_T *datSig;
  double datPowBst;
  emxArray_creal_T *datPowTmp;
  emxArray_creal_T *noise;
  emxArray_real_T *r;
  emxArray_real_T *b;
  emxArray_real_T *c_y;
  emxArray_real_T *d_y;
  int nx;
  double b_r;
  emxArray_creal_T *A;
  int k;
  double im_tmp;
  int idxRnd;
  double datAmp;
  double temp_re;
  double temp_im;
  creal_T e_y[4];
  int i3;
  int aoffset;
  int temp_re_tmp;
  double brm;
  double bim;
  double b_bim;
  double t_re;
  creal_T b_b[4];
  int b_i;
  double b_datSig[2];
  int i4;
  int i5;
  int coffset;
  int boffset;
  if (isInitialized_survey_dml_core_2 == false) {
    survey_dml_core_2_initialize();
  }

  (void)INDX_FIG;

  /* ------------------------------------------------------------------------------- */
  /*  */
  /*   Filename       : survey_dml_core_2.m */
  /*   Author         : Huang Leilei */
  /*   Created        : 2020-06-04 */
  /*   Description    : core calculation of dml */
  /*  */
  /* ------------------------------------------------------------------------------- */
  /* 'survey_dml_core_2:13' DATA_ANG_TST     = -DATA_RNG_ANG_TST:DATA_DLT_ANG_TST:DATA_RNG_ANG_TST; */
  emxInit_real_T(&DATA_ANG_TST, 2);
  if (rtIsNaN(-DATA_RNG_ANG_TST) || rtIsNaN(DATA_DLT_ANG_TST) || rtIsNaN
      (DATA_RNG_ANG_TST)) {
    i = DATA_ANG_TST->size[0] * DATA_ANG_TST->size[1];
    DATA_ANG_TST->size[0] = 1;
    DATA_ANG_TST->size[1] = 1;
    emxEnsureCapacity_real_T(DATA_ANG_TST, i);
    DATA_ANG_TST->data[0] = rtNaN;
  } else if ((DATA_DLT_ANG_TST == 0.0) || ((-DATA_RNG_ANG_TST < DATA_RNG_ANG_TST)
              && (DATA_DLT_ANG_TST < 0.0)) || ((DATA_RNG_ANG_TST <
               -DATA_RNG_ANG_TST) && (DATA_DLT_ANG_TST > 0.0))) {
    DATA_ANG_TST->size[0] = 1;
    DATA_ANG_TST->size[1] = 0;
  } else if ((rtIsInf(-DATA_RNG_ANG_TST) || rtIsInf(DATA_RNG_ANG_TST)) &&
             (rtIsInf(DATA_DLT_ANG_TST) || (-DATA_RNG_ANG_TST ==
               DATA_RNG_ANG_TST))) {
    i = DATA_ANG_TST->size[0] * DATA_ANG_TST->size[1];
    DATA_ANG_TST->size[0] = 1;
    DATA_ANG_TST->size[1] = 1;
    emxEnsureCapacity_real_T(DATA_ANG_TST, i);
    DATA_ANG_TST->data[0] = rtNaN;
  } else if (rtIsInf(DATA_DLT_ANG_TST)) {
    i = DATA_ANG_TST->size[0] * DATA_ANG_TST->size[1];
    DATA_ANG_TST->size[0] = 1;
    DATA_ANG_TST->size[1] = 1;
    emxEnsureCapacity_real_T(DATA_ANG_TST, i);
    DATA_ANG_TST->data[0] = -DATA_RNG_ANG_TST;
  } else if ((floor(-DATA_RNG_ANG_TST) == -DATA_RNG_ANG_TST) && (floor
              (DATA_DLT_ANG_TST) == DATA_DLT_ANG_TST)) {
    i = DATA_ANG_TST->size[0] * DATA_ANG_TST->size[1];
    DATA_ANG_TST->size[0] = 1;
    loop_ub = (int)floor((DATA_RNG_ANG_TST - (-DATA_RNG_ANG_TST)) /
                         DATA_DLT_ANG_TST);
    DATA_ANG_TST->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(DATA_ANG_TST, i);
    for (i = 0; i <= loop_ub; i++) {
      DATA_ANG_TST->data[i] = -DATA_RNG_ANG_TST + DATA_DLT_ANG_TST * (double)i;
    }
  } else {
    eml_float_colon(-DATA_RNG_ANG_TST, DATA_DLT_ANG_TST, DATA_RNG_ANG_TST,
                    DATA_ANG_TST);
  }

  emxInit_creal_T(&datPTst, 4);

  /* 'survey_dml_core_2:14' NUMB_ANT         = numel(DATA_DIS_ANT); */
  NUMB_ANT = DATA_DIS_ANT->size[0];

  /* 'survey_dml_core_2:15' NUMB_OBJ         = numel(DATA_ANG_OBJ); */
  /* 'survey_dml_core_2:16' NUMB_ANG_TST     = numel(DATA_ANG_TST); */
  NUMB_ANG_TST = DATA_ANG_TST->size[1] - 2;

  /* *** MAIN BODY ***************************************************************** */
  /*  prepare datP */
  /* 'survey_dml_core_2:23' datPTst = complex(zeros(NUMB_ANT, NUMB_ANT, NUMB_ANG_TST, NUMB_ANG_TST)); */
  i = datPTst->size[0] * datPTst->size[1] * datPTst->size[2] * datPTst->size[3];
  datPTst->size[0] = DATA_DIS_ANT->size[0];
  datPTst->size[1] = DATA_DIS_ANT->size[0];
  datPTst->size[2] = DATA_ANG_TST->size[1];
  datPTst->size[3] = DATA_ANG_TST->size[1];
  emxEnsureCapacity_creal_T(datPTst, i);
  loop_ub = DATA_DIS_ANT->size[0] * DATA_DIS_ANT->size[0] * DATA_ANG_TST->size[1]
    * DATA_ANG_TST->size[1];
  for (i = 0; i < loop_ub; i++) {
    datPTst->data[i].re = 0.0;
    datPTst->data[i].im = 0.0;
  }

  /* 'survey_dml_core_2:24' for idxAng0 = 1:NUMB_ANG_TST */
  i = DATA_ANG_TST->size[1];
  emxInit_creal_T(&datA, 2);
  emxInit_creal_T(&datP, 2);
  emxInit_creal_T(&datSmpSum, 1);
  emxInit_creal_T(&y, 1);
  emxInit_creal_T(&b_y, 2);
  emxInit_creal_T(&a, 2);
  for (idxAng0 = 0; idxAng0 < i; idxAng0++) {
    /* 'survey_dml_core_2:25' datAng0 = DATA_ANG_TST(idxAng0); */
    /* 'survey_dml_core_2:26' for idxAng1 = (idxAng0+1):NUMB_ANG_TST */
    i1 = NUMB_ANG_TST - idxAng0;
    for (idxAng1 = 0; idxAng1 <= i1; idxAng1++) {
      b_idxAng1 = ((unsigned int)idxAng0 + idxAng1) + 2U;

      /* 'survey_dml_core_2:27' datAng1 = DATA_ANG_TST(idxAng1); */
      /* 'survey_dml_core_2:28' datA = [exp(1i * 2 * pi * DATA_DIS_ANT * sin(datAng0 / 180 * pi))     ... */
      /* 'survey_dml_core_2:29'                ,exp(1i * 2 * pi * DATA_DIS_ANT * sin(datAng1 / 180 * pi))]    ... */
      /* 'survey_dml_core_2:30'         ; */
      t_im = sin(DATA_ANG_TST->data[idxAng0] / 180.0 * 3.1415926535897931);
      i2 = y->size[0];
      y->size[0] = DATA_DIS_ANT->size[0];
      emxEnsureCapacity_creal_T(y, i2);
      loop_ub = DATA_DIS_ANT->size[0];
      datPowBst = sin(DATA_ANG_TST->data[(int)b_idxAng1 - 1] / 180.0 *
                      3.1415926535897931);
      i2 = datSmpSum->size[0];
      datSmpSum->size[0] = DATA_DIS_ANT->size[0];
      emxEnsureCapacity_creal_T(datSmpSum, i2);
      for (i2 = 0; i2 < loop_ub; i2++) {
        b_r = DATA_DIS_ANT->data[i2] * 0.0;
        im_tmp = DATA_DIS_ANT->data[i2] * 6.2831853071795862;
        y->data[i2].re = t_im * b_r;
        y->data[i2].im = t_im * im_tmp;
        datSmpSum->data[i2].re = datPowBst * b_r;
        datSmpSum->data[i2].im = datPowBst * im_tmp;
      }

      nx = y->size[0];
      for (k = 0; k < nx; k++) {
        if (y->data[k].im == 0.0) {
          y->data[k].re = exp(y->data[k].re);
          y->data[k].im = 0.0;
        } else if (rtIsInf(y->data[k].im) && rtIsInf(y->data[k].re) && (y->
                    data[k].re < 0.0)) {
          y->data[k].re = 0.0;
          y->data[k].im = 0.0;
        } else {
          b_r = exp(y->data[k].re / 2.0);
          y->data[k].re = b_r * (b_r * cos(y->data[k].im));
          y->data[k].im = b_r * (b_r * sin(y->data[k].im));
        }
      }

      nx = datSmpSum->size[0];
      for (k = 0; k < nx; k++) {
        if (datSmpSum->data[k].im == 0.0) {
          datSmpSum->data[k].re = exp(datSmpSum->data[k].re);
          datSmpSum->data[k].im = 0.0;
        } else if (rtIsInf(datSmpSum->data[k].im) && rtIsInf(datSmpSum->data[k].
                    re) && (datSmpSum->data[k].re < 0.0)) {
          datSmpSum->data[k].re = 0.0;
          datSmpSum->data[k].im = 0.0;
        } else {
          b_r = exp(datSmpSum->data[k].re / 2.0);
          datSmpSum->data[k].re = b_r * (b_r * cos(datSmpSum->data[k].im));
          datSmpSum->data[k].im = b_r * (b_r * sin(datSmpSum->data[k].im));
        }
      }

      i2 = datA->size[0] * datA->size[1];
      datA->size[0] = y->size[0];
      datA->size[1] = 2;
      emxEnsureCapacity_creal_T(datA, i2);
      loop_ub = y->size[0];
      for (i2 = 0; i2 < loop_ub; i2++) {
        datA->data[i2] = y->data[i2];
      }

      loop_ub = datSmpSum->size[0];
      for (i2 = 0; i2 < loop_ub; i2++) {
        datA->data[i2 + datA->size[0]] = datSmpSum->data[i2];
      }

      /* 'survey_dml_core_2:31' datP = datA * (datA' * datA)^-1 * datA'; */
      i2 = a->size[0] * a->size[1];
      a->size[0] = 2;
      loop_ub = datA->size[0];
      a->size[1] = datA->size[0];
      emxEnsureCapacity_creal_T(a, i2);
      for (i2 = 0; i2 < loop_ub; i2++) {
        a->data[2 * i2].re = datA->data[i2].re;
        a->data[2 * i2].im = -datA->data[i2].im;
        i3 = 2 * i2 + 1;
        a->data[i3].re = datA->data[i2 + datA->size[0]].re;
        a->data[i3].im = -datA->data[i2 + datA->size[0]].im;
      }

      loop_ub = a->size[1];
      if ((a->size[1] == 1) || (datA->size[0] == 1)) {
        e_y[0].re = 0.0;
        e_y[0].im = 0.0;
        for (i2 = 0; i2 < loop_ub; i2++) {
          e_y[0].re += a->data[2 * i2].re * datA->data[i2].re - a->data[2 * i2].
            im * datA->data[i2].im;
          e_y[0].im += a->data[2 * i2].re * datA->data[i2].im + a->data[2 * i2].
            im * datA->data[i2].re;
        }

        e_y[2].re = 0.0;
        e_y[2].im = 0.0;
        for (i2 = 0; i2 < loop_ub; i2++) {
          e_y[2].re += a->data[2 * i2].re * datA->data[i2 + datA->size[0]].re -
            a->data[2 * i2].im * datA->data[i2 + datA->size[0]].im;
          e_y[2].im += a->data[2 * i2].re * datA->data[i2 + datA->size[0]].im +
            a->data[2 * i2].im * datA->data[i2 + datA->size[0]].re;
        }

        e_y[1].re = 0.0;
        e_y[1].im = 0.0;
        for (i2 = 0; i2 < loop_ub; i2++) {
          i3 = 2 * i2 + 1;
          e_y[1].re += a->data[i3].re * datA->data[i2].re - a->data[i3].im *
            datA->data[i2].im;
          e_y[1].im += a->data[i3].re * datA->data[i2].im + a->data[i3].im *
            datA->data[i2].re;
        }

        e_y[3].re = 0.0;
        e_y[3].im = 0.0;
        for (i2 = 0; i2 < loop_ub; i2++) {
          i3 = 2 * i2 + 1;
          e_y[3].re += a->data[i3].re * datA->data[i2 + datA->size[0]].re -
            a->data[i3].im * datA->data[i2 + datA->size[0]].im;
          e_y[3].im += a->data[i3].re * datA->data[i2 + datA->size[0]].im +
            a->data[i3].im * datA->data[i2 + datA->size[0]].re;
        }
      } else {
        e_y[0].re = 0.0;
        e_y[0].im = 0.0;
        e_y[1].re = 0.0;
        e_y[1].im = 0.0;
        for (k = 0; k < loop_ub; k++) {
          aoffset = k << 1;
          e_y[0].re += datA->data[k].re * a->data[aoffset].re - datA->data[k].im
            * a->data[aoffset].im;
          e_y[0].im += datA->data[k].re * a->data[aoffset].im + datA->data[k].im
            * a->data[aoffset].re;
          e_y[1].re += datA->data[k].re * a->data[aoffset + 1].re - datA->data[k]
            .im * a->data[aoffset + 1].im;
          e_y[1].im += datA->data[k].re * a->data[aoffset + 1].im + datA->data[k]
            .im * a->data[aoffset + 1].re;
        }

        e_y[2].re = 0.0;
        e_y[2].im = 0.0;
        e_y[3].re = 0.0;
        e_y[3].im = 0.0;
        for (k = 0; k < loop_ub; k++) {
          aoffset = k << 1;
          temp_re_tmp = loop_ub + k;
          e_y[2].re += datA->data[temp_re_tmp].re * a->data[aoffset].re -
            datA->data[temp_re_tmp].im * a->data[aoffset].im;
          e_y[2].im += datA->data[temp_re_tmp].re * a->data[aoffset].im +
            datA->data[temp_re_tmp].im * a->data[aoffset].re;
          e_y[3].re += datA->data[temp_re_tmp].re * a->data[aoffset + 1].re -
            datA->data[temp_re_tmp].im * a->data[aoffset + 1].im;
          e_y[3].im += datA->data[temp_re_tmp].re * a->data[aoffset + 1].im +
            datA->data[temp_re_tmp].im * a->data[aoffset + 1].re;
        }
      }

      datAmp = fabs(e_y[0].re);
      brm = fabs(e_y[1].re);
      bim = fabs(e_y[0].im);
      b_bim = fabs(e_y[1].im);
      if (brm + b_bim > datAmp + bim) {
        if (e_y[1].im == 0.0) {
          if (e_y[0].im == 0.0) {
            temp_re = e_y[0].re / e_y[1].re;
            temp_im = 0.0;
          } else if (e_y[0].re == 0.0) {
            temp_re = 0.0;
            temp_im = e_y[0].im / e_y[1].re;
          } else {
            temp_re = e_y[0].re / e_y[1].re;
            temp_im = e_y[0].im / e_y[1].re;
          }
        } else if (e_y[1].re == 0.0) {
          if (e_y[0].re == 0.0) {
            temp_re = e_y[0].im / e_y[1].im;
            temp_im = 0.0;
          } else if (e_y[0].im == 0.0) {
            temp_re = 0.0;
            temp_im = -(e_y[0].re / e_y[1].im);
          } else {
            temp_re = e_y[0].im / e_y[1].im;
            temp_im = -(e_y[0].re / e_y[1].im);
          }
        } else if (brm > b_bim) {
          datPowBst = e_y[1].im / e_y[1].re;
          im_tmp = e_y[1].re + datPowBst * e_y[1].im;
          temp_re = (e_y[0].re + datPowBst * e_y[0].im) / im_tmp;
          temp_im = (e_y[0].im - datPowBst * e_y[0].re) / im_tmp;
        } else if (b_bim == brm) {
          if (e_y[1].re > 0.0) {
            datPowBst = 0.5;
          } else {
            datPowBst = -0.5;
          }

          if (e_y[1].im > 0.0) {
            im_tmp = 0.5;
          } else {
            im_tmp = -0.5;
          }

          temp_re = (e_y[0].re * datPowBst + e_y[0].im * im_tmp) / brm;
          temp_im = (e_y[0].im * datPowBst - e_y[0].re * im_tmp) / brm;
        } else {
          datPowBst = e_y[1].re / e_y[1].im;
          im_tmp = e_y[1].im + datPowBst * e_y[1].re;
          temp_re = (datPowBst * e_y[0].re + e_y[0].im) / im_tmp;
          temp_im = (datPowBst * e_y[0].im - e_y[0].re) / im_tmp;
        }

        im_tmp = (temp_re * e_y[3].re - temp_im * e_y[3].im) - e_y[2].re;
        b_r = (temp_re * e_y[3].im + temp_im * e_y[3].re) - e_y[2].im;
        if (b_r == 0.0) {
          t_re = 1.0 / im_tmp;
          t_im = 0.0;
        } else if (im_tmp == 0.0) {
          t_re = 0.0;
          t_im = -(1.0 / b_r);
        } else {
          datAmp = fabs(im_tmp);
          bim = fabs(b_r);
          if (datAmp > bim) {
            datPowBst = b_r / im_tmp;
            im_tmp += datPowBst * b_r;
            t_re = (1.0 + datPowBst * 0.0) / im_tmp;
            t_im = (0.0 - datPowBst) / im_tmp;
          } else if (bim == datAmp) {
            if (im_tmp > 0.0) {
              datPowBst = 0.5;
            } else {
              datPowBst = -0.5;
            }

            if (b_r > 0.0) {
              im_tmp = 0.5;
            } else {
              im_tmp = -0.5;
            }

            t_re = (datPowBst + 0.0 * im_tmp) / datAmp;
            t_im = (0.0 * datPowBst - im_tmp) / datAmp;
          } else {
            datPowBst = im_tmp / b_r;
            im_tmp = b_r + datPowBst * im_tmp;
            t_re = datPowBst / im_tmp;
            t_im = (datPowBst * 0.0 - 1.0) / im_tmp;
          }
        }

        if (e_y[1].im == 0.0) {
          if (e_y[3].im == 0.0) {
            b_r = e_y[3].re / e_y[1].re;
            im_tmp = 0.0;
          } else if (e_y[3].re == 0.0) {
            b_r = 0.0;
            im_tmp = e_y[3].im / e_y[1].re;
          } else {
            b_r = e_y[3].re / e_y[1].re;
            im_tmp = e_y[3].im / e_y[1].re;
          }
        } else if (e_y[1].re == 0.0) {
          if (e_y[3].re == 0.0) {
            b_r = e_y[3].im / e_y[1].im;
            im_tmp = 0.0;
          } else if (e_y[3].im == 0.0) {
            b_r = 0.0;
            im_tmp = -(e_y[3].re / e_y[1].im);
          } else {
            b_r = e_y[3].im / e_y[1].im;
            im_tmp = -(e_y[3].re / e_y[1].im);
          }
        } else if (brm > b_bim) {
          datPowBst = e_y[1].im / e_y[1].re;
          im_tmp = e_y[1].re + datPowBst * e_y[1].im;
          b_r = (e_y[3].re + datPowBst * e_y[3].im) / im_tmp;
          im_tmp = (e_y[3].im - datPowBst * e_y[3].re) / im_tmp;
        } else if (b_bim == brm) {
          if (e_y[1].re > 0.0) {
            datPowBst = 0.5;
          } else {
            datPowBst = -0.5;
          }

          if (e_y[1].im > 0.0) {
            im_tmp = 0.5;
          } else {
            im_tmp = -0.5;
          }

          b_r = (e_y[3].re * datPowBst + e_y[3].im * im_tmp) / brm;
          im_tmp = (e_y[3].im * datPowBst - e_y[3].re * im_tmp) / brm;
        } else {
          datPowBst = e_y[1].re / e_y[1].im;
          im_tmp = e_y[1].im + datPowBst * e_y[1].re;
          b_r = (datPowBst * e_y[3].re + e_y[3].im) / im_tmp;
          im_tmp = (datPowBst * e_y[3].im - e_y[3].re) / im_tmp;
        }

        b_b[0].re = b_r * t_re - im_tmp * t_im;
        b_b[0].im = b_r * t_im + im_tmp * t_re;
        b_b[1].re = -t_re;
        b_b[1].im = -t_im;
        if (e_y[1].im == 0.0) {
          if (-e_y[2].im == 0.0) {
            b_r = -e_y[2].re / e_y[1].re;
            im_tmp = 0.0;
          } else if (-e_y[2].re == 0.0) {
            b_r = 0.0;
            im_tmp = -e_y[2].im / e_y[1].re;
          } else {
            b_r = -e_y[2].re / e_y[1].re;
            im_tmp = -e_y[2].im / e_y[1].re;
          }
        } else if (e_y[1].re == 0.0) {
          if (-e_y[2].re == 0.0) {
            b_r = -e_y[2].im / e_y[1].im;
            im_tmp = 0.0;
          } else if (-e_y[2].im == 0.0) {
            b_r = 0.0;
            im_tmp = -(-e_y[2].re / e_y[1].im);
          } else {
            b_r = -e_y[2].im / e_y[1].im;
            im_tmp = -(-e_y[2].re / e_y[1].im);
          }
        } else if (brm > b_bim) {
          datPowBst = e_y[1].im / e_y[1].re;
          im_tmp = e_y[1].re + datPowBst * e_y[1].im;
          b_r = (-e_y[2].re + datPowBst * -e_y[2].im) / im_tmp;
          im_tmp = (-e_y[2].im - datPowBst * -e_y[2].re) / im_tmp;
        } else if (b_bim == brm) {
          if (e_y[1].re > 0.0) {
            datPowBst = 0.5;
          } else {
            datPowBst = -0.5;
          }

          if (e_y[1].im > 0.0) {
            im_tmp = 0.5;
          } else {
            im_tmp = -0.5;
          }

          b_r = (-e_y[2].re * datPowBst + -e_y[2].im * im_tmp) / brm;
          im_tmp = (-e_y[2].im * datPowBst - -e_y[2].re * im_tmp) / brm;
        } else {
          datPowBst = e_y[1].re / e_y[1].im;
          im_tmp = e_y[1].im + datPowBst * e_y[1].re;
          b_r = (datPowBst * -e_y[2].re + -e_y[2].im) / im_tmp;
          im_tmp = (datPowBst * -e_y[2].im - (-e_y[2].re)) / im_tmp;
        }

        b_b[2].re = b_r * t_re - im_tmp * t_im;
        b_b[2].im = b_r * t_im + im_tmp * t_re;
        b_b[3].re = temp_re * t_re - temp_im * t_im;
        b_b[3].im = temp_re * t_im + temp_im * t_re;
      } else {
        if (e_y[0].im == 0.0) {
          if (e_y[1].im == 0.0) {
            temp_re = e_y[1].re / e_y[0].re;
            temp_im = 0.0;
          } else if (e_y[1].re == 0.0) {
            temp_re = 0.0;
            temp_im = e_y[1].im / e_y[0].re;
          } else {
            temp_re = e_y[1].re / e_y[0].re;
            temp_im = e_y[1].im / e_y[0].re;
          }
        } else if (e_y[0].re == 0.0) {
          if (e_y[1].re == 0.0) {
            temp_re = e_y[1].im / e_y[0].im;
            temp_im = 0.0;
          } else if (e_y[1].im == 0.0) {
            temp_re = 0.0;
            temp_im = -(e_y[1].re / e_y[0].im);
          } else {
            temp_re = e_y[1].im / e_y[0].im;
            temp_im = -(e_y[1].re / e_y[0].im);
          }
        } else if (datAmp > bim) {
          datPowBst = e_y[0].im / e_y[0].re;
          im_tmp = e_y[0].re + datPowBst * e_y[0].im;
          temp_re = (e_y[1].re + datPowBst * e_y[1].im) / im_tmp;
          temp_im = (e_y[1].im - datPowBst * e_y[1].re) / im_tmp;
        } else if (bim == datAmp) {
          if (e_y[0].re > 0.0) {
            datPowBst = 0.5;
          } else {
            datPowBst = -0.5;
          }

          if (e_y[0].im > 0.0) {
            im_tmp = 0.5;
          } else {
            im_tmp = -0.5;
          }

          temp_re = (e_y[1].re * datPowBst + e_y[1].im * im_tmp) / datAmp;
          temp_im = (e_y[1].im * datPowBst - e_y[1].re * im_tmp) / datAmp;
        } else {
          datPowBst = e_y[0].re / e_y[0].im;
          im_tmp = e_y[0].im + datPowBst * e_y[0].re;
          temp_re = (datPowBst * e_y[1].re + e_y[1].im) / im_tmp;
          temp_im = (datPowBst * e_y[1].im - e_y[1].re) / im_tmp;
        }

        im_tmp = e_y[3].re - (temp_re * e_y[2].re - temp_im * e_y[2].im);
        b_r = e_y[3].im - (temp_re * e_y[2].im + temp_im * e_y[2].re);
        if (b_r == 0.0) {
          t_re = 1.0 / im_tmp;
          t_im = 0.0;
        } else if (im_tmp == 0.0) {
          t_re = 0.0;
          t_im = -(1.0 / b_r);
        } else {
          brm = fabs(im_tmp);
          b_bim = fabs(b_r);
          if (brm > b_bim) {
            datPowBst = b_r / im_tmp;
            im_tmp += datPowBst * b_r;
            t_re = (1.0 + datPowBst * 0.0) / im_tmp;
            t_im = (0.0 - datPowBst) / im_tmp;
          } else if (b_bim == brm) {
            if (im_tmp > 0.0) {
              datPowBst = 0.5;
            } else {
              datPowBst = -0.5;
            }

            if (b_r > 0.0) {
              im_tmp = 0.5;
            } else {
              im_tmp = -0.5;
            }

            t_re = (datPowBst + 0.0 * im_tmp) / brm;
            t_im = (0.0 * datPowBst - im_tmp) / brm;
          } else {
            datPowBst = im_tmp / b_r;
            im_tmp = b_r + datPowBst * im_tmp;
            t_re = datPowBst / im_tmp;
            t_im = (datPowBst * 0.0 - 1.0) / im_tmp;
          }
        }

        if (e_y[0].im == 0.0) {
          if (e_y[3].im == 0.0) {
            b_r = e_y[3].re / e_y[0].re;
            im_tmp = 0.0;
          } else if (e_y[3].re == 0.0) {
            b_r = 0.0;
            im_tmp = e_y[3].im / e_y[0].re;
          } else {
            b_r = e_y[3].re / e_y[0].re;
            im_tmp = e_y[3].im / e_y[0].re;
          }
        } else if (e_y[0].re == 0.0) {
          if (e_y[3].re == 0.0) {
            b_r = e_y[3].im / e_y[0].im;
            im_tmp = 0.0;
          } else if (e_y[3].im == 0.0) {
            b_r = 0.0;
            im_tmp = -(e_y[3].re / e_y[0].im);
          } else {
            b_r = e_y[3].im / e_y[0].im;
            im_tmp = -(e_y[3].re / e_y[0].im);
          }
        } else if (datAmp > bim) {
          datPowBst = e_y[0].im / e_y[0].re;
          im_tmp = e_y[0].re + datPowBst * e_y[0].im;
          b_r = (e_y[3].re + datPowBst * e_y[3].im) / im_tmp;
          im_tmp = (e_y[3].im - datPowBst * e_y[3].re) / im_tmp;
        } else if (bim == datAmp) {
          if (e_y[0].re > 0.0) {
            datPowBst = 0.5;
          } else {
            datPowBst = -0.5;
          }

          if (e_y[0].im > 0.0) {
            im_tmp = 0.5;
          } else {
            im_tmp = -0.5;
          }

          b_r = (e_y[3].re * datPowBst + e_y[3].im * im_tmp) / datAmp;
          im_tmp = (e_y[3].im * datPowBst - e_y[3].re * im_tmp) / datAmp;
        } else {
          datPowBst = e_y[0].re / e_y[0].im;
          im_tmp = e_y[0].im + datPowBst * e_y[0].re;
          b_r = (datPowBst * e_y[3].re + e_y[3].im) / im_tmp;
          im_tmp = (datPowBst * e_y[3].im - e_y[3].re) / im_tmp;
        }

        b_b[0].re = b_r * t_re - im_tmp * t_im;
        b_b[0].im = b_r * t_im + im_tmp * t_re;
        b_b[1].re = -temp_re * t_re - -temp_im * t_im;
        b_b[1].im = -temp_re * t_im + -temp_im * t_re;
        if (e_y[0].im == 0.0) {
          if (-e_y[2].im == 0.0) {
            b_r = -e_y[2].re / e_y[0].re;
            im_tmp = 0.0;
          } else if (-e_y[2].re == 0.0) {
            b_r = 0.0;
            im_tmp = -e_y[2].im / e_y[0].re;
          } else {
            b_r = -e_y[2].re / e_y[0].re;
            im_tmp = -e_y[2].im / e_y[0].re;
          }
        } else if (e_y[0].re == 0.0) {
          if (-e_y[2].re == 0.0) {
            b_r = -e_y[2].im / e_y[0].im;
            im_tmp = 0.0;
          } else if (-e_y[2].im == 0.0) {
            b_r = 0.0;
            im_tmp = -(-e_y[2].re / e_y[0].im);
          } else {
            b_r = -e_y[2].im / e_y[0].im;
            im_tmp = -(-e_y[2].re / e_y[0].im);
          }
        } else if (datAmp > bim) {
          datPowBst = e_y[0].im / e_y[0].re;
          im_tmp = e_y[0].re + datPowBst * e_y[0].im;
          b_r = (-e_y[2].re + datPowBst * -e_y[2].im) / im_tmp;
          im_tmp = (-e_y[2].im - datPowBst * -e_y[2].re) / im_tmp;
        } else if (bim == datAmp) {
          if (e_y[0].re > 0.0) {
            datPowBst = 0.5;
          } else {
            datPowBst = -0.5;
          }

          if (e_y[0].im > 0.0) {
            im_tmp = 0.5;
          } else {
            im_tmp = -0.5;
          }

          b_r = (-e_y[2].re * datPowBst + -e_y[2].im * im_tmp) / datAmp;
          im_tmp = (-e_y[2].im * datPowBst - -e_y[2].re * im_tmp) / datAmp;
        } else {
          datPowBst = e_y[0].re / e_y[0].im;
          im_tmp = e_y[0].im + datPowBst * e_y[0].re;
          b_r = (datPowBst * -e_y[2].re + -e_y[2].im) / im_tmp;
          im_tmp = (datPowBst * -e_y[2].im - (-e_y[2].re)) / im_tmp;
        }

        b_b[2].re = b_r * t_re - im_tmp * t_im;
        b_b[2].im = b_r * t_im + im_tmp * t_re;
        b_b[3].re = t_re;
        b_b[3].im = t_im;
      }

      i2 = datA->size[0];
      i3 = b_y->size[0] * b_y->size[1];
      b_y->size[0] = datA->size[0];
      b_y->size[1] = 2;
      emxEnsureCapacity_creal_T(b_y, i3);
      for (b_i = 0; b_i < i2; b_i++) {
        b_y->data[b_i].re = 0.0;
        b_y->data[b_i].im = 0.0;
      }

      temp_re = b_b[0].re;
      temp_im = b_b[0].im;
      for (b_i = 0; b_i < i2; b_i++) {
        b_y->data[b_i].re += temp_re * datA->data[b_i].re - temp_im * datA->
          data[b_i].im;
        b_y->data[b_i].im += temp_re * datA->data[b_i].im + temp_im * datA->
          data[b_i].re;
      }

      temp_re = b_b[1].re;
      temp_im = b_b[1].im;
      for (b_i = 0; b_i < i2; b_i++) {
        temp_re_tmp = i2 + b_i;
        b_y->data[b_i].re += temp_re * datA->data[temp_re_tmp].re - temp_im *
          datA->data[temp_re_tmp].im;
        b_y->data[b_i].im += temp_re * datA->data[temp_re_tmp].im + temp_im *
          datA->data[temp_re_tmp].re;
      }

      for (b_i = 0; b_i < i2; b_i++) {
        i3 = i2 + b_i;
        b_y->data[i3].re = 0.0;
        b_y->data[i3].im = 0.0;
      }

      temp_re = b_b[2].re;
      temp_im = b_b[2].im;
      for (b_i = 0; b_i < i2; b_i++) {
        i3 = i2 + b_i;
        b_y->data[i3].re += temp_re * datA->data[b_i].re - temp_im * datA->
          data[b_i].im;
        b_y->data[i3].im += temp_re * datA->data[b_i].im + temp_im * datA->
          data[b_i].re;
      }

      temp_re = b_b[3].re;
      temp_im = b_b[3].im;
      for (b_i = 0; b_i < i2; b_i++) {
        temp_re_tmp = i2 + b_i;
        i3 = i2 + b_i;
        b_y->data[i3].re += temp_re * datA->data[temp_re_tmp].re - temp_im *
          datA->data[temp_re_tmp].im;
        b_y->data[i3].im += temp_re * datA->data[temp_re_tmp].im + temp_im *
          datA->data[temp_re_tmp].re;
      }

      i2 = a->size[0] * a->size[1];
      a->size[0] = 2;
      loop_ub = datA->size[0];
      a->size[1] = datA->size[0];
      emxEnsureCapacity_creal_T(a, i2);
      for (i2 = 0; i2 < loop_ub; i2++) {
        a->data[2 * i2].re = datA->data[i2].re;
        a->data[2 * i2].im = -datA->data[i2].im;
        i3 = 2 * i2 + 1;
        a->data[i3].re = datA->data[i2 + datA->size[0]].re;
        a->data[i3].im = -datA->data[i2 + datA->size[0]].im;
      }

      aoffset = b_y->size[0];
      i2 = a->size[1];
      i3 = datP->size[0] * datP->size[1];
      datP->size[0] = b_y->size[0];
      datP->size[1] = a->size[1];
      emxEnsureCapacity_creal_T(datP, i3);
      for (loop_ub = 0; loop_ub < i2; loop_ub++) {
        coffset = loop_ub * aoffset;
        boffset = loop_ub << 1;
        for (b_i = 0; b_i < aoffset; b_i++) {
          i3 = coffset + b_i;
          datP->data[i3].re = 0.0;
          datP->data[i3].im = 0.0;
        }

        temp_re = a->data[boffset].re;
        temp_im = a->data[boffset].im;
        for (b_i = 0; b_i < aoffset; b_i++) {
          i3 = coffset + b_i;
          datP->data[i3].re += temp_re * b_y->data[b_i].re - temp_im * b_y->
            data[b_i].im;
          datP->data[i3].im += temp_re * b_y->data[b_i].im + temp_im * b_y->
            data[b_i].re;
        }

        temp_re = a->data[boffset + 1].re;
        temp_im = a->data[boffset + 1].im;
        for (b_i = 0; b_i < aoffset; b_i++) {
          temp_re_tmp = aoffset + b_i;
          i3 = coffset + b_i;
          datP->data[i3].re += temp_re * b_y->data[temp_re_tmp].re - temp_im *
            b_y->data[temp_re_tmp].im;
          datP->data[i3].im += temp_re * b_y->data[temp_re_tmp].im + temp_im *
            b_y->data[temp_re_tmp].re;
        }
      }

      /* 'survey_dml_core_2:32' datPTst(:, :, idxAng0, idxAng1) = datP; */
      loop_ub = datP->size[1];
      for (i2 = 0; i2 < loop_ub; i2++) {
        nx = datP->size[0];
        for (i3 = 0; i3 < nx; i3++) {
          datPTst->data[((i3 + datPTst->size[0] * i2) + datPTst->size[0] *
                         datPTst->size[1] * idxAng0) + datPTst->size[0] *
            datPTst->size[1] * datPTst->size[2] * ((int)b_idxAng1 - 1)] =
            datP->data[i3 + datP->size[0] * i2];
        }
      }
    }
  }

  emxFree_creal_T(&a);
  emxFree_creal_T(&b_y);
  emxFree_creal_T(&datA);
  emxFree_real_T(&DATA_ANG_TST);

  /*  traverse */
  /* 'survey_dml_core_2:38' idxAngRnd = zeros(2, NUMB_RND); */
  i = idxAngRnd->size[0] * idxAngRnd->size[1];
  idxAngRnd->size[0] = 2;
  i1 = (int)NUMB_RND;
  idxAngRnd->size[1] = i1;
  emxEnsureCapacity_real_T(idxAngRnd, i);
  loop_ub = i1 << 1;
  for (i = 0; i < loop_ub; i++) {
    idxAngRnd->data[i] = 0.0;
  }

  /* 'survey_dml_core_2:39' for idxRnd = 1:NUMB_RND */
  emxInit_creal_T(&datSig, 1);
  emxInit_creal_T(&datPowTmp, 2);
  emxInit_creal_T(&noise, 2);
  emxInit_real_T(&r, 2);
  emxInit_real_T(&b, 2);
  emxInit_real_T(&c_y, 1);
  emxInit_real_T(&d_y, 1);
  emxInit_creal_T(&A, 2);
  for (idxRnd = 0; idxRnd < i1; idxRnd++) {
    /*     %% prepare datSig */
    /* 'survey_dml_core_2:41' datSig = complex(zeros(NUMB_ANT, 1)); */
    i = datSig->size[0];
    datSig->size[0] = NUMB_ANT;
    emxEnsureCapacity_creal_T(datSig, i);
    for (i = 0; i < NUMB_ANT; i++) {
      datSig->data[i].re = 0.0;
      datSig->data[i].im = 0.0;
    }

    /* 'survey_dml_core_2:42' for idxObj = 1:NUMB_OBJ */
    loop_ub = DATA_DIS_ANT->size[0];

    /* 'survey_dml_core_2:43' datAmp = 10^(DATA_POW_OBJ(idxObj) / 20) / 2^0.5; */
    datAmp = rt_powd_snf(10.0, DATA_POW_OBJ[0] / 20.0) / 1.4142135623730951;

    /* 'survey_dml_core_2:44' datPha = exp(-1i * 2 * pi * rand); */
    t_im = b_rand();
    temp_re = t_im * -0.0;
    temp_im = t_im * -6.2831853071795862;
    if (temp_im == 0.0) {
      temp_re = exp(temp_re);
      temp_im = 0.0;
    } else {
      b_r = exp(temp_re / 2.0);
      temp_re = b_r * (b_r * cos(temp_im));
      temp_im = b_r * (b_r * sin(temp_im));
    }

    /* 'survey_dml_core_2:45' datSig = datSig + datAmp .* exp(1i * 2 * pi * DATA_DIS_ANT * sin(DATA_ANG_OBJ(idxObj) / 180 * pi)) * datPha; */
    t_im = sin(DATA_ANG_OBJ[0] / 180.0 * 3.1415926535897931);
    i = y->size[0];
    y->size[0] = DATA_DIS_ANT->size[0];
    emxEnsureCapacity_creal_T(y, i);
    for (i = 0; i < loop_ub; i++) {
      y->data[i].re = t_im * (DATA_DIS_ANT->data[i] * 0.0);
      y->data[i].im = t_im * (DATA_DIS_ANT->data[i] * 6.2831853071795862);
    }

    nx = y->size[0];
    for (k = 0; k < nx; k++) {
      if (y->data[k].im == 0.0) {
        y->data[k].re = exp(y->data[k].re);
        y->data[k].im = 0.0;
      } else if (rtIsInf(y->data[k].im) && rtIsInf(y->data[k].re) && (y->data[k]
                  .re < 0.0)) {
        y->data[k].re = 0.0;
        y->data[k].im = 0.0;
      } else {
        b_r = exp(y->data[k].re / 2.0);
        y->data[k].re = b_r * (b_r * cos(y->data[k].im));
        y->data[k].im = b_r * (b_r * sin(y->data[k].im));
      }
    }

    nx = datSig->size[0];
    for (i = 0; i < nx; i++) {
      datPowBst = datAmp * y->data[i].re;
      im_tmp = datAmp * y->data[i].im;
      datSig->data[i].re += datPowBst * temp_re - im_tmp * temp_im;
      datSig->data[i].im += datPowBst * temp_im + im_tmp * temp_re;
    }

    /* 'survey_dml_core_2:43' datAmp = 10^(DATA_POW_OBJ(idxObj) / 20) / 2^0.5; */
    datAmp = rt_powd_snf(10.0, DATA_POW_OBJ[1] / 20.0) / 1.4142135623730951;

    /* 'survey_dml_core_2:44' datPha = exp(-1i * 2 * pi * rand); */
    t_im = b_rand();
    temp_re = t_im * -0.0;
    temp_im = t_im * -6.2831853071795862;
    if (temp_im == 0.0) {
      temp_re = exp(temp_re);
      temp_im = 0.0;
    } else {
      b_r = exp(temp_re / 2.0);
      temp_re = b_r * (b_r * cos(temp_im));
      temp_im = b_r * (b_r * sin(temp_im));
    }

    /* 'survey_dml_core_2:45' datSig = datSig + datAmp .* exp(1i * 2 * pi * DATA_DIS_ANT * sin(DATA_ANG_OBJ(idxObj) / 180 * pi)) * datPha; */
    t_im = sin(DATA_ANG_OBJ[1] / 180.0 * 3.1415926535897931);
    i = y->size[0];
    y->size[0] = DATA_DIS_ANT->size[0];
    emxEnsureCapacity_creal_T(y, i);
    for (i = 0; i < loop_ub; i++) {
      y->data[i].re = t_im * (DATA_DIS_ANT->data[i] * 0.0);
      y->data[i].im = t_im * (DATA_DIS_ANT->data[i] * 6.2831853071795862);
    }

    nx = y->size[0];
    for (k = 0; k < nx; k++) {
      if (y->data[k].im == 0.0) {
        y->data[k].re = exp(y->data[k].re);
        y->data[k].im = 0.0;
      } else if (rtIsInf(y->data[k].im) && rtIsInf(y->data[k].re) && (y->data[k]
                  .re < 0.0)) {
        y->data[k].re = 0.0;
        y->data[k].im = 0.0;
      } else {
        b_r = exp(y->data[k].re / 2.0);
        y->data[k].re = b_r * (b_r * cos(y->data[k].im));
        y->data[k].im = b_r * (b_r * sin(y->data[k].im));
      }
    }

    nx = datSig->size[0];
    for (i = 0; i < nx; i++) {
      datPowBst = datAmp * y->data[i].re;
      im_tmp = datAmp * y->data[i].im;
      datSig->data[i].re += datPowBst * temp_re - im_tmp * temp_im;
      datSig->data[i].im += datPowBst * temp_im + im_tmp * temp_re;
    }

    /*     %% prepare datR */
    /* 'survey_dml_core_2:50' datSmpSum = complex(zeros(NUMB_ANT, 1)); */
    i = datSmpSum->size[0];
    datSmpSum->size[0] = NUMB_ANT;
    emxEnsureCapacity_creal_T(datSmpSum, i);
    for (i = 0; i < NUMB_ANT; i++) {
      datSmpSum->data[i].re = 0.0;
      datSmpSum->data[i].im = 0.0;
    }

    /* 'survey_dml_core_2:51' for idxSmp = 1:NUMB_SMP */
    i = (int)NUMB_SMP;
    for (aoffset = 0; aoffset < i; aoffset++) {
      /* 'survey_dml_core_2:52' datSmpTmp = awgn(datSig, DATA_SNR, 'measured'); */
      nx = datSig->size[0];
      i2 = c_y->size[0];
      c_y->size[0] = datSig->size[0];
      emxEnsureCapacity_real_T(c_y, i2);
      for (k = 0; k < nx; k++) {
        c_y->data[k] = rt_hypotd_snf(datSig->data[k].re, datSig->data[k].im);
      }

      i2 = c_y->size[0];
      i3 = d_y->size[0];
      d_y->size[0] = c_y->size[0];
      emxEnsureCapacity_real_T(d_y, i3);
      for (k = 0; k < i2; k++) {
        d_y->data[k] = rt_powd_snf(c_y->data[k], 2.0);
      }

      i2 = d_y->size[0];
      if (d_y->size[0] == 0) {
        b_r = 0.0;
      } else {
        b_r = d_y->data[0];
        for (k = 2; k <= i2; k++) {
          b_r += d_y->data[k - 1];
        }
      }

      b_datSig[0] = datSig->size[0];
      b_datSig[1] = 1.0;
      randn(b_datSig, r);
      b_datSig[0] = datSig->size[0];
      b_datSig[1] = 1.0;
      randn(b_datSig, b);
      b_r = sqrt(b_r / (double)datSig->size[0] / rt_powd_snf(10.0, DATA_SNR /
                  10.0) / 2.0);
      i2 = noise->size[0] * noise->size[1];
      noise->size[0] = r->size[0];
      noise->size[1] = 1;
      emxEnsureCapacity_creal_T(noise, i2);
      loop_ub = r->size[0] * r->size[1];
      for (i2 = 0; i2 < loop_ub; i2++) {
        noise->data[i2].re = b_r * (r->data[i2] + 0.0 * b->data[i2]);
        noise->data[i2].im = b_r * b->data[i2];
      }

      /* 'survey_dml_core_2:53' datSmpTmp = datSmpTmp .* DATA_COE_WIN; */
      /* 'survey_dml_core_2:54' datSmpSum = datSmpSum + datSmpTmp; */
      loop_ub = datSmpSum->size[0];
      for (i2 = 0; i2 < loop_ub; i2++) {
        datSmpSum->data[i2].re += (datSig->data[i2].re + noise->data[i2].re) *
          DATA_COE_WIN->data[i2];
        datSmpSum->data[i2].im += (datSig->data[i2].im + noise->data[i2].im) *
          DATA_COE_WIN->data[i2];
      }
    }

    /* 'survey_dml_core_2:56' datX = datSmpSum / NUMB_SMP; */
    loop_ub = datSmpSum->size[0];
    for (i = 0; i < loop_ub; i++) {
      if (datSmpSum->data[i].im == 0.0) {
        b_r = datSmpSum->data[i].re / NUMB_SMP;
        datPowBst = 0.0;
      } else if (datSmpSum->data[i].re == 0.0) {
        b_r = 0.0;
        datPowBst = datSmpSum->data[i].im / NUMB_SMP;
      } else {
        b_r = datSmpSum->data[i].re / NUMB_SMP;
        datPowBst = datSmpSum->data[i].im / NUMB_SMP;
      }

      datSmpSum->data[i].re = b_r;
      datSmpSum->data[i].im = datPowBst;
    }

    /* 'survey_dml_core_2:57' datR = datX * datX'; */
    loop_ub = datSmpSum->size[0];
    i = datP->size[0] * datP->size[1];
    datP->size[0] = datSmpSum->size[0];
    datP->size[1] = datSmpSum->size[0];
    emxEnsureCapacity_creal_T(datP, i);
    for (i = 0; i < loop_ub; i++) {
      nx = datSmpSum->size[0];
      for (i2 = 0; i2 < nx; i2++) {
        datP->data[i2 + datP->size[0] * i].re = datSmpSum->data[i2].re *
          datSmpSum->data[i].re - datSmpSum->data[i2].im * -datSmpSum->data[i].
          im;
        datP->data[i2 + datP->size[0] * i].im = datSmpSum->data[i2].re *
          -datSmpSum->data[i].im + datSmpSum->data[i2].im * datSmpSum->data[i].
          re;
      }
    }

    /*     %% get best cost and index */
    /* 'survey_dml_core_2:61' datPowTst = zeros(NUMB_ANG_TST, NUMB_ANG_TST) - inf; */
    /* 'survey_dml_core_2:62' datPowBst = -inf; */
    datPowBst = rtMinusInf;

    /* 'survey_dml_core_2:63' for idxAng0 = 1:NUMB_ANG_TST */
    i = NUMB_ANG_TST + 1;
    for (idxAng0 = 0; idxAng0 <= i; idxAng0++) {
      /* 'survey_dml_core_2:64' for idxAng1 = idxAng0+1:NUMB_ANG_TST */
      i2 = NUMB_ANG_TST - idxAng0;
      for (idxAng1 = 0; idxAng1 <= i2; idxAng1++) {
        b_idxAng1 = ((unsigned int)idxAng0 + idxAng1) + 2U;

        /* 'survey_dml_core_2:65' datPowTmp = datPTst(:, :, idxAng0, idxAng1) * datR; */
        loop_ub = datPTst->size[1];
        if ((datPTst->size[1] == 1) || (datP->size[0] == 1)) {
          nx = datPTst->size[0];
          i3 = datPowTmp->size[0] * datPowTmp->size[1];
          datPowTmp->size[0] = datPTst->size[0];
          datPowTmp->size[1] = datP->size[1];
          emxEnsureCapacity_creal_T(datPowTmp, i3);
          for (i3 = 0; i3 < nx; i3++) {
            aoffset = datP->size[1];
            for (i4 = 0; i4 < aoffset; i4++) {
              datPowTmp->data[i3 + datPowTmp->size[0] * i4].re = 0.0;
              datPowTmp->data[i3 + datPowTmp->size[0] * i4].im = 0.0;
              for (i5 = 0; i5 < loop_ub; i5++) {
                b_i = (int)b_idxAng1 - 1;
                datPowTmp->data[i3 + datPowTmp->size[0] * i4].re +=
                  datPTst->data[((i3 + datPTst->size[0] * i5) + datPTst->size[0]
                                 * datPTst->size[1] * idxAng0) + datPTst->size[0]
                  * datPTst->size[1] * datPTst->size[2] * b_i].re * datP->
                  data[i5 + datP->size[0] * i4].re - datPTst->data[((i3 +
                  datPTst->size[0] * i5) + datPTst->size[0] * datPTst->size[1] *
                  idxAng0) + datPTst->size[0] * datPTst->size[1] * datPTst->
                  size[2] * b_i].im * datP->data[i5 + datP->size[0] * i4].im;
                datPowTmp->data[i3 + datPowTmp->size[0] * i4].im +=
                  datPTst->data[((i3 + datPTst->size[0] * i5) + datPTst->size[0]
                                 * datPTst->size[1] * idxAng0) + datPTst->size[0]
                  * datPTst->size[1] * datPTst->size[2] * b_i].re * datP->
                  data[i5 + datP->size[0] * i4].im + datPTst->data[((i3 +
                  datPTst->size[0] * i5) + datPTst->size[0] * datPTst->size[1] *
                  idxAng0) + datPTst->size[0] * datPTst->size[1] * datPTst->
                  size[2] * b_i].im * datP->data[i5 + datP->size[0] * i4].re;
              }
            }
          }
        } else {
          nx = datPTst->size[0];
          i3 = A->size[0] * A->size[1];
          A->size[0] = datPTst->size[0];
          A->size[1] = datPTst->size[1];
          emxEnsureCapacity_creal_T(A, i3);
          for (i3 = 0; i3 < loop_ub; i3++) {
            for (i4 = 0; i4 < nx; i4++) {
              A->data[i4 + A->size[0] * i3] = datPTst->data[((i4 + datPTst->
                size[0] * i3) + datPTst->size[0] * datPTst->size[1] * idxAng0) +
                datPTst->size[0] * datPTst->size[1] * datPTst->size[2] * ((int)
                b_idxAng1 - 1)];
            }
          }

          i3 = datPTst->size[0];
          i4 = datPTst->size[1];
          i5 = datP->size[1];
          nx = datPowTmp->size[0] * datPowTmp->size[1];
          datPowTmp->size[0] = datPTst->size[0];
          datPowTmp->size[1] = datP->size[1];
          emxEnsureCapacity_creal_T(datPowTmp, nx);
          for (loop_ub = 0; loop_ub < i5; loop_ub++) {
            coffset = loop_ub * i3;
            boffset = loop_ub * i4;
            for (b_i = 0; b_i < i3; b_i++) {
              nx = coffset + b_i;
              datPowTmp->data[nx].re = 0.0;
              datPowTmp->data[nx].im = 0.0;
            }

            for (k = 0; k < i4; k++) {
              aoffset = k * i3;
              temp_re_tmp = boffset + k;
              temp_re = datP->data[temp_re_tmp].re;
              temp_im = datP->data[temp_re_tmp].im;
              for (b_i = 0; b_i < i3; b_i++) {
                temp_re_tmp = aoffset + b_i;
                nx = coffset + b_i;
                datPowTmp->data[nx].re += temp_re * A->data[temp_re_tmp].re -
                  temp_im * A->data[temp_re_tmp].im;
                datPowTmp->data[nx].im += temp_re * A->data[temp_re_tmp].im +
                  temp_im * A->data[temp_re_tmp].re;
              }
            }
          }
        }

        /* 'survey_dml_core_2:66' datPow = 20 * log10(abs(trace(datPowTmp))); */
        t_re = 0.0;
        t_im = 0.0;
        i3 = datPowTmp->size[0];
        for (k = 0; k < i3; k++) {
          t_re += datPowTmp->data[k + datPowTmp->size[0] * k].re;
          t_im += datPowTmp->data[k + datPowTmp->size[0] * k].im;
        }

        b_r = 20.0 * log10(rt_hypotd_snf(t_re, t_im));

        /* datPowTmpA = datPTst(:, :, idxAng0, idxAng1) * datRTmp; */
        /* datPowA = 20 * log10(norm(datPowTmpA)); */
        /* 'survey_dml_core_2:69' if datPow > datPowBst */
        if (b_r > datPowBst) {
          /* 'survey_dml_core_2:70' datPowBst = datPow; */
          datPowBst = b_r;

          /* 'survey_dml_core_2:71' idxAngRnd(:, idxRnd) = [idxAng0, idxAng1]; */
          idxAngRnd->data[2 * idxRnd] = (unsigned int)(idxAng0 + 1);
          idxAngRnd->data[2 * idxRnd + 1] = b_idxAng1;
        }

        /* 'survey_dml_core_2:73' datPowTst(idxAng0, idxAng1) = datPow; */
      }
    }
  }

  emxFree_creal_T(&A);
  emxFree_real_T(&d_y);
  emxFree_real_T(&c_y);
  emxFree_real_T(&b);
  emxFree_real_T(&r);
  emxFree_creal_T(&noise);
  emxFree_creal_T(&y);
  emxFree_creal_T(&datPowTmp);
  emxFree_creal_T(&datSmpSum);
  emxFree_creal_T(&datSig);
  emxFree_creal_T(&datP);
  emxFree_creal_T(&datPTst);
}

/*
 * File trailer for survey_dml_core_2.c
 *
 * [EOF]
 */

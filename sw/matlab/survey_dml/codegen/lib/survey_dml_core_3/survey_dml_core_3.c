/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: survey_dml_core_3.c
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 05-Jun-2020 14:33:42
 */

/* Include Files */
#include "survey_dml_core_3.h"
#include "colon.h"
#include "rand.h"
#include "randn.h"
#include "rt_nonfinite.h"
#include "survey_dml_core_3_data.h"
#include "survey_dml_core_3_emxutil.h"
#include "survey_dml_core_3_initialize.h"
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
 * function idxAngRnd = survey_dml_core_3(NUMB_RND, NUMB_SMP, INDX_FIG, DATA_DIS_ANT, DATA_COE_WIN, DATA_ANG_OBJ, DATA_POW_OBJ, DATA_SNR, DATA_DLT_ANG_TST, DATA_RNG_ANG_TST)
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
void survey_dml_core_3(double NUMB_RND, double NUMB_SMP, double INDX_FIG, const
  emxArray_real_T *DATA_DIS_ANT, const emxArray_real_T *DATA_COE_WIN, const
  double DATA_ANG_OBJ[2], const double DATA_POW_OBJ[2], double DATA_SNR, double
  DATA_DLT_ANG_TST, double DATA_RNG_ANG_TST, emxArray_real_T *idxAngRnd)
{
  emxArray_real_T *DATA_ANG_TST;
  int i;
  emxArray_creal_T *datVTst;
  int loop_ub;
  int NUMB_ANT;
  int NUMB_ANG_TST;
  emxArray_creal_T *datSmpSum;
  int aoffset;
  emxArray_creal_T *datCTst;
  double b;
  int i1;
  int nx;
  int k;
  emxArray_creal_T *datA;
  emxArray_creal_T *datSig;
  double r;
  emxArray_creal_T *a;
  int idxAng0;
  int idxAng1;
  int i2;
  emxArray_creal_T *b_a;
  int b_tmp_tmp;
  emxArray_creal_T *noise;
  double datPow;
  emxArray_real_T *b_r;
  emxArray_real_T *b_b;
  emxArray_real_T *y;
  emxArray_real_T *b_y;
  double t_re;
  double temp_re;
  double temp_im;
  creal_T c_y[4];
  double brm;
  double b_brm;
  double bim;
  double b_bim;
  double datPowBst;
  double b_datSig[2];
  creal_T c_datSig[2];
  creal_T datSig_idx_0;
  unsigned int b_idxAng1;
  if (isInitialized_survey_dml_core_3 == false) {
    survey_dml_core_3_initialize();
  }

  (void)INDX_FIG;

  /* ------------------------------------------------------------------------------- */
  /*  */
  /*   Filename       : survey_dml_core_3.m */
  /*   Author         : Huang Leilei */
  /*   Created        : 2020-06-05 */
  /*   Description    : core calculation of dml */
  /*  */
  /* ------------------------------------------------------------------------------- */
  /* 'survey_dml_core_3:13' DATA_ANG_TST     = -DATA_RNG_ANG_TST:DATA_DLT_ANG_TST:DATA_RNG_ANG_TST; */
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

  emxInit_creal_T(&datVTst, 2);

  /* 'survey_dml_core_3:14' NUMB_ANT         = numel(DATA_DIS_ANT); */
  NUMB_ANT = DATA_DIS_ANT->size[0];

  /* 'survey_dml_core_3:15' NUMB_OBJ         = numel(DATA_ANG_OBJ); */
  /* 'survey_dml_core_3:16' NUMB_ANG_TST     = numel(DATA_ANG_TST); */
  NUMB_ANG_TST = DATA_ANG_TST->size[1] - 1;

  /* *** MAIN BODY ***************************************************************** */
  /*  prepare datV and datC */
  /* 'survey_dml_core_3:23' datVTst = complex(zeros(NUMB_ANT, NUMB_ANG_TST)); */
  i = datVTst->size[0] * datVTst->size[1];
  datVTst->size[0] = DATA_DIS_ANT->size[0];
  datVTst->size[1] = DATA_ANG_TST->size[1];
  emxEnsureCapacity_creal_T(datVTst, i);
  loop_ub = DATA_DIS_ANT->size[0] * DATA_ANG_TST->size[1];
  for (i = 0; i < loop_ub; i++) {
    datVTst->data[i].re = 0.0;
    datVTst->data[i].im = 0.0;
  }

  /* 'survey_dml_core_3:24' for idxAng = 1:NUMB_ANG_TST */
  i = DATA_ANG_TST->size[1];
  emxInit_creal_T(&datSmpSum, 1);
  for (aoffset = 0; aoffset < i; aoffset++) {
    /* 'survey_dml_core_3:25' datAng = DATA_ANG_TST(idxAng); */
    /* 'survey_dml_core_3:26' datVTst(:, idxAng) = exp(1i * 2 * pi * DATA_DIS_ANT * sin(datAng / 180 * pi)); */
    b = sin(DATA_ANG_TST->data[aoffset] / 180.0 * 3.1415926535897931);
    loop_ub = DATA_DIS_ANT->size[0];
    i1 = datSmpSum->size[0];
    datSmpSum->size[0] = DATA_DIS_ANT->size[0];
    emxEnsureCapacity_creal_T(datSmpSum, i1);
    for (i1 = 0; i1 < loop_ub; i1++) {
      datSmpSum->data[i1].re = b * (DATA_DIS_ANT->data[i1] * 0.0);
      datSmpSum->data[i1].im = b * (DATA_DIS_ANT->data[i1] * 6.2831853071795862);
    }

    nx = datSmpSum->size[0];
    for (k = 0; k < nx; k++) {
      if (datSmpSum->data[k].im == 0.0) {
        datSmpSum->data[k].re = exp(datSmpSum->data[k].re);
        datSmpSum->data[k].im = 0.0;
      } else if (rtIsInf(datSmpSum->data[k].im) && rtIsInf(datSmpSum->data[k].re)
                 && (datSmpSum->data[k].re < 0.0)) {
        datSmpSum->data[k].re = 0.0;
        datSmpSum->data[k].im = 0.0;
      } else {
        r = exp(datSmpSum->data[k].re / 2.0);
        datSmpSum->data[k].re = r * (r * cos(datSmpSum->data[k].im));
        datSmpSum->data[k].im = r * (r * sin(datSmpSum->data[k].im));
      }
    }

    loop_ub = datSmpSum->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      datVTst->data[i1 + datVTst->size[0] * aoffset] = datSmpSum->data[i1];
    }
  }

  emxInit_creal_T(&datCTst, 4);

  /* 'survey_dml_core_3:28' datCTst = complex(zeros(2, 2, NUMB_ANG_TST, NUMB_ANG_TST)); */
  i = datCTst->size[0] * datCTst->size[1] * datCTst->size[2] * datCTst->size[3];
  datCTst->size[0] = 2;
  datCTst->size[1] = 2;
  datCTst->size[2] = DATA_ANG_TST->size[1];
  datCTst->size[3] = DATA_ANG_TST->size[1];
  emxEnsureCapacity_creal_T(datCTst, i);
  loop_ub = (DATA_ANG_TST->size[1] << 2) * DATA_ANG_TST->size[1];
  for (i = 0; i < loop_ub; i++) {
    datCTst->data[i].re = 0.0;
    datCTst->data[i].im = 0.0;
  }

  /* 'survey_dml_core_3:29' for idxAng0 = 1:NUMB_ANG_TST */
  i = DATA_ANG_TST->size[1];
  emxInit_creal_T(&datA, 2);
  emxInit_creal_T(&datSig, 1);
  emxInit_creal_T(&a, 2);
  for (idxAng0 = 0; idxAng0 < i; idxAng0++) {
    /* 'survey_dml_core_3:30' datAng0 = DATA_ANG_TST(idxAng0); */
    /* 'survey_dml_core_3:31' for idxAng1 = (idxAng0+1):NUMB_ANG_TST */
    i1 = NUMB_ANG_TST - idxAng0;
    for (idxAng1 = 0; idxAng1 < i1; idxAng1++) {
      /* 'survey_dml_core_3:32' datAng1 = DATA_ANG_TST(idxAng1); */
      /* 'survey_dml_core_3:33' datA = [exp(1i * 2 * pi * DATA_DIS_ANT * sin(datAng0 / 180 * pi))     ... */
      /* 'survey_dml_core_3:34'                ,exp(1i * 2 * pi * DATA_DIS_ANT * sin(datAng1 / 180 * pi))]    ... */
      /* 'survey_dml_core_3:35'         ; */
      b = sin(DATA_ANG_TST->data[idxAng0] / 180.0 * 3.1415926535897931);
      i2 = datSmpSum->size[0];
      datSmpSum->size[0] = DATA_DIS_ANT->size[0];
      emxEnsureCapacity_creal_T(datSmpSum, i2);
      loop_ub = DATA_DIS_ANT->size[0];
      b_tmp_tmp = (int)((unsigned int)idxAng0 + idxAng1) + 1;
      datPow = sin(DATA_ANG_TST->data[b_tmp_tmp] / 180.0 * 3.1415926535897931);
      i2 = datSig->size[0];
      datSig->size[0] = DATA_DIS_ANT->size[0];
      emxEnsureCapacity_creal_T(datSig, i2);
      for (i2 = 0; i2 < loop_ub; i2++) {
        r = DATA_DIS_ANT->data[i2] * 0.0;
        t_re = DATA_DIS_ANT->data[i2] * 6.2831853071795862;
        datSmpSum->data[i2].re = b * r;
        datSmpSum->data[i2].im = b * t_re;
        datSig->data[i2].re = datPow * r;
        datSig->data[i2].im = datPow * t_re;
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
          r = exp(datSmpSum->data[k].re / 2.0);
          datSmpSum->data[k].re = r * (r * cos(datSmpSum->data[k].im));
          datSmpSum->data[k].im = r * (r * sin(datSmpSum->data[k].im));
        }
      }

      nx = datSig->size[0];
      for (k = 0; k < nx; k++) {
        if (datSig->data[k].im == 0.0) {
          datSig->data[k].re = exp(datSig->data[k].re);
          datSig->data[k].im = 0.0;
        } else if (rtIsInf(datSig->data[k].im) && rtIsInf(datSig->data[k].re) &&
                   (datSig->data[k].re < 0.0)) {
          datSig->data[k].re = 0.0;
          datSig->data[k].im = 0.0;
        } else {
          r = exp(datSig->data[k].re / 2.0);
          datSig->data[k].re = r * (r * cos(datSig->data[k].im));
          datSig->data[k].im = r * (r * sin(datSig->data[k].im));
        }
      }

      i2 = datA->size[0] * datA->size[1];
      datA->size[0] = datSmpSum->size[0];
      datA->size[1] = 2;
      emxEnsureCapacity_creal_T(datA, i2);
      loop_ub = datSmpSum->size[0];
      for (i2 = 0; i2 < loop_ub; i2++) {
        datA->data[i2] = datSmpSum->data[i2];
      }

      loop_ub = datSig->size[0];
      for (i2 = 0; i2 < loop_ub; i2++) {
        datA->data[i2 + datA->size[0]] = datSig->data[i2];
      }

      /* 'survey_dml_core_3:36' datC = (datA' * datA)^-1; */
      i2 = a->size[0] * a->size[1];
      a->size[0] = 2;
      loop_ub = datA->size[0];
      a->size[1] = datA->size[0];
      emxEnsureCapacity_creal_T(a, i2);
      for (i2 = 0; i2 < loop_ub; i2++) {
        a->data[2 * i2].re = datA->data[i2].re;
        a->data[2 * i2].im = -datA->data[i2].im;
        nx = 2 * i2 + 1;
        a->data[nx].re = datA->data[i2 + datA->size[0]].re;
        a->data[nx].im = -datA->data[i2 + datA->size[0]].im;
      }

      loop_ub = a->size[1];
      if ((a->size[1] == 1) || (datA->size[0] == 1)) {
        c_y[0].re = 0.0;
        c_y[0].im = 0.0;
        for (i2 = 0; i2 < loop_ub; i2++) {
          c_y[0].re += a->data[2 * i2].re * datA->data[i2].re - a->data[2 * i2].
            im * datA->data[i2].im;
          c_y[0].im += a->data[2 * i2].re * datA->data[i2].im + a->data[2 * i2].
            im * datA->data[i2].re;
        }

        c_y[2].re = 0.0;
        c_y[2].im = 0.0;
        for (i2 = 0; i2 < loop_ub; i2++) {
          c_y[2].re += a->data[2 * i2].re * datA->data[i2 + datA->size[0]].re -
            a->data[2 * i2].im * datA->data[i2 + datA->size[0]].im;
          c_y[2].im += a->data[2 * i2].re * datA->data[i2 + datA->size[0]].im +
            a->data[2 * i2].im * datA->data[i2 + datA->size[0]].re;
        }

        c_y[1].re = 0.0;
        c_y[1].im = 0.0;
        for (i2 = 0; i2 < loop_ub; i2++) {
          nx = 2 * i2 + 1;
          c_y[1].re += a->data[nx].re * datA->data[i2].re - a->data[nx].im *
            datA->data[i2].im;
          c_y[1].im += a->data[nx].re * datA->data[i2].im + a->data[nx].im *
            datA->data[i2].re;
        }

        c_y[3].re = 0.0;
        c_y[3].im = 0.0;
        for (i2 = 0; i2 < loop_ub; i2++) {
          nx = 2 * i2 + 1;
          c_y[3].re += a->data[nx].re * datA->data[i2 + datA->size[0]].re -
            a->data[nx].im * datA->data[i2 + datA->size[0]].im;
          c_y[3].im += a->data[nx].re * datA->data[i2 + datA->size[0]].im +
            a->data[nx].im * datA->data[i2 + datA->size[0]].re;
        }
      } else {
        c_y[0].re = 0.0;
        c_y[0].im = 0.0;
        c_y[1].re = 0.0;
        c_y[1].im = 0.0;
        for (k = 0; k < loop_ub; k++) {
          aoffset = k << 1;
          c_y[0].re += datA->data[k].re * a->data[aoffset].re - datA->data[k].im
            * a->data[aoffset].im;
          c_y[0].im += datA->data[k].re * a->data[aoffset].im + datA->data[k].im
            * a->data[aoffset].re;
          c_y[1].re += datA->data[k].re * a->data[aoffset + 1].re - datA->data[k]
            .im * a->data[aoffset + 1].im;
          c_y[1].im += datA->data[k].re * a->data[aoffset + 1].im + datA->data[k]
            .im * a->data[aoffset + 1].re;
        }

        c_y[2].re = 0.0;
        c_y[2].im = 0.0;
        c_y[3].re = 0.0;
        c_y[3].im = 0.0;
        for (k = 0; k < loop_ub; k++) {
          aoffset = k << 1;
          nx = loop_ub + k;
          c_y[2].re += datA->data[nx].re * a->data[aoffset].re - datA->data[nx].
            im * a->data[aoffset].im;
          c_y[2].im += datA->data[nx].re * a->data[aoffset].im + datA->data[nx].
            im * a->data[aoffset].re;
          c_y[3].re += datA->data[nx].re * a->data[aoffset + 1].re - datA->
            data[nx].im * a->data[aoffset + 1].im;
          c_y[3].im += datA->data[nx].re * a->data[aoffset + 1].im + datA->
            data[nx].im * a->data[aoffset + 1].re;
        }
      }

      brm = fabs(c_y[0].re);
      b_brm = fabs(c_y[1].re);
      bim = fabs(c_y[0].im);
      b_bim = fabs(c_y[1].im);
      if (b_brm + b_bim > brm + bim) {
        if (c_y[1].im == 0.0) {
          if (c_y[0].im == 0.0) {
            temp_re = c_y[0].re / c_y[1].re;
            temp_im = 0.0;
          } else if (c_y[0].re == 0.0) {
            temp_re = 0.0;
            temp_im = c_y[0].im / c_y[1].re;
          } else {
            temp_re = c_y[0].re / c_y[1].re;
            temp_im = c_y[0].im / c_y[1].re;
          }
        } else if (c_y[1].re == 0.0) {
          if (c_y[0].re == 0.0) {
            temp_re = c_y[0].im / c_y[1].im;
            temp_im = 0.0;
          } else if (c_y[0].im == 0.0) {
            temp_re = 0.0;
            temp_im = -(c_y[0].re / c_y[1].im);
          } else {
            temp_re = c_y[0].im / c_y[1].im;
            temp_im = -(c_y[0].re / c_y[1].im);
          }
        } else if (b_brm > b_bim) {
          datPowBst = c_y[1].im / c_y[1].re;
          datPow = c_y[1].re + datPowBst * c_y[1].im;
          temp_re = (c_y[0].re + datPowBst * c_y[0].im) / datPow;
          temp_im = (c_y[0].im - datPowBst * c_y[0].re) / datPow;
        } else if (b_bim == b_brm) {
          if (c_y[1].re > 0.0) {
            datPowBst = 0.5;
          } else {
            datPowBst = -0.5;
          }

          if (c_y[1].im > 0.0) {
            datPow = 0.5;
          } else {
            datPow = -0.5;
          }

          temp_re = (c_y[0].re * datPowBst + c_y[0].im * datPow) / b_brm;
          temp_im = (c_y[0].im * datPowBst - c_y[0].re * datPow) / b_brm;
        } else {
          datPowBst = c_y[1].re / c_y[1].im;
          datPow = c_y[1].im + datPowBst * c_y[1].re;
          temp_re = (datPowBst * c_y[0].re + c_y[0].im) / datPow;
          temp_im = (datPowBst * c_y[0].im - c_y[0].re) / datPow;
        }

        datPow = (temp_re * c_y[3].re - temp_im * c_y[3].im) - c_y[2].re;
        r = (temp_re * c_y[3].im + temp_im * c_y[3].re) - c_y[2].im;
        if (r == 0.0) {
          t_re = 1.0 / datPow;
          r = 0.0;
        } else if (datPow == 0.0) {
          t_re = 0.0;
          r = -(1.0 / r);
        } else {
          brm = fabs(datPow);
          bim = fabs(r);
          if (brm > bim) {
            datPowBst = r / datPow;
            datPow += datPowBst * r;
            t_re = (1.0 + datPowBst * 0.0) / datPow;
            r = (0.0 - datPowBst) / datPow;
          } else if (bim == brm) {
            if (datPow > 0.0) {
              datPowBst = 0.5;
            } else {
              datPowBst = -0.5;
            }

            if (r > 0.0) {
              datPow = 0.5;
            } else {
              datPow = -0.5;
            }

            t_re = (datPowBst + 0.0 * datPow) / brm;
            r = (0.0 * datPowBst - datPow) / brm;
          } else {
            datPowBst = datPow / r;
            datPow = r + datPowBst * datPow;
            t_re = datPowBst / datPow;
            r = (datPowBst * 0.0 - 1.0) / datPow;
          }
        }

        if (c_y[1].im == 0.0) {
          if (c_y[3].im == 0.0) {
            b = c_y[3].re / c_y[1].re;
            datPow = 0.0;
          } else if (c_y[3].re == 0.0) {
            b = 0.0;
            datPow = c_y[3].im / c_y[1].re;
          } else {
            b = c_y[3].re / c_y[1].re;
            datPow = c_y[3].im / c_y[1].re;
          }
        } else if (c_y[1].re == 0.0) {
          if (c_y[3].re == 0.0) {
            b = c_y[3].im / c_y[1].im;
            datPow = 0.0;
          } else if (c_y[3].im == 0.0) {
            b = 0.0;
            datPow = -(c_y[3].re / c_y[1].im);
          } else {
            b = c_y[3].im / c_y[1].im;
            datPow = -(c_y[3].re / c_y[1].im);
          }
        } else if (b_brm > b_bim) {
          datPowBst = c_y[1].im / c_y[1].re;
          datPow = c_y[1].re + datPowBst * c_y[1].im;
          b = (c_y[3].re + datPowBst * c_y[3].im) / datPow;
          datPow = (c_y[3].im - datPowBst * c_y[3].re) / datPow;
        } else if (b_bim == b_brm) {
          if (c_y[1].re > 0.0) {
            datPowBst = 0.5;
          } else {
            datPowBst = -0.5;
          }

          if (c_y[1].im > 0.0) {
            datPow = 0.5;
          } else {
            datPow = -0.5;
          }

          b = (c_y[3].re * datPowBst + c_y[3].im * datPow) / b_brm;
          datPow = (c_y[3].im * datPowBst - c_y[3].re * datPow) / b_brm;
        } else {
          datPowBst = c_y[1].re / c_y[1].im;
          datPow = c_y[1].im + datPowBst * c_y[1].re;
          b = (datPowBst * c_y[3].re + c_y[3].im) / datPow;
          datPow = (datPowBst * c_y[3].im - c_y[3].re) / datPow;
        }

        datCTst->data[4 * idxAng0 + 4 * datCTst->size[2] * b_tmp_tmp].re = b *
          t_re - datPow * r;
        datCTst->data[4 * idxAng0 + 4 * datCTst->size[2] * b_tmp_tmp].im = b * r
          + datPow * t_re;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * b_tmp_tmp) + 1].re =
          -t_re;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * b_tmp_tmp) + 1].im =
          -r;
        if (c_y[1].im == 0.0) {
          if (-c_y[2].im == 0.0) {
            b = -c_y[2].re / c_y[1].re;
            datPow = 0.0;
          } else if (-c_y[2].re == 0.0) {
            b = 0.0;
            datPow = -c_y[2].im / c_y[1].re;
          } else {
            b = -c_y[2].re / c_y[1].re;
            datPow = -c_y[2].im / c_y[1].re;
          }
        } else if (c_y[1].re == 0.0) {
          if (-c_y[2].re == 0.0) {
            b = -c_y[2].im / c_y[1].im;
            datPow = 0.0;
          } else if (-c_y[2].im == 0.0) {
            b = 0.0;
            datPow = -(-c_y[2].re / c_y[1].im);
          } else {
            b = -c_y[2].im / c_y[1].im;
            datPow = -(-c_y[2].re / c_y[1].im);
          }
        } else if (b_brm > b_bim) {
          datPowBst = c_y[1].im / c_y[1].re;
          datPow = c_y[1].re + datPowBst * c_y[1].im;
          b = (-c_y[2].re + datPowBst * -c_y[2].im) / datPow;
          datPow = (-c_y[2].im - datPowBst * -c_y[2].re) / datPow;
        } else if (b_bim == b_brm) {
          if (c_y[1].re > 0.0) {
            datPowBst = 0.5;
          } else {
            datPowBst = -0.5;
          }

          if (c_y[1].im > 0.0) {
            datPow = 0.5;
          } else {
            datPow = -0.5;
          }

          b = (-c_y[2].re * datPowBst + -c_y[2].im * datPow) / b_brm;
          datPow = (-c_y[2].im * datPowBst - -c_y[2].re * datPow) / b_brm;
        } else {
          datPowBst = c_y[1].re / c_y[1].im;
          datPow = c_y[1].im + datPowBst * c_y[1].re;
          b = (datPowBst * -c_y[2].re + -c_y[2].im) / datPow;
          datPow = (datPowBst * -c_y[2].im - (-c_y[2].re)) / datPow;
        }

        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * b_tmp_tmp) + 2].re =
          b * t_re - datPow * r;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * b_tmp_tmp) + 2].im =
          b * r + datPow * t_re;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * b_tmp_tmp) + 3].re =
          temp_re * t_re - temp_im * r;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * b_tmp_tmp) + 3].im =
          temp_re * r + temp_im * t_re;
      } else {
        if (c_y[0].im == 0.0) {
          if (c_y[1].im == 0.0) {
            temp_re = c_y[1].re / c_y[0].re;
            temp_im = 0.0;
          } else if (c_y[1].re == 0.0) {
            temp_re = 0.0;
            temp_im = c_y[1].im / c_y[0].re;
          } else {
            temp_re = c_y[1].re / c_y[0].re;
            temp_im = c_y[1].im / c_y[0].re;
          }
        } else if (c_y[0].re == 0.0) {
          if (c_y[1].re == 0.0) {
            temp_re = c_y[1].im / c_y[0].im;
            temp_im = 0.0;
          } else if (c_y[1].im == 0.0) {
            temp_re = 0.0;
            temp_im = -(c_y[1].re / c_y[0].im);
          } else {
            temp_re = c_y[1].im / c_y[0].im;
            temp_im = -(c_y[1].re / c_y[0].im);
          }
        } else if (brm > bim) {
          datPowBst = c_y[0].im / c_y[0].re;
          datPow = c_y[0].re + datPowBst * c_y[0].im;
          temp_re = (c_y[1].re + datPowBst * c_y[1].im) / datPow;
          temp_im = (c_y[1].im - datPowBst * c_y[1].re) / datPow;
        } else if (bim == brm) {
          if (c_y[0].re > 0.0) {
            datPowBst = 0.5;
          } else {
            datPowBst = -0.5;
          }

          if (c_y[0].im > 0.0) {
            datPow = 0.5;
          } else {
            datPow = -0.5;
          }

          temp_re = (c_y[1].re * datPowBst + c_y[1].im * datPow) / brm;
          temp_im = (c_y[1].im * datPowBst - c_y[1].re * datPow) / brm;
        } else {
          datPowBst = c_y[0].re / c_y[0].im;
          datPow = c_y[0].im + datPowBst * c_y[0].re;
          temp_re = (datPowBst * c_y[1].re + c_y[1].im) / datPow;
          temp_im = (datPowBst * c_y[1].im - c_y[1].re) / datPow;
        }

        datPow = c_y[3].re - (temp_re * c_y[2].re - temp_im * c_y[2].im);
        r = c_y[3].im - (temp_re * c_y[2].im + temp_im * c_y[2].re);
        if (r == 0.0) {
          t_re = 1.0 / datPow;
          r = 0.0;
        } else if (datPow == 0.0) {
          t_re = 0.0;
          r = -(1.0 / r);
        } else {
          b_brm = fabs(datPow);
          b_bim = fabs(r);
          if (b_brm > b_bim) {
            datPowBst = r / datPow;
            datPow += datPowBst * r;
            t_re = (1.0 + datPowBst * 0.0) / datPow;
            r = (0.0 - datPowBst) / datPow;
          } else if (b_bim == b_brm) {
            if (datPow > 0.0) {
              datPowBst = 0.5;
            } else {
              datPowBst = -0.5;
            }

            if (r > 0.0) {
              datPow = 0.5;
            } else {
              datPow = -0.5;
            }

            t_re = (datPowBst + 0.0 * datPow) / b_brm;
            r = (0.0 * datPowBst - datPow) / b_brm;
          } else {
            datPowBst = datPow / r;
            datPow = r + datPowBst * datPow;
            t_re = datPowBst / datPow;
            r = (datPowBst * 0.0 - 1.0) / datPow;
          }
        }

        if (c_y[0].im == 0.0) {
          if (c_y[3].im == 0.0) {
            b = c_y[3].re / c_y[0].re;
            datPow = 0.0;
          } else if (c_y[3].re == 0.0) {
            b = 0.0;
            datPow = c_y[3].im / c_y[0].re;
          } else {
            b = c_y[3].re / c_y[0].re;
            datPow = c_y[3].im / c_y[0].re;
          }
        } else if (c_y[0].re == 0.0) {
          if (c_y[3].re == 0.0) {
            b = c_y[3].im / c_y[0].im;
            datPow = 0.0;
          } else if (c_y[3].im == 0.0) {
            b = 0.0;
            datPow = -(c_y[3].re / c_y[0].im);
          } else {
            b = c_y[3].im / c_y[0].im;
            datPow = -(c_y[3].re / c_y[0].im);
          }
        } else if (brm > bim) {
          datPowBst = c_y[0].im / c_y[0].re;
          datPow = c_y[0].re + datPowBst * c_y[0].im;
          b = (c_y[3].re + datPowBst * c_y[3].im) / datPow;
          datPow = (c_y[3].im - datPowBst * c_y[3].re) / datPow;
        } else if (bim == brm) {
          if (c_y[0].re > 0.0) {
            datPowBst = 0.5;
          } else {
            datPowBst = -0.5;
          }

          if (c_y[0].im > 0.0) {
            datPow = 0.5;
          } else {
            datPow = -0.5;
          }

          b = (c_y[3].re * datPowBst + c_y[3].im * datPow) / brm;
          datPow = (c_y[3].im * datPowBst - c_y[3].re * datPow) / brm;
        } else {
          datPowBst = c_y[0].re / c_y[0].im;
          datPow = c_y[0].im + datPowBst * c_y[0].re;
          b = (datPowBst * c_y[3].re + c_y[3].im) / datPow;
          datPow = (datPowBst * c_y[3].im - c_y[3].re) / datPow;
        }

        datCTst->data[4 * idxAng0 + 4 * datCTst->size[2] * b_tmp_tmp].re = b *
          t_re - datPow * r;
        datCTst->data[4 * idxAng0 + 4 * datCTst->size[2] * b_tmp_tmp].im = b * r
          + datPow * t_re;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * b_tmp_tmp) + 1].re =
          -temp_re * t_re - -temp_im * r;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * b_tmp_tmp) + 1].im =
          -temp_re * r + -temp_im * t_re;
        if (c_y[0].im == 0.0) {
          if (-c_y[2].im == 0.0) {
            b = -c_y[2].re / c_y[0].re;
            datPow = 0.0;
          } else if (-c_y[2].re == 0.0) {
            b = 0.0;
            datPow = -c_y[2].im / c_y[0].re;
          } else {
            b = -c_y[2].re / c_y[0].re;
            datPow = -c_y[2].im / c_y[0].re;
          }
        } else if (c_y[0].re == 0.0) {
          if (-c_y[2].re == 0.0) {
            b = -c_y[2].im / c_y[0].im;
            datPow = 0.0;
          } else if (-c_y[2].im == 0.0) {
            b = 0.0;
            datPow = -(-c_y[2].re / c_y[0].im);
          } else {
            b = -c_y[2].im / c_y[0].im;
            datPow = -(-c_y[2].re / c_y[0].im);
          }
        } else if (brm > bim) {
          datPowBst = c_y[0].im / c_y[0].re;
          datPow = c_y[0].re + datPowBst * c_y[0].im;
          b = (-c_y[2].re + datPowBst * -c_y[2].im) / datPow;
          datPow = (-c_y[2].im - datPowBst * -c_y[2].re) / datPow;
        } else if (bim == brm) {
          if (c_y[0].re > 0.0) {
            datPowBst = 0.5;
          } else {
            datPowBst = -0.5;
          }

          if (c_y[0].im > 0.0) {
            datPow = 0.5;
          } else {
            datPow = -0.5;
          }

          b = (-c_y[2].re * datPowBst + -c_y[2].im * datPow) / brm;
          datPow = (-c_y[2].im * datPowBst - -c_y[2].re * datPow) / brm;
        } else {
          datPowBst = c_y[0].re / c_y[0].im;
          datPow = c_y[0].im + datPowBst * c_y[0].re;
          b = (datPowBst * -c_y[2].re + -c_y[2].im) / datPow;
          datPow = (datPowBst * -c_y[2].im - (-c_y[2].re)) / datPow;
        }

        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * b_tmp_tmp) + 2].re =
          b * t_re - datPow * r;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * b_tmp_tmp) + 2].im =
          b * r + datPow * t_re;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * b_tmp_tmp) + 3].re =
          t_re;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * b_tmp_tmp) + 3].im =
          r;
      }

      /* 'survey_dml_core_3:37' datCTst(:, :, idxAng0, idxAng1) = datC; */
    }
  }

  emxFree_creal_T(&a);
  emxFree_creal_T(&datA);
  emxFree_real_T(&DATA_ANG_TST);

  /*  traverse */
  /* 'survey_dml_core_3:43' idxAngRnd = zeros(2, NUMB_RND); */
  i = idxAngRnd->size[0] * idxAngRnd->size[1];
  idxAngRnd->size[0] = 2;
  i1 = (int)NUMB_RND;
  idxAngRnd->size[1] = i1;
  emxEnsureCapacity_real_T(idxAngRnd, i);
  loop_ub = i1 << 1;
  for (i = 0; i < loop_ub; i++) {
    idxAngRnd->data[i] = 0.0;
  }

  /* 'survey_dml_core_3:44' for idxRnd = 1:NUMB_RND */
  emxInit_creal_T(&b_a, 2);
  emxInit_creal_T(&noise, 2);
  emxInit_real_T(&b_r, 2);
  emxInit_real_T(&b_b, 2);
  emxInit_real_T(&y, 1);
  emxInit_real_T(&b_y, 1);
  for (b_tmp_tmp = 0; b_tmp_tmp < i1; b_tmp_tmp++) {
    /*     %% prepare datSig */
    /* 'survey_dml_core_3:46' datSig = complex(zeros(NUMB_ANT, 1)); */
    i = datSig->size[0];
    datSig->size[0] = NUMB_ANT;
    emxEnsureCapacity_creal_T(datSig, i);
    for (i = 0; i < NUMB_ANT; i++) {
      datSig->data[i].re = 0.0;
      datSig->data[i].im = 0.0;
    }

    /* 'survey_dml_core_3:47' for idxObj = 1:NUMB_OBJ */
    loop_ub = DATA_DIS_ANT->size[0];

    /* 'survey_dml_core_3:48' datAmp = 10^(DATA_POW_OBJ(idxObj) / 20) / 2^0.5; */
    t_re = rt_powd_snf(10.0, DATA_POW_OBJ[0] / 20.0) / 1.4142135623730951;

    /* 'survey_dml_core_3:49' datPha = exp(-1i * 2 * pi * rand); */
    b = b_rand();
    temp_re = b * -0.0;
    temp_im = b * -6.2831853071795862;
    if (temp_im == 0.0) {
      temp_re = exp(temp_re);
      temp_im = 0.0;
    } else {
      r = exp(temp_re / 2.0);
      temp_re = r * (r * cos(temp_im));
      temp_im = r * (r * sin(temp_im));
    }

    /* 'survey_dml_core_3:50' datSig = datSig + datAmp .* exp(1i * 2 * pi * DATA_DIS_ANT * sin(DATA_ANG_OBJ(idxObj) / 180 * pi)) * datPha; */
    b = sin(DATA_ANG_OBJ[0] / 180.0 * 3.1415926535897931);
    i = datSmpSum->size[0];
    datSmpSum->size[0] = DATA_DIS_ANT->size[0];
    emxEnsureCapacity_creal_T(datSmpSum, i);
    for (i = 0; i < loop_ub; i++) {
      datSmpSum->data[i].re = b * (DATA_DIS_ANT->data[i] * 0.0);
      datSmpSum->data[i].im = b * (DATA_DIS_ANT->data[i] * 6.2831853071795862);
    }

    nx = datSmpSum->size[0];
    for (k = 0; k < nx; k++) {
      if (datSmpSum->data[k].im == 0.0) {
        datSmpSum->data[k].re = exp(datSmpSum->data[k].re);
        datSmpSum->data[k].im = 0.0;
      } else if (rtIsInf(datSmpSum->data[k].im) && rtIsInf(datSmpSum->data[k].re)
                 && (datSmpSum->data[k].re < 0.0)) {
        datSmpSum->data[k].re = 0.0;
        datSmpSum->data[k].im = 0.0;
      } else {
        r = exp(datSmpSum->data[k].re / 2.0);
        datSmpSum->data[k].re = r * (r * cos(datSmpSum->data[k].im));
        datSmpSum->data[k].im = r * (r * sin(datSmpSum->data[k].im));
      }
    }

    aoffset = datSig->size[0];
    for (i = 0; i < aoffset; i++) {
      datPow = t_re * datSmpSum->data[i].re;
      r = t_re * datSmpSum->data[i].im;
      datSig->data[i].re += datPow * temp_re - r * temp_im;
      datSig->data[i].im += datPow * temp_im + r * temp_re;
    }

    /* 'survey_dml_core_3:48' datAmp = 10^(DATA_POW_OBJ(idxObj) / 20) / 2^0.5; */
    t_re = rt_powd_snf(10.0, DATA_POW_OBJ[1] / 20.0) / 1.4142135623730951;

    /* 'survey_dml_core_3:49' datPha = exp(-1i * 2 * pi * rand); */
    b = b_rand();
    temp_re = b * -0.0;
    temp_im = b * -6.2831853071795862;
    if (temp_im == 0.0) {
      temp_re = exp(temp_re);
      temp_im = 0.0;
    } else {
      r = exp(temp_re / 2.0);
      temp_re = r * (r * cos(temp_im));
      temp_im = r * (r * sin(temp_im));
    }

    /* 'survey_dml_core_3:50' datSig = datSig + datAmp .* exp(1i * 2 * pi * DATA_DIS_ANT * sin(DATA_ANG_OBJ(idxObj) / 180 * pi)) * datPha; */
    b = sin(DATA_ANG_OBJ[1] / 180.0 * 3.1415926535897931);
    i = datSmpSum->size[0];
    datSmpSum->size[0] = DATA_DIS_ANT->size[0];
    emxEnsureCapacity_creal_T(datSmpSum, i);
    for (i = 0; i < loop_ub; i++) {
      datSmpSum->data[i].re = b * (DATA_DIS_ANT->data[i] * 0.0);
      datSmpSum->data[i].im = b * (DATA_DIS_ANT->data[i] * 6.2831853071795862);
    }

    nx = datSmpSum->size[0];
    for (k = 0; k < nx; k++) {
      if (datSmpSum->data[k].im == 0.0) {
        datSmpSum->data[k].re = exp(datSmpSum->data[k].re);
        datSmpSum->data[k].im = 0.0;
      } else if (rtIsInf(datSmpSum->data[k].im) && rtIsInf(datSmpSum->data[k].re)
                 && (datSmpSum->data[k].re < 0.0)) {
        datSmpSum->data[k].re = 0.0;
        datSmpSum->data[k].im = 0.0;
      } else {
        r = exp(datSmpSum->data[k].re / 2.0);
        datSmpSum->data[k].re = r * (r * cos(datSmpSum->data[k].im));
        datSmpSum->data[k].im = r * (r * sin(datSmpSum->data[k].im));
      }
    }

    aoffset = datSig->size[0];
    for (i = 0; i < aoffset; i++) {
      datPow = t_re * datSmpSum->data[i].re;
      r = t_re * datSmpSum->data[i].im;
      datSig->data[i].re += datPow * temp_re - r * temp_im;
      datSig->data[i].im += datPow * temp_im + r * temp_re;
    }

    /*     %% prepare datR */
    /* 'survey_dml_core_3:55' datSmpSum = complex(zeros(NUMB_ANT, 1)); */
    i = datSmpSum->size[0];
    datSmpSum->size[0] = NUMB_ANT;
    emxEnsureCapacity_creal_T(datSmpSum, i);
    for (i = 0; i < NUMB_ANT; i++) {
      datSmpSum->data[i].re = 0.0;
      datSmpSum->data[i].im = 0.0;
    }

    /* 'survey_dml_core_3:56' for idxSmp = 1:NUMB_SMP */
    i = (int)NUMB_SMP;
    for (aoffset = 0; aoffset < i; aoffset++) {
      /* 'survey_dml_core_3:57' datSmpTmp = awgn(datSig, DATA_SNR, 'measured'); */
      nx = datSig->size[0];
      i2 = y->size[0];
      y->size[0] = datSig->size[0];
      emxEnsureCapacity_real_T(y, i2);
      for (k = 0; k < nx; k++) {
        y->data[k] = rt_hypotd_snf(datSig->data[k].re, datSig->data[k].im);
      }

      i2 = y->size[0];
      nx = b_y->size[0];
      b_y->size[0] = y->size[0];
      emxEnsureCapacity_real_T(b_y, nx);
      for (k = 0; k < i2; k++) {
        b_y->data[k] = rt_powd_snf(y->data[k], 2.0);
      }

      i2 = b_y->size[0];
      if (b_y->size[0] == 0) {
        datPow = 0.0;
      } else {
        datPow = b_y->data[0];
        for (k = 2; k <= i2; k++) {
          datPow += b_y->data[k - 1];
        }
      }

      b_datSig[0] = datSig->size[0];
      b_datSig[1] = 1.0;
      randn(b_datSig, b_r);
      b_datSig[0] = datSig->size[0];
      b_datSig[1] = 1.0;
      randn(b_datSig, b_b);
      datPow = sqrt(datPow / (double)datSig->size[0] / rt_powd_snf(10.0,
        DATA_SNR / 10.0) / 2.0);
      i2 = noise->size[0] * noise->size[1];
      noise->size[0] = b_r->size[0];
      noise->size[1] = 1;
      emxEnsureCapacity_creal_T(noise, i2);
      loop_ub = b_r->size[0] * b_r->size[1];
      for (i2 = 0; i2 < loop_ub; i2++) {
        noise->data[i2].re = datPow * (b_r->data[i2] + 0.0 * b_b->data[i2]);
        noise->data[i2].im = datPow * b_b->data[i2];
      }

      /* 'survey_dml_core_3:58' datSmpTmp = datSmpTmp .* DATA_COE_WIN; */
      /* 'survey_dml_core_3:59' datSmpSum = datSmpSum + datSmpTmp; */
      loop_ub = datSmpSum->size[0];
      for (i2 = 0; i2 < loop_ub; i2++) {
        datSmpSum->data[i2].re += (datSig->data[i2].re + noise->data[i2].re) *
          DATA_COE_WIN->data[i2];
        datSmpSum->data[i2].im += (datSig->data[i2].im + noise->data[i2].im) *
          DATA_COE_WIN->data[i2];
      }
    }

    /* 'survey_dml_core_3:61' datX = datSmpSum / NUMB_SMP; */
    loop_ub = datSmpSum->size[0];
    for (i = 0; i < loop_ub; i++) {
      if (datSmpSum->data[i].im == 0.0) {
        datPow = datSmpSum->data[i].re / NUMB_SMP;
        r = 0.0;
      } else if (datSmpSum->data[i].re == 0.0) {
        datPow = 0.0;
        r = datSmpSum->data[i].im / NUMB_SMP;
      } else {
        datPow = datSmpSum->data[i].re / NUMB_SMP;
        r = datSmpSum->data[i].im / NUMB_SMP;
      }

      datSmpSum->data[i].re = datPow;
      datSmpSum->data[i].im = r;
    }

    /* 'survey_dml_core_3:62' datR = datX * datX'; */
    /*     %% prepare datVX */
    /* 'survey_dml_core_3:66' datVXTst = complex(zeros(NUMB_ANG_TST, 1)); */
    i = datSig->size[0];
    datSig->size[0] = NUMB_ANG_TST + 1;
    emxEnsureCapacity_creal_T(datSig, i);

    /* 'survey_dml_core_3:67' for idxAng = 1:NUMB_ANG_TST */
    for (aoffset = 0; aoffset <= NUMB_ANG_TST; aoffset++) {
      datSig->data[aoffset].re = 0.0;
      datSig->data[aoffset].im = 0.0;

      /* 'survey_dml_core_3:68' datVXTst(idxAng) = datVTst(:, idxAng)' * datX; */
      loop_ub = datVTst->size[0];
      i = b_a->size[0] * b_a->size[1];
      b_a->size[0] = 1;
      b_a->size[1] = datVTst->size[0];
      emxEnsureCapacity_creal_T(b_a, i);
      for (i = 0; i < loop_ub; i++) {
        b_a->data[i].re = datVTst->data[i + datVTst->size[0] * aoffset].re;
        b_a->data[i].im = -datVTst->data[i + datVTst->size[0] * aoffset].im;
      }

      loop_ub = b_a->size[1];
      if ((b_a->size[1] == 1) || (datSmpSum->size[0] == 1)) {
        datPow = 0.0;
        r = 0.0;
        for (i = 0; i < loop_ub; i++) {
          datPow += b_a->data[i].re * datSmpSum->data[i].re - b_a->data[i].im *
            datSmpSum->data[i].im;
          r += b_a->data[i].re * datSmpSum->data[i].im + b_a->data[i].im *
            datSmpSum->data[i].re;
        }

        datSig->data[aoffset].re = datPow;
        datSig->data[aoffset].im = r;
      } else {
        datPow = 0.0;
        r = 0.0;
        for (i = 0; i < loop_ub; i++) {
          datPow += b_a->data[i].re * datSmpSum->data[i].re - b_a->data[i].im *
            datSmpSum->data[i].im;
          r += b_a->data[i].re * datSmpSum->data[i].im + b_a->data[i].im *
            datSmpSum->data[i].re;
        }

        datSig->data[aoffset].re = datPow;
        datSig->data[aoffset].im = r;
      }
    }

    /*     %% get best cost and index */
    /* 'survey_dml_core_3:73' datPowTst = zeros(NUMB_ANG_TST, NUMB_ANG_TST) - inf; */
    /* 'survey_dml_core_3:74' datPowBst = -inf; */
    datPowBst = rtMinusInf;

    /* 'survey_dml_core_3:75' for idxAng0 = 1:NUMB_ANG_TST */
    for (idxAng0 = 0; idxAng0 <= NUMB_ANG_TST; idxAng0++) {
      /* 'survey_dml_core_3:76' for idxAng1 = idxAng0+1:NUMB_ANG_TST */
      i = NUMB_ANG_TST - idxAng0;
      if (0 <= i - 1) {
        c_datSig[0].re = datSig->data[idxAng0].re;
        c_datSig[0].im = -datSig->data[idxAng0].im;
        datSig_idx_0 = datSig->data[idxAng0];
      }

      for (idxAng1 = 0; idxAng1 < i; idxAng1++) {
        b_idxAng1 = ((unsigned int)idxAng0 + idxAng1) + 2U;

        /* 'survey_dml_core_3:77' datPowTmp = [datVXTst(idxAng0); datVXTst(idxAng1)]' * datCTst(:, :, idxAng0, idxAng1) * [datVXTst(idxAng0); datVXTst(idxAng1)]; */
        i2 = (int)b_idxAng1 - 1;
        datPow = (c_datSig[0].re * datCTst->data[4 * idxAng0 + 4 * datCTst->
                  size[2] * i2].re - c_datSig[0].im * datCTst->data[4 * idxAng0
                  + 4 * datCTst->size[2] * i2].im) + (datSig->data[i2].re *
          datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * i2) + 1].re -
          -datSig->data[i2].im * datCTst->data[(4 * idxAng0 + 4 * datCTst->size
          [2] * i2) + 1].im);
        r = (c_datSig[0].re * datCTst->data[4 * idxAng0 + 4 * datCTst->size[2] *
             i2].im + c_datSig[0].im * datCTst->data[4 * idxAng0 + 4 *
             datCTst->size[2] * i2].re) + (datSig->data[i2].re * datCTst->data
          [(4 * idxAng0 + 4 * datCTst->size[2] * i2) + 1].im + -datSig->data[i2]
          .im * datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * i2) + 1].re);
        t_re = datPow * datSig_idx_0.re - r * datSig_idx_0.im;
        b = datPow * datSig_idx_0.im + r * datSig_idx_0.re;
        datPow = (c_datSig[0].re * datCTst->data[(4 * idxAng0 + 4 *
                   datCTst->size[2] * i2) + 2].re - c_datSig[0].im *
                  datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * i2) + 2].
                  im) + (datSig->data[i2].re * datCTst->data[(4 * idxAng0 + 4 *
          datCTst->size[2] * i2) + 3].re - -datSig->data[i2].im * datCTst->data
                         [(4 * idxAng0 + 4 * datCTst->size[2] * i2) + 3].im);
        r = (c_datSig[0].re * datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] *
              i2) + 2].im + c_datSig[0].im * datCTst->data[(4 * idxAng0 + 4 *
              datCTst->size[2] * i2) + 2].re) + (datSig->data[i2].re *
          datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * i2) + 3].im +
          -datSig->data[i2].im * datCTst->data[(4 * idxAng0 + 4 * datCTst->size
          [2] * i2) + 3].re);
        t_re += datPow * datSig->data[i2].re - r * datSig->data[i2].im;
        b += datPow * datSig->data[i2].im + r * datSig->data[i2].re;

        /* 'survey_dml_core_3:78' datPow = 20 * log10(abs(datPowTmp)); */
        datPow = 20.0 * log10(rt_hypotd_snf(t_re, b));

        /* 'survey_dml_core_3:79' if datPow > datPowBst */
        if (datPow > datPowBst) {
          /* 'survey_dml_core_3:80' datPowBst = datPow; */
          datPowBst = datPow;

          /* 'survey_dml_core_3:81' idxAngRnd(:, idxRnd) = [idxAng0, idxAng1]; */
          idxAngRnd->data[2 * b_tmp_tmp] = (unsigned int)(idxAng0 + 1);
          idxAngRnd->data[2 * b_tmp_tmp + 1] = b_idxAng1;
        }

        /* 'survey_dml_core_3:83' datPowTst(idxAng0, idxAng1) = datPow; */
      }
    }
  }

  emxFree_real_T(&b_y);
  emxFree_real_T(&y);
  emxFree_real_T(&b_b);
  emxFree_real_T(&b_r);
  emxFree_creal_T(&noise);
  emxFree_creal_T(&b_a);
  emxFree_creal_T(&datSmpSum);
  emxFree_creal_T(&datSig);
  emxFree_creal_T(&datCTst);
  emxFree_creal_T(&datVTst);
}

/*
 * File trailer for survey_dml_core_3.c
 *
 * [EOF]
 */

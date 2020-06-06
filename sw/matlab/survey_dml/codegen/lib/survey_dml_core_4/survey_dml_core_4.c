/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: survey_dml_core_4.c
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 06-Jun-2020 22:16:48
 */

/* Include Files */
#include "survey_dml_core_4.h"
#include "awgn.h"
#include "colon.h"
#include "rand.h"
#include "rt_nonfinite.h"
#include "survey_dml_core_4_data.h"
#include "survey_dml_core_4_emxutil.h"
#include "survey_dml_core_4_initialize.h"
#include "survey_dml_core_4_rtwutil.h"
#include <math.h>

/* Function Definitions */

/*
 * function idxAngRnd = survey_dml_core_4(NUMB_RND, NUMB_SMP, INDX_FIG, DATA_DIS_ANT, DATA_COE_WIN, DATA_ANG_OBJ, DATA_POW_OBJ, DATA_SNR, DATA_DLT_ANG_TST, DATA_RNG_ANG_TST)
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
void survey_dml_core_4(double NUMB_RND, double NUMB_SMP, double INDX_FIG, const
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
  int DATA_STP_RGH;
  emxArray_creal_T *datSmpSum;
  int aoffset;
  emxArray_creal_T *datCTst;
  double b;
  int i1;
  int nx;
  int k;
  emxArray_creal_T *datA;
  emxArray_creal_T *datSmpTmp;
  double r;
  emxArray_creal_T *a;
  int idxAng0;
  int idxAng1;
  int i2;
  emxArray_creal_T *datSig;
  int idxAngBstRgh1;
  emxArray_creal_T *b_a;
  double t_im;
  int idxRnd;
  double datAmp;
  double temp_re;
  double temp_im;
  creal_T y[4];
  int i3;
  double brm;
  double b_brm;
  double bim;
  double b_bim;
  double t_re;
  creal_T b_datSmpTmp[2];
  unsigned int u;
  creal_T datSmpTmp_idx_0;
  int b_idxAng0;
  if (isInitialized_survey_dml_core_4 == false) {
    survey_dml_core_4_initialize();
  }

  (void)INDX_FIG;

  /* ------------------------------------------------------------------------------- */
  /*  */
  /*   Filename       : survey_dml_core_4.m */
  /*   Author         : Huang Leilei */
  /*   Created        : 2020-06-05 */
  /*   Description    : core calculation of dml */
  /*  */
  /* ------------------------------------------------------------------------------- */
  /* 'survey_dml_core_4:13' DATA_ANG_TST = -DATA_RNG_ANG_TST:DATA_DLT_ANG_TST:DATA_RNG_ANG_TST; */
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

  /* 'survey_dml_core_4:14' NUMB_ANT     = numel(DATA_DIS_ANT); */
  NUMB_ANT = DATA_DIS_ANT->size[0];

  /* 'survey_dml_core_4:15' NUMB_OBJ     = numel(DATA_ANG_OBJ); */
  /* 'survey_dml_core_4:16' NUMB_ANG_TST = numel(DATA_ANG_TST); */
  NUMB_ANG_TST = DATA_ANG_TST->size[1];

  /* 'survey_dml_core_4:17' DATA_STP_RGH = floor(sqrt(NUMB_ANG_TST)); */
  DATA_STP_RGH = (int)floor(sqrt(DATA_ANG_TST->size[1]));

  /* 'survey_dml_core_4:18' DATA_RNG_RFN = ceil(DATA_STP_RGH); */
  /* *** MAIN BODY ***************************************************************** */
  /*  prepare datV and datC */
  /* 'survey_dml_core_4:25' datVTst = complex(zeros(NUMB_ANT, NUMB_ANG_TST)); */
  i = datVTst->size[0] * datVTst->size[1];
  datVTst->size[0] = DATA_DIS_ANT->size[0];
  datVTst->size[1] = DATA_ANG_TST->size[1];
  emxEnsureCapacity_creal_T(datVTst, i);
  loop_ub = DATA_DIS_ANT->size[0] * DATA_ANG_TST->size[1];
  for (i = 0; i < loop_ub; i++) {
    datVTst->data[i].re = 0.0;
    datVTst->data[i].im = 0.0;
  }

  /* 'survey_dml_core_4:26' for idxAng = 1:NUMB_ANG_TST */
  i = DATA_ANG_TST->size[1];
  emxInit_creal_T(&datSmpSum, 1);
  for (aoffset = 0; aoffset < i; aoffset++) {
    /* 'survey_dml_core_4:27' datAng = DATA_ANG_TST(idxAng); */
    /* 'survey_dml_core_4:28' datVTst(:, idxAng) = exp(1i * 2 * pi * DATA_DIS_ANT * sin(datAng / 180 * pi)); */
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

  /* 'survey_dml_core_4:30' datCTst = complex(zeros(2, 2, NUMB_ANG_TST, NUMB_ANG_TST)); */
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

  /* 'survey_dml_core_4:31' for idxAng0 = 1:NUMB_ANG_TST */
  i = DATA_ANG_TST->size[1];
  emxInit_creal_T(&datA, 2);
  emxInit_creal_T(&datSmpTmp, 1);
  emxInit_creal_T(&a, 2);
  for (idxAng0 = 0; idxAng0 < i; idxAng0++) {
    /* 'survey_dml_core_4:32' datAng0 = DATA_ANG_TST(idxAng0); */
    /* 'survey_dml_core_4:33' for idxAng1 = (idxAng0+1):NUMB_ANG_TST */
    i1 = NUMB_ANG_TST - idxAng0;
    for (idxAng1 = 0; idxAng1 <= i1 - 2; idxAng1++) {
      /* 'survey_dml_core_4:34' datAng1 = DATA_ANG_TST(idxAng1); */
      /* 'survey_dml_core_4:35' datA = [exp(1i * 2 * pi * DATA_DIS_ANT * sin(datAng0 / 180 * pi))     ... */
      /* 'survey_dml_core_4:36'                ,exp(1i * 2 * pi * DATA_DIS_ANT * sin(datAng1 / 180 * pi))]    ... */
      /* 'survey_dml_core_4:37'         ; */
      b = sin(DATA_ANG_TST->data[idxAng0] / 180.0 * 3.1415926535897931);
      i2 = datSmpSum->size[0];
      datSmpSum->size[0] = DATA_DIS_ANT->size[0];
      emxEnsureCapacity_creal_T(datSmpSum, i2);
      loop_ub = DATA_DIS_ANT->size[0];
      idxAngBstRgh1 = (int)((unsigned int)idxAng0 + idxAng1) + 1;
      t_im = sin(DATA_ANG_TST->data[idxAngBstRgh1] / 180.0 * 3.1415926535897931);
      i2 = datSmpTmp->size[0];
      datSmpTmp->size[0] = DATA_DIS_ANT->size[0];
      emxEnsureCapacity_creal_T(datSmpTmp, i2);
      for (i2 = 0; i2 < loop_ub; i2++) {
        r = DATA_DIS_ANT->data[i2] * 0.0;
        datAmp = DATA_DIS_ANT->data[i2] * 6.2831853071795862;
        datSmpSum->data[i2].re = b * r;
        datSmpSum->data[i2].im = b * datAmp;
        datSmpTmp->data[i2].re = t_im * r;
        datSmpTmp->data[i2].im = t_im * datAmp;
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

      nx = datSmpTmp->size[0];
      for (k = 0; k < nx; k++) {
        if (datSmpTmp->data[k].im == 0.0) {
          datSmpTmp->data[k].re = exp(datSmpTmp->data[k].re);
          datSmpTmp->data[k].im = 0.0;
        } else if (rtIsInf(datSmpTmp->data[k].im) && rtIsInf(datSmpTmp->data[k].
                    re) && (datSmpTmp->data[k].re < 0.0)) {
          datSmpTmp->data[k].re = 0.0;
          datSmpTmp->data[k].im = 0.0;
        } else {
          r = exp(datSmpTmp->data[k].re / 2.0);
          datSmpTmp->data[k].re = r * (r * cos(datSmpTmp->data[k].im));
          datSmpTmp->data[k].im = r * (r * sin(datSmpTmp->data[k].im));
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

      loop_ub = datSmpTmp->size[0];
      for (i2 = 0; i2 < loop_ub; i2++) {
        datA->data[i2 + datA->size[0]] = datSmpTmp->data[i2];
      }

      /* 'survey_dml_core_4:38' datC = (datA' * datA)^-1; */
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
        y[0].re = 0.0;
        y[0].im = 0.0;
        for (i2 = 0; i2 < loop_ub; i2++) {
          y[0].re += a->data[2 * i2].re * datA->data[i2].re - a->data[2 * i2].im
            * datA->data[i2].im;
          y[0].im += a->data[2 * i2].re * datA->data[i2].im + a->data[2 * i2].im
            * datA->data[i2].re;
        }

        y[2].re = 0.0;
        y[2].im = 0.0;
        for (i2 = 0; i2 < loop_ub; i2++) {
          y[2].re += a->data[2 * i2].re * datA->data[i2 + datA->size[0]].re -
            a->data[2 * i2].im * datA->data[i2 + datA->size[0]].im;
          y[2].im += a->data[2 * i2].re * datA->data[i2 + datA->size[0]].im +
            a->data[2 * i2].im * datA->data[i2 + datA->size[0]].re;
        }

        y[1].re = 0.0;
        y[1].im = 0.0;
        for (i2 = 0; i2 < loop_ub; i2++) {
          i3 = 2 * i2 + 1;
          y[1].re += a->data[i3].re * datA->data[i2].re - a->data[i3].im *
            datA->data[i2].im;
          y[1].im += a->data[i3].re * datA->data[i2].im + a->data[i3].im *
            datA->data[i2].re;
        }

        y[3].re = 0.0;
        y[3].im = 0.0;
        for (i2 = 0; i2 < loop_ub; i2++) {
          i3 = 2 * i2 + 1;
          y[3].re += a->data[i3].re * datA->data[i2 + datA->size[0]].re -
            a->data[i3].im * datA->data[i2 + datA->size[0]].im;
          y[3].im += a->data[i3].re * datA->data[i2 + datA->size[0]].im +
            a->data[i3].im * datA->data[i2 + datA->size[0]].re;
        }
      } else {
        y[0].re = 0.0;
        y[0].im = 0.0;
        y[1].re = 0.0;
        y[1].im = 0.0;
        for (k = 0; k < loop_ub; k++) {
          aoffset = k << 1;
          y[0].re += datA->data[k].re * a->data[aoffset].re - datA->data[k].im *
            a->data[aoffset].im;
          y[0].im += datA->data[k].re * a->data[aoffset].im + datA->data[k].im *
            a->data[aoffset].re;
          y[1].re += datA->data[k].re * a->data[aoffset + 1].re - datA->data[k].
            im * a->data[aoffset + 1].im;
          y[1].im += datA->data[k].re * a->data[aoffset + 1].im + datA->data[k].
            im * a->data[aoffset + 1].re;
        }

        y[2].re = 0.0;
        y[2].im = 0.0;
        y[3].re = 0.0;
        y[3].im = 0.0;
        for (k = 0; k < loop_ub; k++) {
          aoffset = k << 1;
          nx = loop_ub + k;
          y[2].re += datA->data[nx].re * a->data[aoffset].re - datA->data[nx].im
            * a->data[aoffset].im;
          y[2].im += datA->data[nx].re * a->data[aoffset].im + datA->data[nx].im
            * a->data[aoffset].re;
          y[3].re += datA->data[nx].re * a->data[aoffset + 1].re - datA->data[nx]
            .im * a->data[aoffset + 1].im;
          y[3].im += datA->data[nx].re * a->data[aoffset + 1].im + datA->data[nx]
            .im * a->data[aoffset + 1].re;
        }
      }

      brm = fabs(y[0].re);
      b_brm = fabs(y[1].re);
      bim = fabs(y[0].im);
      b_bim = fabs(y[1].im);
      if (b_brm + b_bim > brm + bim) {
        if (y[1].im == 0.0) {
          if (y[0].im == 0.0) {
            temp_re = y[0].re / y[1].re;
            temp_im = 0.0;
          } else if (y[0].re == 0.0) {
            temp_re = 0.0;
            temp_im = y[0].im / y[1].re;
          } else {
            temp_re = y[0].re / y[1].re;
            temp_im = y[0].im / y[1].re;
          }
        } else if (y[1].re == 0.0) {
          if (y[0].re == 0.0) {
            temp_re = y[0].im / y[1].im;
            temp_im = 0.0;
          } else if (y[0].im == 0.0) {
            temp_re = 0.0;
            temp_im = -(y[0].re / y[1].im);
          } else {
            temp_re = y[0].im / y[1].im;
            temp_im = -(y[0].re / y[1].im);
          }
        } else if (b_brm > b_bim) {
          b = y[1].im / y[1].re;
          r = y[1].re + b * y[1].im;
          temp_re = (y[0].re + b * y[0].im) / r;
          temp_im = (y[0].im - b * y[0].re) / r;
        } else if (b_bim == b_brm) {
          if (y[1].re > 0.0) {
            b = 0.5;
          } else {
            b = -0.5;
          }

          if (y[1].im > 0.0) {
            r = 0.5;
          } else {
            r = -0.5;
          }

          temp_re = (y[0].re * b + y[0].im * r) / b_brm;
          temp_im = (y[0].im * b - y[0].re * r) / b_brm;
        } else {
          b = y[1].re / y[1].im;
          r = y[1].im + b * y[1].re;
          temp_re = (b * y[0].re + y[0].im) / r;
          temp_im = (b * y[0].im - y[0].re) / r;
        }

        r = (temp_re * y[3].re - temp_im * y[3].im) - y[2].re;
        t_im = (temp_re * y[3].im + temp_im * y[3].re) - y[2].im;
        if (t_im == 0.0) {
          t_re = 1.0 / r;
          t_im = 0.0;
        } else if (r == 0.0) {
          t_re = 0.0;
          t_im = -(1.0 / t_im);
        } else {
          brm = fabs(r);
          bim = fabs(t_im);
          if (brm > bim) {
            b = t_im / r;
            r += b * t_im;
            t_re = (1.0 + b * 0.0) / r;
            t_im = (0.0 - b) / r;
          } else if (bim == brm) {
            if (r > 0.0) {
              b = 0.5;
            } else {
              b = -0.5;
            }

            if (t_im > 0.0) {
              r = 0.5;
            } else {
              r = -0.5;
            }

            t_re = (b + 0.0 * r) / brm;
            t_im = (0.0 * b - r) / brm;
          } else {
            b = r / t_im;
            r = t_im + b * r;
            t_re = b / r;
            t_im = (b * 0.0 - 1.0) / r;
          }
        }

        if (y[1].im == 0.0) {
          if (y[3].im == 0.0) {
            datAmp = y[3].re / y[1].re;
            r = 0.0;
          } else if (y[3].re == 0.0) {
            datAmp = 0.0;
            r = y[3].im / y[1].re;
          } else {
            datAmp = y[3].re / y[1].re;
            r = y[3].im / y[1].re;
          }
        } else if (y[1].re == 0.0) {
          if (y[3].re == 0.0) {
            datAmp = y[3].im / y[1].im;
            r = 0.0;
          } else if (y[3].im == 0.0) {
            datAmp = 0.0;
            r = -(y[3].re / y[1].im);
          } else {
            datAmp = y[3].im / y[1].im;
            r = -(y[3].re / y[1].im);
          }
        } else if (b_brm > b_bim) {
          b = y[1].im / y[1].re;
          r = y[1].re + b * y[1].im;
          datAmp = (y[3].re + b * y[3].im) / r;
          r = (y[3].im - b * y[3].re) / r;
        } else if (b_bim == b_brm) {
          if (y[1].re > 0.0) {
            b = 0.5;
          } else {
            b = -0.5;
          }

          if (y[1].im > 0.0) {
            r = 0.5;
          } else {
            r = -0.5;
          }

          datAmp = (y[3].re * b + y[3].im * r) / b_brm;
          r = (y[3].im * b - y[3].re * r) / b_brm;
        } else {
          b = y[1].re / y[1].im;
          r = y[1].im + b * y[1].re;
          datAmp = (b * y[3].re + y[3].im) / r;
          r = (b * y[3].im - y[3].re) / r;
        }

        datCTst->data[4 * idxAng0 + 4 * datCTst->size[2] * idxAngBstRgh1].re =
          datAmp * t_re - r * t_im;
        datCTst->data[4 * idxAng0 + 4 * datCTst->size[2] * idxAngBstRgh1].im =
          datAmp * t_im + r * t_re;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * idxAngBstRgh1) + 1].
          re = -t_re;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * idxAngBstRgh1) + 1].
          im = -t_im;
        if (y[1].im == 0.0) {
          if (-y[2].im == 0.0) {
            datAmp = -y[2].re / y[1].re;
            r = 0.0;
          } else if (-y[2].re == 0.0) {
            datAmp = 0.0;
            r = -y[2].im / y[1].re;
          } else {
            datAmp = -y[2].re / y[1].re;
            r = -y[2].im / y[1].re;
          }
        } else if (y[1].re == 0.0) {
          if (-y[2].re == 0.0) {
            datAmp = -y[2].im / y[1].im;
            r = 0.0;
          } else if (-y[2].im == 0.0) {
            datAmp = 0.0;
            r = -(-y[2].re / y[1].im);
          } else {
            datAmp = -y[2].im / y[1].im;
            r = -(-y[2].re / y[1].im);
          }
        } else if (b_brm > b_bim) {
          b = y[1].im / y[1].re;
          r = y[1].re + b * y[1].im;
          datAmp = (-y[2].re + b * -y[2].im) / r;
          r = (-y[2].im - b * -y[2].re) / r;
        } else if (b_bim == b_brm) {
          if (y[1].re > 0.0) {
            b = 0.5;
          } else {
            b = -0.5;
          }

          if (y[1].im > 0.0) {
            r = 0.5;
          } else {
            r = -0.5;
          }

          datAmp = (-y[2].re * b + -y[2].im * r) / b_brm;
          r = (-y[2].im * b - -y[2].re * r) / b_brm;
        } else {
          b = y[1].re / y[1].im;
          r = y[1].im + b * y[1].re;
          datAmp = (b * -y[2].re + -y[2].im) / r;
          r = (b * -y[2].im - (-y[2].re)) / r;
        }

        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * idxAngBstRgh1) + 2].
          re = datAmp * t_re - r * t_im;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * idxAngBstRgh1) + 2].
          im = datAmp * t_im + r * t_re;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * idxAngBstRgh1) + 3].
          re = temp_re * t_re - temp_im * t_im;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * idxAngBstRgh1) + 3].
          im = temp_re * t_im + temp_im * t_re;
      } else {
        if (y[0].im == 0.0) {
          if (y[1].im == 0.0) {
            temp_re = y[1].re / y[0].re;
            temp_im = 0.0;
          } else if (y[1].re == 0.0) {
            temp_re = 0.0;
            temp_im = y[1].im / y[0].re;
          } else {
            temp_re = y[1].re / y[0].re;
            temp_im = y[1].im / y[0].re;
          }
        } else if (y[0].re == 0.0) {
          if (y[1].re == 0.0) {
            temp_re = y[1].im / y[0].im;
            temp_im = 0.0;
          } else if (y[1].im == 0.0) {
            temp_re = 0.0;
            temp_im = -(y[1].re / y[0].im);
          } else {
            temp_re = y[1].im / y[0].im;
            temp_im = -(y[1].re / y[0].im);
          }
        } else if (brm > bim) {
          b = y[0].im / y[0].re;
          r = y[0].re + b * y[0].im;
          temp_re = (y[1].re + b * y[1].im) / r;
          temp_im = (y[1].im - b * y[1].re) / r;
        } else if (bim == brm) {
          if (y[0].re > 0.0) {
            b = 0.5;
          } else {
            b = -0.5;
          }

          if (y[0].im > 0.0) {
            r = 0.5;
          } else {
            r = -0.5;
          }

          temp_re = (y[1].re * b + y[1].im * r) / brm;
          temp_im = (y[1].im * b - y[1].re * r) / brm;
        } else {
          b = y[0].re / y[0].im;
          r = y[0].im + b * y[0].re;
          temp_re = (b * y[1].re + y[1].im) / r;
          temp_im = (b * y[1].im - y[1].re) / r;
        }

        r = y[3].re - (temp_re * y[2].re - temp_im * y[2].im);
        t_im = y[3].im - (temp_re * y[2].im + temp_im * y[2].re);
        if (t_im == 0.0) {
          t_re = 1.0 / r;
          t_im = 0.0;
        } else if (r == 0.0) {
          t_re = 0.0;
          t_im = -(1.0 / t_im);
        } else {
          b_brm = fabs(r);
          b_bim = fabs(t_im);
          if (b_brm > b_bim) {
            b = t_im / r;
            r += b * t_im;
            t_re = (1.0 + b * 0.0) / r;
            t_im = (0.0 - b) / r;
          } else if (b_bim == b_brm) {
            if (r > 0.0) {
              b = 0.5;
            } else {
              b = -0.5;
            }

            if (t_im > 0.0) {
              r = 0.5;
            } else {
              r = -0.5;
            }

            t_re = (b + 0.0 * r) / b_brm;
            t_im = (0.0 * b - r) / b_brm;
          } else {
            b = r / t_im;
            r = t_im + b * r;
            t_re = b / r;
            t_im = (b * 0.0 - 1.0) / r;
          }
        }

        if (y[0].im == 0.0) {
          if (y[3].im == 0.0) {
            datAmp = y[3].re / y[0].re;
            r = 0.0;
          } else if (y[3].re == 0.0) {
            datAmp = 0.0;
            r = y[3].im / y[0].re;
          } else {
            datAmp = y[3].re / y[0].re;
            r = y[3].im / y[0].re;
          }
        } else if (y[0].re == 0.0) {
          if (y[3].re == 0.0) {
            datAmp = y[3].im / y[0].im;
            r = 0.0;
          } else if (y[3].im == 0.0) {
            datAmp = 0.0;
            r = -(y[3].re / y[0].im);
          } else {
            datAmp = y[3].im / y[0].im;
            r = -(y[3].re / y[0].im);
          }
        } else if (brm > bim) {
          b = y[0].im / y[0].re;
          r = y[0].re + b * y[0].im;
          datAmp = (y[3].re + b * y[3].im) / r;
          r = (y[3].im - b * y[3].re) / r;
        } else if (bim == brm) {
          if (y[0].re > 0.0) {
            b = 0.5;
          } else {
            b = -0.5;
          }

          if (y[0].im > 0.0) {
            r = 0.5;
          } else {
            r = -0.5;
          }

          datAmp = (y[3].re * b + y[3].im * r) / brm;
          r = (y[3].im * b - y[3].re * r) / brm;
        } else {
          b = y[0].re / y[0].im;
          r = y[0].im + b * y[0].re;
          datAmp = (b * y[3].re + y[3].im) / r;
          r = (b * y[3].im - y[3].re) / r;
        }

        datCTst->data[4 * idxAng0 + 4 * datCTst->size[2] * idxAngBstRgh1].re =
          datAmp * t_re - r * t_im;
        datCTst->data[4 * idxAng0 + 4 * datCTst->size[2] * idxAngBstRgh1].im =
          datAmp * t_im + r * t_re;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * idxAngBstRgh1) + 1].
          re = -temp_re * t_re - -temp_im * t_im;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * idxAngBstRgh1) + 1].
          im = -temp_re * t_im + -temp_im * t_re;
        if (y[0].im == 0.0) {
          if (-y[2].im == 0.0) {
            datAmp = -y[2].re / y[0].re;
            r = 0.0;
          } else if (-y[2].re == 0.0) {
            datAmp = 0.0;
            r = -y[2].im / y[0].re;
          } else {
            datAmp = -y[2].re / y[0].re;
            r = -y[2].im / y[0].re;
          }
        } else if (y[0].re == 0.0) {
          if (-y[2].re == 0.0) {
            datAmp = -y[2].im / y[0].im;
            r = 0.0;
          } else if (-y[2].im == 0.0) {
            datAmp = 0.0;
            r = -(-y[2].re / y[0].im);
          } else {
            datAmp = -y[2].im / y[0].im;
            r = -(-y[2].re / y[0].im);
          }
        } else if (brm > bim) {
          b = y[0].im / y[0].re;
          r = y[0].re + b * y[0].im;
          datAmp = (-y[2].re + b * -y[2].im) / r;
          r = (-y[2].im - b * -y[2].re) / r;
        } else if (bim == brm) {
          if (y[0].re > 0.0) {
            b = 0.5;
          } else {
            b = -0.5;
          }

          if (y[0].im > 0.0) {
            r = 0.5;
          } else {
            r = -0.5;
          }

          datAmp = (-y[2].re * b + -y[2].im * r) / brm;
          r = (-y[2].im * b - -y[2].re * r) / brm;
        } else {
          b = y[0].re / y[0].im;
          r = y[0].im + b * y[0].re;
          datAmp = (b * -y[2].re + -y[2].im) / r;
          r = (b * -y[2].im - (-y[2].re)) / r;
        }

        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * idxAngBstRgh1) + 2].
          re = datAmp * t_re - r * t_im;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * idxAngBstRgh1) + 2].
          im = datAmp * t_im + r * t_re;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * idxAngBstRgh1) + 3].
          re = t_re;
        datCTst->data[(4 * idxAng0 + 4 * datCTst->size[2] * idxAngBstRgh1) + 3].
          im = t_im;
      }

      /* 'survey_dml_core_4:39' datCTst(:, :, idxAng0, idxAng1) = datC; */
    }
  }

  emxFree_creal_T(&a);
  emxFree_creal_T(&datA);
  emxFree_real_T(&DATA_ANG_TST);

  /*  traverse */
  /* 'survey_dml_core_4:45' idxAngRnd = zeros(2, NUMB_RND); */
  i = idxAngRnd->size[0] * idxAngRnd->size[1];
  idxAngRnd->size[0] = 2;
  i1 = (int)NUMB_RND;
  idxAngRnd->size[1] = i1;
  emxEnsureCapacity_real_T(idxAngRnd, i);
  loop_ub = i1 << 1;
  for (i = 0; i < loop_ub; i++) {
    idxAngRnd->data[i] = 0.0;
  }

  /* 'survey_dml_core_4:46' for idxRnd = 1:NUMB_RND */
  emxInit_creal_T(&datSig, 1);
  emxInit_creal_T(&b_a, 2);
  for (idxRnd = 0; idxRnd < i1; idxRnd++) {
    /*     %% prepare datSig */
    /* 'survey_dml_core_4:48' datSig = complex(zeros(NUMB_ANT, 1)); */
    i = datSig->size[0];
    datSig->size[0] = NUMB_ANT;
    emxEnsureCapacity_creal_T(datSig, i);
    for (i = 0; i < NUMB_ANT; i++) {
      datSig->data[i].re = 0.0;
      datSig->data[i].im = 0.0;
    }

    /* 'survey_dml_core_4:49' for idxObj = 1:NUMB_OBJ */
    loop_ub = DATA_DIS_ANT->size[0];

    /* 'survey_dml_core_4:50' datAmp = 10^(DATA_POW_OBJ(idxObj) / 20) / 2^0.5; */
    datAmp = rt_powd_snf(10.0, DATA_POW_OBJ[0] / 20.0) / 1.4142135623730951;

    /* 'survey_dml_core_4:51' datPha = exp(-1i * 2 * pi * rand); */
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

    /* 'survey_dml_core_4:52' datSig = datSig + datAmp .* exp(1i * 2 * pi * DATA_DIS_ANT * sin(DATA_ANG_OBJ(idxObj) / 180 * pi)) * datPha; */
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
      r = datAmp * datSmpSum->data[i].re;
      t_im = datAmp * datSmpSum->data[i].im;
      datSig->data[i].re += r * temp_re - t_im * temp_im;
      datSig->data[i].im += r * temp_im + t_im * temp_re;
    }

    /* 'survey_dml_core_4:50' datAmp = 10^(DATA_POW_OBJ(idxObj) / 20) / 2^0.5; */
    datAmp = rt_powd_snf(10.0, DATA_POW_OBJ[1] / 20.0) / 1.4142135623730951;

    /* 'survey_dml_core_4:51' datPha = exp(-1i * 2 * pi * rand); */
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

    /* 'survey_dml_core_4:52' datSig = datSig + datAmp .* exp(1i * 2 * pi * DATA_DIS_ANT * sin(DATA_ANG_OBJ(idxObj) / 180 * pi)) * datPha; */
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
      r = datAmp * datSmpSum->data[i].re;
      t_im = datAmp * datSmpSum->data[i].im;
      datSig->data[i].re += r * temp_re - t_im * temp_im;
      datSig->data[i].im += r * temp_im + t_im * temp_re;
    }

    /*     %% prepare datR */
    /* 'survey_dml_core_4:57' datSmpSum = complex(zeros(NUMB_ANT, 1)); */
    i = datSmpSum->size[0];
    datSmpSum->size[0] = NUMB_ANT;
    emxEnsureCapacity_creal_T(datSmpSum, i);
    for (i = 0; i < NUMB_ANT; i++) {
      datSmpSum->data[i].re = 0.0;
      datSmpSum->data[i].im = 0.0;
    }

    /* 'survey_dml_core_4:58' for idxSmp = 1:NUMB_SMP */
    i = (int)NUMB_SMP;
    for (aoffset = 0; aoffset < i; aoffset++) {
      /* 'survey_dml_core_4:59' datSmpTmp = awgn(datSig, DATA_SNR, 'measured'); */
      awgn(datSig, DATA_SNR, datSmpTmp);

      /* 'survey_dml_core_4:60' datSmpTmp = datSmpTmp .* DATA_COE_WIN; */
      loop_ub = datSmpTmp->size[0];
      for (i2 = 0; i2 < loop_ub; i2++) {
        datSmpTmp->data[i2].re *= DATA_COE_WIN->data[i2];
        datSmpTmp->data[i2].im *= DATA_COE_WIN->data[i2];
      }

      /* 'survey_dml_core_4:61' datSmpSum = datSmpSum + datSmpTmp; */
      loop_ub = datSmpSum->size[0];
      for (i2 = 0; i2 < loop_ub; i2++) {
        datSmpSum->data[i2].re += datSmpTmp->data[i2].re;
        datSmpSum->data[i2].im += datSmpTmp->data[i2].im;
      }
    }

    /* 'survey_dml_core_4:63' datX = datSmpSum / NUMB_SMP; */
    loop_ub = datSmpSum->size[0];
    for (i = 0; i < loop_ub; i++) {
      if (datSmpSum->data[i].im == 0.0) {
        r = datSmpSum->data[i].re / NUMB_SMP;
        t_im = 0.0;
      } else if (datSmpSum->data[i].re == 0.0) {
        r = 0.0;
        t_im = datSmpSum->data[i].im / NUMB_SMP;
      } else {
        r = datSmpSum->data[i].re / NUMB_SMP;
        t_im = datSmpSum->data[i].im / NUMB_SMP;
      }

      datSmpSum->data[i].re = r;
      datSmpSum->data[i].im = t_im;
    }

    /* 'survey_dml_core_4:64' datR = datX * datX'; */
    /*     %% prepare datVX */
    /* 'survey_dml_core_4:68' datVXTst = complex(zeros(NUMB_ANG_TST, 1)); */
    i = datSmpTmp->size[0];
    datSmpTmp->size[0] = NUMB_ANG_TST;
    emxEnsureCapacity_creal_T(datSmpTmp, i);

    /* 'survey_dml_core_4:69' for idxAng = 1:NUMB_ANG_TST */
    for (aoffset = 0; aoffset < NUMB_ANG_TST; aoffset++) {
      datSmpTmp->data[aoffset].re = 0.0;
      datSmpTmp->data[aoffset].im = 0.0;

      /* 'survey_dml_core_4:70' datVXTst(idxAng) = datVTst(:, idxAng)' * datX; */
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
        r = 0.0;
        t_im = 0.0;
        for (i = 0; i < loop_ub; i++) {
          r += b_a->data[i].re * datSmpSum->data[i].re - b_a->data[i].im *
            datSmpSum->data[i].im;
          t_im += b_a->data[i].re * datSmpSum->data[i].im + b_a->data[i].im *
            datSmpSum->data[i].re;
        }

        datSmpTmp->data[aoffset].re = r;
        datSmpTmp->data[aoffset].im = t_im;
      } else {
        r = 0.0;
        t_im = 0.0;
        for (i = 0; i < loop_ub; i++) {
          r += b_a->data[i].re * datSmpSum->data[i].re - b_a->data[i].im *
            datSmpSum->data[i].im;
          t_im += b_a->data[i].re * datSmpSum->data[i].im + b_a->data[i].im *
            datSmpSum->data[i].re;
        }

        datSmpTmp->data[aoffset].re = r;
        datSmpTmp->data[aoffset].im = t_im;
      }
    }

    /*     %% get best cost and index (rough) */
    /* 'survey_dml_core_4:75' datPowTst = zeros(NUMB_ANG_TST, NUMB_ANG_TST) - inf; */
    /* 'survey_dml_core_4:76' datPowBst = -inf; */
    t_re = rtMinusInf;

    /* 'survey_dml_core_4:77' for idxAng0 = 1:DATA_STP_RGH:NUMB_ANG_TST */
    i = (int)(((double)NUMB_ANG_TST + ((double)DATA_STP_RGH - 1.0)) / (double)
              DATA_STP_RGH);
    for (idxAng0 = 0; idxAng0 < i; idxAng0++) {
      brm = (double)idxAng0 * (double)DATA_STP_RGH + 1.0;

      /* 'survey_dml_core_4:78' for idxAng1 = 2:DATA_STP_RGH:NUMB_ANG_TST */
      i2 = (int)(((double)NUMB_ANG_TST + ((double)DATA_STP_RGH - 2.0)) / (double)
                 DATA_STP_RGH);
      for (idxAng1 = 0; idxAng1 < i2; idxAng1++) {
        b_brm = (double)idxAng1 * (double)DATA_STP_RGH + 2.0;

        /* 'survey_dml_core_4:79' if idxAng0 < idxAng1 && idxAng1 <= NUMB_ANG_TST */
        if ((brm < b_brm) && (b_brm <= NUMB_ANG_TST)) {
          /* 'survey_dml_core_4:80' datPowTmp = [datVXTst(idxAng0); datVXTst(idxAng1)]' * datCTst(:, :, idxAng0, idxAng1) * [datVXTst(idxAng0); datVXTst(idxAng1)]; */
          i3 = (int)brm - 1;
          b_datSmpTmp[0].re = datSmpTmp->data[i3].re;
          b_datSmpTmp[0].im = -datSmpTmp->data[i3].im;
          nx = (int)b_brm - 1;
          datSmpTmp_idx_0 = datSmpTmp->data[i3];
          aoffset = (int)b_brm - 1;
          r = (datSmpTmp->data[i3].re * datCTst->data[4 * i3 + 4 * datCTst->
               size[2] * aoffset].re - -datSmpTmp->data[i3].im * datCTst->data[4
               * i3 + 4 * datCTst->size[2] * aoffset].im) + (datSmpTmp->data[nx]
            .re * datCTst->data[(4 * i3 + 4 * datCTst->size[2] * aoffset) + 1].
            re - -datSmpTmp->data[nx].im * datCTst->data[(4 * i3 + 4 *
            datCTst->size[2] * aoffset) + 1].im);
          t_im = (datSmpTmp->data[i3].re * datCTst->data[4 * i3 + 4 *
                  datCTst->size[2] * aoffset].im + -datSmpTmp->data[i3].im *
                  datCTst->data[4 * i3 + 4 * datCTst->size[2] * aoffset].re) +
            (datSmpTmp->data[nx].re * datCTst->data[(4 * i3 + 4 * datCTst->size
              [2] * aoffset) + 1].im + -datSmpTmp->data[nx].im * datCTst->data
             [(4 * i3 + 4 * datCTst->size[2] * aoffset) + 1].re);
          b = r * datSmpTmp->data[i3].re - t_im * datSmpTmp->data[i3].im;
          datAmp = r * datSmpTmp->data[i3].im + t_im * datSmpTmp->data[i3].re;
          r = (datSmpTmp->data[i3].re * datCTst->data[(4 * i3 + 4 *
                datCTst->size[2] * aoffset) + 2].re - -datSmpTmp->data[i3].im *
               datCTst->data[(4 * i3 + 4 * datCTst->size[2] * aoffset) + 2].im)
            + (datSmpTmp->data[nx].re * datCTst->data[(4 * i3 + 4 *
                datCTst->size[2] * aoffset) + 3].re - -datSmpTmp->data[nx].im *
               datCTst->data[(4 * i3 + 4 * datCTst->size[2] * aoffset) + 3].im);
          t_im = (datSmpTmp->data[i3].re * datCTst->data[(4 * i3 + 4 *
                   datCTst->size[2] * aoffset) + 2].im + -datSmpTmp->data[i3].im
                  * datCTst->data[(4 * i3 + 4 * datCTst->size[2] * aoffset) + 2]
                  .re) + (datSmpTmp->data[nx].re * datCTst->data[(4 * i3 + 4 *
            datCTst->size[2] * aoffset) + 3].im + -datSmpTmp->data[nx].im *
                          datCTst->data[(4 * i3 + 4 * datCTst->size[2] * aoffset)
                          + 3].re);
          b += r * datSmpTmp->data[nx].re - t_im * datSmpTmp->data[nx].im;
          datAmp += r * datSmpTmp->data[nx].im + t_im * datSmpTmp->data[nx].re;

          /* 'survey_dml_core_4:81' datPow = 20 * log10(abs(datPowTmp)); */
          r = 20.0 * log10(rt_hypotd_snf(b, datAmp));

          /* 'survey_dml_core_4:82' if datPow > datPowBst */
          if (r > t_re) {
            /* 'survey_dml_core_4:83' datPowBst = datPow; */
            t_re = r;

            /* 'survey_dml_core_4:84' idxAngRnd(:, idxRnd) = [idxAng0, idxAng1]; */
            idxAngRnd->data[2 * idxRnd] = brm;
            idxAngRnd->data[2 * idxRnd + 1] = b_brm;
          }

          /* 'survey_dml_core_4:86' datPowTst(idxAng0, idxAng1) = datPow; */
        }
      }
    }

    /*     %% get best cost and index (refined) */
    /* 'survey_dml_core_4:93' idxAngBstRgh0 = idxAngRnd(1, idxRnd); */
    aoffset = (int)idxAngRnd->data[2 * idxRnd];

    /* 'survey_dml_core_4:94' idxAngBstRgh1 = idxAngRnd(2, idxRnd); */
    k = 2 * idxRnd + 1;
    idxAngBstRgh1 = (int)idxAngRnd->data[k];

    /* 'survey_dml_core_4:95' for idxAng0 = max(1,idxAngBstRgh0-DATA_RNG_RFN):min(NUMB_ANG_TST,idxAngBstRgh0+DATA_RNG_RFN) */
    i = (int)fmax(1.0, aoffset - DATA_STP_RGH);
    i2 = (int)fmin(NUMB_ANG_TST, (unsigned int)aoffset + DATA_STP_RGH) - i;
    for (idxAng0 = 0; idxAng0 <= i2; idxAng0++) {
      brm = (unsigned int)i + idxAng0;

      /* 'survey_dml_core_4:96' for idxAng1 = max(idxAng0+1,idxAngBstRgh1-DATA_RNG_RFN):min(NUMB_ANG_TST,idxAngBstRgh1+DATA_RNG_RFN) */
      u = (unsigned int)fmax(brm + 1.0, idxAngBstRgh1 - DATA_STP_RGH);
      i3 = (int)(fmin(NUMB_ANG_TST, (unsigned int)idxAngBstRgh1 + DATA_STP_RGH)
                 + (1.0 - (double)u));
      if (0 <= i3 - 1) {
        nx = (int)brm - 1;
        b_datSmpTmp[0].re = datSmpTmp->data[nx].re;
        b_datSmpTmp[0].im = -datSmpTmp->data[nx].im;
        b_idxAng0 = (int)brm;
        datSmpTmp_idx_0 = datSmpTmp->data[nx];
      }

      for (idxAng1 = 0; idxAng1 < i3; idxAng1++) {
        b_brm = (double)u + (double)idxAng1;

        /* 'survey_dml_core_4:97' datPowTmp = [datVXTst(idxAng0); datVXTst(idxAng1)]' * datCTst(:, :, idxAng0, idxAng1) * [datVXTst(idxAng0); datVXTst(idxAng1)]; */
        nx = (int)b_brm - 1;
        r = (b_datSmpTmp[0].re * datCTst->data[4 * (b_idxAng0 - 1) + 4 *
             datCTst->size[2] * nx].re - b_datSmpTmp[0].im * datCTst->data[4 *
             (b_idxAng0 - 1) + 4 * datCTst->size[2] * nx].im) + (datSmpTmp->
          data[nx].re * datCTst->data[(4 * (b_idxAng0 - 1) + 4 * datCTst->size[2]
          * nx) + 1].re - -datSmpTmp->data[nx].im * datCTst->data[(4 *
          (b_idxAng0 - 1) + 4 * datCTst->size[2] * nx) + 1].im);
        t_im = (b_datSmpTmp[0].re * datCTst->data[4 * (b_idxAng0 - 1) + 4 *
                datCTst->size[2] * nx].im + b_datSmpTmp[0].im * datCTst->data[4 *
                (b_idxAng0 - 1) + 4 * datCTst->size[2] * nx].re) +
          (datSmpTmp->data[nx].re * datCTst->data[(4 * (b_idxAng0 - 1) + 4 *
            datCTst->size[2] * nx) + 1].im + -datSmpTmp->data[nx].im *
           datCTst->data[(4 * (b_idxAng0 - 1) + 4 * datCTst->size[2] * nx) + 1].
           re);
        b = r * datSmpTmp_idx_0.re - t_im * datSmpTmp_idx_0.im;
        datAmp = r * datSmpTmp_idx_0.im + t_im * datSmpTmp_idx_0.re;
        r = (b_datSmpTmp[0].re * datCTst->data[(4 * (b_idxAng0 - 1) + 4 *
              datCTst->size[2] * nx) + 2].re - b_datSmpTmp[0].im * datCTst->
             data[(4 * (b_idxAng0 - 1) + 4 * datCTst->size[2] * nx) + 2].im) +
          (datSmpTmp->data[nx].re * datCTst->data[(4 * (b_idxAng0 - 1) + 4 *
            datCTst->size[2] * nx) + 3].re - -datSmpTmp->data[nx].im *
           datCTst->data[(4 * (b_idxAng0 - 1) + 4 * datCTst->size[2] * nx) + 3].
           im);
        t_im = (b_datSmpTmp[0].re * datCTst->data[(4 * (b_idxAng0 - 1) + 4 *
                 datCTst->size[2] * nx) + 2].im + b_datSmpTmp[0].im *
                datCTst->data[(4 * (b_idxAng0 - 1) + 4 * datCTst->size[2] * nx)
                + 2].re) + (datSmpTmp->data[nx].re * datCTst->data[(4 *
          (b_idxAng0 - 1) + 4 * datCTst->size[2] * nx) + 3].im +
                            -datSmpTmp->data[nx].im * datCTst->data[(4 *
          (b_idxAng0 - 1) + 4 * datCTst->size[2] * nx) + 3].re);
        b += r * datSmpTmp->data[nx].re - t_im * datSmpTmp->data[nx].im;
        datAmp += r * datSmpTmp->data[nx].im + t_im * datSmpTmp->data[nx].re;

        /* 'survey_dml_core_4:98' datPow = 20 * log10(abs(datPowTmp)); */
        r = 20.0 * log10(rt_hypotd_snf(b, datAmp));

        /* 'survey_dml_core_4:99' if datPow > datPowBst */
        if (r > t_re) {
          /* 'survey_dml_core_4:100' datPowBst = datPow; */
          t_re = r;

          /* 'survey_dml_core_4:101' idxAngRnd(:, idxRnd) = [idxAng0, idxAng1]; */
          idxAngRnd->data[2 * idxRnd] = brm;
          idxAngRnd->data[k] = b_brm;
        }

        /* 'survey_dml_core_4:103' datPowTst(idxAng0, idxAng1) = datPow; */
      }
    }
  }

  emxFree_creal_T(&b_a);
  emxFree_creal_T(&datSmpTmp);
  emxFree_creal_T(&datSmpSum);
  emxFree_creal_T(&datSig);
  emxFree_creal_T(&datCTst);
  emxFree_creal_T(&datVTst);
}

/*
 * File trailer for survey_dml_core_4.c
 *
 * [EOF]
 */

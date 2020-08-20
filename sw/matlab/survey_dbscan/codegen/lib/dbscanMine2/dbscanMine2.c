/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: dbscanMine2.c
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 20-Aug-2020 15:51:06
 */

/* Include Files */
#include "dbscanMine2.h"
#include "dbscanMine2_emxutil.h"

/* Function Definitions */

/*
 * function [idxGrpLst, idxKnlLst] = dbscanMine2(cost, epsilon, minpts)
 * declare spaces
 * Arguments    : const emxArray_real_T *cost
 *                double epsilon
 *                double minpts
 *                emxArray_real_T *idxGrpLst
 *                emxArray_real_T *idxKnlLst
 * Return Type  : void
 */
void dbscanMine2(const emxArray_real_T *cost, double epsilon, double minpts,
                 emxArray_real_T *idxGrpLst, emxArray_real_T *idxKnlLst)
{
  int i;
  int nz;
  emxArray_uint32_T *idxTstLst;
  double idxGrpCur;
  emxArray_boolean_T *idxNeiLst;
  emxArray_real_T *x;
  int idxDat;
  int i1;
  int k;
  double idxTstNum;
  double idxTstJ;
  int idxTstI;
  int i2;
  double y;

  /* ------------------------------------------------------------------------------- */
  /*  */
  /*   Filename       : dbscanMine2.m */
  /*   Author         : Huang Leilei */
  /*   Created        : 2020-08-17 */
  /*   Description    : my dbscan */
  /*  */
  /* ------------------------------------------------------------------------------- */
  /* 'dbscanMine2:12' numDat    = size(cost, 1); */
  /* 'dbscanMine2:13' idxGrpLst = zeros(numDat, 1); */
  i = idxGrpLst->size[0];
  idxGrpLst->size[0] = cost->size[0];
  emxEnsureCapacity_real_T(idxGrpLst, i);
  nz = cost->size[0];
  for (i = 0; i < nz; i++) {
    idxGrpLst->data[i] = 0.0;
  }

  /* 'dbscanMine2:14' idxKnlLst = zeros(numDat, 1); */
  i = idxKnlLst->size[0];
  idxKnlLst->size[0] = cost->size[0];
  emxEnsureCapacity_real_T(idxKnlLst, i);
  nz = cost->size[0];
  for (i = 0; i < nz; i++) {
    idxKnlLst->data[i] = 0.0;
  }

  emxInit_uint32_T(&idxTstLst, 1);

  /* 'dbscanMine2:15' idxTstLst = zeros(numDat, 1); */
  i = idxTstLst->size[0];
  idxTstLst->size[0] = cost->size[0];
  emxEnsureCapacity_uint32_T(idxTstLst, i);
  nz = cost->size[0];
  for (i = 0; i < nz; i++) {
    idxTstLst->data[i] = 0U;
  }

  /*  main loop */
  /* 'dbscanMine2:18' idxGrpCur = 0; */
  idxGrpCur = 0.0;

  /* 'dbscanMine2:19' for idxDat = 1:numDat */
  i = cost->size[0];
  emxInit_boolean_T(&idxNeiLst, 2);
  emxInit_real_T(&x, 2);
  for (idxDat = 0; idxDat < i; idxDat++) {
    /*  if not visited */
    /* 'dbscanMine2:21' if idxGrpLst(idxDat) == 0 */
    if (idxGrpLst->data[idxDat] == 0.0) {
      /*  get neighbour */
      /* 'dbscanMine2:23' idxNeiLst = cost(idxDat,:) <= epsilon; */
      nz = cost->size[1];
      i1 = idxNeiLst->size[0] * idxNeiLst->size[1];
      idxNeiLst->size[0] = 1;
      idxNeiLst->size[1] = cost->size[1];
      emxEnsureCapacity_boolean_T(idxNeiLst, i1);
      for (i1 = 0; i1 < nz; i1++) {
        idxNeiLst->data[i1] = (cost->data[idxDat + cost->size[0] * i1] <=
          epsilon);
      }

      /*  test minpts condition: pass */
      /* 'dbscanMine2:26' if sum(idxNeiLst) >= minpts */
      i1 = idxNeiLst->size[1];
      if (idxNeiLst->size[1] == 0) {
        nz = 0;
      } else {
        nz = idxNeiLst->data[0];
        for (k = 2; k <= i1; k++) {
          nz += idxNeiLst->data[k - 1];
        }
      }

      if (nz >= minpts) {
        /*  mark it with a new group(cluster) id */
        /* 'dbscanMine2:28' idxGrpCur = idxGrpCur + 1; */
        idxGrpCur++;

        /* 'dbscanMine2:29' idxGrpLst(idxDat) = idxGrpCur; */
        idxGrpLst->data[idxDat] = idxGrpCur;

        /*  mark it as a kernal points */
        /* 'dbscanMine2:32' idxKnlLst(idxDat) = 1; */
        idxKnlLst->data[idxDat] = 1.0;

        /*  expand this group (cluster) */
        /* 'dbscanMine2:35' idxTstNum = 1; */
        idxTstNum = 1.0;

        /* 'dbscanMine2:36' idxTstLst(idxTstNum) = idxDat; */
        idxTstLst->data[0] = (unsigned int)(idxDat + 1);

        /* 'dbscanMine2:37' idxTstJ = 1; */
        /* 'dbscanMine2:38' while idxTstJ <= idxTstNum */
        for (idxTstJ = 1.0; idxTstJ <= idxTstNum; idxTstJ++) {
          /*  get neighbour */
          /* 'dbscanMine2:40' idxNeiLst = cost(idxTstLst(idxTstJ),:) <= epsilon; */
          nz = cost->size[1];
          i1 = idxNeiLst->size[0] * idxNeiLst->size[1];
          idxNeiLst->size[0] = 1;
          idxNeiLst->size[1] = cost->size[1];
          emxEnsureCapacity_boolean_T(idxNeiLst, i1);
          for (i1 = 0; i1 < nz; i1++) {
            idxNeiLst->data[i1] = (cost->data[((int)idxTstLst->data[(int)idxTstJ
              - 1] + cost->size[0] * i1) - 1] <= epsilon);
          }

          /*  loop each neighbour */
          /* 'dbscanMine2:43' for idxTstI = 1:numDat */
          i1 = cost->size[0];
          for (idxTstI = 0; idxTstI < i1; idxTstI++) {
            /*  test if it is an unvisited neigbour: pass */
            /* 'dbscanMine2:45' if idxNeiLst(idxTstI) == 1 && idxGrpLst(idxTstI) == 0 */
            if (idxNeiLst->data[idxTstI] && (idxGrpLst->data[idxTstI] == 0.0)) {
              /*  mark it with the current group(cluster) id */
              /* 'dbscanMine2:47' idxGrpLst(idxTstI) = idxGrpCur; */
              idxGrpLst->data[idxTstI] = idxGrpCur;

              /*  test minpts condition: pass */
              /* 'dbscanMine2:50' if sum(cost(idxTstI,:)) >= minpts */
              nz = cost->size[1];
              i2 = x->size[0] * x->size[1];
              x->size[0] = 1;
              x->size[1] = cost->size[1];
              emxEnsureCapacity_real_T(x, i2);
              for (i2 = 0; i2 < nz; i2++) {
                x->data[i2] = cost->data[idxTstI + cost->size[0] * i2];
              }

              i2 = cost->size[1];
              if (cost->size[1] == 0) {
                y = 0.0;
              } else {
                y = cost->data[idxTstI];
                for (k = 2; k <= i2; k++) {
                  y += x->data[k - 1];
                }
              }

              if (y >= minpts) {
                /*  mark it as a kernal points */
                /* 'dbscanMine2:52' idxKnlLst(idxTstI) = 1; */
                idxKnlLst->data[idxTstI] = 1.0;

                /*  add it to test list */
                /* 'dbscanMine2:55' idxTstNum = idxTstNum + 1; */
                idxTstNum++;

                /* 'dbscanMine2:56' idxTstLst(idxTstNum) = idxTstI; */
                idxTstLst->data[(int)idxTstNum - 1] = (unsigned int)(idxTstI + 1);
              }
            }
          }

          /*  udpate counter */
          /* 'dbscanMine2:62' idxTstJ = idxTstJ + 1; */
        }

        /*  test minpts condition: fail */
      } else {
        /* 'dbscanMine2:66' else */
        /* 'dbscanMine2:67' idxGrpLst(idxDat) = -1; */
        idxGrpLst->data[idxDat] = -1.0;
      }
    }
  }

  emxFree_real_T(&x);
  emxFree_boolean_T(&idxNeiLst);
  emxFree_uint32_T(&idxTstLst);
}

/*
 * File trailer for dbscanMine2.c
 *
 * [EOF]
 */

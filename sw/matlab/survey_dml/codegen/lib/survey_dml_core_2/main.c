/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: main.c
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 05-Jun-2020 12:19:16
 */

/*************************************************************************/
/* This automatically generated example C main file shows how to call    */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/

/* Include Files */
#include <stdio.h>
#include "main.h"
#include "rt_nonfinite.h"
#include "survey_dml_core_2.h"
#include "survey_dml_core_2_emxAPI.h"
#include "survey_dml_core_2_terminate.h"

/* Function Declarations */
static void argInit_1x2_real_T(double result[2]);
static emxArray_real_T *argInit_Unboundedx1_real_T(void);
static double argInit_real_T(void);
static void main_survey_dml_core_2(void);

/* Function Definitions */

/*
 * Arguments    : double result[2]
 * Return Type  : void
 */
static void argInit_1x2_real_T(double result[2])
{
  double result_tmp;

  /* Loop over the array to initialize each element. */
  /* Set the value of the array element.
     Change this value to the value that the application requires. */
  result_tmp = argInit_real_T();
  result[0] = result_tmp;

  /* Set the value of the array element.
     Change this value to the value that the application requires. */
  result[1] = result_tmp;
}

/*
 * Arguments    : void
 * Return Type  : emxArray_real_T *
 */
static emxArray_real_T *argInit_Unboundedx1_real_T(void)
{
  emxArray_real_T *result;
  static const int iv[1] = { 2 };

  int idx0;

  /* Set the size of the array.
     Change this size to the value that the application requires. */
  result = emxCreateND_real_T(1, iv);

  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < result->size[0U]; idx0++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result->data[idx0] = argInit_real_T();
  }

  return result;
}

/*
 * Arguments    : void
 * Return Type  : double
 */
static double argInit_real_T(void)
{
  return 0.0;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void main_survey_dml_core_2(void)
{
  // variables
  double NUMB_RND = 1000;
  double NUMB_SMP = 1;
  double INDX_FIG = 1;

  int NUMB_ANT[1] = {4};
  emxArray_real_T *DATA_DIS_ANT = emxCreateND_real_T(1, NUMB_ANT);
  DATA_DIS_ANT->data[0] = 0.0 / 2;
  DATA_DIS_ANT->data[1] = 1.0 / 2;
  DATA_DIS_ANT->data[2] = 2.0 / 2;
  DATA_DIS_ANT->data[3] = 3.0 / 2;
  emxArray_real_T *DATA_COE_WIN = emxCreateND_real_T(1, NUMB_ANT);
  DATA_COE_WIN->data[0] = 1;
  DATA_COE_WIN->data[1] = 1;
  DATA_COE_WIN->data[2] = 1;
  DATA_COE_WIN->data[3] = 1;

  double DATA_ANG_OBJ[2]  = {-12, 12};
  double DATA_POW_OBJ[2]  = {0, 0};
  double DATA_SNR         = 20;
  double DATA_DLT_ANG_TST = 1;
  double DATA_RNG_ANG_TST = 89;

  int NUMB_VEC[1] = {NUMB_RND};
  emxArray_real_T *idxAngRnd = emxCreateND_real_T(2, NUMB_VEC);

  // core
  survey_dml_core_2(NUMB_RND, NUMB_SMP, INDX_FIG, 
                    DATA_DIS_ANT, DATA_COE_WIN, 
                    DATA_ANG_OBJ, DATA_POW_OBJ, DATA_SNR,
                    DATA_DLT_ANG_TST, DATA_RNG_ANG_TST,
                    idxAngRnd
  );
  if (NUMB_RND == 1) {
    printf("detected angle is %.2f, %.2f", idxAngRnd->data[0], idxAngRnd->data[1]);
  }

  // post
  emxDestroyArray_real_T(idxAngRnd);
  emxDestroyArray_real_T(DATA_COE_WIN);
  emxDestroyArray_real_T(DATA_DIS_ANT);
}

/*
 * Arguments    : int argc
 *                const char * const argv[]
 * Return Type  : int
 */
int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;

  /* The initialize function is being called automatically from your entry-point function. So, a call to initialize is not included here. */
  /* Invoke the entry-point functions.
     You can call entry-point functions multiple times. */
  main_survey_dml_core_2();

  /* Terminate the application.
     You do not need to do this more than one time. */
  survey_dml_core_2_terminate();
  return 0;
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */

/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: main.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 26-Dec-2020 21:05:29
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
#include "main.h"
#include "BCHIndices.h"
#include "BCHIndices_emxAPI.h"
#include "BCHIndices_terminate.h"
#include "ChannelEst.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static void argInit_1x3_char_T(char result[3]);
static void argInit_1x6_char_T(char result[6]);
static char argInit_char_T(void);
static creal_T argInit_creal_T(void);
static emxArray_creal_T *argInit_d1200x14_creal_T(void);
static void argInit_d1xd6_char_T(char result_data[], int result_size[2]);
static double argInit_real_T(void);
static void argInit_struct0_T(struct0_T *result);
static void argInit_struct1_T(struct1_T *result);
static void main_BCHIndices(void);
static void main_ChannelEst(void);

/* Function Definitions */

/*
 * Arguments    : char result[3]
 * Return Type  : void
 */
static void argInit_1x3_char_T(char result[3])
{
  char result_tmp;

  /* Loop over the array to initialize each element. */
  /* Set the value of the array element.
     Change this value to the value that the application requires. */
  result_tmp = argInit_char_T();
  result[0] = result_tmp;

  /* Set the value of the array element.
     Change this value to the value that the application requires. */
  result[1] = result_tmp;

  /* Set the value of the array element.
     Change this value to the value that the application requires. */
  result[2] = result_tmp;
}

/*
 * Arguments    : char result[6]
 * Return Type  : void
 */
static void argInit_1x6_char_T(char result[6])
{
  int idx1;

  /* Loop over the array to initialize each element. */
  for (idx1 = 0; idx1 < 6; idx1++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result[idx1] = argInit_char_T();
  }
}

/*
 * Arguments    : void
 * Return Type  : char
 */
static char argInit_char_T(void)
{
  return '?';
}

/*
 * Arguments    : void
 * Return Type  : creal_T
 */
static creal_T argInit_creal_T(void)
{
  creal_T result;
  double re_tmp;

  /* Set the value of the complex variable.
     Change this value to the value that the application requires. */
  re_tmp = argInit_real_T();
  result.re = re_tmp;
  result.im = re_tmp;
  return result;
}

/*
 * Arguments    : void
 * Return Type  : emxArray_creal_T *
 */
static emxArray_creal_T *argInit_d1200x14_creal_T(void)
{
  emxArray_creal_T *result;
  int idx0;
  int idx1;

  /* Set the size of the array.
     Change this size to the value that the application requires. */
  result = emxCreate_creal_T(2, 14);

  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < result->size[0U]; idx0++) {
    for (idx1 = 0; idx1 < 14; idx1++) {
      /* Set the value of the array element.
         Change this value to the value that the application requires. */
      result->data[idx0 + result->size[0] * idx1] = argInit_creal_T();
    }
  }

  return result;
}

/*
 * Arguments    : char result_data[]
 *                int result_size[2]
 * Return Type  : void
 */
static void argInit_d1xd6_char_T(char result_data[], int result_size[2])
{
  int idx1;

  /* Set the size of the array.
     Change this size to the value that the application requires. */
  result_size[0] = 1;
  result_size[1] = 2;

  /* Loop over the array to initialize each element. */
  for (idx1 = 0; idx1 < 2; idx1++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result_data[idx1] = argInit_char_T();
  }
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
 * Arguments    : struct0_T *result
 * Return Type  : void
 */
static void argInit_struct0_T(struct0_T *result)
{
  double result_tmp;

  /* Set the value of each structure field.
     Change this value to the value that the application requires. */
  result_tmp = argInit_real_T();
  result->NCellID = result_tmp;
  result->NSubframe = result_tmp;
  result->TDDConfig = result_tmp;
  result->SSC = result_tmp;
  result->CellRefP = result_tmp;
  result->NFrame = result_tmp;
  result->CFI = result_tmp;
  result->NDLRB = result_tmp;
  argInit_1x3_char_T(result->DuplexMode);
  argInit_1x6_char_T(result->CyclicPrefix);
}

/*
 * Arguments    : struct1_T *result
 * Return Type  : void
 */
static void argInit_struct1_T(struct1_T *result)
{
  double result_tmp;

  /* Set the value of each structure field.
     Change this value to the value that the application requires. */
  result_tmp = argInit_real_T();
  result->NCellID = result_tmp;
  result->NSubframe = result_tmp;
  result->TDDConfig = result_tmp;
  result->SSC = result_tmp;
  result->CellRefP = result_tmp;
  result->NFrame = result_tmp;
  result->CFI = result_tmp;
  result->NDLRB = result_tmp;
  argInit_1x3_char_T(result->DuplexMode);
  argInit_1x6_char_T(result->CyclicPrefix);
  argInit_d1xd6_char_T(result->PHICHDuration.data, result->PHICHDuration.size);
  argInit_d1xd6_char_T(result->Ng.data, result->Ng.size);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void main_BCHIndices(void)
{
  struct0_T r;
  double index_data[288];
  int index_size[2];

  /* Initialize function 'BCHIndices' input arguments. */
  /* Initialize function input argument 'enb'. */
  /* Call the entry-point 'BCHIndices'. */
  argInit_struct0_T(&r);
  BCHIndices(&r, index_data, index_size);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void main_ChannelEst(void)
{
  emxArray_creal_T *TempChan;
  emxArray_creal_T *rcvSigFreq;
  double mmseA1_tmp;
  struct1_T r;
  emxInitArray_creal_T(&TempChan, 3);

  /* Initialize function 'ChannelEst' input arguments. */
  /* Initialize function input argument 'rcvSigFreq'. */
  rcvSigFreq = argInit_d1200x14_creal_T();
  mmseA1_tmp = argInit_real_T();

  /* Initialize function input argument 'ENB'. */
  /* Call the entry-point 'ChannelEst'. */
  argInit_struct1_T(&r);
  ChannelEst(rcvSigFreq, mmseA1_tmp, mmseA1_tmp, mmseA1_tmp, mmseA1_tmp, &r,
             TempChan);
  emxDestroyArray_creal_T(TempChan);
  emxDestroyArray_creal_T(rcvSigFreq);
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
  main_BCHIndices();
  main_ChannelEst();

  /* Terminate the application.
     You do not need to do this more than one time. */
  BCHIndices_terminate();
  return 0;
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */

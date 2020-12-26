/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: BCHIndices_terminate.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 26-Dec-2020 21:05:29
 */

/* Include Files */
#include "BCHIndices_terminate.h"
#include "BCHIndices.h"
#include "BCHIndices_data.h"
#include "ChannelEst.h"
#include "rt_nonfinite.h"

/* Function Definitions */

/*
 * Arguments    : void
 * Return Type  : void
 */
void BCHIndices_terminate(void)
{
  /* (no terminate code required) */
  isInitialized_BCHIndices = false;
}

/*
 * File trailer for BCHIndices_terminate.c
 *
 * [EOF]
 */

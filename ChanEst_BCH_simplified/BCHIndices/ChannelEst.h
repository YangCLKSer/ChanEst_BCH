/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: ChannelEst.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 05-Jan-2021 17:59:07
 */

#ifndef CHANNELEST_H
#define CHANNELEST_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "BCHIndices_types.h"

/* Function Declarations */
extern void ChannelEst(const emxArray_creal_T *rcvSigFreq, double mmseA1, double
  mmseB1, double mmseA2, double mmseB2, const struct1_T *ENB, emxArray_creal_T
  *TempChan);

#endif

/*
 * File trailer for ChannelEst.h
 *
 * [EOF]
 */

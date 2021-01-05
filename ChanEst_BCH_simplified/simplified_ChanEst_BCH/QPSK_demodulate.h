/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: QPSK_demodulate.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 05-Jan-2021 17:59:07
 */

#ifndef QPSK_DEMODULATE_H
#define QPSK_DEMODULATE_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "BCHIndices_types.h"

/* Function Declarations */
extern void QPSK_demodulate(const creal_T inSymbols[240], const double inPower
  [240], double outBits[480]);

#endif

/*
 * File trailer for QPSK_demodulate.h
 *
 * [EOF]
 */

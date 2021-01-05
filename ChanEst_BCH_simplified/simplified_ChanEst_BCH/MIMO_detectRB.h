/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: MIMO_detectRB.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 05-Jan-2021 17:59:07
 */

#ifndef MIMO_DETECTRB_H
#define MIMO_DETECTRB_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "BCHIndices_types.h"

/* Function Declarations */
extern void MIMO_detectRB(const creal_T RxData[240], const creal_T equCH[960],
  double methodDetect, creal_T Detected_RB[240], double ampd[240]);

#endif

/*
 * File trailer for MIMO_detectRB.h
 *
 * [EOF]
 */

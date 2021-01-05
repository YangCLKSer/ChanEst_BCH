/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: PBCH_Decode.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 05-Jan-2021 17:59:07
 */

#ifndef PBCH_DECODE_H
#define PBCH_DECODE_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "BCHIndices_types.h"

/* Function Declarations */
extern void PBCH_Decode(const creal_T sym[240], const creal_T chanEst[960],
  const struct0_T *enb, creal_T bits[480], creal_T symbols[240], double *nfmod4,
  double trblk[24], double *cellrefp);

#endif

/*
 * File trailer for PBCH_Decode.h
 *
 * [EOF]
 */

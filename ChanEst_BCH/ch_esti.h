/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: ch_esti.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 26-Dec-2020 21:05:29
 */

#ifndef CH_ESTI_H
#define CH_ESTI_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "BCHIndices_types.h"

/* Function Declarations */
extern void ch_esti(const emxArray_creal_T *RxDataBCE, double enb_NDLRB, const
                    char enb_CyclicPrefix[6], double enb_NCellID, double
                    enb_NSubframe, double enb_CellRefP, emxArray_creal_T *hEst);

#endif

/*
 * File trailer for ch_esti.h
 *
 * [EOF]
 */

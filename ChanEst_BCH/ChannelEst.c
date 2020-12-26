/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: ChannelEst.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 26-Dec-2020 21:05:29
 */

/* Include Files */
#include "ChannelEst.h"
#include "BCHIndices.h"
#include "BCHIndices_data.h"
#include "BCHIndices_emxutil.h"
#include "BCHIndices_initialize.h"
#include "ch_esti.h"
#include "rt_nonfinite.h"

/* Function Definitions */

/*
 * Arguments    : const emxArray_creal_T *rcvSigFreq
 *                double mmseA1
 *                double mmseB1
 *                double mmseA2
 *                double mmseB2
 *                const struct1_T *ENB
 *                emxArray_creal_T *TempChan
 * Return Type  : void
 */
void ChannelEst(const emxArray_creal_T *rcvSigFreq, double mmseA1, double mmseB1,
                double mmseA2, double mmseB2, const struct1_T *ENB,
                emxArray_creal_T *TempChan)
{
  emxArray_creal_T *rcvSigFreq72;
  int i;
  int loop_ub;
  emxArray_creal_T *b_rcvSigFreq;
  int b_loop_ub;
  int i1;
  emxArray_creal_T *hEst;
  emxArray_creal_T *b_hEst;
  int c_loop_ub;
  (void)mmseA1;
  (void)mmseB1;
  (void)mmseA2;
  (void)mmseB2;
  if (!isInitialized_BCHIndices) {
    BCHIndices_initialize();
  }

  emxInit_creal_T(&rcvSigFreq72, 2);
  i = rcvSigFreq72->size[0] * rcvSigFreq72->size[1];
  rcvSigFreq72->size[0] = 1;
  loop_ub = (int)(14.0 * ENB->NDLRB * 12.0);
  rcvSigFreq72->size[1] = loop_ub;
  emxEnsureCapacity_creal_T(rcvSigFreq72, i);
  for (i = 0; i < loop_ub; i++) {
    rcvSigFreq72->data[i].re = 0.0;
    rcvSigFreq72->data[i].im = 0.0;
  }

  emxInit_creal_T(&b_rcvSigFreq, 2);
  b_loop_ub = rcvSigFreq->size[0];
  i = b_rcvSigFreq->size[0] * b_rcvSigFreq->size[1];
  b_rcvSigFreq->size[0] = rcvSigFreq->size[0];
  b_rcvSigFreq->size[1] = 14;
  emxEnsureCapacity_creal_T(b_rcvSigFreq, i);
  for (i = 0; i < 14; i++) {
    for (i1 = 0; i1 < b_loop_ub; i1++) {
      b_rcvSigFreq->data[i1 + b_rcvSigFreq->size[0] * i] = rcvSigFreq->data[i1 +
        rcvSigFreq->size[0] * i];
    }
  }

  for (i = 0; i < loop_ub; i++) {
    rcvSigFreq72->data[i] = b_rcvSigFreq->data[i];
  }

  emxFree_creal_T(&b_rcvSigFreq);
  emxInit_creal_T(&hEst, 2);
  ch_esti(rcvSigFreq72, ENB->NDLRB, ENB->CyclicPrefix, ENB->NCellID,
          ENB->NSubframe, ENB->CellRefP, hEst);

  /*      [ hEst ] = pcfich_ch_esti( rcvSigFreq72, ns,mmseA1, mmseB1, mmseA2, mmseB2 ); */
  i = TempChan->size[0] * TempChan->size[1] * TempChan->size[2];
  TempChan->size[0] = 1;
  TempChan->size[1] = hEst->size[0];
  TempChan->size[2] = hEst->size[1];
  emxEnsureCapacity_creal_T(TempChan, i);
  loop_ub = hEst->size[0] * hEst->size[1];
  emxFree_creal_T(&rcvSigFreq72);
  for (i = 0; i < loop_ub; i++) {
    TempChan->data[i].re = 0.0;
    TempChan->data[i].im = 0.0;
  }

  emxInit_creal_T(&b_hEst, 2);
  if (1 > hEst->size[0]) {
    loop_ub = 0;
  } else {
    loop_ub = hEst->size[0];
  }

  b_loop_ub = TempChan->size[1];
  c_loop_ub = hEst->size[1] - 1;
  i = b_hEst->size[0] * b_hEst->size[1];
  b_hEst->size[0] = loop_ub;
  b_hEst->size[1] = hEst->size[1];
  emxEnsureCapacity_creal_T(b_hEst, i);
  for (i = 0; i <= c_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_hEst->data[i1 + b_hEst->size[0] * i] = hEst->data[i1 + hEst->size[0] * i];
    }
  }

  i = hEst->size[0] * hEst->size[1];
  hEst->size[0] = b_hEst->size[0];
  hEst->size[1] = b_hEst->size[1];
  emxEnsureCapacity_creal_T(hEst, i);
  loop_ub = b_hEst->size[1];
  for (i = 0; i < loop_ub; i++) {
    c_loop_ub = b_hEst->size[0];
    for (i1 = 0; i1 < c_loop_ub; i1++) {
      hEst->data[i1 + hEst->size[0] * i] = b_hEst->data[i1 + b_hEst->size[0] * i];
    }
  }

  emxFree_creal_T(&b_hEst);
  c_loop_ub = TempChan->size[1];
  loop_ub = TempChan->size[2];
  for (i = 0; i < loop_ub; i++) {
    for (i1 = 0; i1 < b_loop_ub; i1++) {
      TempChan->data[i1 + TempChan->size[1] * i] = hEst->data[i1 + c_loop_ub * i];
    }
  }

  emxFree_creal_T(&hEst);
}

/*
 * File trailer for ChannelEst.c
 *
 * [EOF]
 */

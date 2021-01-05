/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: ch_esti.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 05-Jan-2021 17:59:07
 */

/* Include Files */
#include "ch_esti.h"
#include "BCHIndices.h"
#include "BCHIndices_emxutil.h"
#include "ChannelEst.h"
#include "PBCH_Decode.h"
#include "ch_esti_dct.h"
#include "ch_esti_prseqGen.h"
#include "ch_esti_time_intp.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */

/*
 * Channel Estimation function
 *  Abstract:
 *    This function deals with the received data and produces the channel
 *    estimation results, also removes the RS for later produres.
 *  Input:
 *    RxDataBCE, received data before channel estimation,
 *        size() = [ numRxAnt, Len ], Len = numRBDL*12
 *    methodCE, method used for channel estimation, ABCD
 *        A: 2D estimation,
 *        B: time interp, 1-linear
 *        C: freq interp, 1-linear, 2-mmse, 3-dft, 4-dct
 *        D: rs estimation, 1-ls, 2-mmse
 *        Note that, mmse algorithm and 2D estimation can not be used.
 *    Pc, cut off windows for dft and dct algorithm, 0.2 - 1.0
 *    startSlot, the index of start slot of RxDataBCE
 *    NID, ID number of Cell
 *    numTxAnt, number of Tx antenna
 *        Note that, numTxAnt is assumed to be 1 for current version
 *    numRBDL, number of resource blocks downlink
 *    numSymDL, number of ofdms per slot, sometimes = numSym
 *    NCP, type of CP, 1-normal, 0-extend
 *  Output:
 *    hEst, channel freq estimation output, with rs
 *        size() = [ numTxAnt*numRxAnt, Len ], Len = Len in RxDataBCE.
 *  Reference:
 *    [1] 3GPP TS 36.211 V8.4.0 (2008-09).
 *  Author:
 *    Yang Yushan     2009-04-09
 *  Version History:
 *    Yang Yushan     2009-04-09      Created
 * ------------------------------------------------------------------------------%
 *  assume that numTxAnt = 1 for current version.
 * Arguments    : const emxArray_creal_T *RxDataBCE
 *                double enb_NDLRB
 *                const char enb_CyclicPrefix[6]
 *                double enb_NCellID
 *                double enb_NSubframe
 *                double enb_CellRefP
 *                emxArray_creal_T *hEst
 * Return Type  : void
 */
void ch_esti(const emxArray_creal_T *RxDataBCE, double enb_NDLRB, const char
             enb_CyclicPrefix[6], double enb_NCellID, double enb_NSubframe,
             double enb_CellRefP, emxArray_creal_T *hEst)
{
  double startSlot;
  int i;
  boolean_T empty_non_axis_sizes;
  boolean_T x[6];
  static const char cv[6] = { 'N', 'o', 'r', 'm', 'a', 'l' };

  int k;
  boolean_T exitg1;
  int NCP;
  int numSymDL;
  double numOFDM;
  int i1;
  int loop_ub;
  emxArray_real_T *locOFDMWithRS;
  emxArray_real_T *locRS;
  emxArray_creal_T *valRS;
  emxArray_creal_T *temphEst;
  emxArray_real_T *tempLoc;
  emxArray_creal_T *tempVal;
  emxArray_uint32_T *b_locOFDMWithRS;
  emxArray_real_T *m;
  emxArray_int32_T *r;
  emxArray_creal_T *b_RxDataBCE;
  emxArray_real_T *b_locRS;
  emxArray_creal_T *b_valRS;
  int idxAntPort;
  double idxSlot;
  int i_ofdm;
  int i2;
  double V;
  boolean_T guard1 = false;
  double Vshift;
  int n;
  double b;
  int i3;
  int b_loop_ub;
  int i4;
  int c_loop_ub;
  double C[440];
  signed char i5;
  int i6;
  emxArray_real_T c_locOFDMWithRS;
  int d_locOFDMWithRS[1];
  int e_locOFDMWithRS[1];
  startSlot = enb_NSubframe * 2.0;
  for (i = 0; i < 6; i++) {
    x[i] = (enb_CyclicPrefix[i] == cv[i]);
  }

  empty_non_axis_sizes = true;
  k = 0;
  exitg1 = false;
  while ((!exitg1) && (k < 6)) {
    if (!x[k]) {
      empty_non_axis_sizes = false;
      exitg1 = true;
    } else {
      k++;
    }
  }

  if (empty_non_axis_sizes) {
    NCP = 1;
    numSymDL = 7;
  } else {
    NCP = 0;
    numSymDL = 6;
  }

  /*  init */
  numOFDM = (double)RxDataBCE->size[1] / enb_NDLRB / 12.0;

  /*  number of RS per ofdm */
  i = (int)enb_CellRefP;
  i1 = hEst->size[0] * hEst->size[1];
  hEst->size[0] = i;
  hEst->size[1] = RxDataBCE->size[1];
  emxEnsureCapacity_creal_T(hEst, i1);
  loop_ub = i * RxDataBCE->size[1];
  for (i = 0; i < loop_ub; i++) {
    hEst->data[i].re = 0.0;
    hEst->data[i].im = 0.0;
  }

  i = (int)((enb_CellRefP - 1.0) + 1.0);
  emxInit_real_T(&locOFDMWithRS, 2);
  emxInit_real_T(&locRS, 2);
  emxInit_creal_T(&valRS, 2);
  emxInit_creal_T(&temphEst, 2);
  emxInit_real_T(&tempLoc, 2);
  emxInit_creal_T(&tempVal, 2);
  emxInit_uint32_T(&b_locOFDMWithRS, 1);
  emxInit_real_T(&m, 2);
  emxInit_int32_T(&r, 1);
  emxInit_creal_T(&b_RxDataBCE, 2);
  emxInit_real_T(&b_locRS, 2);
  emxInit_creal_T(&b_valRS, 2);
  for (idxAntPort = 0; idxAntPort < i; idxAntPort++) {
    /*     %% recover reference signals */
    locOFDMWithRS->size[0] = 0;
    locOFDMWithRS->size[1] = 1;
    locRS->size[0] = 0;
    locRS->size[1] = 0;
    valRS->size[0] = 0;
    valRS->size[1] = 0;
    idxSlot = startSlot;
    i1 = (int)((numOFDM - 1.0) + 1.0);
    for (i_ofdm = 0; i_ofdm < i1; i_ofdm++) {
      if (i_ofdm == 0) {
        k = 0;
      } else {
        k = (int)fmod(i_ofdm, numSymDL);
      }

      if ((k == 0) && (0 != i_ofdm)) {
        idxSlot++;
      }

      /*  Reference Signals Generation method, current only for Cell-specific. */
      /*  Abstract:  */
      /*    Reference signals generation contains two parts, sequence generation and */
      /*    mapping to resource elements. */
      /*  Input: */
      /*    idxSlot, index of slot, 0,1,...,19. */
      /*    idxSym, index of symbol, 0,1,2,3,4,5,6. */
      /*    NID, number of Cell ID */
      /*    numRBDL, number of resource blocks for downlink */
      /*    idxAntPort, index of antenna port, 0,1,2,3. */
      /*  Output:    */
      /*    locRS, location of reference signals */
      /*    valRS, value of reference signals corresponding to locRS */
      /*  Note that, if the idxSym has no reference signals, locRS and valRS return [] */
      /*  Reference:  */
      /*    [1] 3GPP TS 36.211 V8.4.0 (2008-09), 6.10.1, pp.62-64 */
      /*  Author:    */
      /*    Yang Yushan     2009-04-09 */
      /*  Version History: */
      /*    Yang Yushan     2009-04-09      Revised */
      /*    Wang Qingwen    Created */
      /* ------------------------------------------------------------------------------% */
      /*  a const defined by 3GPP LTE Downlink */
      /* NCP = 1; % normal CP */
      /* numSymDL = 7; */
      tempLoc->size[0] = 0;
      tempLoc->size[1] = 0;
      tempVal->size[0] = 0;
      tempVal->size[1] = 0;
      V = 0.0;

      /*  RS location generation */
      guard1 = false;
      switch (idxAntPort) {
       case 0:
        if (0 == k) {
          guard1 = true;
        } else {
          if (numSymDL - 3 == k) {
            V = 3.0;
            guard1 = true;
          }
        }
        break;

       case 1:
        if (0 == k) {
          V = 3.0;
          guard1 = true;
        } else {
          if (numSymDL - 3 == k) {
            guard1 = true;
          }
        }
        break;

       case 2:
        if (1 == k) {
          if (rtIsNaN(idxSlot) || rtIsInf(idxSlot)) {
            V = rtNaN;
          } else if (idxSlot == 0.0) {
            V = 0.0;
          } else {
            V = fmod(idxSlot, 2.0);
            if (V == 0.0) {
              V = 0.0;
            } else {
              if (idxSlot < 0.0) {
                V += 2.0;
              }
            }
          }

          V *= 3.0;
          guard1 = true;
        }
        break;

       case 3:
        if (1 == k) {
          if (rtIsNaN(idxSlot) || rtIsInf(idxSlot)) {
            V = rtNaN;
          } else if (idxSlot == 0.0) {
            V = 0.0;
          } else {
            V = fmod(idxSlot, 2.0);
            if (V == 0.0) {
              V = 0.0;
            } else {
              if (idxSlot < 0.0) {
                V += 2.0;
              }
            }
          }

          V = 3.0 * V + 3.0;
          guard1 = true;
        }
        break;

       default:
        guard1 = true;
        break;
      }

      if (guard1) {
        if (rtIsNaN(enb_NCellID) || rtIsInf(enb_NCellID)) {
          Vshift = rtNaN;
        } else if (enb_NCellID == 0.0) {
          Vshift = 0.0;
        } else {
          Vshift = fmod(enb_NCellID, 6.0);
          if (Vshift == 0.0) {
            Vshift = 0.0;
          } else {
            if (enb_NCellID < 0.0) {
              Vshift += 6.0;
            }
          }
        }

        b = 2.0 * enb_NDLRB - 1.0;
        if (rtIsNaN(b)) {
          i2 = m->size[0] * m->size[1];
          m->size[0] = 1;
          m->size[1] = 1;
          emxEnsureCapacity_real_T(m, i2);
          m->data[0] = rtNaN;
        } else if (b < 0.0) {
          m->size[0] = 1;
          m->size[1] = 0;
        } else if (rtIsInf(b) && (0.0 == b)) {
          i2 = m->size[0] * m->size[1];
          m->size[0] = 1;
          m->size[1] = 1;
          emxEnsureCapacity_real_T(m, i2);
          m->data[0] = rtNaN;
        } else {
          i2 = m->size[0] * m->size[1];
          m->size[0] = 1;
          loop_ub = (int)floor(b);
          m->size[1] = loop_ub + 1;
          emxEnsureCapacity_real_T(m, i2);
          for (i2 = 0; i2 <= loop_ub; i2++) {
            m->data[i2] = i2;
          }
        }

        Vshift += V;
        if (rtIsNaN(Vshift)) {
          V = rtNaN;
        } else if (Vshift == 0.0) {
          V = 0.0;
        } else {
          V = fmod(Vshift, 6.0);
          if (V == 0.0) {
            V = 0.0;
          } else {
            if (Vshift < 0.0) {
              V += 6.0;
            }
          }
        }

        i2 = tempLoc->size[0] * tempLoc->size[1];
        tempLoc->size[0] = 1;
        tempLoc->size[1] = m->size[1];
        emxEnsureCapacity_real_T(tempLoc, i2);
        loop_ub = m->size[0] * m->size[1];
        for (i2 = 0; i2 < loop_ub; i2++) {
          tempLoc->data[i2] = (6.0 * m->data[i2] + V) + 1.0;
        }

        /*  RS seq generation */
        ch_esti_prseqGen((1024.0 * ((7.0 * (idxSlot + 1.0) + (double)k) + 1.0) *
                          (2.0 * enb_NCellID + 1.0) + 2.0 * enb_NCellID) +
                         (double)NCP, C);

        /*  RS value correspondint to locRS generation */
        i2 = tempVal->size[0] * tempVal->size[1];
        tempVal->size[0] = 1;
        tempVal->size[1] = m->size[1];
        emxEnsureCapacity_creal_T(tempVal, i2);
        loop_ub = m->size[0] * m->size[1];
        for (i2 = 0; i2 < loop_ub; i2++) {
          k = ((int)(((m->data[i2] + 110.0) - enb_NDLRB) + 1.0) - 1) << 1;
          V = 1.0 - 2.0 * C[k + 1];
          tempVal->data[i2].re = 0.70710678118654746 * ((1.0 - 2.0 * C[k]) + 0.0
            * V);
          tempVal->data[i2].im = 0.70710678118654746 * V;
        }
      }

      if ((tempLoc->size[0] == 0) || (tempLoc->size[1] == 0)) {
        n = 0;
      } else {
        n = tempLoc->size[1];
      }

      if (0 != n) {
        loop_ub = locOFDMWithRS->size[0];
        i2 = b_locOFDMWithRS->size[0];
        b_locOFDMWithRS->size[0] = locOFDMWithRS->size[0] + 1;
        emxEnsureCapacity_uint32_T(b_locOFDMWithRS, i2);
        for (i2 = 0; i2 < loop_ub; i2++) {
          b_locOFDMWithRS->data[i2] = (unsigned int)locOFDMWithRS->data[i2];
        }

        b_locOFDMWithRS->data[locOFDMWithRS->size[0]] = i_ofdm + 1U;
        i2 = locOFDMWithRS->size[0] * locOFDMWithRS->size[1];
        locOFDMWithRS->size[0] = b_locOFDMWithRS->size[0];
        locOFDMWithRS->size[1] = 1;
        emxEnsureCapacity_real_T(locOFDMWithRS, i2);
        loop_ub = b_locOFDMWithRS->size[0];
        for (i2 = 0; i2 < loop_ub; i2++) {
          locOFDMWithRS->data[i2] = b_locOFDMWithRS->data[i2];
        }

        if ((locRS->size[0] != 0) && (locRS->size[1] != 0)) {
          k = locRS->size[1];
        } else if ((tempLoc->size[0] != 0) && (tempLoc->size[1] != 0)) {
          k = tempLoc->size[1];
        } else {
          k = locRS->size[1];
          if (k <= 0) {
            k = 0;
          }

          if (tempLoc->size[1] > k) {
            k = tempLoc->size[1];
          }
        }

        empty_non_axis_sizes = (k == 0);
        if (empty_non_axis_sizes || ((locRS->size[0] != 0) && (locRS->size[1] !=
              0))) {
          loop_ub = locRS->size[0];
        } else {
          loop_ub = 0;
        }

        if (empty_non_axis_sizes || ((tempLoc->size[0] != 0) && (tempLoc->size[1]
              != 0))) {
          i5 = (signed char)tempLoc->size[0];
        } else {
          i5 = 0;
        }

        i2 = b_locRS->size[0] * b_locRS->size[1];
        b_locRS->size[0] = loop_ub + i5;
        b_locRS->size[1] = k;
        emxEnsureCapacity_real_T(b_locRS, i2);
        for (i2 = 0; i2 < k; i2++) {
          for (i6 = 0; i6 < loop_ub; i6++) {
            b_locRS->data[i6 + b_locRS->size[0] * i2] = locRS->data[i6 + loop_ub
              * i2];
          }
        }

        for (i2 = 0; i2 < k; i2++) {
          n = i5;
          for (i6 = 0; i6 < n; i6++) {
            b_locRS->data[(i6 + loop_ub) + b_locRS->size[0] * i2] =
              tempLoc->data[i6 + tempLoc->size[0] * i2];
          }
        }

        i2 = locRS->size[0] * locRS->size[1];
        locRS->size[0] = b_locRS->size[0];
        locRS->size[1] = b_locRS->size[1];
        emxEnsureCapacity_real_T(locRS, i2);
        loop_ub = b_locRS->size[0] * b_locRS->size[1];
        for (i2 = 0; i2 < loop_ub; i2++) {
          locRS->data[i2] = b_locRS->data[i2];
        }

        if ((valRS->size[0] != 0) && (valRS->size[1] != 0)) {
          k = valRS->size[1];
        } else if ((tempVal->size[0] != 0) && (tempVal->size[1] != 0)) {
          k = tempVal->size[1];
        } else {
          k = valRS->size[1];
          if (k <= 0) {
            k = 0;
          }

          if (tempVal->size[1] > k) {
            k = tempVal->size[1];
          }
        }

        empty_non_axis_sizes = (k == 0);
        if (empty_non_axis_sizes || ((valRS->size[0] != 0) && (valRS->size[1] !=
              0))) {
          loop_ub = valRS->size[0];
        } else {
          loop_ub = 0;
        }

        if (empty_non_axis_sizes || ((tempVal->size[0] != 0) && (tempVal->size[1]
              != 0))) {
          i5 = (signed char)tempVal->size[0];
        } else {
          i5 = 0;
        }

        i2 = b_valRS->size[0] * b_valRS->size[1];
        b_valRS->size[0] = loop_ub + i5;
        b_valRS->size[1] = k;
        emxEnsureCapacity_creal_T(b_valRS, i2);
        for (i2 = 0; i2 < k; i2++) {
          for (i6 = 0; i6 < loop_ub; i6++) {
            b_valRS->data[i6 + b_valRS->size[0] * i2] = valRS->data[i6 + loop_ub
              * i2];
          }
        }

        for (i2 = 0; i2 < k; i2++) {
          n = i5;
          for (i6 = 0; i6 < n; i6++) {
            b_valRS->data[(i6 + loop_ub) + b_valRS->size[0] * i2] =
              tempVal->data[i6 + tempVal->size[0] * i2];
          }
        }

        i2 = valRS->size[0] * valRS->size[1];
        valRS->size[0] = b_valRS->size[0];
        valRS->size[1] = b_valRS->size[1];
        emxEnsureCapacity_creal_T(valRS, i2);
        loop_ub = b_valRS->size[0] * b_valRS->size[1];
        for (i2 = 0; i2 < loop_ub; i2++) {
          valRS->data[i2] = b_valRS->data[i2];
        }
      }
    }

    /*  default value */
    /*      if (idxAntPort<2) */
    /*          numOFDMWithRSPS = 2; */
    /*      else */
    /*          numOFDMWithRSPS = 1; */
    /*      end */
    /*     %% estimation with locRS and valRS */
    /*      temphEst = ch_esti_unit( RxData, methodCE, Pc, mmseA1, mmseB1, mmseA2, mmseB2, locOFDMWithRS, locRS, valRS ); */
    /*  ls on rs locations */
    loop_ub = RxDataBCE->size[1];
    i1 = b_RxDataBCE->size[0] * b_RxDataBCE->size[1];
    b_RxDataBCE->size[0] = 1;
    b_RxDataBCE->size[1] = RxDataBCE->size[1];
    emxEnsureCapacity_creal_T(b_RxDataBCE, i1);
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_RxDataBCE->data[i1] = RxDataBCE->data[i1];
    }

    k = (int)(enb_NDLRB * 12.0);
    i1 = b_valRS->size[0] * b_valRS->size[1];
    loop_ub = (int)numOFDM;
    b_valRS->size[0] = loop_ub;
    b_valRS->size[1] = k;
    emxEnsureCapacity_creal_T(b_valRS, i1);
    for (i1 = 0; i1 < k; i1++) {
      for (i2 = 0; i2 < loop_ub; i2++) {
        b_valRS->data[i2 + b_valRS->size[0] * i1] = b_RxDataBCE->data[i1 + k *
          i2];
      }
    }

    /*  LS estimation of channel frequency response on RS locations */
    /*  Abstract:  */
    /*    This function is ls estimation of channel frequency response. */
    /*  Input: */
    /*    RxDataBCE, the input Rx data before channel estimation. */
    /*    locOFDMWithRS, locations of ofdms with rs. */
    /*    locRS, locations of rs in ofdms with rs. */
    /*    valRS, value of rs, the same as that in trasmitter, after recovered. */
    /*  Output:    */
    /*    hEst, channel freq estimation output after ls estimation. */
    /*  Reference:  */
    /*    NULL. */
    /*  Author:    */
    /*    Yang Yushan     2009-04-09 */
    /*  Version History: */
    /*    Yang Yushan     2009-04-09      Created */
    /* ------------------------------------------------------------------------------% */
    i1 = temphEst->size[0] * temphEst->size[1];
    temphEst->size[0] = b_valRS->size[0];
    temphEst->size[1] = b_valRS->size[1];
    emxEnsureCapacity_creal_T(temphEst, i1);
    loop_ub = b_valRS->size[0] * b_valRS->size[1];
    for (i1 = 0; i1 < loop_ub; i1++) {
      temphEst->data[i1].re = 0.0;
      temphEst->data[i1].im = 0.0;
    }

    /*  estimation on reference symbols */
    /* ----- LS estimation */
    i1 = locOFDMWithRS->size[0];
    if (0 <= locOFDMWithRS->size[0] - 1) {
      i3 = locRS->size[1];
      b_loop_ub = locRS->size[1];
      i4 = locRS->size[1];
      c_loop_ub = locRS->size[1];
    }

    for (n = 0; n < i1; n++) {
      i2 = r->size[0];
      r->size[0] = i3;
      emxEnsureCapacity_int32_T(r, i2);
      for (i2 = 0; i2 < b_loop_ub; i2++) {
        r->data[i2] = (int)locRS->data[n + locRS->size[0] * i2] - 1;
      }

      k = (int)(unsigned int)locOFDMWithRS->data[n];
      i2 = b_RxDataBCE->size[0] * b_RxDataBCE->size[1];
      b_RxDataBCE->size[0] = 1;
      b_RxDataBCE->size[1] = i4;
      emxEnsureCapacity_creal_T(b_RxDataBCE, i2);
      for (i2 = 0; i2 < c_loop_ub; i2++) {
        b_RxDataBCE->data[i2] = b_valRS->data[(k + b_valRS->size[0] * ((int)
          locRS->data[n + locRS->size[0] * i2] - 1)) - 1];
      }

      loop_ub = b_RxDataBCE->size[1];
      for (i2 = 0; i2 < loop_ub; i2++) {
        if (valRS->data[n + valRS->size[0] * i2].im == 0.0) {
          if (b_RxDataBCE->data[i2].im == 0.0) {
            temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].re =
              b_RxDataBCE->data[i2].re / valRS->data[n + valRS->size[0] * i2].re;
            temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].im = 0.0;
          } else if (b_RxDataBCE->data[i2].re == 0.0) {
            temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].re = 0.0;
            temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].im =
              b_RxDataBCE->data[i2].im / valRS->data[n + valRS->size[0] * i2].re;
          } else {
            temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].re =
              b_RxDataBCE->data[i2].re / valRS->data[n + valRS->size[0] * i2].re;
            temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].im =
              b_RxDataBCE->data[i2].im / valRS->data[n + valRS->size[0] * i2].re;
          }
        } else if (valRS->data[n + valRS->size[0] * i2].re == 0.0) {
          if (b_RxDataBCE->data[i2].re == 0.0) {
            temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].re =
              b_RxDataBCE->data[i2].im / valRS->data[n + valRS->size[0] * i2].im;
            temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].im = 0.0;
          } else if (b_RxDataBCE->data[i2].im == 0.0) {
            temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].re = 0.0;
            temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].im =
              -(b_RxDataBCE->data[i2].re / valRS->data[n + valRS->size[0] * i2].
                im);
          } else {
            temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].re =
              b_RxDataBCE->data[i2].im / valRS->data[n + valRS->size[0] * i2].im;
            temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].im =
              -(b_RxDataBCE->data[i2].re / valRS->data[n + valRS->size[0] * i2].
                im);
          }
        } else {
          b = fabs(valRS->data[n + valRS->size[0] * i2].re);
          V = fabs(valRS->data[n + valRS->size[0] * i2].im);
          if (b > V) {
            V = valRS->data[n + valRS->size[0] * i2].im / valRS->data[n +
              valRS->size[0] * i2].re;
            Vshift = valRS->data[n + valRS->size[0] * i2].re + V * valRS->data[n
              + valRS->size[0] * i2].im;
            temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].re =
              (b_RxDataBCE->data[i2].re + V * b_RxDataBCE->data[i2].im) / Vshift;
            temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].im =
              (b_RxDataBCE->data[i2].im - V * b_RxDataBCE->data[i2].re) / Vshift;
          } else if (V == b) {
            if (valRS->data[n + valRS->size[0] * i2].re > 0.0) {
              V = 0.5;
            } else {
              V = -0.5;
            }

            if (valRS->data[n + valRS->size[0] * i2].im > 0.0) {
              Vshift = 0.5;
            } else {
              Vshift = -0.5;
            }

            temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].re =
              (b_RxDataBCE->data[i2].re * V + b_RxDataBCE->data[i2].im * Vshift)
              / b;
            temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].im =
              (b_RxDataBCE->data[i2].im * V - b_RxDataBCE->data[i2].re * Vshift)
              / b;
          } else {
            V = valRS->data[n + valRS->size[0] * i2].re / valRS->data[n +
              valRS->size[0] * i2].im;
            Vshift = valRS->data[n + valRS->size[0] * i2].im + V * valRS->data[n
              + valRS->size[0] * i2].re;
            temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].re = (V *
              b_RxDataBCE->data[i2].re + b_RxDataBCE->data[i2].im) / Vshift;
            temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].im = (V *
              b_RxDataBCE->data[i2].im - b_RxDataBCE->data[i2].re) / Vshift;
          }
        }
      }
    }

    /*  plot(locRS(1,:), real(hEst(locOFDMWithRS(1),locRS(1,:))),'+r'); */
    /*  plot(locRS(2,:), real(hEst(locOFDMWithRS(2),locRS(2,:))),'or'); */
    /*  plot(locRS(3,:), real(hEst(locOFDMWithRS(3),locRS(3,:))),'+g'); */
    /*  plot(locRS(4,:), real(hEst(locOFDMWithRS(4),locRS(4,:))),'og'); */
    /*  time pre average          */
    /*          if(1==length(locOFDMWithRS)) */
    /*              sumLS = temphEst( locOFDMWithRS, : );  */
    /*              templocRS = locRS; */
    /*          else */
    /*              sumLS = sum( temphEst( locOFDMWithRS, : ) ) / length( locOFDMWithRS ) * 2;         */
    /*              templocRS = zeros( length(locOFDMWithRS), 2*numRS ); */
    /*              for k = 1 : length(locOFDMWithRS) */
    /*                  templocRS( k, 1:2:end ) = locRS( 1,: ); */
    /*                  templocRS( k, 2:2:end ) = locRS( 2,: ); */
    /*              end     */
    /*          end */
    /*          for k = 1 : length(locOFDMWithRS) */
    /*              temphEst( locOFDMWithRS(k), templocRS(k,:) ) = sumLS( templocRS(k,:) );      */
    /*          end */
    /*          % 1*RS average */
    /*          Pc = 0.1; % 1*RS average */
    /*          templocRS = locRS; */
    /*          % 2*RS average */
    /*          Pc = 0.05; % 2*RS average        */
    /*  freq interpolation */
    /* plot(locRS(1,:), real(temphEst(locOFDMWithRS(1),locRS(1,:))),'+r'); */
    /* plot(templocRS(1,:), real(temphEst(locOFDMWithRS(1),templocRS(1,:))),'+r'); */
    /* temphEst = ch_esti_lin( temphEst, locOFDMWithRS, locRS );     */
    k = locOFDMWithRS->size[0];
    c_locOFDMWithRS = *locOFDMWithRS;
    d_locOFDMWithRS[0] = k;
    c_locOFDMWithRS.size = &d_locOFDMWithRS[0];
    c_locOFDMWithRS.numDimensions = 1;
    ch_esti_dct(temphEst, &c_locOFDMWithRS, locRS);

    /* temphEst = ch_esti_dft( temphEst, locOFDMWithRS, templocRS, Pc ); */
    /* temphEst = ch_esti_lmmse( temphEst, locOFDMWithRS, locRS, mmseB1, mmseB2 ); */
    /*  time interpolation */
    k = locOFDMWithRS->size[0];
    c_locOFDMWithRS = *locOFDMWithRS;
    e_locOFDMWithRS[0] = k;
    c_locOFDMWithRS.size = &e_locOFDMWithRS[0];
    c_locOFDMWithRS.numDimensions = 1;
    ch_esti_time_intp(temphEst, &c_locOFDMWithRS);
    i1 = b_valRS->size[0] * b_valRS->size[1];
    b_valRS->size[0] = temphEst->size[1];
    b_valRS->size[1] = temphEst->size[0];
    emxEnsureCapacity_creal_T(b_valRS, i1);
    loop_ub = temphEst->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      n = temphEst->size[1];
      for (i2 = 0; i2 < n; i2++) {
        b_valRS->data[i2 + b_valRS->size[0] * i1] = temphEst->data[i1 +
          temphEst->size[0] * i2];
      }
    }

    k = (int)(enb_NDLRB * 12.0 * numOFDM);
    for (i1 = 0; i1 < k; i1++) {
      hEst->data[idxAntPort + hEst->size[0] * i1] = b_valRS->data[i1];
    }
  }

  emxFree_creal_T(&b_valRS);
  emxFree_real_T(&b_locRS);
  emxFree_creal_T(&b_RxDataBCE);
  emxFree_int32_T(&r);
  emxFree_real_T(&m);
  emxFree_uint32_T(&b_locOFDMWithRS);
  emxFree_creal_T(&tempVal);
  emxFree_real_T(&tempLoc);
  emxFree_creal_T(&temphEst);
  emxFree_creal_T(&valRS);
  emxFree_real_T(&locRS);
  emxFree_real_T(&locOFDMWithRS);
}

/*
 * File trailer for ch_esti.c
 *
 * [EOF]
 */

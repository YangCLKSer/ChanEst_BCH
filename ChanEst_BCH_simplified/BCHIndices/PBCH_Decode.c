/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: PBCH_Decode.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 05-Jan-2021 17:59:07
 */

/* Include Files */
#include "PBCH_Decode.h"
#include "BCHIndices.h"
#include "BCHIndices_data.h"
#include "BCHIndices_initialize.h"
#include "ChannelEst.h"
#include "MIMO_detectRB.h"
#include "QPSK_demodulate.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static void bch_decode(const double demod[480], double NID, creal_T sbits[480],
  double *nfmod4);

/* Function Definitions */

/*
 * Arguments    : const double demod[480]
 *                double NID
 *                creal_T sbits[480]
 *                double *nfmod4
 * Return Type  : void
 */
static void bch_decode(const double demod[480], double NID, creal_T sbits[480],
  double *nfmod4)
{
  double Cinit;
  signed char x1[3520];
  int i;
  double x2[3520];
  int x;
  double r;
  double PRsqnC[1920];
  *nfmod4 = 3.0;
  Cinit = NID;

  /* PRSEQ_GENR Summary of this function goes here */
  /*    Detailed explanation goes here */
  memset(&x1[0], 0, 3520U * sizeof(signed char));
  x1[0] = 1;
  for (i = 0; i < 3489; i++) {
    x = x1[i + 3] + x1[i];
    if (x == 0) {
      x1[i + 31] = 0;
    } else {
      x1[i + 31] = (signed char)fmod(x, 2.0);
    }
  }

  memset(&x2[0], 0, 3520U * sizeof(double));
  for (i = 0; i < 31; i++) {
    if (rtIsNaN(Cinit) || rtIsInf(Cinit)) {
      r = rtNaN;
    } else if (Cinit == 0.0) {
      r = 0.0;
    } else {
      r = fmod(Cinit, 2.0);
      if (r == 0.0) {
        r = 0.0;
      } else {
        if (Cinit < 0.0) {
          r += 2.0;
        }
      }
    }

    x2[i] = r;
    Cinit /= 2.0;
    Cinit = floor(Cinit);
  }

  for (i = 0; i < 3489; i++) {
    Cinit = ((x2[i + 3] + x2[i + 2]) + x2[i + 1]) + x2[i];
    if (rtIsNaN(Cinit)) {
      r = rtNaN;
    } else if (Cinit == 0.0) {
      r = 0.0;
    } else {
      r = fmod(Cinit, 2.0);
      if (r == 0.0) {
        r = 0.0;
      } else {
        if (Cinit < 0.0) {
          r += 2.0;
        }
      }
    }

    x2[i + 31] = r;
  }

  for (i = 0; i < 1920; i++) {
    Cinit = (double)x1[i + 1600] + x2[i + 1600];
    if (rtIsNaN(Cinit)) {
      r = rtNaN;
    } else if (Cinit == 0.0) {
      r = 0.0;
    } else {
      r = fmod(Cinit, 2.0);
      if (r == 0.0) {
        r = 0.0;
      } else {
        if (Cinit < 0.0) {
          r += 2.0;
        }
      }
    }

    PRsqnC[i] = r;
  }

  for (i = 0; i < 480; i++) {
    sbits[i].re = demod[i] * (1.0 - 2.0 * PRsqnC[i + 1440]);
    sbits[i].im = 0.0;
  }

  /* [trblk, sucess] = cc_decode(sbits(1:period), NumTxAnt, period); */
}

/*
 * Arguments    : const creal_T sym[240]
 *                const creal_T chanEst[960]
 *                const struct0_T *enb
 *                creal_T bits[480]
 *                creal_T symbols[240]
 *                double *nfmod4
 *                double trblk[24]
 *                double *cellrefp
 * Return Type  : void
 */
void PBCH_Decode(const creal_T sym[240], const creal_T chanEst[960], const
                 struct0_T *enb, creal_T bits[480], creal_T symbols[240], double
                 *nfmod4, double trblk[24], double *cellrefp)
{
  double chanAmp[240];
  double dv[480];
  if (!isInitialized_BCHIndices) {
    BCHIndices_initialize();
  }

  *cellrefp = 1.0;
  MIMO_detectRB(sym, chanEst, 0.0, symbols, chanAmp);
  QPSK_demodulate(symbols, chanAmp, dv);
  bch_decode(dv, enb->NCellID, bits, nfmod4);
  memset(&trblk[0], 0, 24U * sizeof(double));
}

/*
 * File trailer for PBCH_Decode.c
 *
 * [EOF]
 */

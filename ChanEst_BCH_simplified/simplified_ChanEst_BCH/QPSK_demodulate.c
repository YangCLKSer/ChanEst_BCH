/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: QPSK_demodulate.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 05-Jan-2021 17:59:07
 */

/* Include Files */
#include "QPSK_demodulate.h"
#include "BCHIndices.h"
#include "BCHIndices_rtwutil.h"
#include "ChannelEst.h"
#include "PBCH_Decode.h"
#include "ch_esti_dct.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */

/*
 * Arguments    : const creal_T inSymbols[240]
 *                const double inPower[240]
 *                double outBits[480]
 * Return Type  : void
 */
void QPSK_demodulate(const creal_T inSymbols[240], const double inPower[240],
                     double outBits[480])
{
  int k;
  double d;
  double d1;
  double d2;
  double d3;
  double d4;
  int ex_tmp;
  for (k = 0; k < 240; k++) {
    d = 0.70710678118654746 * inPower[k] - inSymbols[k].re;
    d1 = 0.70710678118654746 * inPower[k] - inSymbols[k].im;
    d2 = rt_powd_snf(rt_hypotd_snf(d, d1), 2.0);
    d3 = -0.70710678118654746 * inPower[k] - inSymbols[k].im;
    d = rt_powd_snf(rt_hypotd_snf(d, d3), 2.0);
    d4 = -0.70710678118654746 * inPower[k] - inSymbols[k].re;
    d1 = rt_powd_snf(rt_hypotd_snf(d4, d1), 2.0);
    d3 = rt_powd_snf(rt_hypotd_snf(d4, d3), 2.0);
    ex_tmp = k << 1;
    outBits[ex_tmp] = -(fmin(d2, d) - fmin(d1, d3));
    outBits[ex_tmp + 1] = -(fmin(d2, d1) - fmin(d, d3));
  }
}

/*
 * File trailer for QPSK_demodulate.c
 *
 * [EOF]
 */

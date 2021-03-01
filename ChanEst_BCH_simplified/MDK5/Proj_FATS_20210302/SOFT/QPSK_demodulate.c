#pragma once
/* Include Files */
#include "QPSK_demodulate.h"

void QPSK_demodulate(FLOAT_TYPE outBits[480],const struct_complex inSymbols[240], const FLOAT_TYPE inPower[240])
{
  int k;
  FLOAT_TYPE d;
  FLOAT_TYPE d1;
  FLOAT_TYPE d2;
  FLOAT_TYPE d3;
  FLOAT_TYPE d4;
  int ex_tmp;
  for (k = 0; k < 240; k++) {
    d = 0.70710678118654746 * inPower[k] - inSymbols[k].re;
    d1 = 0.70710678118654746 * inPower[k] - inSymbols[k].im;
    d2 = (pow(d, 2) + pow(d1, 2));
    d3 = -0.70710678118654746 * inPower[k] - inSymbols[k].im;
    d = (pow(d, 2) + pow(d3, 2));
    d4 = -0.70710678118654746 * inPower[k] - inSymbols[k].re;
    d1 = (pow(d4, 2) + pow(d1, 2));
    d3 = (pow(d4, 2) + pow(d3, 2));
    ex_tmp = k << 1;
    outBits[ex_tmp] = -(fmin(d2, d) - fmin(d1, d3));
    outBits[ex_tmp + 1] = -(fmin(d2, d1) - fmin(d, d3));
  }
}

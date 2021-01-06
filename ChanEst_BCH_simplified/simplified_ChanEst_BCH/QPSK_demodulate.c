/* Include Files */
#include "QPSK_demodulate.h"
#include <math.h>

void QPSK_demodulate(const struct_creal inSymbols[240], const double inPower[240],
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
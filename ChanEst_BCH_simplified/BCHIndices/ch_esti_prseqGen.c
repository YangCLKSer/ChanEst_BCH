/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: ch_esti_prseqGen.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 05-Jan-2021 17:59:07
 */

/* Include Files */
#include "ch_esti_prseqGen.h"
#include "BCHIndices.h"
#include "BCHIndices_rtwutil.h"
#include "ChannelEst.h"
#include "PBCH_Decode.h"
#include "ch_esti_dct.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */

/*
 * Pseudo-Random Sequence Generation method
 *  Abstract:
 *    Pseudo-random sequences are defined by a length-31 Gold sequence.
 *  Input:
 *    lenC, length of output seq C
 *    Cinit, intialization of pseudo-random generator
 *  Output:
 *    C, pseudo-random sequence
 *  Reference:
 *    [1] 3GPP TS 36.211 V8.4.0 (2008-09), 7.2, pp.75-76
 *  Author:
 *    Yang Yushan     2009-04-09
 *  Version History:
 *    Yang Yushan     2009-04-09      Revised
 *    Wang Qingwen    Created
 * ------------------------------------------------------------------------------%
 *  Init
 * Arguments    : double Cinit
 *                double C[440]
 * Return Type  : void
 */
void ch_esti_prseqGen(double Cinit, double C[440])
{
  signed char x1[2040];
  signed char x2[2040];
  double mid;
  int n;
  int counter;
  signed char out[31];

  /*  a const used for generator */
  /*  malloc memory for output C */
  /*  length of Gold seq current used */
  memset(&x1[0], 0, 2040U * sizeof(signed char));
  x1[0] = 1;

  /*  init of x1 */
  memset(&x2[0], 0, 2040U * sizeof(signed char));
  mid = Cinit;
  for (n = 0; n < 31; n++) {
    out[n] = 0;
  }

  for (counter = 0; counter < 31; counter++) {
    n = (int)rt_powd_snf(2.0, 31.0 - ((double)counter + 1.0));
    if (mid >= n) {
      out[30 - counter] = 1;
      mid -= (double)n;
    }
  }

  for (n = 0; n < 31; n++) {
    x2[n] = out[n];
  }

  /*  init of x2 */
  /*  Calculate Gold seq with 2 m-seq x1 and x2 */
  for (n = 0; n < 2009; n++) {
    counter = x1[n + 3] + x1[n];
    if (counter == 0) {
      x1[n + 31] = 0;
    } else {
      x1[n + 31] = (signed char)fmod(counter, 2.0);
    }

    counter = ((x2[n + 3] + x2[n + 2]) + x2[n + 1]) + x2[n];
    if (counter == 0) {
      x2[n + 31] = 0;
    } else {
      x2[n + 31] = (signed char)fmod(counter, 2.0);
    }
  }

  /*  Calculate output seq C */
  for (n = 0; n < 440; n++) {
    counter = x1[n + 1600] + x2[n + 1600];
    if (counter == 0) {
      C[n] = 0.0;
    } else {
      C[n] = fmod(counter, 2.0);
    }
  }

  /* return; */
}

/*
 * File trailer for ch_esti_prseqGen.c
 *
 * [EOF]
 */

/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: BCHIndices.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 26-Dec-2020 21:05:29
 */

/* Include Files */
#include "BCHIndices.h"
#include "BCHIndices_data.h"
#include "BCHIndices_initialize.h"
#include "ChannelEst.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */

/*
 * Arguments    : const struct0_T *enb
 *                double index_data[]
 *                int index_size[2]
 * Return Type  : void
 */
void BCHIndices(const struct0_T *enb, double index_data[], int index_size[2])
{
  double Bshift;
  int counter;
  int out_c;
  double RSindex[24];
  int b_counter;
  signed char out[72];
  int res;
  boolean_T exitg1;
  signed char BCHindex01_data[72];
  int trueCount;
  boolean_T y;
  int k;
  boolean_T exitg2;
  if (!isInitialized_BCHIndices) {
    BCHIndices_initialize();
  }

  if (rtIsNaN(enb->NCellID) || rtIsInf(enb->NCellID)) {
    Bshift = rtNaN;
  } else if (enb->NCellID == 0.0) {
    Bshift = 0.0;
  } else {
    Bshift = fmod(enb->NCellID, 3.0);
    if (Bshift == 0.0) {
      Bshift = 0.0;
    } else {
      if (enb->NCellID < 0.0) {
        Bshift += 3.0;
      }
    }
  }

  for (counter = 0; counter < 24; counter++) {
    RSindex[counter] = 3.0 * (double)counter + Bshift;
  }

  out_c = 0;
  for (b_counter = 0; b_counter < 72; b_counter++) {
    out[b_counter] = (signed char)b_counter;
  }

  for (counter = 0; counter < 72; counter++) {
    res = 0;
    b_counter = 1;
    exitg1 = false;
    while ((!exitg1) && (b_counter - 1 < 24)) {
      trueCount = 0;
      if (b_counter == counter) {
        trueCount = 1;
      }

      y = (trueCount != 0);
      if (y) {
        k = 0;
        exitg2 = false;
        while ((!exitg2) && (k <= trueCount - 1)) {
          if (RSindex[0] == 0.0) {
            y = false;
            exitg2 = true;
          } else {
            k = 1;
          }
        }
      }

      if (y) {
        res = 1;
        exitg1 = true;
      } else {
        b_counter++;
      }
    }

    if (res == 0) {
      out[out_c] = (signed char)counter;
      out_c++;
    }
  }

  if (1 > out_c) {
    counter = 0;
  } else {
    counter = out_c;
  }

  for (b_counter = 0; b_counter < counter; b_counter++) {
    BCHindex01_data[b_counter] = (signed char)(out[b_counter] + 1);
  }

  index_size[0] = 1;
  index_size[1] = (counter + counter) + 144;
  for (b_counter = 0; b_counter < counter; b_counter++) {
    index_data[b_counter] = (double)BCHindex01_data[b_counter] + 504.0;
  }

  for (b_counter = 0; b_counter < counter; b_counter++) {
    index_data[b_counter + counter] = (double)BCHindex01_data[b_counter] + 576.0;
  }

  for (b_counter = 0; b_counter < 72; b_counter++) {
    res = (b_counter + counter) + counter;
    index_data[res] = (double)b_counter + 649.0;
    index_data[res + 72] = (double)b_counter + 721.0;
  }
}

/*
 * File trailer for BCHIndices.c
 *
 * [EOF]
 */

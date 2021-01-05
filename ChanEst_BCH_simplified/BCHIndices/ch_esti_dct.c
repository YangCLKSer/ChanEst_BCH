/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: ch_esti_dct.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 05-Jan-2021 17:59:07
 */

/* Include Files */
#include "ch_esti_dct.h"
#include "BCHIndices.h"
#include "BCHIndices_emxutil.h"
#include "BCHIndices_rtwutil.h"
#include "ChannelEst.h"
#include "PBCH_Decode.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static double rt_atan2d_snf(double u0, double u1);
static double rt_roundd_snf(double u);

/* Function Definitions */

/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_atan2d_snf(double u0, double u1)
{
  double y;
  int b_u0;
  int b_u1;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    if (u0 > 0.0) {
      b_u0 = 1;
    } else {
      b_u0 = -1;
    }

    if (u1 > 0.0) {
      b_u1 = 1;
    } else {
      b_u1 = -1;
    }

    y = atan2(b_u0, b_u1);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

/*
 * Arguments    : double u
 * Return Type  : double
 */
static double rt_roundd_snf(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

/*
 * DCT frequency interpolation of channel frequency response on ofdms with RS
 *  Abstract:
 *    This function is dct frequency interpolation for ofdms with rs.
 *  Input:
 *    hEst, the estimation results after ls sampling.
 *    locOFDMWithRS, locations of ofdms with rs.
 *    locRS, locations of rs in ofdms with rs.
 *    Pc, the energy extract coefficient.
 *  Output:
 *    hEst, channel freq estimation output after dct interpolation.
 *  Reference:
 *    NULL.
 *  Author:
 *    Yang Yushan     2009-04-09
 *  Version History:
 *    Yang Yushan     2009-04-09      Created
 * ------------------------------------------------------------------------------%
 * Arguments    : emxArray_creal_T *hEst
 *                const emxArray_real_T *locOFDMWithRS
 *                const emxArray_real_T *locRS
 * Return Type  : void
 */
void ch_esti_dct(emxArray_creal_T *hEst, const emxArray_real_T *locOFDMWithRS,
                 const emxArray_real_T *locRS)
{
  int numSym;
  int numPc;
  int u1;
  int i;
  int i1;
  emxArray_creal_T *gEst;
  int loop_ub;
  emxArray_creal_T *aa;
  emxArray_creal_T *ww;
  int b_loop_ub;
  emxArray_creal_T *yy;
  emxArray_creal_T *y;
  int i2;
  emxArray_creal_T *z;
  unsigned int b_y;
  emxArray_real_T *a;
  double c_y;
  emxArray_creal_T *Mn;
  int i3;
  emxArray_real_T *d_y;
  int c_loop_ub;
  emxArray_creal_T *b_yy;
  int n;
  int i4;
  int i5;
  int m;
  int md2;
  int d_loop_ub;
  double b;
  int e_loop_ub;
  int aa_idx_1;
  int f_loop_ub;
  double r;
  int k;
  double W_im;
  double xtmp_im;
  int b_i;
  double W_re;
  unsigned int unnamed_idx_0;
  unsigned int unnamed_idx_1;
  double d;
  signed char i6;
  int i_sym;
  numSym = hEst->size[1];
  if ((locRS->size[0] == 0) || (locRS->size[1] == 0)) {
    u1 = 0;
  } else {
    numPc = locRS->size[0];
    u1 = locRS->size[1];
    if (numPc > u1) {
      u1 = numPc;
    }
  }

  /*  number of RS per ofdm */
  /*  estimation on ofdms with rs */
  /* ---- DCT Interpolation */
  numPc = (int)ceil((double)u1 * 0.1 / 2.0) << 1;
  i = locOFDMWithRS->size[0];
  if (0 <= locOFDMWithRS->size[0] - 1) {
    i1 = locRS->size[1];
    loop_ub = locRS->size[1];
    if (1 > locRS->size[1]) {
      b_loop_ub = 0;
    } else {
      b_loop_ub = locRS->size[1];
    }

    i2 = locRS->size[1];
    b_y = (unsigned int)locRS->size[1] << 1;
    c_y = sqrt(2.0 * (double)locRS->size[1]);
    i3 = locRS->size[1];
    c_loop_ub = 2 * locRS->size[1];
    if (locRS->size[1] + 1U > ((unsigned int)locRS->size[1] << 1)) {
      i4 = 0;
    } else {
      i4 = locRS->size[1];
    }

    m = b_loop_ub - 1;
    md2 = b_loop_ub >> 1;
    if (1 > locRS->size[1]) {
      d_loop_ub = -1;
    } else {
      d_loop_ub = locRS->size[1] - 1;
    }

    b = sqrt((double)numSym / (double)u1);
    if (1 > numPc) {
      e_loop_ub = 0;
    } else {
      e_loop_ub = numPc;
    }

    aa_idx_1 = numSym - numPc;
  }

  emxInit_creal_T(&gEst, 2);
  emxInit_creal_T(&aa, 1);
  emxInit_creal_T(&ww, 1);
  emxInit_creal_T(&yy, 2);
  emxInit_creal_T(&y, 1);
  emxInit_creal_T(&z, 2);
  emxInit_real_T(&a, 2);
  emxInit_creal_T(&Mn, 2);
  emxInit_real_T(&d_y, 2);
  emxInit_creal_T(&b_yy, 2);
  for (n = 0; n < i; n++) {
    i5 = gEst->size[0] * gEst->size[1];
    gEst->size[0] = 1;
    gEst->size[1] = i1;
    emxEnsureCapacity_creal_T(gEst, i5);
    for (i5 = 0; i5 < loop_ub; i5++) {
      gEst->data[i5] = hEst->data[((int)locOFDMWithRS->data[n] + hEst->size[0] *
        ((int)locRS->data[n + locRS->size[0] * i5] - 1)) - 1];
    }

    /*  If input is a vector, make it a column: */
    /*  Cast to enforce precision rules */
    /*  Pad or truncate input if necessary */
    i5 = aa->size[0];
    aa->size[0] = b_loop_ub;
    emxEnsureCapacity_creal_T(aa, i5);
    for (i5 = 0; i5 < b_loop_ub; i5++) {
      aa->data[i5] = gEst->data[i5];
    }

    /*  Compute weights to multiply DFT coefficients (cast enforces the precision */
    /*  rules) */
    if (i2 - 1 < 0) {
      a->size[0] = 1;
      a->size[1] = 0;
    } else {
      i5 = a->size[0] * a->size[1];
      a->size[0] = 1;
      a->size[1] = locRS->size[1];
      emxEnsureCapacity_real_T(a, i5);
      f_loop_ub = locRS->size[1] - 1;
      for (i5 = 0; i5 <= f_loop_ub; i5++) {
        a->data[i5] = i5;
      }
    }

    i5 = z->size[0] * z->size[1];
    z->size[0] = 1;
    z->size[1] = a->size[1];
    emxEnsureCapacity_creal_T(z, i5);
    f_loop_ub = a->size[0] * a->size[1];
    for (i5 = 0; i5 < f_loop_ub; i5++) {
      r = a->data[i5];
      W_im = 0.0 * r * 3.1415926535897931;
      xtmp_im = -r * 3.1415926535897931;
      if (xtmp_im == 0.0) {
        z->data[i5].re = W_im / (double)b_y;
        z->data[i5].im = 0.0;
      } else if (W_im == 0.0) {
        z->data[i5].re = 0.0;
        z->data[i5].im = xtmp_im / (double)b_y;
      } else {
        z->data[i5].re = rtNaN;
        z->data[i5].im = xtmp_im / (double)b_y;
      }
    }

    numPc = z->size[1];
    for (k = 0; k < numPc; k++) {
      if (z->data[k].im == 0.0) {
        z->data[k].re = exp(z->data[k].re);
        z->data[k].im = 0.0;
      } else if (rtIsInf(z->data[k].im) && rtIsInf(z->data[k].re) && (z->data[k]
                  .re < 0.0)) {
        z->data[k].re = 0.0;
        z->data[k].im = 0.0;
      } else {
        r = exp(z->data[k].re / 2.0);
        z->data[k].re = r * (r * cos(z->data[k].im));
        z->data[k].im = r * (r * sin(z->data[k].im));
      }
    }

    i5 = ww->size[0];
    ww->size[0] = z->size[1];
    emxEnsureCapacity_creal_T(ww, i5);
    f_loop_ub = z->size[1];
    for (i5 = 0; i5 < f_loop_ub; i5++) {
      if (z->data[i5].im == 0.0) {
        ww->data[i5].re = z->data[i5].re / c_y;
        ww->data[i5].im = 0.0;
      } else if (z->data[i5].re == 0.0) {
        ww->data[i5].re = 0.0;
        ww->data[i5].im = z->data[i5].im / c_y;
      } else {
        ww->data[i5].re = z->data[i5].re / c_y;
        ww->data[i5].im = z->data[i5].im / c_y;
      }
    }

    if (ww->data[0].im == 0.0) {
      r = ww->data[0].re / 1.4142135623730951;
      xtmp_im = 0.0;
    } else if (ww->data[0].re == 0.0) {
      r = 0.0;
      xtmp_im = ww->data[0].im / 1.4142135623730951;
    } else {
      r = ww->data[0].re / 1.4142135623730951;
      xtmp_im = ww->data[0].im / 1.4142135623730951;
    }

    ww->data[0].re = r;
    ww->data[0].im = xtmp_im;

    /*  odd case */
    /*  Form intermediate even-symmetric matrix */
    i5 = y->size[0];
    y->size[0] = 2 * i3;
    emxEnsureCapacity_creal_T(y, i5);
    for (i5 = 0; i5 < c_loop_ub; i5++) {
      y->data[i5].re = 0.0;
      y->data[i5].im = 0.0;
    }

    for (i5 = 0; i5 < b_loop_ub; i5++) {
      y->data[i5] = gEst->data[i5];
    }

    for (b_i = 0; b_i < md2; b_i++) {
      r = aa->data[b_i].re;
      xtmp_im = aa->data[b_i].im;
      numPc = m - b_i;
      aa->data[b_i] = aa->data[numPc];
      aa->data[numPc].re = r;
      aa->data[numPc].im = xtmp_im;
    }

    f_loop_ub = aa->size[0];
    for (i5 = 0; i5 < f_loop_ub; i5++) {
      y->data[i4 + i5] = aa->data[i5];
    }

    /*  Compute the FFT and keep the appropriate portion: */
    if (y->size[0] - 1 < 0) {
      a->size[0] = 1;
      a->size[1] = 0;
    } else {
      i5 = a->size[0] * a->size[1];
      a->size[0] = 1;
      a->size[1] = y->size[0];
      emxEnsureCapacity_real_T(a, i5);
      f_loop_ub = y->size[0] - 1;
      for (i5 = 0; i5 <= f_loop_ub; i5++) {
        a->data[i5] = i5;
      }
    }

    W_im = -6.2831853071795862 / (double)y->size[0];
    if (W_im == 0.0) {
      W_re = 1.0;
      W_im = 0.0;
    } else {
      W_re = cos(W_im);
      W_im = sin(W_im);
    }

    i5 = d_y->size[0] * d_y->size[1];
    d_y->size[0] = a->size[1];
    d_y->size[1] = a->size[1];
    emxEnsureCapacity_real_T(d_y, i5);
    f_loop_ub = a->size[1];
    for (i5 = 0; i5 < f_loop_ub; i5++) {
      numPc = a->size[1];
      for (k = 0; k < numPc; k++) {
        d_y->data[k + d_y->size[0] * i5] = a->data[k] * a->data[i5];
      }
    }

    unnamed_idx_0 = (unsigned int)d_y->size[0];
    unnamed_idx_1 = (unsigned int)d_y->size[1];
    i5 = Mn->size[0] * Mn->size[1];
    Mn->size[0] = (int)unnamed_idx_0;
    Mn->size[1] = (int)unnamed_idx_1;
    emxEnsureCapacity_creal_T(Mn, i5);
    numPc = (int)unnamed_idx_0 * (int)unnamed_idx_1;
    for (k = 0; k < numPc; k++) {
      if ((W_im == 0.0) && (W_re >= 0.0)) {
        Mn->data[k].re = rt_powd_snf(W_re, d_y->data[k]);
        Mn->data[k].im = 0.0;
      } else if ((W_re == 0.0) && (floor(d_y->data[k]) == d_y->data[k])) {
        xtmp_im = rt_powd_snf(W_im, d_y->data[k]);
        if (rtIsNaN(d_y->data[k]) || rtIsInf(d_y->data[k])) {
          r = rtNaN;
        } else if (d_y->data[k] == 0.0) {
          r = 0.0;
        } else {
          r = fmod(d_y->data[k], 4.0);
          if (r == 0.0) {
            r = 0.0;
          } else {
            if (d_y->data[k] < 0.0) {
              r += 4.0;
            }
          }
        }

        d = rt_roundd_snf(r);
        if (d < 128.0) {
          i6 = (signed char)d;
        } else {
          i6 = 0;
        }

        if (i6 == 3) {
          Mn->data[k].re = 0.0;
          Mn->data[k].im = -xtmp_im;
        } else if (i6 == 2) {
          Mn->data[k].re = -xtmp_im;
          Mn->data[k].im = 0.0;
        } else if (i6 == 1) {
          Mn->data[k].re = 0.0;
          Mn->data[k].im = xtmp_im;
        } else {
          Mn->data[k].re = xtmp_im;
          Mn->data[k].im = 0.0;
        }
      } else if ((W_im == 0.0) && rtIsInf(d_y->data[k]) && (fabs(W_re) == 1.0))
      {
        Mn->data[k].re = 1.0;
        Mn->data[k].im = 0.0;
      } else {
        if (W_im == 0.0) {
          if (W_re < 0.0) {
            r = log(fabs(W_re));
            xtmp_im = 3.1415926535897931;
          } else {
            r = log(W_re);
            xtmp_im = 0.0;
          }
        } else {
          r = log(rt_hypotd_snf(W_re, W_im));
          xtmp_im = rt_atan2d_snf(W_im, W_re);
        }

        r *= d_y->data[k];
        xtmp_im *= d_y->data[k];
        if (xtmp_im == 0.0) {
          Mn->data[k].re = exp(r);
          Mn->data[k].im = 0.0;
        } else if (rtIsInf(xtmp_im) && rtIsInf(r) && (r < 0.0)) {
          Mn->data[k].re = 0.0;
          Mn->data[k].im = 0.0;
        } else {
          r = exp(r / 2.0);
          Mn->data[k].re = r * (r * cos(xtmp_im));
          Mn->data[k].im = r * (r * sin(xtmp_im));
        }
      }
    }

    numPc = Mn->size[0];
    f_loop_ub = Mn->size[1];
    i5 = aa->size[0];
    aa->size[0] = Mn->size[0];
    emxEnsureCapacity_creal_T(aa, i5);
    for (b_i = 0; b_i < numPc; b_i++) {
      r = 0.0;
      xtmp_im = 0.0;
      for (k = 0; k < f_loop_ub; k++) {
        r += Mn->data[k * Mn->size[0] + b_i].re * y->data[k].re - Mn->data[k *
          Mn->size[0] + b_i].im * y->data[k].im;
        xtmp_im += Mn->data[k * Mn->size[0] + b_i].re * y->data[k].im + Mn->
          data[k * Mn->size[0] + b_i].im * y->data[k].re;
      }

      aa->data[b_i].re = r;
      aa->data[b_i].im = xtmp_im;
    }

    if (y->size[0] == 1) {
      i5 = yy->size[0] * yy->size[1];
      yy->size[0] = 1;
      yy->size[1] = aa->size[0];
      emxEnsureCapacity_creal_T(yy, i5);
      f_loop_ub = aa->size[0];
      for (i5 = 0; i5 < f_loop_ub; i5++) {
        yy->data[yy->size[0] * i5] = aa->data[i5];
      }
    } else {
      i5 = yy->size[0] * yy->size[1];
      yy->size[0] = aa->size[0];
      yy->size[1] = 1;
      emxEnsureCapacity_creal_T(yy, i5);
      f_loop_ub = aa->size[0];
      for (i5 = 0; i5 < f_loop_ub; i5++) {
        yy->data[i5] = aa->data[i5];
      }
    }

    f_loop_ub = yy->size[1] - 1;
    i5 = b_yy->size[0] * b_yy->size[1];
    b_yy->size[0] = d_loop_ub + 1;
    b_yy->size[1] = yy->size[1];
    emxEnsureCapacity_creal_T(b_yy, i5);
    for (i5 = 0; i5 <= f_loop_ub; i5++) {
      for (k = 0; k <= d_loop_ub; k++) {
        b_yy->data[k + b_yy->size[0] * i5] = yy->data[k + yy->size[0] * i5];
      }
    }

    i5 = yy->size[0] * yy->size[1];
    yy->size[0] = b_yy->size[0];
    yy->size[1] = b_yy->size[1];
    emxEnsureCapacity_creal_T(yy, i5);
    f_loop_ub = b_yy->size[0] * b_yy->size[1];
    for (i5 = 0; i5 < f_loop_ub; i5++) {
      yy->data[i5] = b_yy->data[i5];
    }

    /*  Multiply FFT by weights: */
    f_loop_ub = ww->size[0];
    i5 = gEst->size[0] * gEst->size[1];
    gEst->size[0] = 1;
    gEst->size[1] = ww->size[0];
    emxEnsureCapacity_creal_T(gEst, i5);
    for (i5 = 0; i5 < f_loop_ub; i5++) {
      gEst->data[i5].re = b * (ww->data[i5].re * yy->data[i5].re - ww->data[i5].
        im * yy->data[i5].im);
      gEst->data[i5].im = b * (ww->data[i5].re * yy->data[i5].im + ww->data[i5].
        im * yy->data[i5].re);
    }

    /* plot(abs(gEst)); */
    i5 = z->size[0] * z->size[1];
    z->size[0] = 1;
    z->size[1] = e_loop_ub + aa_idx_1;
    emxEnsureCapacity_creal_T(z, i5);
    for (i5 = 0; i5 < e_loop_ub; i5++) {
      z->data[i5] = gEst->data[i5];
    }

    for (i5 = 0; i5 < aa_idx_1; i5++) {
      k = i5 + e_loop_ub;
      z->data[k].re = 0.0;
      z->data[k].im = 0.0;
    }

    i5 = gEst->size[0] * gEst->size[1];
    gEst->size[0] = 1;
    gEst->size[1] = z->size[1];
    emxEnsureCapacity_creal_T(gEst, i5);
    f_loop_ub = z->size[0] * z->size[1];
    for (i5 = 0; i5 < f_loop_ub; i5++) {
      gEst->data[i5] = z->data[i5];
    }

    /*      maxgEst = max( abs(gEst) ); */
    /*      gEst = gEst( find( abs(gEst) > 0.05*maxgEst ) );     */
    /*  Extended Inverse Discrete Cosine Transform. */
    /*  Abstract:  */
    /*    This function is eidct, for dct interpolation. */
    /*  Input: */
    /*    y, input dct. */
    /*    m, the useful data used for interpolation. */
    /*  Output:    */
    /*    x, the interpolated results after idct. */
    /*  Reference:  */
    /*    NULL. */
    /*  Author:    */
    /*    Yang Yushan     2009-04-09 */
    /*  Version History: */
    /*    Yang Yushan     2009-04-09      Created */
    /* ------------------------------------------------------------------------------% */
    numPc = gEst->size[1];
    i5 = z->size[0] * z->size[1];
    z->size[0] = 1;
    z->size[1] = gEst->size[1];
    emxEnsureCapacity_creal_T(z, i5);
    f_loop_ub = gEst->size[1];
    for (i5 = 0; i5 < f_loop_ub; i5++) {
      z->data[i5].re = 0.0;
      z->data[i5].im = 0.0;
    }

    r = sqrt(2.0 / (double)gEst->size[1]);
    i5 = a->size[0] * a->size[1];
    a->size[0] = 1;
    a->size[1] = gEst->size[1];
    emxEnsureCapacity_real_T(a, i5);
    f_loop_ub = gEst->size[1];
    for (i5 = 0; i5 < f_loop_ub; i5++) {
      a->data[i5] = r;
    }

    a->data[0] = 1.0 / sqrt(gEst->size[1]);
    i5 = gEst->size[1];
    for (f_loop_ub = 0; f_loop_ub < i5; f_loop_ub++) {
      for (k = 0; k < u1; k++) {
        d = cos(3.1415926535897931 * ((2.0 * ((double)f_loop_ub + 1.0) + (double)
                  numPc / (double)u1) - 2.0) * (((double)k + 1.0) - 1.0) / (2.0 *
                 (double)numPc));
        z->data[f_loop_ub].re += a->data[k] * gEst->data[k].re * d;
        z->data[f_loop_ub].im += a->data[k] * gEst->data[k].im * d;
      }
    }

    f_loop_ub = z->size[1];
    for (i5 = 0; i5 < f_loop_ub; i5++) {
      hEst->data[((int)locOFDMWithRS->data[n] + hEst->size[0] * i5) - 1] =
        z->data[i5];
    }

    if (1.0 != locRS->data[n]) {
      d = (locRS->data[n + locRS->size[0] * (locRS->size[1] - 1)] - locRS->
           data[n]) + 1.0;
      if (1.0 > d) {
        f_loop_ub = 0;
      } else {
        f_loop_ub = (int)d;
      }

      d = locRS->data[n];
      if (d > locRS->data[n + locRS->size[0] * (locRS->size[1] - 1)]) {
        i5 = 1;
      } else {
        i5 = (int)d;
      }

      d = locOFDMWithRS->data[n];
      k = z->size[0] * z->size[1];
      z->size[0] = 1;
      z->size[1] = f_loop_ub;
      emxEnsureCapacity_creal_T(z, k);
      for (k = 0; k < f_loop_ub; k++) {
        z->data[k] = hEst->data[((int)d + hEst->size[0] * k) - 1];
      }

      f_loop_ub = z->size[1];
      for (k = 0; k < f_loop_ub; k++) {
        hEst->data[((int)d + hEst->size[0] * ((i5 + k) - 1)) - 1] = z->data[k];
      }

      i5 = (int)(locRS->data[n] - 1.0);
      for (i_sym = 0; i_sym < i5; i_sym++) {
        d = locRS->data[n];
        b_i = (int)locOFDMWithRS->data[n] - 1;
        numPc = (int)d - 1;
        f_loop_ub = (int)locRS->data[n + locRS->size[0]] - 1;
        r = d - ((double)i_sym + 1.0);
        W_im = (hEst->data[b_i + hEst->size[0] * numPc].re - hEst->data[b_i +
                hEst->size[0] * f_loop_ub].re) * r;
        xtmp_im = (hEst->data[b_i + hEst->size[0] * numPc].im - hEst->data[b_i +
                   hEst->size[0] * f_loop_ub].im) * r;
        if (xtmp_im == 0.0) {
          r = W_im / 6.0;
          xtmp_im = 0.0;
        } else if (W_im == 0.0) {
          r = 0.0;
          xtmp_im /= 6.0;
        } else {
          r = W_im / 6.0;
          xtmp_im /= 6.0;
        }

        hEst->data[b_i + hEst->size[0] * i_sym].re = hEst->data[b_i + hEst->
          size[0] * numPc].re + r;
        hEst->data[b_i + hEst->size[0] * i_sym].im = hEst->data[b_i + hEst->
          size[0] * numPc].im + xtmp_im;
      }
    }

    if (numSym != locRS->data[n + locRS->size[0] * (locRS->size[1] - 1)]) {
      d = locRS->data[n + locRS->size[0] * (locRS->size[1] - 1)] + 1.0;
      i5 = (int)((double)numSym + (1.0 - d));
      for (i_sym = 0; i_sym < i5; i_sym++) {
        W_re = d + (double)i_sym;
        b_i = (int)locOFDMWithRS->data[n] - 1;
        W_im = (hEst->data[b_i + hEst->size[0] * ((int)locRS->data[n +
                 locRS->size[0] * (locRS->size[1] - 1)] - 1)].re - hEst->
                data[b_i + hEst->size[0] * ((int)locRS->data[n + locRS->size[0] *
                 (locRS->size[1] - 2)] - 1)].re) * (W_re - locRS->data[n +
          locRS->size[0] * (locRS->size[1] - 1)]);
        xtmp_im = (hEst->data[b_i + hEst->size[0] * ((int)locRS->data[n +
                    locRS->size[0] * (locRS->size[1] - 1)] - 1)].im - hEst->
                   data[b_i + hEst->size[0] * ((int)locRS->data[n + locRS->size
                    [0] * (locRS->size[1] - 2)] - 1)].im) * (W_re - locRS->
          data[n + locRS->size[0] * (locRS->size[1] - 1)]);
        if (xtmp_im == 0.0) {
          r = W_im / 6.0;
          xtmp_im = 0.0;
        } else if (W_im == 0.0) {
          r = 0.0;
          xtmp_im /= 6.0;
        } else {
          r = W_im / 6.0;
          xtmp_im /= 6.0;
        }

        k = (int)W_re - 1;
        hEst->data[b_i + hEst->size[0] * k].re = hEst->data[b_i + hEst->size[0] *
          ((int)locRS->data[n + locRS->size[0] * (locRS->size[1] - 1)] - 1)].re
          + r;
        hEst->data[b_i + hEst->size[0] * k].im = hEst->data[b_i + hEst->size[0] *
          ((int)locRS->data[n + locRS->size[0] * (locRS->size[1] - 1)] - 1)].im
          + xtmp_im;
      }
    }
  }

  emxFree_creal_T(&b_yy);
  emxFree_real_T(&d_y);
  emxFree_creal_T(&Mn);
  emxFree_real_T(&a);
  emxFree_creal_T(&z);
  emxFree_creal_T(&y);
  emxFree_creal_T(&yy);
  emxFree_creal_T(&ww);
  emxFree_creal_T(&aa);
  emxFree_creal_T(&gEst);

  /*  plot([1:numSym], real(hEst(locOFDMWithRS(1),:)),'-r'); */
}

/*
 * File trailer for ch_esti_dct.c
 *
 * [EOF]
 */

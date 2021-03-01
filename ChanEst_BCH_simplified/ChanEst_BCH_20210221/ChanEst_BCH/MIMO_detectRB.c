#pragma once
/* Include Files */
#include "MIMO_detectRB.h"

/* Function Definitions */
static void b_mld2sfbc(const double Received[120], const double mimoCH[240],
  double Detected[120], double ampd[120])
{
  int idxStrm;
  int i;
  int unnamed_idx_0_tmp;
  int b_unnamed_idx_0_tmp;
  int Detected_tmp;
  int i1;
  double d;
  double d1;
  double scale;
  double b_scale;
  double absxk;
  double t;
  double y;
  double b_y;

  /*  */
  memset(&Detected[0], 0, 120U * sizeof(double));
  for (idxStrm = 0; idxStrm < 60; idxStrm++) {
    unnamed_idx_0_tmp = (idxStrm + 1) << 1;
    b_unnamed_idx_0_tmp = unnamed_idx_0_tmp - 2;
    unnamed_idx_0_tmp--;
    Detected_tmp = idxStrm << 1;
    i = (((idxStrm + 1) << 1) - 2) << 1;
    i1 = (((idxStrm + 1) << 1) - 1) << 1;
    d = mimoCH[i1 + 1];
    Detected[Detected_tmp] = mimoCH[i] * Received[b_unnamed_idx_0_tmp] + d *
      Received[unnamed_idx_0_tmp];
    d1 = mimoCH[i + 1];
    Detected[Detected_tmp + 1] = -d1 * Received[b_unnamed_idx_0_tmp] + mimoCH[i1]
      * Received[unnamed_idx_0_tmp];
    scale = 3.3121686421112381E-170;
    b_scale = 3.3121686421112381E-170;
    absxk = fabs(mimoCH[i]);
    if (absxk > 3.3121686421112381E-170) {
      y = 1.0;
      scale = absxk;
    } else {
      t = absxk / 3.3121686421112381E-170;
      y = t * t;
    }

    absxk = fabs(mimoCH[i1]);
    if (absxk > 3.3121686421112381E-170) {
      b_y = 1.0;
      b_scale = absxk;
    } else {
      t = absxk / 3.3121686421112381E-170;
      b_y = t * t;
    }

    absxk = fabs(d1);
    if (absxk > scale) {
      t = scale / absxk;
      y = y * t * t + 1.0;
      scale = absxk;
    } else {
      t = absxk / scale;
      y += t * t;
    }

    absxk = fabs(d);
    if (absxk > b_scale) {
      t = b_scale / absxk;
      b_y = b_y * t * t + 1.0;
      b_scale = absxk;
    } else {
      t = absxk / b_scale;
      b_y += t * t;
    }

    ampd[b_unnamed_idx_0_tmp] = scale * sqrt(y);
    ampd[unnamed_idx_0_tmp] = b_scale * sqrt(b_y);
  }

  for (i = 0; i < 120; i++) {
    Detected[i] /= ampd[i];
    ampd[i] *= 0.70710678118654757;
  }
}

static void mld2sfbc(const struct_complex Received[240], const struct_complex mimoCH[960],
                     double Detected[240], double ampd[240])
{
  int idxStrm;
  int i;
  int i1;
  unsigned char uv[2];
  int Detected_tmp;
  int tmpCH_tmp;
  struct_complex tmpCH[8];
  int b_tmpCH_tmp;

  memset(&Detected[0], 0, 240U * sizeof(double));
  for (idxStrm = 0; idxStrm < 120; idxStrm++) {
    i = (idxStrm + 1) << 1;
    i1 = i - 2;
    i--;
    uv[0] = (unsigned char)i1;
    uv[1] = (unsigned char)i;
    for (Detected_tmp = 0; Detected_tmp < 2; Detected_tmp++) {
      tmpCH_tmp = uv[Detected_tmp] << 2;
      b_tmpCH_tmp = Detected_tmp << 2;
      tmpCH[b_tmpCH_tmp] = mimoCH[tmpCH_tmp];
      tmpCH[b_tmpCH_tmp + 1] = mimoCH[tmpCH_tmp + 1];
      tmpCH[b_tmpCH_tmp + 2] = mimoCH[tmpCH_tmp + 2];
      tmpCH[b_tmpCH_tmp + 3] = mimoCH[tmpCH_tmp + 3];
    }

    Detected_tmp = idxStrm << 1;
    Detected[Detected_tmp] = (tmpCH[0].re * Received[i1].re - -tmpCH[0].im *
      Received[i1].im) + (tmpCH[5].re * Received[i].re - tmpCH[5].im *
                          -Received[i].im);
    Detected[Detected_tmp + 1] = (-tmpCH[1].re * Received[i1].re - -tmpCH[1].im *
      -Received[i1].im) + (tmpCH[4].re * Received[i].re - -tmpCH[4].im *
      Received[i].im);
    ampd[i1] = complexNorm(tmpCH[0]);
    ampd[i] = complexNorm(tmpCH[4]);
  }

  for (i = 0; i < 240; i++) {
    Detected[i] /= ampd[i];
    ampd[i] *= 0.70710678118654757;
  }
}

static void mld4sfbcfstd(const struct_complex Received[240], const struct_complex mimoCH[960],
  double Detected[240], double ampd[240])
{
  int i;
  double tmpRec[120];
  double temp_mimoCH[240];
  double tmpDetd1[120];
  double ampd1[120];
  int tmpRec_tmp;
  int b_tmpRec_tmp;
  double tmpDetd2[120];
  double ampd2[120];
  int c_tmpRec_tmp;
  int temp_mimoCH_tmp;
  int b_temp_mimoCH_tmp;
  int c_temp_mimoCH_tmp;

  /*  */
  for (i = 0; i < 60; i++) {
    tmpRec_tmp = i << 2;
    b_tmpRec_tmp = i << 1;
    tmpRec[b_tmpRec_tmp] = Received[tmpRec_tmp].re;
    c_tmpRec_tmp = tmpRec_tmp + 1;
    b_tmpRec_tmp++;
    tmpRec[b_tmpRec_tmp] = Received[c_tmpRec_tmp].re;
    b_temp_mimoCH_tmp = i << 4;
    temp_mimoCH[tmpRec_tmp] = mimoCH[b_temp_mimoCH_tmp].re;
    temp_mimoCH_tmp = c_tmpRec_tmp << 2;
    c_temp_mimoCH_tmp = b_tmpRec_tmp << 1;
    temp_mimoCH[c_temp_mimoCH_tmp] = mimoCH[temp_mimoCH_tmp].re;
    temp_mimoCH[c_tmpRec_tmp] = mimoCH[b_temp_mimoCH_tmp + 2].re;
    temp_mimoCH[c_temp_mimoCH_tmp + 1] = mimoCH[temp_mimoCH_tmp + 2].re;
  }

  b_mld2sfbc(tmpRec, temp_mimoCH, tmpDetd1, ampd1);
  for (i = 0; i < 60; i++) {
    tmpRec_tmp = i << 2;
    b_tmpRec_tmp = i << 1;
    c_tmpRec_tmp = tmpRec_tmp + 2;
    tmpRec[b_tmpRec_tmp] = Received[c_tmpRec_tmp].re;
    temp_mimoCH_tmp = tmpRec_tmp + 3;
    b_tmpRec_tmp++;
    tmpRec[b_tmpRec_tmp] = Received[temp_mimoCH_tmp].re;
    b_temp_mimoCH_tmp = c_tmpRec_tmp << 2;
    temp_mimoCH[tmpRec_tmp] = mimoCH[b_temp_mimoCH_tmp + 1].re;
    temp_mimoCH_tmp <<= 2;
    c_temp_mimoCH_tmp = b_tmpRec_tmp << 1;
    temp_mimoCH[c_temp_mimoCH_tmp] = mimoCH[temp_mimoCH_tmp + 1].re;
    temp_mimoCH[tmpRec_tmp + 1] = mimoCH[b_temp_mimoCH_tmp + 3].re;
    temp_mimoCH[c_temp_mimoCH_tmp + 1] = mimoCH[temp_mimoCH_tmp + 3].re;
  }

  b_mld2sfbc(tmpRec, temp_mimoCH, tmpDetd2, ampd2);
  for (i = 0; i < 60; i++) {
    temp_mimoCH_tmp = i << 1;
    b_tmpRec_tmp = i << 2;
    Detected[b_tmpRec_tmp] = tmpDetd1[temp_mimoCH_tmp];
    c_temp_mimoCH_tmp = temp_mimoCH_tmp + 1;
    Detected[b_tmpRec_tmp + 1] = tmpDetd1[c_temp_mimoCH_tmp];
    Detected[b_tmpRec_tmp + 2] = tmpDetd2[temp_mimoCH_tmp];
    Detected[b_tmpRec_tmp + 3] = tmpDetd2[c_temp_mimoCH_tmp];
    ampd[b_tmpRec_tmp] = ampd1[temp_mimoCH_tmp];
    c_temp_mimoCH_tmp = temp_mimoCH_tmp + 1;
    ampd[b_tmpRec_tmp + 1] = ampd1[c_temp_mimoCH_tmp];
    ampd[b_tmpRec_tmp + 2] = ampd2[temp_mimoCH_tmp];
    ampd[b_tmpRec_tmp + 3] = ampd2[c_temp_mimoCH_tmp];
  }
}

void mrc(struct_complex Detected_RB[240],double ampd[240], const struct_complex RxData[240], const struct_complex equCH[960])
{
    double im,re;
    int i_strm;
    for (i_strm = 0; i_strm < 240; i_strm++) {
        im = complexNorm(equCH[i_strm]);
        ampd[i_strm] = im;
        re = equCH[i_strm].re / im;
        im = -equCH[i_strm].im / im;

        Detected_RB[i_strm].re = re * RxData[i_strm].re - im * RxData[i_strm].im;
        Detected_RB[i_strm].im = re * RxData[i_strm].im + im * RxData[i_strm].re;
    }
}

void MIMO_detectRB(struct_complex Detected_RB[240], double ampd[240], const struct_complex RxData[240], 
    const struct_complex equCH[960], int methodDetect)
{
    double b_Detected_RB[240];
    int i_strm;
    int re_tmp;
    double im;
    double re;

    /* %%%%%%%%%%%%%%%%%%%%%%% DETECTION MODE DEFINATION %%%%%%%%%%%%%%%%%%%%% */
    /*  Detection Method    Interference      Oredering    Interference   Mode */
    /*                      Nulling                        Cancellation */
    /*  MRC(or MF)          N/A               N/A               N/A       0 */
    /*  MLD for SFBC        N/A               N/A               N/A       1 */
    /*  MLD for SFBC-FSTD   N/A               N/A               N/A       2 */
    /*  ZF                  ZF                N/A               ZF        3 */
    /*  MMSE                MMSE              N/A               ZF        4 */
    /*  ZF-OSIC             ZF                optimal           ZF        5 */
    /*  MMSE-OSIC           MMSE              optimal           ZF        6 */
    /* %%%%%%%%%%%%%%%%%%%%%%%%%%%% DETECTION MODE END %%%%%%%%%%%%%%%%%%%%%%% */
    /*  */
    switch (methodDetect) {
    case 0:
        mrc(Detected_RB, ampd, RxData, equCH);

    break;

    case 1:
        mld2sfbc(RxData, equCH, b_Detected_RB, ampd);
        for (i_strm = 0; i_strm < 240; i_strm++) {
            Detected_RB[i_strm].re = b_Detected_RB[i_strm];
            Detected_RB[i_strm].im = 0.0;
        }
    break;

    case 2:
        mld4sfbcfstd(RxData, equCH, b_Detected_RB, ampd);
        for (i_strm = 0; i_strm < 240; i_strm++) {
            Detected_RB[i_strm].re = b_Detected_RB[i_strm];
            Detected_RB[i_strm].im = 0.0;
        }
    break;

    default:
        mrc(Detected_RB, ampd, RxData, equCH);
    break;
    }
}

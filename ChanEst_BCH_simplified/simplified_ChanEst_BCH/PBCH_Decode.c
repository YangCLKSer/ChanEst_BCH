/* Include Files */
#include "PBCH_Decode.h"
#include <math.h>
#include <string.h>

/* Function Declarations */

//
void PBCH_Decode(double bits[480], struct_complex symbols[240], int* nfmod4, int trblk[24],
    int* cellrefp, struct_complex sym[240], struct_complex chanEst[960], struct_ENB* enb)
{
    int NID;
    int crp[3] = { 1,2,4 };
    int i, sucess, c;
    double chanAmp[240];
    double demod[480];
    NID = enb->NCellID;

    *nfmod4 = 0;
    sucess = 1;
    *cellrefp = 0;

    for (i = 0; i < 3; i++)
    {
        c = crp[i];
        *cellrefp = c;
        MIMO_detectRB(symbols, chanAmp, sym, chanEst, c / 2, 4);
        QPSK_demodulate(demod, symbols, chanAmp);
        bch_decode(bits, nfmod4, trblk, &sucess, demod, NID, c);
        if (sucess)
            break;
    }
    if (sucess == 0)
        *cellrefp = 0;
}

void bch_decode(double sbits[480], int* nfmod4,
    int trblk[24], int* sucess, double demod[480], int NID, int NumTxAnt)
{
    int period, i, step;
    int scramC[1920] = { 0 };
    double b_trblk[24];
    
    Sync_PRsqn_genr(scramC, NID);//³¤¶È1920,+-1
    period = 1920 / 4;

    for (i = 0; i < 480; i++)
    {
        sbits[i] = 0.0;
    }
    for (i = 0; i < 24; i++)
    {
        trblk[i] = 0;
    }

    *sucess = 1;

    for (*nfmod4 = 3; *nfmod4 > 0; (*nfmod4)--)
    {
        step = *nfmod4 * period;
        for (i = 0; i < 480; i++)
        {
            sbits[i] = demod[i] * (double)scramC[i + step];
        }
        //[trblk, sucess] = cc_decode(sbits(1:period), NumTxAnt, period);
        *sucess = cc_decode(b_trblk, sbits, 24 , (double)period, 0, (double)NumTxAnt, 0, 0, 0);
        for (i = 0; i < 24; i++)
        {
            trblk[i] = (int)b_trblk[i];
        }
        if (*sucess)
            break;
    }
}

void Sync_PRsqn_genr(int PRsqnC[1920], int Cinit)
{
    int Nc = 1600, PRlength = 1920;
    int i, loop_ub;
    int x1[1600 + 1920] = { 1 };
    int x2[1600 + 1920] = { 0 };

    for (i = 0; i < 31; i++)
    {
        x2[i] = Cinit % 2;
        Cinit = Cinit / 2;
    }
    
    loop_ub = PRlength + Nc - 31;
    for (i = 0; i < loop_ub; i++)
    {
        x2[i + 31] = (x2[i + 3] + x2[i + 2] + x2[i + 1] + x2[i]) % 2;
        x1[i + 31] = (x1[i + 3] + x1[i]) % 2;
    }

    for (i = 0; i < PRlength; i++)
    {
        PRsqnC[i] = 1 - 2 * ((x1[i + Nc] + x2[i + Nc]) % 2);
    }
}

/*void bch_decode(const double demod[480], double NID, struct_complex sbits[480],
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

  * PRSEQ_GENR Summary of this function goes here *
  *    Detailed explanation goes here *
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
    if (Cinit == 0.0) {
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
    if (Cinit == 0.0) {
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
    if (Cinit == 0.0) {
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

  * [trblk, sucess] = cc_decode(sbits(1:period), NumTxAnt, period); *
}

void PBCH_Decode(struct_complex bits[480], struct_complex symbols[240], double *nfmod4, 
    double trblk[24], double *cellrefp,const struct_complex sym[240], const struct_complex chanEst[960], 
    const struct_ENB *enb)
{
  double chanAmp[240];
  double dv[480];

  *cellrefp = 1.0;
  MIMO_detectRB(sym, chanEst, 0.0, symbols, chanAmp);
  QPSK_demodulate(symbols, chanAmp, dv);
  bch_decode(dv, enb->NCellID, bits, nfmod4);
  memset(&trblk[0], 0, 24U * sizeof(double));
}
*/


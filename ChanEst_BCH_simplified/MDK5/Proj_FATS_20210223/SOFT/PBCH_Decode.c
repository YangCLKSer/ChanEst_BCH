#pragma once

/* Include Files */
#include "PBCH_Decode.h"

/* Function Declarations */

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
        MIMO_detectRB(symbols, chanAmp, sym, chanEst, c / 2); //modmode在此未使用
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
    
    Sync_PRsqn_genr(scramC, NID);//长度1920,+-1
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

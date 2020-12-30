#include "typedefs.h"
#include "datadefs.h"
#include "Array_Ctrl.h"
#include "ch_esti_prseqGen.h"
#include <math.h>




/*------------------------ch_esti_rsGen.m---------------------------*/
            /* ------------------------------------------------------------------------------% */
            /* numMaxRBDL = 110; a const defined by 3GPP LTE Downlink */
            /* NCP = 1; % normal CP */
            /* numSymDL = 7; */
void ch_esti_rsGen(ARRAY_int32* locRS,ARRAY_creal* valRS,int idxSlot, int idxSym, int NID, int idxAntPort,
    int numRBDL, int numSymDL, int NCP)
{
    int numMaxRBDL = 110;   // a const defined by 3GPP LTE Downlink
    int V = 0, i, loop_ub, Vshift, lenC, Cinit;
    double sqrt_res;
    ARRAY_int32* C, * m;//C初始化发生在ch_esti_prseqGen内部
    ARRAY_creal* RSSeq;
    //初始化m
    Init_int32(&m, 2);
    i = m->size[0] * m->size[1];
    m->size[0] = 1;
    loop_ub = (int)(2*numRBDL);
    m->size[1] = loop_ub;
    EnsureCapacity_real(m, i);
    for (i = 0; i < loop_ub; i++) {
        m->data[i] = i + numMaxRBDL - numRBDL + 1;
    }


    // RS location generation
    switch (idxAntPort)
    {
    case 0:
        if (0 == idxSym)
            V = 0;
        else if (numSymDL - 3 == idxSym)
            V = 3;
        else
            return;
    case 1:
        if (0 == idxSym)
            V = 0;
        else if (numSymDL - 3 == idxSym)
            V = 3;
        else
            return;
    case 2:
        if (1 == idxSym)
            V = 3 * (idxAntPort%2);
        else
            return;
    case 3:
        if (0 == idxSym)
            V = 3 + 3 * (idxSlot%2);
        else
            return;
    }
    Vshift = ((NID % 6) + V) % 6 + 1;
    Init_int32(&locRS, 2);
    i = locRS->size[0] * locRS->size[1];
    locRS->size[0] = 1;
    loop_ub = (int)(2 * numRBDL);
    locRS->size[1] = loop_ub;
    EnsureCapacity_real(locRS, i);
    for (i = 0; i < loop_ub; i++) {
        locRS->data[i] = 6 * i + Vshift;
    }

    //RS seq generation
    lenC = 4 * numMaxRBDL;
    Cinit = (1 << 10) * (7 * (idxSlot + 1) + idxSym + 1) * (2 * NID + 1) + 2 * NID + NCP;
    ch_esti_prseqGen(C, lenC, Cinit);

    Init_creal(&RSSeq, 2);
    i = RSSeq->size[0] * RSSeq->size[1];
    RSSeq->size[0] = 1;
    loop_ub = (int)(2 * numMaxRBDL);
    RSSeq->size[1] = loop_ub;
    EnsureCapacity_real(RSSeq, i);
    sqrt_res = sqrt(2) / 2;
    for (i = 0; i < loop_ub; i++) {
        RSSeq->data[i].re = sqrt_res * (1 - 2 * C->data[2 * i]);
        RSSeq->data[i].im = sqrt_res * (1 - 2 * C->data[2 * i + 1]);
    }

    //RS value corresponding to locRS generation
    Init_creal(&valRS, 2);
    i = valRS->size[0] * valRS->size[1];
    valRS->size[0] = 1;
    loop_ub = (int)(2 * numRBDL);
    valRS->size[1] = loop_ub;
    EnsureCapacity_real(valRS, i);
    sqrt_res = sqrt(2) / 2;
    for (i = 0; i < loop_ub; i++) {
        valRS->data[i].re = RSSeq->data[m->data[i]].re;
        valRS->data[i].im = RSSeq->data[m->data[i]].im;
    }

    Free_int32(m);
    Free_int32(C);
    Free_creal(RSSeq);
}
#include "ch_esti_rsGen.h"




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
    ARRAY_int32* C, * mm;
    //int* mm = new int[2 * numRBDL];//C初始化发生在ch_esti_prseqGen内部
    //ARRAY_creal* RSSeq;

    // RS location generation
    switch (idxAntPort)
    {
        case 0:
        {
            if (0 == idxSym)
                V = 0;
            else if (numSymDL - 3 == idxSym)
                V = 3;
            else
                return;
            break;
        }
        case 1:
        {
            if (0 == idxSym)
                V = 0;
            else if (numSymDL - 3 == idxSym)
                V = 3;
            else
                return;
            break;
        }
        case 2:
        {
            if (1 == idxSym)
                V = 3 * (idxAntPort%2);
            else
                return;
            break;
        }
        case 3:
        {
            if (0 == idxSym)
                V = 3 + 3 * (idxSlot%2);
            else
                return;
        }
        default:
        {
            V = 0;
            return;
        }
    }
    Vshift = ((NID % 6) + V) % 6 + 1;
    i = locRS->size[0] * locRS->size[1];
    locRS->size[0] = 1;
    loop_ub = (int)(2 * numRBDL);
    locRS->size[1] = loop_ub;
    EnsureCapacity_int32(locRS, i);
    for (i = 0; i < loop_ub; i++) {
        locRS->data[i] = 6 * i + Vshift;
    }

    //RS seq generation
    lenC = 4 * numMaxRBDL;
    Cinit = (1 << 10) * (7 * (idxSlot + 1) + idxSym + 1) * (2 * NID + 1) + 2 * NID + NCP;
    Init_int32(&C, 2);
    ch_esti_prseqGen(C, lenC, Cinit);

    /*Init_creal(&RSSeq, 2);
    i = RSSeq->size[0] * RSSeq->size[1];
    RSSeq->size[0] = 1;
    loop_ub = (int)(2 * numMaxRBDL);
    RSSeq->size[1] = loop_ub;
    EnsureCapacity_creal(RSSeq, i);
    sqrt_res = 0.70710678118654757;
    for (i = 0; i < loop_ub; i++) {
        RSSeq->data[i].re = sqrt_res * (double)(1 - 2 * C->data[2 * i]);
        RSSeq->data[i].im = sqrt_res * (double)(1 - 2 * C->data[2 * i + 1]);
    }*/
    
    //初始化mm
    Init_int32(&mm, 2);
    i = mm->size[0] * mm->size[1];
    mm->size[0] = 1;
    loop_ub = (2 * numRBDL);
    mm->size[1] = loop_ub;
    EnsureCapacity_int32(mm, i);
    for (i = 0; i < loop_ub; i++) {
        mm->data[i] = i + numMaxRBDL - numRBDL + 1;
        //printf("%d:%d\n", i + 1, mm->data[i]);
    }

    //RS value corresponding to locRS generation
    i = valRS->size[0] * valRS->size[1];
    valRS->size[0] = 1;
    loop_ub = (int)(2 * numRBDL);
    valRS->size[1] = loop_ub;
    EnsureCapacity_creal(valRS, i);
    sqrt_res = 0.70710678118654757;
    for (i = 0; i < loop_ub; i++) {
        valRS->data[i].re = sqrt_res * (double)(1 - 2 * (C->data[2 * (mm->data[i]) - 2]));
        valRS->data[i].im = sqrt_res * (double)(1 - 2 * (C->data[2 * (mm->data[i]) - 1]));
    }
    
    Free_int32(&C);
    Free_int32(&mm);
    //delete[] mm;
    
    //Free_creal(&RSSeq);
}
#include "ChanEst.h"

void ChannelEst(ARRAY_creal* TempChan, ARRAY_creal* rcvSigFreq, double mmseA1, \
    double mmseB1, double mmseA2, double mmseB2, struct_ENB* ENB)
{
    ARRAY_creal* rcvSigFreq72;
    int i, loop_ub, b_loop_ub, i1, c_loop_ub;
    ARRAY_creal* b_rcvSigFreq, * hEst, * b_hEst;

    //申请变量rcvSigFreq72，（可省略）并初始化
    Init_creal(&rcvSigFreq72, 2);
    i = rcvSigFreq72->size[0] * rcvSigFreq72->size[1];
    rcvSigFreq72->size[0] = 1;
    loop_ub = (int)(14.0 * ENB->NDLRB * 12.0);
    rcvSigFreq72->size[1] = loop_ub;
    EnsureCapacity_creal(rcvSigFreq72, i);
    for (i = 0; i < loop_ub; i++) {
        rcvSigFreq72->data[i].re = 0.0;
        rcvSigFreq72->data[i].im = 0.0;
    }


    Init_creal(&b_rcvSigFreq, 2);
    b_loop_ub = rcvSigFreq->size[0];
    i = b_rcvSigFreq->size[0] * b_rcvSigFreq->size[1];
    b_rcvSigFreq->size[0] = rcvSigFreq->size[0];
    b_rcvSigFreq->size[1] = 14;
    EnsureCapacity_creal(b_rcvSigFreq, i);
    for (i = 0; i < b_loop_ub; i++) {
        for (i1 = 0; i1 < 14; i1++) {
            b_rcvSigFreq->data[i1 + b_rcvSigFreq->size[1] * i] = rcvSigFreq->data[i1 +
                rcvSigFreq->size[1] * i];
        }
    }

    for (i = 0; i < loop_ub; i++) {
        rcvSigFreq72->data[i] = b_rcvSigFreq->data[i];
    }

    Free_creal(&b_rcvSigFreq);


    //求解信道估计
    Init_creal(&hEst, 2);
    ch_esti(hEst, rcvSigFreq72, ENB);

    //TempChan初始化
    i = TempChan->size[0] * TempChan->size[1] * TempChan->size[2];
    TempChan->size[0] = 1;
    TempChan->size[1] = hEst->size[0];
    TempChan->size[2] = hEst->size[1];
    EnsureCapacity_creal(TempChan, i);
    loop_ub = hEst->size[0] * hEst->size[1];
    
    Free_creal(&rcvSigFreq72);

    for (i = 0; i < loop_ub; i++) {
        TempChan->data[i].re = 0.0;
        TempChan->data[i].im = 0.0;
    }

    Init_creal(&b_hEst, 2);
    if (1 > hEst->size[0]) {
        loop_ub = 0;
    }
    else {
        loop_ub = hEst->size[0];
    }

    b_loop_ub = TempChan->size[1];
    c_loop_ub = hEst->size[1] - 1;
    i = b_hEst->size[0] * b_hEst->size[1];
    b_hEst->size[0] = loop_ub;
    b_hEst->size[1] = hEst->size[1];
    EnsureCapacity_creal(b_hEst, i);
    for (i = 0; i <= c_loop_ub; i++) {
        for (i1 = 0; i1 < loop_ub; i1++) {
            b_hEst->data[i1 + b_hEst->size[0] * i] = hEst->data[i1 + hEst->size[0] * i];
        }
    }

    i = hEst->size[0] * hEst->size[1];
    hEst->size[0] = b_hEst->size[0];
    hEst->size[1] = b_hEst->size[1];
    EnsureCapacity_creal(hEst, i);
    loop_ub = b_hEst->size[1];
    for (i = 0; i < loop_ub; i++) {
        c_loop_ub = b_hEst->size[0];
        for (i1 = 0; i1 < c_loop_ub; i1++) {
            hEst->data[i1 + hEst->size[0] * i] = b_hEst->data[i1 + b_hEst->size[0] * i];
        }
    }

    Free_creal(&b_hEst);

    //TempChan赋值
    c_loop_ub = TempChan->size[1];
    loop_ub = TempChan->size[2];
    for (i = 0; i < loop_ub; i++) {
        for (i1 = 0; i1 < b_loop_ub; i1++) {
            TempChan->data[i1 + TempChan->size[1] * i] = hEst->data[i1 + c_loop_ub * i];
        }
    }

    Free_creal(&hEst);
}

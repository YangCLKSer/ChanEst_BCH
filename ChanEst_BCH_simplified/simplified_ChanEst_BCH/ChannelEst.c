#include "ChannelEst.h"
#include "typedefs.h"
#include "ARRAY_Ctrl.h"
#include <math.h>

/*
 * Arguments    : const ARRAY_creal *rcvSigFreq
 *                double mmseA1
 *                double mmseB1
 *                double mmseA2
 *                double mmseB2
 *                const struct_ENB *ENB
 *                ARRAY_creal *TempChan
 * Return Type  : void
 */

void ChannelEst(const ARRAY_creal* rcvSigFreq, double mmseA1, double mmseB1,
    double mmseA2, double mmseB2, const struct_ENB* ENB, ARRAY_creal* TempChan)
{
    ARRAY_creal* rcvSigFreq72;
    int i;
    int loop_ub;
    ARRAY_creal* b_rcvSigFreq;
    int b_loop_ub;
    int i1;
    ARRAY_creal* hEst;
    ARRAY_creal* b_hEst;
    int c_loop_ub;
    (void)mmseA1;
    (void)mmseB1;
    (void)mmseA2;
    (void)mmseB2;

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
    for (i = 0; i < 14; i++) {
        for (i1 = 0; i1 < b_loop_ub; i1++) {
            b_rcvSigFreq->data[i1 + b_rcvSigFreq->size[0] * i] = rcvSigFreq->data[i1 +
                rcvSigFreq->size[0] * i];
        }
    }

    for (i = 0; i < loop_ub; i++) {
        rcvSigFreq72->data[i] = b_rcvSigFreq->data[i];
    }

    Free_creal(&b_rcvSigFreq);


    //求解信道估计
    Init_creal(&hEst, 2);
    ch_esti(rcvSigFreq72, ENB->NDLRB, ENB->CyclicPrefix, ENB->NCellID,
        ENB->NSubframe, ENB->CellRefP, hEst);

    //hEst转换为TempChan
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
    c_loop_ub = TempChan->size[1];
    loop_ub = TempChan->size[2];
    for (i = 0; i < loop_ub; i++) {
        for (i1 = 0; i1 < b_loop_ub; i1++) {
            TempChan->data[i1 + TempChan->size[1] * i] = hEst->data[i1 + c_loop_ub * i];
        }
    }

    Free_creal(&hEst);
}

/*
 * Channel Estimation function
 *  Abstract:
 *    This function deals with the received data and produces the channel
 *    estimation results, also removes the RS for later produres.
 *  Input:
 *    RxDataBCE, received data before channel estimation,
 *        size() = [ numRxAnt, Len ], Len = numRBDL*12
 *    methodCE, method used for channel estimation, ABCD
 *        A: 2D estimation,
 *        B: time interp, 1-linear
 *        C: freq interp, 1-linear, 2-mmse, 3-dft, 4-dct
 *        D: rs estimation, 1-ls, 2-mmse
 *        Note that, mmse algorithm and 2D estimation can not be used.
 *    Pc, cut off windows for dft and dct algorithm, 0.2 - 1.0
 *    startSlot, the index of start slot of RxDataBCE
 *    NID, ID number of Cell
 *    numTxAnt, number of Tx antenna
 *        Note that, numTxAnt is assumed to be 1 for current version
 *    numRBDL, number of resource blocks downlink
 *    numSymDL, number of ofdms per slot, sometimes = numSym
 *    NCP, type of CP, 1-normal, 0-extend
 *  Output:
 *    hEst, channel freq estimation output, with rs
 *        size() = [ numTxAnt*numRxAnt, Len ], Len = Len in RxDataBCE.
 *  Reference:
 *    [1] 3GPP TS 36.211 V8.4.0 (2008-09).
 *  Author:
 *    Yang Yushan     2009-04-09
 *  Version History:
 *    Yang Yushan     2009-04-09      Created
 * ------------------------------------------------------------------------------%
 *  assume that numTxAnt = 1 for current version.
 * Arguments    : const ARRAY_creal *RxDataBCE
 *                double enb_NDLRB
 *                const char enb_CyclicPrefix[6]
 *                double enb_NCellID
 *                double enb_NSubframe
 *                double enb_CellRefP
 *                ARRAY_creal *hEst
 * Return Type  : void
 */

void ch_esti(const ARRAY_creal* RxDataBCE, double enb_NDLRB, const char
    enb_CyclicPrefix[6], double enb_NCellID, double enb_NSubframe,
    double enb_CellRefP, ARRAY_creal* hEst)
{
    double startSlot;
    int i;
    boolean empty_non_axis_sizes;
    boolean x[6];
    static const char cv[6] = { 'N', 'o', 'r', 'm', 'a', 'l' };

    int k;
    boolean exitg1;
    int NCP;
    int numSymDL;
    double numOFDM;
    int i1;
    int loop_ub;
    ARRAY_real* locOFDMWithRS;
    ARRAY_real* locRS;
    ARRAY_creal* valRS;
    ARRAY_creal* temphEst;
    ARRAY_real* tempLoc;
    ARRAY_creal* tempVal;
    ARRAY_uint32* b_locOFDMWithRS;
    ARRAY_real* m;
    ARRAY_int32* r;
    ARRAY_creal* b_RxDataBCE;
    ARRAY_real* b_locRS;
    ARRAY_creal* b_valRS;
    int idxAntPort;
    double idxSlot;
    int i_ofdm;
    int i2;
    double V;
    boolean_T guard1 = false;
    double Vshift;
    int n;
    double b;
    int i3;
    int b_loop_ub;
    int i4;
    int c_loop_ub;
    double C[440];
    signed char i5;
    int i6;
    ARRAY_real c_locOFDMWithRS;
    int d_locOFDMWithRS[1];
    int e_locOFDMWithRS[1];
    startSlot = enb_NSubframe * 2.0;
    for (i = 0; i < 6; i++) {
        x[i] = (enb_CyclicPrefix[i] == cv[i]);
    }

    empty_non_axis_sizes = true;
    k = 0;
    exitg1 = false;
    while ((!exitg1) && (k < 6)) {
        if (!x[k]) {
            empty_non_axis_sizes = false;
            exitg1 = true;
        }
        else {
            k++;
        }
    }

    if (empty_non_axis_sizes) {
        NCP = 1;
        numSymDL = 7;
    }
    else {
        NCP = 0;
        numSymDL = 6;
    }

    /*  init */
    numOFDM = (double)RxDataBCE->size[1] / enb_NDLRB / 12.0;

    /*  number of RS per ofdm */
    i = (int)enb_CellRefP;
    i1 = hEst->size[0] * hEst->size[1];
    hEst->size[0] = i;
    hEst->size[1] = RxDataBCE->size[1];
    EnsureCapacity_creal(hEst, i1);
    loop_ub = i * RxDataBCE->size[1];
    for (i = 0; i < loop_ub; i++) {
        hEst->data[i].re = 0.0;
        hEst->data[i].im = 0.0;
    }

    i = (int)((enb_CellRefP - 1.0) + 1.0);
    Init_real(&locOFDMWithRS, 2);
    Init_real(&locRS, 2);
    Init_creal(&valRS, 2);
    Init_creal(&temphEst, 2);
    Init_real(&tempLoc, 2);
    Init_creal(&tempVal, 2);
    Init_uint32(&b_locOFDMWithRS, 1);
    Init_real(&m, 2);
    Init_int32(&r, 1);
    Init_creal(&b_RxDataBCE, 2);
    Init_real(&b_locRS, 2);
    Init_creal(&b_valRS, 2);
    for (idxAntPort = 0; idxAntPort < i; idxAntPort++) {
        /*     %% recover reference signals */
        locOFDMWithRS->size[0] = 0;
        locOFDMWithRS->size[1] = 1;
        locRS->size[0] = 0;
        locRS->size[1] = 0;
        valRS->size[0] = 0;
        valRS->size[1] = 0;
        idxSlot = startSlot;
        i1 = (int)((numOFDM - 1.0) + 1.0);
        for (i_ofdm = 0; i_ofdm < i1; i_ofdm++) {
            if (i_ofdm == 0) {
                k = 0;
            }
            else {
                k = (int)fmod(i_ofdm, numSymDL);
            }

            if ((k == 0) && (0 != i_ofdm)) {
                idxSlot++;
            }


            /*------------------------ch_esti_rsGen.m---------------------------*/
            /* ------------------------------------------------------------------------------% */
            /* numMaxRBDL = 110; a const defined by 3GPP LTE Downlink */
            /* NCP = 1; % normal CP */
            /* numSymDL = 7; */
            tempLoc->size[0] = 0;
            tempLoc->size[1] = 0;
            tempVal->size[0] = 0;
            tempVal->size[1] = 0;
            V = 0.0;

            /*  RS location generation */
            guard1 = false;
            switch (idxAntPort) {
            case 0:
                if (0 == k) {
                    guard1 = true;
                }
                else {
                    if (numSymDL - 3 == k) {
                        V = 3.0;
                        guard1 = true;
                    }
                }
                break;

            case 1:
                if (0 == k) {
                    V = 3.0;
                    guard1 = true;
                }
                else {
                    if (numSymDL - 3 == k) {
                        guard1 = true;
                    }
                }
                break;

            case 2:
                //如下为计算mod，可以化简为int操作
                if (1 == k) {
                    if (idxSlot == 0.0) {
                        V = 0.0;
                    }
                    else {
                        V = fmod(idxSlot, 2.0);
                        if (V == 0.0) {
                            V = 0.0;
                        }
                        else {
                            if (idxSlot < 0.0) {
                                V += 2.0;
                            }
                        }
                    }

                    V *= 3.0;
                    guard1 = true;
                }
                break;

            case 3:
                if (1 == k) {
                    if (idxSlot == 0.0) {
                        V = 0.0;
                    }
                    else {
                        V = fmod(idxSlot, 2.0);
                        if (V == 0.0) {
                            V = 0.0;
                        }
                        else {
                            if (idxSlot < 0.0) {
                                V += 2.0;
                            }
                        }
                    }

                    V = 3.0 * V + 3.0;
                    guard1 = true;
                }
                break;

            default:
                guard1 = true;
                break;
            }

            if (guard1) {
                if (enb_NCellID == 0.0) {
                    Vshift = 0.0;
                }
                else {
                    Vshift = fmod(enb_NCellID, 6.0);
                    if (Vshift == 0.0) {
                        Vshift = 0.0;
                    }
                    else {
                        if (enb_NCellID < 0.0) {
                            Vshift += 6.0;
                        }
                    }
                }

                b = 2.0 * enb_NDLRB - 1.0;
                if (b < 0.0) {
                    m->size[0] = 1;
                    m->size[1] = 0;
                }
                else {
                    i2 = m->size[0] * m->size[1];
                    m->size[0] = 1;
                    loop_ub = (int)floor(b);
                    m->size[1] = loop_ub + 1;
                    EnsureCapacity_real(m, i2);
                    for (i2 = 0; i2 <= loop_ub; i2++) {
                        m->data[i2] = i2;
                    }
                }

                Vshift += V;
                if (Vshift == 0.0) {
                    V = 0.0;
                }
                else {
                    V = fmod(Vshift, 6.0);
                    if (V == 0.0) {
                        V = 0.0;
                    }
                    else {
                        if (Vshift < 0.0) {
                            V += 6.0;
                        }
                    }
                }

                i2 = tempLoc->size[0] * tempLoc->size[1];
                tempLoc->size[0] = 1;
                tempLoc->size[1] = m->size[1];
                EnsureCapacity_real(tempLoc, i2);
                loop_ub = m->size[0] * m->size[1];
                for (i2 = 0; i2 < loop_ub; i2++) {
                    tempLoc->data[i2] = (6.0 * m->data[i2] + V) + 1.0;
                }

                /*  RS seq generation */
                ch_esti_prseqGen((1024.0 * ((7.0 * (idxSlot + 1.0) + (double)k) + 1.0) *
                    (2.0 * enb_NCellID + 1.0) + 2.0 * enb_NCellID) +
                    (double)NCP, C);

                /*  RS value correspondint to locRS generation */
                i2 = tempVal->size[0] * tempVal->size[1];
                tempVal->size[0] = 1;
                tempVal->size[1] = m->size[1];
                EnsureCapacity_creal(tempVal, i2);
                loop_ub = m->size[0] * m->size[1];
                for (i2 = 0; i2 < loop_ub; i2++) {
                    k = ((int)(((m->data[i2] + 110.0) - enb_NDLRB) + 1.0) - 1) << 1;
                    V = 1.0 - 2.0 * C[k + 1];
                    tempVal->data[i2].re = 0.70710678118654746 * ((1.0 - 2.0 * C[k]) + 0.0
                        * V);
                    tempVal->data[i2].im = 0.70710678118654746 * V;
                }
            }
            /*------------------------end of ch_esti_rsGen.m---------------------*/
            
            if ((tempLoc->size[0] == 0) || (tempLoc->size[1] == 0)) {
                n = 0;
            }
            else {
                n = tempLoc->size[1];
            }

            

            if (0 != n) {
                loop_ub = locOFDMWithRS->size[0];
                i2 = b_locOFDMWithRS->size[0];
                b_locOFDMWithRS->size[0] = locOFDMWithRS->size[0] + 1;
                EnsureCapacity_uint32(b_locOFDMWithRS, i2);
                for (i2 = 0; i2 < loop_ub; i2++) {
                    b_locOFDMWithRS->data[i2] = (unsigned int)locOFDMWithRS->data[i2];
                }

                b_locOFDMWithRS->data[locOFDMWithRS->size[0]] = i_ofdm + 1U;
                i2 = locOFDMWithRS->size[0] * locOFDMWithRS->size[1];
                locOFDMWithRS->size[0] = b_locOFDMWithRS->size[0];
                locOFDMWithRS->size[1] = 1;
                EnsureCapacity_real(locOFDMWithRS, i2);
                loop_ub = b_locOFDMWithRS->size[0];
                for (i2 = 0; i2 < loop_ub; i2++) {
                    locOFDMWithRS->data[i2] = b_locOFDMWithRS->data[i2];
                }

                if ((locRS->size[0] != 0) && (locRS->size[1] != 0)) {
                    k = locRS->size[1];
                }
                else if ((tempLoc->size[0] != 0) && (tempLoc->size[1] != 0)) {
                    k = tempLoc->size[1];
                }
                else {
                    k = locRS->size[1];
                    if (k <= 0) {
                        k = 0;
                    }

                    if (tempLoc->size[1] > k) {
                        k = tempLoc->size[1];
                    }
                }

                empty_non_axis_sizes = (k == 0);
                if (empty_non_axis_sizes || ((locRS->size[0] != 0) && (locRS->size[1] !=
                    0))) {
                    loop_ub = locRS->size[0];
                }
                else {
                    loop_ub = 0;
                }

                if (empty_non_axis_sizes || ((tempLoc->size[0] != 0) && (tempLoc->size[1]
                    != 0))) {
                    i5 = (signed char)tempLoc->size[0];
                }
                else {
                    i5 = 0;
                }

                i2 = b_locRS->size[0] * b_locRS->size[1];
                b_locRS->size[0] = loop_ub + i5;
                b_locRS->size[1] = k;
                EnsureCapacity_real(b_locRS, i2);
                for (i2 = 0; i2 < k; i2++) {
                    for (i6 = 0; i6 < loop_ub; i6++) {
                        b_locRS->data[i6 + b_locRS->size[0] * i2] = locRS->data[i6 + loop_ub
                            * i2];
                    }
                }

                for (i2 = 0; i2 < k; i2++) {
                    n = i5;
                    for (i6 = 0; i6 < n; i6++) {
                        b_locRS->data[(i6 + loop_ub) + b_locRS->size[0] * i2] =
                            tempLoc->data[i6 + tempLoc->size[0] * i2];
                    }
                }

                i2 = locRS->size[0] * locRS->size[1];
                locRS->size[0] = b_locRS->size[0];
                locRS->size[1] = b_locRS->size[1];
                EnsureCapacity_real(locRS, i2);
                loop_ub = b_locRS->size[0] * b_locRS->size[1];
                for (i2 = 0; i2 < loop_ub; i2++) {
                    locRS->data[i2] = b_locRS->data[i2];
                }

                if ((valRS->size[0] != 0) && (valRS->size[1] != 0)) {
                    k = valRS->size[1];
                }
                else if ((tempVal->size[0] != 0) && (tempVal->size[1] != 0)) {
                    k = tempVal->size[1];
                }
                else {
                    k = valRS->size[1];
                    if (k <= 0) {
                        k = 0;
                    }

                    if (tempVal->size[1] > k) {
                        k = tempVal->size[1];
                    }
                }

                empty_non_axis_sizes = (k == 0);
                if (empty_non_axis_sizes || ((valRS->size[0] != 0) && (valRS->size[1] !=
                    0))) {
                    loop_ub = valRS->size[0];
                }
                else {
                    loop_ub = 0;
                }

                if (empty_non_axis_sizes || ((tempVal->size[0] != 0) && (tempVal->size[1]
                    != 0))) {
                    i5 = (signed char)tempVal->size[0];
                }
                else {
                    i5 = 0;
                }

                i2 = b_valRS->size[0] * b_valRS->size[1];
                b_valRS->size[0] = loop_ub + i5;
                b_valRS->size[1] = k;
                EnsureCapacity_creal(b_valRS, i2);
                for (i2 = 0; i2 < k; i2++) {
                    for (i6 = 0; i6 < loop_ub; i6++) {
                        b_valRS->data[i6 + b_valRS->size[0] * i2] = valRS->data[i6 + loop_ub
                            * i2];
                    }
                }

                for (i2 = 0; i2 < k; i2++) {
                    n = i5;
                    for (i6 = 0; i6 < n; i6++) {
                        b_valRS->data[(i6 + loop_ub) + b_valRS->size[0] * i2] =
                            tempVal->data[i6 + tempVal->size[0] * i2];
                    }
                }

                i2 = valRS->size[0] * valRS->size[1];
                valRS->size[0] = b_valRS->size[0];
                valRS->size[1] = b_valRS->size[1];
                EnsureCapacity_creal(valRS, i2);
                loop_ub = b_valRS->size[0] * b_valRS->size[1];
                for (i2 = 0; i2 < loop_ub; i2++) {
                    valRS->data[i2] = b_valRS->data[i2];
                }
            }
        }

        /*  ls on rs locations */
        loop_ub = RxDataBCE->size[1];
        i1 = b_RxDataBCE->size[0] * b_RxDataBCE->size[1];
        b_RxDataBCE->size[0] = 1;
        b_RxDataBCE->size[1] = RxDataBCE->size[1];
        EnsureCapacity_creal(b_RxDataBCE, i1);
        for (i1 = 0; i1 < loop_ub; i1++) {
            b_RxDataBCE->data[i1] = RxDataBCE->data[i1];
        }

        k = (int)(enb_NDLRB * 12.0);
        i1 = b_valRS->size[0] * b_valRS->size[1];
        loop_ub = (int)numOFDM;
        b_valRS->size[0] = loop_ub;
        b_valRS->size[1] = k;
        EnsureCapacity_creal(b_valRS, i1);
        for (i1 = 0; i1 < k; i1++) {
            for (i2 = 0; i2 < loop_ub; i2++) {
                b_valRS->data[i2 + b_valRS->size[0] * i1] = b_RxDataBCE->data[i1 + k *
                    i2];
            }
        }

        /* ------------------------------------------------------------------------------% */
        i1 = temphEst->size[0] * temphEst->size[1];
        temphEst->size[0] = b_valRS->size[0];
        temphEst->size[1] = b_valRS->size[1];
        EnsureCapacity_creal(temphEst, i1);
        loop_ub = b_valRS->size[0] * b_valRS->size[1];
        for (i1 = 0; i1 < loop_ub; i1++) {
            temphEst->data[i1].re = 0.0;
            temphEst->data[i1].im = 0.0;
        }

        /*--------------------------ch_esti_ls.m---------------------------------*/
        /*  estimation on reference symbols */
        /* ----- LS estimation */
        i1 = locOFDMWithRS->size[0];
        if (0 <= locOFDMWithRS->size[0] - 1) {
            i3 = locRS->size[1];
            b_loop_ub = locRS->size[1];
            i4 = locRS->size[1];
            c_loop_ub = locRS->size[1];
        }

        for (n = 0; n < i1; n++) {
            i2 = r->size[0];
            r->size[0] = i3;
            EnsureCapacity_int32(r, i2);
            for (i2 = 0; i2 < b_loop_ub; i2++) {
                r->data[i2] = (int)locRS->data[n + locRS->size[0] * i2] - 1;
            }

            k = (int)(unsigned int)locOFDMWithRS->data[n];
            i2 = b_RxDataBCE->size[0] * b_RxDataBCE->size[1];
            b_RxDataBCE->size[0] = 1;
            b_RxDataBCE->size[1] = i4;
            EnsureCapacity_creal(b_RxDataBCE, i2);
            for (i2 = 0; i2 < c_loop_ub; i2++) {
                b_RxDataBCE->data[i2] = b_valRS->data[(k + b_valRS->size[0] * ((int)
                    locRS->data[n + locRS->size[0] * i2] - 1)) - 1];
            }

            loop_ub = b_RxDataBCE->size[1];
            for (i2 = 0; i2 < loop_ub; i2++) {
                if (valRS->data[n + valRS->size[0] * i2].im == 0.0) {
                    if (b_RxDataBCE->data[i2].im == 0.0) {
                        temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].re =
                            b_RxDataBCE->data[i2].re / valRS->data[n + valRS->size[0] * i2].re;
                        temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].im = 0.0;
                    }
                    else if (b_RxDataBCE->data[i2].re == 0.0) {
                        temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].re = 0.0;
                        temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].im =
                            b_RxDataBCE->data[i2].im / valRS->data[n + valRS->size[0] * i2].re;
                    }
                    else {
                        temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].re =
                            b_RxDataBCE->data[i2].re / valRS->data[n + valRS->size[0] * i2].re;
                        temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].im =
                            b_RxDataBCE->data[i2].im / valRS->data[n + valRS->size[0] * i2].re;
                    }
                }
                else if (valRS->data[n + valRS->size[0] * i2].re == 0.0) {
                    if (b_RxDataBCE->data[i2].re == 0.0) {
                        temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].re =
                            b_RxDataBCE->data[i2].im / valRS->data[n + valRS->size[0] * i2].im;
                        temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].im = 0.0;
                    }
                    else if (b_RxDataBCE->data[i2].im == 0.0) {
                        temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].re = 0.0;
                        temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].im =
                            -(b_RxDataBCE->data[i2].re / valRS->data[n + valRS->size[0] * i2].
                                im);
                    }
                    else {
                        temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].re =
                            b_RxDataBCE->data[i2].im / valRS->data[n + valRS->size[0] * i2].im;
                        temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].im =
                            -(b_RxDataBCE->data[i2].re / valRS->data[n + valRS->size[0] * i2].
                                im);
                    }
                }
                else {
                    b = fabs(valRS->data[n + valRS->size[0] * i2].re);
                    V = fabs(valRS->data[n + valRS->size[0] * i2].im);
                    if (b > V) {
                        V = valRS->data[n + valRS->size[0] * i2].im / valRS->data[n +
                            valRS->size[0] * i2].re;
                        Vshift = valRS->data[n + valRS->size[0] * i2].re + V * valRS->data[n
                            + valRS->size[0] * i2].im;
                        temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].re =
                            (b_RxDataBCE->data[i2].re + V * b_RxDataBCE->data[i2].im) / Vshift;
                        temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].im =
                            (b_RxDataBCE->data[i2].im - V * b_RxDataBCE->data[i2].re) / Vshift;
                    }
                    else if (V == b) {
                        if (valRS->data[n + valRS->size[0] * i2].re > 0.0) {
                            V = 0.5;
                        }
                        else {
                            V = -0.5;
                        }

                        if (valRS->data[n + valRS->size[0] * i2].im > 0.0) {
                            Vshift = 0.5;
                        }
                        else {
                            Vshift = -0.5;
                        }

                        temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].re =
                            (b_RxDataBCE->data[i2].re * V + b_RxDataBCE->data[i2].im * Vshift)
                            / b;
                        temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].im =
                            (b_RxDataBCE->data[i2].im * V - b_RxDataBCE->data[i2].re * Vshift)
                            / b;
                    }
                    else {
                        V = valRS->data[n + valRS->size[0] * i2].re / valRS->data[n +
                            valRS->size[0] * i2].im;
                        Vshift = valRS->data[n + valRS->size[0] * i2].im + V * valRS->data[n
                            + valRS->size[0] * i2].re;
                        temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].re = (V *
                            b_RxDataBCE->data[i2].re + b_RxDataBCE->data[i2].im) / Vshift;
                        temphEst->data[(k + temphEst->size[0] * r->data[i2]) - 1].im = (V *
                            b_RxDataBCE->data[i2].im - b_RxDataBCE->data[i2].re) / Vshift;
                    }
                }
            }
        }

        k = locOFDMWithRS->size[0];
        c_locOFDMWithRS = *locOFDMWithRS;
        d_locOFDMWithRS[0] = k;
        c_locOFDMWithRS.size = &d_locOFDMWithRS[0];
        c_locOFDMWithRS.numDimensions = 1;
        ch_esti_dct(temphEst, &c_locOFDMWithRS, locRS);

        /*  time interpolation */
        k = locOFDMWithRS->size[0];
        c_locOFDMWithRS = *locOFDMWithRS;
        e_locOFDMWithRS[0] = k;
        c_locOFDMWithRS.size = &e_locOFDMWithRS[0];
        c_locOFDMWithRS.numDimensions = 1;
        ch_estiime_intp(temphEst, &c_locOFDMWithRS);
        i1 = b_valRS->size[0] * b_valRS->size[1];
        b_valRS->size[0] = temphEst->size[1];
        b_valRS->size[1] = temphEst->size[0];
        EnsureCapacity_creal(b_valRS, i1);
        loop_ub = temphEst->size[0];
        for (i1 = 0; i1 < loop_ub; i1++) {
            n = temphEst->size[1];
            for (i2 = 0; i2 < n; i2++) {
                b_valRS->data[i2 + b_valRS->size[0] * i1] = temphEst->data[i1 +
                    temphEst->size[0] * i2];
            }
        }

        k = (int)(enb_NDLRB * 12.0 * numOFDM);
        for (i1 = 0; i1 < k; i1++) {
            hEst->data[idxAntPort + hEst->size[0] * i1] = b_valRS->data[i1];
        }
    }

    Free_creal(&b_valRS);
    Free_real(&b_locRS);
    Free_creal(&b_RxDataBCE);
    Free_int32(&r);
    Free_real(&m);
    Free_uint32(&b_locOFDMWithRS);
    Free_creal(&tempVal);
    Free_real(&tempLoc);
    Free_creal(&temphEst);
    Free_creal(&valRS);
    Free_real(&locRS);
    Free_real(&locOFDMWithRS);
}

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
    if (u1 == 0.0) {
        if (u0 > 0.0) {
            y = RT_PI / 2.0;
        }
        else if (u0 < 0.0) {
            y = -(RT_PI / 2.0);
        }
        else {
            y = 0.0;
        }
    }
    else {
        y = atan2(u0, u1);
    }

    return y;
}

/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_hypotd_snf(double u0, double u1)
{
    double y;
    double a;
    a = fabs(u0);
    y = fabs(u1);
    if (a < y) {
        a /= y;
        y *= sqrt(a * a + 1.0);
    }
    else if (a > y) {
        y /= a;
        y = a * sqrt(y * y + 1.0);
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
        }
        else if (u > -0.5) {
            y = u * 0.0;
        }
        else {
            y = ceil(u - 0.5);
        }
    }
    else {
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
 * Arguments    : ARRAY_creal *hEst
 *                const ARRAY_real *locOFDMWithRS
 *                const ARRAY_real *locRS
 * Return Type  : void
 */
void ch_esti_dct(ARRAY_creal* hEst, const ARRAY_real* locOFDMWithRS,
    const ARRAY_real* locRS)
{
    int numSym;
    int numPc;
    int u1;
    int i;
    int i1;
    ARRAY_creal* gEst;
    int loop_ub;
    ARRAY_creal* aa;
    ARRAY_creal* ww;
    int b_loop_ub;
    ARRAY_creal* yy;
    ARRAY_creal* y;
    int i2;
    ARRAY_creal* z;
    unsigned int b_y;
    ARRAY_real* a;
    double c_y;
    ARRAY_creal* Mn;
    int i3;
    ARRAY_real* d_y;
    int c_loop_ub;
    ARRAY_creal* b_yy;
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
    }
    else {
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
        }
        else {
            b_loop_ub = locRS->size[1];
        }

        i2 = locRS->size[1];
        b_y = (unsigned int)locRS->size[1] << 1;
        c_y = sqrt(2.0 * (double)locRS->size[1]);
        i3 = locRS->size[1];
        c_loop_ub = 2 * locRS->size[1];
        if (locRS->size[1] + 1U > ((unsigned int)locRS->size[1] << 1)) {
            i4 = 0;
        }
        else {
            i4 = locRS->size[1];
        }

        m = b_loop_ub - 1;
        md2 = b_loop_ub >> 1;
        if (1 > locRS->size[1]) {
            d_loop_ub = -1;
        }
        else {
            d_loop_ub = locRS->size[1] - 1;
        }

        b = sqrt((double)numSym / (double)u1);
        if (1 > numPc) {
            e_loop_ub = 0;
        }
        else {
            e_loop_ub = numPc;
        }

        aa_idx_1 = numSym - numPc;
    }

    Init_creal(&gEst, 2);
    Init_creal(&aa, 1);
    Init_creal(&ww, 1);
    Init_creal(&yy, 2);
    Init_creal(&y, 1);
    Init_creal(&z, 2);
    Init_real(&a, 2);
    Init_creal(&Mn, 2);
    Init_real(&d_y, 2);
    Init_creal(&b_yy, 2);
    for (n = 0; n < i; n++) {
        i5 = gEst->size[0] * gEst->size[1];
        gEst->size[0] = 1;
        gEst->size[1] = i1;
        EnsureCapacity_creal(gEst, i5);
        for (i5 = 0; i5 < loop_ub; i5++) {
            gEst->data[i5] = hEst->data[((int)locOFDMWithRS->data[n] + hEst->size[0] *
                ((int)locRS->data[n + locRS->size[0] * i5] - 1)) - 1];
        }

        /*--------------------------my_dct.m-------------------------------------*/
        /*  If input is a vector, make it a column: */
        /*  Cast to enforce precision rules */
        /*  Pad or truncate input if necessary */
        i5 = aa->size[0];
        aa->size[0] = b_loop_ub;
        EnsureCapacity_creal(aa, i5);
        for (i5 = 0; i5 < b_loop_ub; i5++) {
            aa->data[i5] = gEst->data[i5];
        }

        /*  Compute weights to multiply DFT coefficients (cast enforces the precision */
        /*  rules) */
        if (i2 - 1 < 0) {
            a->size[0] = 1;
            a->size[1] = 0;
        }
        else {
            i5 = a->size[0] * a->size[1];
            a->size[0] = 1;
            a->size[1] = locRS->size[1];
            EnsureCapacity_real(a, i5);
            f_loop_ub = locRS->size[1] - 1;
            for (i5 = 0; i5 <= f_loop_ub; i5++) {
                a->data[i5] = i5;
            }
        }

        i5 = z->size[0] * z->size[1];
        z->size[0] = 1;
        z->size[1] = a->size[1];
        EnsureCapacity_creal(z, i5);
        f_loop_ub = a->size[0] * a->size[1];
        for (i5 = 0; i5 < f_loop_ub; i5++) {
            r = a->data[i5];
            W_im = 0.0 * r * 3.1415926535897931;
            xtmp_im = -r * 3.1415926535897931;
            if (xtmp_im == 0.0) {
                z->data[i5].re = W_im / (double)b_y;
                z->data[i5].im = 0.0;
            }
            else if (W_im == 0.0) {
                z->data[i5].re = 0.0;
                z->data[i5].im = xtmp_im / (double)b_y;
            }
            else {
                z->data[i5].re = 0.0;
                z->data[i5].im = xtmp_im / (double)b_y;
            }
        }

        numPc = z->size[1];
        for (k = 0; k < numPc; k++) {
            if (z->data[k].im == 0.0) {
                z->data[k].re = exp(z->data[k].re);
                z->data[k].im = 0.0;
            }
            else if (rtIsInf(z->data[k].im) && rtIsInf(z->data[k].re) && (z->data[k]
                .re < 0.0)) {
                z->data[k].re = 0.0;
                z->data[k].im = 0.0;
            }
            else {
                r = exp(z->data[k].re / 2.0);
                z->data[k].re = r * (r * cos(z->data[k].im));
                z->data[k].im = r * (r * sin(z->data[k].im));
            }
        }

        i5 = ww->size[0];
        ww->size[0] = z->size[1];
        EnsureCapacity_creal(ww, i5);
        f_loop_ub = z->size[1];
        for (i5 = 0; i5 < f_loop_ub; i5++) {
            if (z->data[i5].im == 0.0) {
                ww->data[i5].re = z->data[i5].re / c_y;
                ww->data[i5].im = 0.0;
            }
            else if (z->data[i5].re == 0.0) {
                ww->data[i5].re = 0.0;
                ww->data[i5].im = z->data[i5].im / c_y;
            }
            else {
                ww->data[i5].re = z->data[i5].re / c_y;
                ww->data[i5].im = z->data[i5].im / c_y;
            }
        }

        if (ww->data[0].im == 0.0) {
            r = ww->data[0].re / 1.4142135623730951;
            xtmp_im = 0.0;
        }
        else if (ww->data[0].re == 0.0) {
            r = 0.0;
            xtmp_im = ww->data[0].im / 1.4142135623730951;
        }
        else {
            r = ww->data[0].re / 1.4142135623730951;
            xtmp_im = ww->data[0].im / 1.4142135623730951;
        }

        ww->data[0].re = r;
        ww->data[0].im = xtmp_im;

        /*  odd case */
        /*  Form intermediate even-symmetric matrix */
        i5 = y->size[0];
        y->size[0] = 2 * i3;
        EnsureCapacity_creal(y, i5);
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
        }
        else {
            i5 = a->size[0] * a->size[1];
            a->size[0] = 1;
            a->size[1] = y->size[0];
            EnsureCapacity_real(a, i5);
            f_loop_ub = y->size[0] - 1;
            for (i5 = 0; i5 <= f_loop_ub; i5++) {
                a->data[i5] = i5;
            }
        }

        W_im = -6.2831853071795862 / (double)y->size[0];
        if (W_im == 0.0) {
            W_re = 1.0;
            W_im = 0.0;
        }
        else {
            W_re = cos(W_im);
            W_im = sin(W_im);
        }

        i5 = d_y->size[0] * d_y->size[1];
        d_y->size[0] = a->size[1];
        d_y->size[1] = a->size[1];
        EnsureCapacity_real(d_y, i5);
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
        EnsureCapacity_creal(Mn, i5);
        numPc = (int)unnamed_idx_0 * (int)unnamed_idx_1;
        for (k = 0; k < numPc; k++) {
            if ((W_im == 0.0) && (W_re >= 0.0)) {
                Mn->data[k].re = rt_powd_snf(W_re, d_y->data[k]);
                Mn->data[k].im = 0.0;
            }
            else if ((W_re == 0.0) && (floor(d_y->data[k]) == d_y->data[k])) {
                xtmp_im = rt_powd_snf(W_im, d_y->data[k]);
                if (d_y->data[k] == 0.0) {
                    r = 0.0;
                }
                else {
                    r = fmod(d_y->data[k], 4.0);
                    if (r == 0.0) {
                        r = 0.0;
                    }
                    else {
                        if (d_y->data[k] < 0.0) {
                            r += 4.0;
                        }
                    }
                }

                d = rt_roundd_snf(r);
                if (d < 128.0) {
                    i6 = (signed char)d;
                }
                else {
                    i6 = 0;
                }

                if (i6 == 3) {
                    Mn->data[k].re = 0.0;
                    Mn->data[k].im = -xtmp_im;
                }
                else if (i6 == 2) {
                    Mn->data[k].re = -xtmp_im;
                    Mn->data[k].im = 0.0;
                }
                else if (i6 == 1) {
                    Mn->data[k].re = 0.0;
                    Mn->data[k].im = xtmp_im;
                }
                else {
                    Mn->data[k].re = xtmp_im;
                    Mn->data[k].im = 0.0;
                }
            }
            else if ((W_im == 0.0) && rtIsInf(d_y->data[k]) && (fabs(W_re) == 1.0))
            {
                Mn->data[k].re = 1.0;
                Mn->data[k].im = 0.0;
            }
            else {
                if (W_im == 0.0) {
                    if (W_re < 0.0) {
                        r = log(fabs(W_re));
                        xtmp_im = 3.1415926535897931;
                    }
                    else {
                        r = log(W_re);
                        xtmp_im = 0.0;
                    }
                }
                else {
                    r = log(rt_hypotd_snf(W_re, W_im));
                    xtmp_im = rt_atan2d_snf(W_im, W_re);
                }

                r *= d_y->data[k];
                xtmp_im *= d_y->data[k];
                if (xtmp_im == 0.0) {
                    Mn->data[k].re = exp(r);
                    Mn->data[k].im = 0.0;
                }
                else if (rtIsInf(xtmp_im) && rtIsInf(r) && (r < 0.0)) {
                    Mn->data[k].re = 0.0;
                    Mn->data[k].im = 0.0;
                }
                else {
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
        EnsureCapacity_creal(aa, i5);
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
            EnsureCapacity_creal(yy, i5);
            f_loop_ub = aa->size[0];
            for (i5 = 0; i5 < f_loop_ub; i5++) {
                yy->data[yy->size[0] * i5] = aa->data[i5];
            }
        }
        else {
            i5 = yy->size[0] * yy->size[1];
            yy->size[0] = aa->size[0];
            yy->size[1] = 1;
            EnsureCapacity_creal(yy, i5);
            f_loop_ub = aa->size[0];
            for (i5 = 0; i5 < f_loop_ub; i5++) {
                yy->data[i5] = aa->data[i5];
            }
        }

        f_loop_ub = yy->size[1] - 1;
        i5 = b_yy->size[0] * b_yy->size[1];
        b_yy->size[0] = d_loop_ub + 1;
        b_yy->size[1] = yy->size[1];
        EnsureCapacity_creal(b_yy, i5);
        for (i5 = 0; i5 <= f_loop_ub; i5++) {
            for (k = 0; k <= d_loop_ub; k++) {
                b_yy->data[k + b_yy->size[0] * i5] = yy->data[k + yy->size[0] * i5];
            }
        }

        i5 = yy->size[0] * yy->size[1];
        yy->size[0] = b_yy->size[0];
        yy->size[1] = b_yy->size[1];
        EnsureCapacity_creal(yy, i5);
        f_loop_ub = b_yy->size[0] * b_yy->size[1];
        for (i5 = 0; i5 < f_loop_ub; i5++) {
            yy->data[i5] = b_yy->data[i5];
        }

        /*  Multiply FFT by weights: */
        f_loop_ub = ww->size[0];
        i5 = gEst->size[0] * gEst->size[1];
        gEst->size[0] = 1;
        gEst->size[1] = ww->size[0];
        EnsureCapacity_creal(gEst, i5);
        for (i5 = 0; i5 < f_loop_ub; i5++) {
            gEst->data[i5].re = b * (ww->data[i5].re * yy->data[i5].re - ww->data[i5].
                im * yy->data[i5].im);
            gEst->data[i5].im = b * (ww->data[i5].re * yy->data[i5].im + ww->data[i5].
                im * yy->data[i5].re);
        }
        /*--------------------------------end of my_dct.m------------------------*/

        /* plot(abs(gEst)); */
        i5 = z->size[0] * z->size[1];
        z->size[0] = 1;
        z->size[1] = e_loop_ub + aa_idx_1;
        EnsureCapacity_creal(z, i5);
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
        EnsureCapacity_creal(gEst, i5);
        f_loop_ub = z->size[0] * z->size[1];
        for (i5 = 0; i5 < f_loop_ub; i5++) {
            gEst->data[i5] = z->data[i5];
        }

        /*--------------------------ch_esti_eidct.m------------------------------*/
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
        EnsureCapacity_creal(z, i5);
        f_loop_ub = gEst->size[1];
        for (i5 = 0; i5 < f_loop_ub; i5++) {
            z->data[i5].re = 0.0;
            z->data[i5].im = 0.0;
        }

        r = sqrt(2.0 / (double)gEst->size[1]);
        i5 = a->size[0] * a->size[1];
        a->size[0] = 1;
        a->size[1] = gEst->size[1];
        EnsureCapacity_real(a, i5);
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
        /*---------------------------end of ch_esti_eidct.m----------------------*/
        
        if (1.0 != locRS->data[n]) {
            d = (locRS->data[n + locRS->size[0] * (locRS->size[1] - 1)] - locRS->
                data[n]) + 1.0;
            if (1.0 > d) {
                f_loop_ub = 0;
            }
            else {
                f_loop_ub = (int)d;
            }

            d = locRS->data[n];
            if (d > locRS->data[n + locRS->size[0] * (locRS->size[1] - 1)]) {
                i5 = 1;
            }
            else {
                i5 = (int)d;
            }

            d = locOFDMWithRS->data[n];
            k = z->size[0] * z->size[1];
            z->size[0] = 1;
            z->size[1] = f_loop_ub;
            EnsureCapacity_creal(z, k);
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
                }
                else if (W_im == 0.0) {
                    r = 0.0;
                    xtmp_im /= 6.0;
                }
                else {
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
                }
                else if (W_im == 0.0) {
                    r = 0.0;
                    xtmp_im /= 6.0;
                }
                else {
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

    Free_creal(&b_yy);
    Free_real(&d_y);
    Free_creal(&Mn);
    Free_real(&a);
    Free_creal(&z);
    Free_creal(&y);
    Free_creal(&yy);
    Free_creal(&ww);
    Free_creal(&aa);
    Free_creal(&gEst);

}
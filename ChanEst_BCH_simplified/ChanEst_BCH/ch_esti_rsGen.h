#pragma once
#include "typedefs.h"
#include "datadefs.h"
#include "Array_Ctrl.h"
#include "ch_esti_prseqGen.h"
#include <math.h>


/*******************************************************************************
*���ܣ�
*������
*����ֵ����
*˵������
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
    ����          ����          ������
    2021.02.20   V1.0          �����
********************************************************************************/
extern void ch_esti_rsGen(ARRAY_int32* locRS, ARRAY_complex* valRS, int idxSlot, int idxSym, int NID, int idxAntPort,
    int numRBDL, int numSymDL, int NCP);

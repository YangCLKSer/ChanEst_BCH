#pragma once
#include "typedefs.h"
#include "datadefs.h"
#include "Array_Ctrl.h"
#include "ch_esti_prseqGen.h"
#include <math.h>


/*******************************************************************************
*���ܣ��ο��ź����ɣ���Ҫ��ΪPR���в�����ο��ź�λ������
*������
* locRS��int����0ά����1��1ά����ά2*numRBDL�����ɵĲο��ź�λ��
* valRS����������0ά����1��1ά����ά2*numRBDL�����ɵĲο��źŵ�ֵ
* idxSlot��int��index of slot, 0, 1, ..., 19
* idSym��int��index of symbol, 0, 1, 2, 3, 4, 5, 6
* NID��int��С����
* idxAntPort��int�����߶˿����
* numRBDL��int��������Դ������
* numSymDL��int
* NCP��int
*����ֵ����
*˵������
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.1
*������ʷ
*	����          ������		����
*	2021.02.20   �����		V1.0
*	2021.03.02   �����		V1.1���궨�帡��������
********************************************************************************/
extern void ch_esti_rsGen(ARRAY_int32* locRS, ARRAY_complex* valRS, int idxSlot, int idxSym, int NID, int idxAntPort,
    int numRBDL, int numSymDL, int NCP);

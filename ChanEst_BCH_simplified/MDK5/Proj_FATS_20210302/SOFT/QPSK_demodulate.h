#pragma once

/* Include Files */
#include "typedefs.h"
#include <math.h>



/*******************************************************************************
*���ܣ�QPSK���
*������
* outBit�������bit��Ϣ
* inSymbols:���������շ���
* inPower��FLOAT_TYPE��������Ϣ
*����ֵ����
*˵������
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
    ����          ����          ������
    2021.02.20   V1.0          �����
********************************************************************************/
extern void QPSK_demodulate(FLOAT_TYPE outBits[480], const struct_complex inSymbols[240], const FLOAT_TYPE inPower[240]);
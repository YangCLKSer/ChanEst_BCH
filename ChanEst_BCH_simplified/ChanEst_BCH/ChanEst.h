#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"
#include <math.h>
#include "ch_esti.h"

/*******************************************************************************
*���ܣ�OFDM�ŵ�����
*������
* TempChan����������ά��0Ϊ�������������˴��̶�Ϊ1����ά��1Ϊ�����뷢��������֮����ά��2Ϊofdm������������ŵ�����
* rcvSigFreq����������ά��0Ϊ������������ά��1ΪnumRBDL*12*numOFDM����������
* ENB��С����Ϣ
* mmseA1��mmseA2��mmseB1��mmseB2��δʹ�ô˲���
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
extern void ChannelEst(ARRAY_complex* TempChan, ARRAY_complex* rcvSigFreq, FLOAT_TYPE mmseA1, \
    FLOAT_TYPE mmseB1, FLOAT_TYPE mmseA2, FLOAT_TYPE mmseB2, struct_ENB* ENB);

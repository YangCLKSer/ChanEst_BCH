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
*�汾�ţ�V1.0
*������ʷ
    ����          ����          ������
    2021.02.20   V1.0          �����
********************************************************************************/
extern void ChannelEst(ARRAY_complex* TempChan, ARRAY_complex* rcvSigFreq, double mmseA1, \
    double mmseB1, double mmseA2, double mmseB2, struct_ENB* ENB);

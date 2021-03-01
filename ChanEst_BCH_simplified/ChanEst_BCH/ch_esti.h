#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"
#include <math.h>
#include "ch_esti_rsGen.h"
#include "ch_esti_ls.h"
#include "ch_esti_dct.h"
#include "ch_esti_time_intp.h"


/*******************************************************************************
*���ܣ��Բο��źŽ����ŵ����ƣ������ʱƵ��ֵ����ŵ�����
*������
* hEst����������ά��0Ϊ�����뷢��������֮����ά��1Ϊofdm������������ŵ�����
* RxDataBCE����������ά��0Ϊ������������ά��1ΪnumRBDL*12*numOFDM����������
* enb��С����Ϣ
*����ֵ����
*˵����
* Pc��DCT��ֵ��ֹ����ϵ��
* NID��С����
* numTxAnt������������������Ϊ1
* numRBDL��������Դ����
* numSymDL������ofdm������
* NCP��ѭ��ǰ׺���ࡣ1-normal��0-extended
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.1
*������ʷ
*	����          ������		����
*	2021.02.20   �����		V1.0
*	2021.03.02   �����		V1.1���궨�帡��������
********************************************************************************/
extern void ch_esti(ARRAY_complex* hEst, ARRAY_complex* RxDataBCE, struct_ENB* enb);

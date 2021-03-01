#pragma once

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "typedefs.h"
#include "datadefs.h"
#include "Array_Ctrl.h"
#include "MIMO_detectRB.h"
#include "QPSK_demodulate.h"
#include "cc_decode.h"

/* Function Declarations */

/*******************************************************************************
*���ܣ�bch����
*������
* demod��FLOAT_TYPE��QPSK�������
* NID��int��С����
* NumTxAnt��int��������
* sbits��FLOAT_TYPE�������bit
* nfmod4��intָ�룬��֡��
* trblk��int��MIB��Ϣ
* sucess��intָ�룬����ɹ���־
*����ֵ����
*˵������
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
	����          ����          ������
	2021.02.20   V1.0          �����
********************************************************************************/
extern void bch_decode(FLOAT_TYPE sbits[480], int* nfmod4, 
	int trblk[24],int* sucess, FLOAT_TYPE demod[480], int NID, int c);

/*******************************************************************************
*���ܣ�PBCH����
*������
* sym�������������ź�
* chanEst��������PBCH�ŵ�����
* enb��С����Ϣ
* bits��FLOAT_TYPE�������bit
* symbols���������������
* nfmod4��intָ�룬��֡��
* trblk��int��MIB��Ϣ
* cellrefp��int��������
*����ֵ����
*˵������
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
	����          ����          ������
	2021.02.20   V1.0          �����
********************************************************************************/
extern void PBCH_Decode(FLOAT_TYPE bits[480], struct_complex symbols[240], 
	int* nfmod4, int trblk[24], int* cellrefp, struct_complex sym[240], 
	struct_complex chanEst[960], struct_ENB* enb);

/*******************************************************************************
*���ܣ�1920���س�Pseudo-Random���в���
*������
* PRsqnC��int��PR����
* Cinit��int����ʼ������
*����ֵ����
*˵������
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
	����          ����          ������
	2021.02.20   V1.0          �����
********************************************************************************/
extern void Sync_PRsqn_genr(int PRsqnC[1920], int Cinit);


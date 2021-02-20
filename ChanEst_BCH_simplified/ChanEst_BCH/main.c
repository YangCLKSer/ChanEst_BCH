#pragma once
#include "main.h"
//#program warning(disable:4996)

#define CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
void main()
{
	//�ڴ�����ϵ㣬���ͷ�ļ�crtdbg.h��ĩβ_CrtDumpMemoryLeaks()����ʹ��;
	//_CrtSetBreakAlloc(78);

	/*-----------------��������--------------------*/
	struct_ENB ENB;
	int i, j, loop_ub;
	int a, b;
	ARRAY_complex* chanEst, * rxSigFreq;
	ARRAY_int32* bch_ind;
	double bch_bits[480];
	struct_complex pbch_symbols[240];
	int sfmod4;
	int MIB[24];
	struct_complex bchRx[240];
	struct_complex bchHest[960];
	
	/*-------------------ENB���ݳ�ʼ��------------------*/
	ENB.NDLRB = 6;
	strcpy(ENB.DuplexMode, "TDD");
	strcpy(ENB.CyclicPrefix, "Normal");
	ENB.NCellID = 85;
	ENB.NSubframe = 0;
	ENB.TDDConfig = 0;
	ENB.SSC = 0;
	ENB.CellRefP = 4;
	strcpy(ENB.PHICHDuration, "");
	strcpy(ENB.Ng, "");
	ENB.NFrame = 0;
	ENB.CFI = 0;

	/*-------------------�����źų�ʼ��------------------*/
	Init_complex(&rxSigFreq, 2);
	i = rxSigFreq->size[0] * rxSigFreq->size[1];
	rxSigFreq->size[0] = 72;
	rxSigFreq->size[1] = 14;
	loop_ub = rxSigFreq->size[0]* rxSigFreq->size[1];
	EnsureCapacity_complex(rxSigFreq, i);
	for (i = 0; i < loop_ub; i++) {
		rxSigFreq->data[i].re = 0.0;
		rxSigFreq->data[i].im = 0.0;
	}
	

	
	//���Ｔ��ʹ��ǰ��rxSigFreq���и�ֵ���滻�ļ���ȡ

	FILE* fp;
	if ((fp = fopen("C:\\1D\\1SEU\\2020.09_USRP_E310\\C\\C_proj\\ChanEst_BCH_simplified\\ChanEst_BCH\\data\\rxSigFreq.txt", "r")) == NULL)
	{
		//printf("�ļ���ʧ��\n");
		exit(0);
	}
	else
	{
		//printf("�ļ��򿪳ɹ�\n");

		for (i = 0; i < 72 * 2; i++)
		{
			for (j = 0; j < 14; j++)
			{
				if (i < 72)
				{
					fscanf(fp, "%lf\n", &rxSigFreq->data[rxSigFreq->size[1] * i + j].re);
					//printf("%.6lf,\n", rxSigFreq->data[rxSigFreq->size[1] * i + j].re);
				}
				else
				{
					fscanf(fp, "%lf\n", &rxSigFreq->data[rxSigFreq->size[1] * (i - 72) + j].im);
					//printf("%.6lf,\n", rxSigFreq->data[rxSigFreq->size[1] * (i - 72) + j].im);
				}
			}
		}
	}
	fclose(fp);

	/*------------------�ŵ�����-------------------*/
	Init_complex(&chanEst, 3);
	ChannelEst(chanEst, rxSigFreq, 0, 0, 0, 0, &ENB);
	printf("chanEst\n");
	Print_complex(chanEst);

	/*-----------------PBCH���--------------------*/
	Init_int32(&bch_ind, 2);
	BCHIndices(bch_ind, &ENB);
	printf("bch_ind\n");
	Print_int32(bch_ind);

	//ȡBCH�����ϵ��ŵ����ƣ�bchHest = chanEst(:,:,bch_ind);
	for (i = 0; i < chanEst->size[1]; i++) {
		for (j = 0; j < 240; j++)
		{
			bchHest[240 * i + j] = \
				chanEst->data[chanEst->size[2] * i + bch_ind->data[j] - 1];
		}
	}
	

	//ȡBCH���ţ�bchRx = rxSigFreq(bch_ind);
	for (i = 0; i < bch_ind->size[1]; i++)
	{
		//rxSigFreq�������У���ͷΪ0
		a = ((bch_ind->data[i] - 1) / rxSigFreq->size[0]);
		b = ((bch_ind->data[i] - 1) % rxSigFreq->size[0]);
		bchRx[i] = rxSigFreq->data[b * rxSigFreq->size[1] + a];
	}
	
	//�ͷ���ǰ�ڴ棬�˲����԰�������ǰ�Խ�ʡ�ڴ�
	Free_complex(&rxSigFreq);
	Free_complex(&chanEst);
	Free_int32(&bch_ind);

	PBCH_Decode(bch_bits, pbch_symbols, &sfmod4, MIB, &(ENB.CellRefP), bchRx, bchHest, &ENB);

	//�ڴ�й¶���
	_CrtDumpMemoryLeaks();
	return 0;
}

#pragma once
#include "main.h"
//#program warning(disable:4996)

void main()
{
	//struct_complex rxSigFreq[72][14];
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

	Init_complex(&chanEst, 3);
	Init_complex(&rxSigFreq, 2);
	i = rxSigFreq->size[0] * rxSigFreq->size[1];
	rxSigFreq->size[0] = 72;
	rxSigFreq->size[1] = 14;
	loop_ub = rxSigFreq->size[0];
	EnsureCapacity_complex(rxSigFreq, i);
	for (i = 0; i < loop_ub; i++) {
		rxSigFreq->data[i].re = 0.0;
		rxSigFreq->data[i].im = 0.0;
	}

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

	FILE* fp;
	if ((fp = fopen("C:\\1D\\1SEU\\2020.09_USRP_E310\\C\\C_proj\\ChanEst_BCH_simplified\\simplified_ChanEst_BCH\\data\\rxSigFreq.txt", "r")) == NULL)
	{
		//printf("文件打开失败\n");
		exit(0);
	}
	else
	{
		//printf("文件打开成功\n");

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

	Init_complex(&chanEst, 3);
	ChannelEst(chanEst, rxSigFreq, 0, 0, 0, 0, &ENB);
	//printf("chanEst\n");
	//Print_complex(chanEst);

	Init_int32(&bch_ind, 2);
	BCHIndices(bch_ind, &ENB);
	//printf("bch_ind\n");
	//Print_int32(bch_ind);

	/*Init_complex(&bchHest, 3);
	i = bchHest->size[0] * bchHest->size[1] * bchHest->size[2];
	bchHest->size[0] = 1;
	bchHest->size[1] = chanEst->size[1];
	bchHest->size[2] = bch_ind->size[1];
	loop_ub = bchHest->size[2];
	EnsureCapacity_complex(bchHest, i);
	for (i = 0; i < bchHest->size[1]; i++) {
		for (j = 0; j < bchHest->size[2]; j++)
		{
			bchHest->data[loop_ub * i + j] = \
				chanEst->data[chanEst->size[2] * i + bch_ind->data[j] - 1];
		}
	}
	printf("bchHest\n");
	Print_complex(bchHest);*/
	for (i = 0; i < chanEst->size[1]; i++) {
		for (j = 0; j < 240; j++)
		{
			bchHest[240 * i + j] = \
				chanEst->data[chanEst->size[2] * i + bch_ind->data[j] - 1];
		}
	}
	

	
	/*Init_complex(&bchRx, 2);
	i = bchRx->size[0] * bchRx->size[1];
	bchRx->size[0] = 1;
	bchRx->size[1] = bch_ind->size[1];
	loop_ub = bchRx->size[1];
	EnsureCapacity_complex(bchRx, i);
	for (i = 0; i < loop_ub; i++)
	{
		//rxSigFreq的列与行，开头为0
		a = ((bch_ind->data[i] - 1) / rxSigFreq->size[0]);
		b = ((bch_ind->data[i] - 1) % rxSigFreq->size[0]);
		bchRx->data[i] = rxSigFreq->data[b * rxSigFreq->size[1] + a];
	}
	printf("bchRx\n");
	Print_complex(bchRx);*/

	for (i = 0; i < bch_ind->size[1]; i++)
	{
		//rxSigFreq的列与行，开头为0
		a = ((bch_ind->data[i] - 1) / rxSigFreq->size[0]);
		b = ((bch_ind->data[i] - 1) % rxSigFreq->size[0]);
		bchRx[i] = rxSigFreq->data[b * rxSigFreq->size[1] + a];
	}
	
	Free_complex(&rxSigFreq);
	Free_complex(&chanEst);
	Free_int32(&bch_ind);


	/*double bch_bits[480];
	struct_complex pbch_symbols[240];
	int sfmod4;
	int MIB[24];
	struct_complex bchRx[240];
	struct_complex bchHest[960];*/

	//PBCH_Decode(bch_bits, pbch_symbols, MIB, &(ENB.CellRefP), bchRx,bchHest, &ENB);
	PBCH_Decode(bch_bits, pbch_symbols, &sfmod4, MIB, &(ENB.CellRefP), bchRx, bchHest, &ENB);

	return 0;
}

#include "ChanEst.h"
#include <stdio.h>
#include <stdlib.h>
//#program warning(disable:4996)

void main()
{
	//struct_creal rxSigFreq[72][14];
	struct_ENB ENB;
	int i, j, loop_ub;
	ARRAY_creal* chanEst, * rxSigFreq;
	Init_creal(&chanEst, 3);
	Init_creal(&rxSigFreq, 2);
	i = rxSigFreq->size[0] * rxSigFreq->size[1];
	rxSigFreq->size[0] = 72;
	rxSigFreq->size[1] = 14;
	loop_ub = rxSigFreq->size[0];
	EnsureCapacity_creal(rxSigFreq, i);
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
		printf("文件打开失败\n");
		exit(0);
	}
	else
	{
		printf("文件打开成功\n");

		for (i = 0; i < 72 * 2; i++)
		{
			for (j = 0; j < 14; j++)
			{
				if (i < 72)
				{
					fscanf(fp, "%lf\n", &rxSigFreq->data[rxSigFreq->size[1] * i + j].re);
					printf("%.6lf,\n", rxSigFreq->data[rxSigFreq->size[1] * i + j].re);
					//fscanf(fp, "%lf\n", &rxSigFreq[i][j].re);
					//printf("%.6lf,\n", rxSigFreq[i][j].re);
				}
				else
				{
					fscanf(fp, "%lf\n", &rxSigFreq->data[rxSigFreq->size[1] * (i - 72) + j].im);
					printf("%.6lf,\n", rxSigFreq->data[rxSigFreq->size[1] * (i - 72) + j].im);
					//fscanf(fp, "%lf\n", &rxSigFreq[i-72][j].im);
					//printf("%.6lf,\n", rxSigFreq[i-72][j].im);
				}
			}
		}
	}
	fclose(fp);

	ChannelEst(chanEst, rxSigFreq, 0, 0, 0, 0, &ENB);
	

	Free_creal(&chanEst);

	return 0;
}
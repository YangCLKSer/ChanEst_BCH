#pragma once
#include "ch_esti.h"

void ch_esti(ARRAY_complex* hEst, ARRAY_complex* RxDataBCE, struct_ENB* enb)
{
	int NID, startSlot, numTxAnt, NCP, numSymDL,numRBDL;
	int numRxAnt, Len,numOFDM,i,j,loop_ub, numRS, idxAntPort;
	int i_ofdm, idxSlot, idxSym,row,n;
	double Pc;
	ARRAY_int32* locOFDMWithRS;
	ARRAY_int32* locRS, *tempLoc;
	ARRAY_complex* valRS, * tempVal, * RxData,*temphEst;

	NID = enb->NCellID;
	startSlot = 2 * enb->NSubframe;
	numTxAnt = enb->CellRefP;
	if (!strcmp(enb->CyclicPrefix, "Normal"))
	{
		NCP = 1;
		numSymDL = 7;
	}
	else
	{
		NCP = 0;
		numSymDL = 6;
	}
	numRBDL = enb->NDLRB;

	//init
	numRxAnt = RxDataBCE->size[0];
	Len = RxDataBCE->size[1];
	numOFDM = Len / numRBDL / 12;
	numRS = numRBDL * 2;

	i = hEst->size[0] * hEst->size[1];
	hEst->size[0] = numRxAnt * numTxAnt;
	loop_ub = (int)(numRxAnt*numTxAnt*Len);
	hEst->size[1] = Len;
	EnsureCapacity_complex(hEst, i);
	for (i = 0; i < loop_ub; i++) {
		hEst->data[i].re = 0;
		hEst->data[i].im = 0;
	}

	for (idxAntPort = 0; idxAntPort < numTxAnt; idxAntPort++)
	{
		//初始化locOFDMWithRS，locRS，valRS
		Init_int32(&locOFDMWithRS, 2);
		i = locOFDMWithRS->size[0] * locOFDMWithRS->size[1];
		locOFDMWithRS->size[0] = 1;
		loop_ub = (int)(0);
		locOFDMWithRS->size[1] = loop_ub;
		EnsureCapacity_int32(locOFDMWithRS, i);

		Init_int32(&locRS, 2);
		i = locRS->size[0] * locRS->size[1];
		locRS->size[0] = 0;
		loop_ub = (int)(0);
		locRS->size[1] = loop_ub;
		EnsureCapacity_int32(locRS, i);

		Init_complex(&valRS, 2);
		i = valRS->size[0] * valRS->size[1];
		valRS->size[0] = 0;
		loop_ub = (int)(0);
		valRS->size[1] = loop_ub;
		EnsureCapacity_complex(valRS, i);

		idxSlot = startSlot;

		for (i_ofdm = 0; i_ofdm < numOFDM; i_ofdm++)
		{
			if ((i_ofdm % numSymDL) == 0)
				if (i_ofdm)
					idxSlot++;

			idxSym = i_ofdm % numSymDL;
			Init_int32(&tempLoc, 2);
			Init_complex(&tempVal, 2);
			ch_esti_rsGen(tempLoc, tempVal, idxSlot, idxSym, NID, idxAntPort, numRBDL, numSymDL, NCP);
			if (tempLoc->size[1])
			{
				i = locOFDMWithRS->size[0] * locOFDMWithRS->size[1];
				loop_ub = (int)(locOFDMWithRS->size[1] + 1);
				locOFDMWithRS->size[1] = loop_ub;
				EnsureCapacity_int32(locOFDMWithRS, i);
				locOFDMWithRS->data[loop_ub - 1] = i_ofdm + 1;

				i = locRS->size[0] * locRS->size[1];
				locRS->size[0] = (int)(locRS->size[0] + 1);
				loop_ub = (int)(tempLoc->size[1]);
				locRS->size[1] = loop_ub;
				EnsureCapacity_int32(locRS, i);
				for (i = 0; i < loop_ub; i++)
					locRS->data[(locRS->size[0] - 1) * loop_ub + i] = tempLoc->data[i];

				i = valRS->size[0] * valRS->size[1];
				valRS->size[0] = (int)(valRS->size[0] + 1);
				loop_ub = (int)(tempLoc->size[1]);
				valRS->size[1] = loop_ub;
				EnsureCapacity_complex(valRS, i);
				for (i = 0; i < loop_ub; i++)
				{
					valRS->data[(valRS->size[0] - 1) * loop_ub + i].re = tempVal->data[i].re;
					valRS->data[(valRS->size[0] - 1) * loop_ub + i].im = tempVal->data[i].im;
				}
			}
			//初始化变量，以对应空集
			Free_int32(&tempLoc);
			Free_complex(&tempVal);
		}
				
		Pc = 0.1;

		//printf("locOFDMWithRS\n");
		//Print_int32(locOFDMWithRS);
		//printf("locRS\n");
		//Print_int32(locRS);
		//printf("valRS\n");
		//Print_complex(valRS);
		for (n = 0; n < numRxAnt; n++)
		{
			Init_complex(&RxData, 2);
			i = RxData->size[0] * RxData->size[1];
			loop_ub = (int)(numOFDM);
			RxData->size[0] = loop_ub;
			RxData->size[1] = numRBDL * 12;
			EnsureCapacity_complex(RxData, i);
			row = 12 * numRBDL * numOFDM;
			//RxDataBCE对每个接收天线有一个行向量，数据结构上reshape等同于赋值，因此仅做转置。
			for (i = 0; i < loop_ub; i++) {//14行
				for (j = 0; j < 12 * numRBDL; j++)//72列
				{
					RxData->data[i * 12 * numRBDL + j].re = \
						RxDataBCE->data[n * row + j * numOFDM + i].re;
					RxData->data[i * 12 * numRBDL + j].im = \
						RxDataBCE->data[n * row + j * numOFDM + i].im;
				}	
			}

			//printf("RxData\n");
			//Print_complex(RxData);

			Init_complex(&temphEst, 2);
			//LS估计
			ch_esti_ls(temphEst, RxData, locOFDMWithRS, locRS, valRS);
			//printf("temphEst ch_esti_ls\n");
			//Print_complex(temphEst);

			//dct插值
			ch_esti_dct(temphEst, locOFDMWithRS, locRS, Pc);
			//printf("temphEst ch_esti_dct\n");
			//Print_complex(temphEst);

			//时域插值
			ch_esti_time_intp(temphEst, locOFDMWithRS);
			//printf("temphEst ch_esti_time_intp\n");
			//Print_complex(temphEst);


			//reshape
			loop_ub = (int)(Len);
			for (i = 0; i < loop_ub; i++) {
				hEst->data[(n + idxAntPort * numRxAnt) * Len + i].re = \
					temphEst->data[i].re;
				hEst->data[(n + idxAntPort * numRxAnt) * Len + i].im = \
					temphEst->data[i].im;
			}
			//printf("hEst ch_esti\n");
			//Print_complex(hEst);
		}
		Free_int32(&locOFDMWithRS);
		Free_int32(&locRS);
		//Free_int32(&tempLoc);
		Free_complex(&valRS);
		//Free_complex(&tempVal);
		Free_complex(&RxData);
		Free_complex(&temphEst);
	}
	
}

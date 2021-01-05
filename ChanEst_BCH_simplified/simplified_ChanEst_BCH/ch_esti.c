#include "ch_esti.h"

void ch_esti(ARRAY_creal* hEst, ARRAY_creal* RxDataBCE, struct_ENB enb)
{
	int NID, startSlot, numTxAnt, NCP, numSymDL,numRBDL;
	int numRxAnt, Len,numOFDM,i,loop_ub, numRS, idxAntPort;
	int i_ofdm, idxSlot, idxSym,row,n;
	double Pc;
	ARRAY_int32* locOFDMWithRS;
	ARRAY_int32* locRS, *tempLoc;
	ARRAY_creal* valRS, * tempVal, * RxData,*temphEst;

	NID = enb.NCellID;
	startSlot = 2 * enb.NSubframe;
	numTxAnt = enb.CellRefP;
	if (!strcmp(enb.CyclicPrefix, "Normal"))
	{
		NCP = 1;
		numSymDL = 7;
	}
	else
	{
		NCP = 0;
		numSymDL = 6;
	}
	numRBDL = enb.NDLRB;

	//init
	numRxAnt = RxDataBCE->size[0];
	Len = RxDataBCE->size[1];
	numOFDM = Len / numRBDL / 12;
	numRS = numRBDL * 2;

	Init_creal(&hEst, 2);
	i = hEst->size[0] * hEst->size[1];
	hEst->size[0] = numRxAnt * numTxAnt;
	loop_ub = (int)(numRxAnt*numTxAnt*Len);
	hEst->size[1] = Len;
	EnsureCapacity_creal(hEst, i);
	for (i = 0; i < loop_ub; i++) {
		hEst->data[i].re = 0;
		hEst->data[i].im = 0;
	}

	for (idxAntPort = 0; idxAntPort < numTxAnt; idxAntPort++)
	{
		//³õÊ¼»¯locOFDMWithRS£¬locRS£¬valRS
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

		Init_creal(&valRS, 2);
		i = valRS->size[0] * valRS->size[1];
		valRS->size[0] = 0;
		loop_ub = (int)(0);
		valRS->size[1] = loop_ub;
		EnsureCapacity_creal(valRS, i);

		idxSlot = startSlot;

		for (i_ofdm = 0; i_ofdm < numSymDL; i++)
		{
			if (i_ofdm % numSymDL == 0)
				if (i_ofdm)
					idxSlot++;

			idxSym = i_ofdm % numSymDL;
			ch_esti_rsGen(tempLoc, tempVal, idxSlot, idxSym, NID, idxAntPort, numRBDL, numSymDL, NCP);
			if (tempLoc->size[1])
			{
				i = locOFDMWithRS->size[0] * locOFDMWithRS->size[1];
				loop_ub = (int)(locOFDMWithRS->size[1] + 1);
				locOFDMWithRS->size[1] = loop_ub;
				EnsureCapacity_int32(locOFDMWithRS, i);
				locOFDMWithRS->data[loop_ub - 1] = numSymDL;

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
				EnsureCapacity_int32(valRS, i);
				for (i = 0; i < loop_ub; i++)
				{
					valRS->data[(valRS->size[0] - 1) * loop_ub + i].re = tempVal->data[i].re; 
					valRS->data[(valRS->size[0] - 1) * loop_ub + i].im = tempVal->data[i].im;
				}
				
				Pc = 0.1;

				for (n = 0; n < numRxAnt; n++)
				{
					Init_creal(&RxData, 2);
					i = RxData->size[0] * RxData->size[1];
					RxData->size[0] = numRBDL * 12;
					loop_ub = (int)(numOFDM);
					RxData->size[1] = loop_ub;
					EnsureCapacity_creal(RxData, i);
					row = 12 * numRBDL * numOFDM;
					for (i = 0; i < loop_ub*12*numRBDL; i++) {
						RxData->data[i].re = RxDataBCE->data[n * row + i].re;
						RxData->data[i].im = RxDataBCE->data[n * row + i].im;
					}

					
					ch_esti_ls(temphEst, RxData, locOFDMWithRS, locRS, valRS);

					ch_esti_dct(temphEst, locOFDMWithRS, locRS, Pc);

					ch_esti_time_intp(temphEst, locOFDMWithRS);


					//reshape
					loop_ub = (int)(Len);
					for (i = 0; i < loop_ub; i++) {
						hEst->data[(n + idxAntPort * numRxAnt) * Len + i].re = \
							temphEst->data[i].re;
						hEst->data[(n + idxAntPort * numRxAnt) * Len + i].im = \
							temphEst->data[i].im;
					}

				}

			}

			Free_int32(tempLoc);
			Free_creal(tempVal);
		}



	}
}

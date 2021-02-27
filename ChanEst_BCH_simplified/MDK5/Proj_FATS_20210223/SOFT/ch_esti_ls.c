#pragma once
#include "ch_esti_ls.h"

void ch_esti_ls(ARRAY_complex* hEst,ARRAY_complex* RxDataBCE, ARRAY_int32* locOFDMWithRS,
	ARRAY_int32* locRS, ARRAY_complex* valRS)
{
	int numOFDM, numSym,loop_ub,i,n, row_start, col;
	numOFDM = RxDataBCE->size[0];
	numSym = RxDataBCE->size[1];

	
	i = hEst->size[0] * hEst->size[1];
	hEst->size[0] = numOFDM;
	loop_ub = (int)(numSym);
	hEst->size[1] = loop_ub;
	EnsureCapacity_complex(hEst, i);
	for (i = 0; i < loop_ub* hEst->size[0]; i++) {
		hEst->data[i].re = 0;
		hEst->data[i].im = 0;
	}
	
	Print_complex(valRS);
	Print_int32(locOFDMWithRS);
	for (n = 0; n < locOFDMWithRS->size[1]; n++)
	{
		row_start = (locOFDMWithRS->data[n]-1) * hEst->size[1];
		for (i = 0; i < locRS->size[1]; i++)
		{
			col = locRS->data[locRS->size[1] * n + i]-1;
			hEst->data[row_start + col] = complexDiv(RxDataBCE->data[row_start + col], valRS->data[n*valRS->size[1] + i]);
		}
	}
	Print_complex(hEst);
}

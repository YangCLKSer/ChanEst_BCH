#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"

void ch_esti_ls(ARRAY_creal* hEst,ARRAY_creal* RxDataBCE, ARRAY_int32* locOFDMWithRS,
	ARRAY_int32* locRS, ARRAY_creal* valRS)
{
	int numOFDM, numSym,loop_ub,i,n, row_start, col;
	numOFDM = RxDataBCE->size[0];
	numSym = RxDataBCE->size[1];

	Init_creal(&hEst, 2);
	i = hEst->size[0] * hEst->size[1];
	hEst->size[0] = numOFDM;
	loop_ub = (int)(numSym);
	hEst->size[1] = loop_ub;
	EnsureCapacity_creal(hEst, i);
	for (i = 0; i < loop_ub; i++) {
		hEst->data[i].re = 0;
		hEst->data[i].im = 0;
	}

	for (n = 0; n < locOFDMWithRS->size[1]; n++)
	{
		row_start = locOFDMWithRS->data[n] * hEst->size[1];
		col = locRS->data[locRS->size[1] * n + i];
		for (i = 0; i < locRS->size[1]; i++)
		{
			hEst->data[row_start + col] = \
				crealDiv(RxDataBCE->data[row_start + col], valRS->data[n*hEst->size[1] + col]);
		}
	}

}
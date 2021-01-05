#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"

void ch_esti_eidct(ARRAY_creal* hEst, ARRAY_creal* y, int m, int row);
void dct(ARRAY_creal* gEst, ARRAY_creal* temphEst);
void ch_esti_dct(ARRAY_creal* hEst, ARRAY_int32* locOFDMWithRS, ARRAY_int32* locRS, double Pc)
{
	int numOFDM, numSym, numPc, numRS,n,i,loop_ub, OFDMrow,RSrow;
	ARRAY_creal* temphEst, * gEst;
	numOFDM = hEst->size[0];
	numSym = hEst->size[1];
	numRS = locRS->size[1];

	//estimation on ofdms with rs
	// ----DCT Interpolation
	numPc = (int)(((double)numRS) * Pc / 2) * 2 + 2; //ceil
	for (n = 0; n < locOFDMWithRS->size[1]; n++)
	{
		Init_creal(&temphEst, 2);
		i = temphEst->size[0] * temphEst->size[1];
		temphEst->size[0] = 1;
		loop_ub = (int)(locRS->size[1]);
		temphEst->size[1] = loop_ub;
		EnsureCapacity_creal(temphEst, i);
		for (i = 0; i < loop_ub; i++) {
			temphEst->data[i].re = hEst->data[locOFDMWithRS->data[n] * loop_ub \
				+ locRS->data[n * loop_ub + i]].re;
			temphEst->data[i].im = hEst->data[locOFDMWithRS->data[n] * loop_ub \
				+ locRS->data[n * loop_ub + i]].im;
		}

		dct(gEst, temphEst);
		for (i = 0; i < numPc; i++)
		{
			gEst->data[i].re = gEst->data[i].re * sqrt((double)numSym / ((double)numRS));
			gEst->data[i].im = gEst->data[i].im * sqrt((double)numSym / ((double)numRS));
		}
		for (i = numPc; i < gEst->size[1]; i++)
		{
			gEst->data[i].re = 0;
			gEst->data[i].im = 0;
		}
			
		ch_esti_eidct(hEst, numRS, locOFDMWithRS->data[n]);
		
		OFDMrow = locOFDMWithRS->data[n] * hEst->size[1];
		RSrow = locRS->size[1] * n;
		if (locRS->data[n * locRS->size[1]] != 1)
		{
			loop_ub = locRS->data[RSrow + locRS->size[1] - 1]- \
				locRS->data[RSrow]+1;
			for (i = 0; i < loop_ub; i++)
			{
				hEst->data[OFDMrow + i + locRS->data[RSrow] - 1] = hEst->data[OFDMrow + i];
			}
			for (i = 0; i < locRS->data[RSrow]; i++)
			{
				//
				hEst->data[OFDMrow + locRS->data[RSrow + i]].re = \
					hEst->data[OFDMrow + locRS->data[RSrow]] + \
					(hEst->data[OFDMrow + locRS->data[RSrow]] - \
						hEst->data[OFDMrow + locRS->data[RSrow + 1]]) * \
					(locRS->data[RSrow] - i) / 6;
			}
		}
		else
		{
			for (i = 0; i < locRS->data[RSrow + locRS->size[1] - 1] + 1; i++)
			{
				//
				hEst->data[OFDMrow + locRS->data[RSrow + i]].re = \
					hEst->data[OFDMrow + locRS->data[RSrow + locRS->size[1] - 1]] + \
					(hEst->data[OFDMrow + locRS->data[RSrow + locRS->size[1] - 1]] - \
						hEst->data[OFDMrow + locRS->data[RSrow + locRS->size[1] - 2]]) * \
					(i - locRS->data[RSrow + locRS->size[1] - 1]) / 6;
			}
		}
	}
}

void ch_esti_eidct(ARRAY_creal* hEst, ARRAY_creal* y, int m,int row)
{
	int i, k,N,loop_ub;
	double factor;
	N = y->size[1];
	ARRAY_real* w;

	Init_real(&w, 2);
	i = w->size[0] * w->size[1];
	w->size[0] = 1;
	loop_ub = (int)(N);
	w->size[1] = loop_ub;
	EnsureCapacity_real(w, i);
	for (i = 0; i < loop_ub; i++) {
		w->data[i] = sqrt(2/((double)N));
	}
	w->data[0] = 1 / sqrt((double)N);

	for (i = 0; i < hEst->size[1]; i++)
	{
		hEst->data[row * hEst->size[1] + i].re = 0;
		hEst->data[row * hEst->size[1] + i].im = 0;
		for (k = 0; k < m; k++)
		{
			factor = w->data[k] * cos(RT_PI * (2 * i + (double)N / (double)m - 2) * ((double)k - 1) / (2 * (double)N));
			hEst->data[row * hEst->size[1] + k].re += y->data[k].re * factor;
			hEst->data[row * hEst->size[1] + k].im += y->data[k].im * factor;
		}
	}
}

void dct(ARRAY_creal* gEst, ARRAY_creal* temphEst)
{
	int n, m,i ,loop_ub;
	ARRAY_creal* ww;
	struct_creal factor;
	n = temphEst->size[0];
	m = temphEst->size[1];

	Init_creal(&ww, 2);
	i = ww->size[0] * ww->size[1];
	ww->size[0] = n;
	loop_ub = (int)(n);
	ww->size[1] = loop_ub;
	EnsureCapacity_creal(ww, i);
	for (i = 0; i < loop_ub; i++) {
		factor.re = 0;
		factor.im = 
		ww->data=crealExp()
	}
}
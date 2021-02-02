#include "ch_esti_dct.h"

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
			temphEst->data[i].re = hEst->data[(locOFDMWithRS->data[n]-1) * hEst->size[1] \
				+ locRS->data[n * loop_ub + i]-1].re;
			temphEst->data[i].im = hEst->data[(locOFDMWithRS->data[n]-1) * hEst->size[1] \
				+ locRS->data[n * loop_ub + i]-1].im;
		}
		//printf("temphEst in ch_esti_dct\n");
		//Print_creal(temphEst);


		Init_creal(&gEst, 2);
		dct(gEst, temphEst);
		//printf("gEst in ch_esti_dct\n");
		//Print_creal(gEst);
		i = gEst->size[0] * gEst->size[1];
		gEst->size[0] = 1;
		gEst->size[1] = numSym;
		EnsureCapacity_creal(gEst, i);
		for (i = 0; i < numPc; i++)
		{
			gEst->data[i].re = gEst->data[i].re * sqrt((double)numSym / ((double)numRS));
			gEst->data[i].im = gEst->data[i].im * sqrt((double)numSym / ((double)numRS));
		}
		for (i = numPc; i < numSym; i++)
		{
			gEst->data[i].re = 0;
			gEst->data[i].im = 0;
		}
		//printf("gEst in ch_esti_dct\n");
		//Print_creal(gEst);

		ch_esti_eidct(hEst, gEst, numRS, locOFDMWithRS->data[n]-1);
		//printf("hEst after ch_esti_eidct\n");
		//Print_creal(hEst);

		OFDMrow = (locOFDMWithRS->data[n]-1) * hEst->size[1];
		RSrow = locRS->size[1] * n;
		//n行第一个，其中n为0-n-1
		if (locRS->data[n * locRS->size[1]] != 1)
		{
			loop_ub = locRS->data[RSrow + locRS->size[1] - 1]- \
				locRS->data[RSrow]+1;
			//将前面的数据搬到后面，因此先对后面赋值
			for (i = loop_ub-1; i >= 0; i--)
			{
				hEst->data[OFDMrow + i + locRS->data[RSrow]-1] = hEst->data[OFDMrow + i];
			}
			//printf("hEst in ch_esti_dct\n");
			//Print_creal(hEst);
			for (i = 0; i < locRS->data[RSrow]-1; i++)
			{
				//
				hEst->data[OFDMrow + i].re = 
					hEst->data[OFDMrow + locRS->data[RSrow]-1].re + \
					(hEst->data[OFDMrow + locRS->data[RSrow]-1].re - \
						hEst->data[OFDMrow + locRS->data[RSrow + 1]-1].re) * \
					((double)(locRS->data[RSrow] - i-1)) / 6;
				hEst->data[OFDMrow + i].im =
					hEst->data[OFDMrow + locRS->data[RSrow]-1].im + \
					(hEst->data[OFDMrow + locRS->data[RSrow]-1].im - \
						hEst->data[OFDMrow + locRS->data[RSrow + 1]-1].im) * \
					((double)(locRS->data[RSrow] - i-1)) / 6;
			}
		}
		//n行最后一个
		if (locRS->data[(n+1) * locRS->size[1]-1] != numSym)
		{
			for (i = locRS->data[RSrow + locRS->size[1] - 1];i<numSym; i++)
			{
				//
				hEst->data[OFDMrow + i].re = \
					hEst->data[OFDMrow + locRS->data[RSrow + locRS->size[1] - 1]-1].re + \
					(hEst->data[OFDMrow + locRS->data[RSrow + locRS->size[1] - 1]-1].re - \
						hEst->data[OFDMrow + locRS->data[RSrow + locRS->size[1] - 2]-1].re) * \
					(i - locRS->data[RSrow + locRS->size[1] - 1]+1) / 6;
				hEst->data[OFDMrow + i].im = \
					hEst->data[OFDMrow + locRS->data[RSrow + locRS->size[1] - 1]-1].im + \
					(hEst->data[OFDMrow + locRS->data[RSrow + locRS->size[1] - 1]-1].im - \
						hEst->data[OFDMrow + locRS->data[RSrow + locRS->size[1] - 2]-1].im) * \
					(i - locRS->data[RSrow + locRS->size[1] - 1]+1) / 6;
			}
		}
		//printf("hEst in ch_esti_dct\n");
		//Print_creal(hEst);
		Free_creal(&temphEst);
		Free_creal(&gEst);
	}
	
}

//逆dct变换
void ch_esti_eidct(ARRAY_creal* hEst, ARRAY_creal* y, int m,int row)
{
	int i, k,N,loop_ub;
	double factor;
	N = y->size[1];
	/*ARRAY_real* w;

	Init_real(&w, 2);
	i = w->size[0] * w->size[1];
	w->size[0] = 1;
	loop_ub = (int)(N);
	w->size[1] = loop_ub;
	EnsureCapacity_real(w, i);
	for (i = 0; i < loop_ub; i++) {
		w->data[i] = sqrt(2/((double)N));
	}
	w->data[0] = 1 / sqrt((double)N);*/
	

	for (i = 0; i < hEst->size[1]; i++)
	{
		hEst->data[row * hEst->size[1] + i].re = 0;
		hEst->data[row * hEst->size[1] + i].im = 0;
		for (k = 0; k < m; k++)
		{
			if (k == 0)
			{
				factor = 1/sqrt((double)N) * \
					cos(RT_PI * (2 * i + (double)N / (double)m) * ((double)k)/(2 * (double)N));
			}
			else
			{
				factor = sqrt(2/(double)N) * \
					cos(RT_PI * (2 * i + (double)N / (double)m) * ((double)k) / (2 * (double)N));
			}
			 
			hEst->data[row * hEst->size[1] + i].re += y->data[k].re * factor;
			hEst->data[row * hEst->size[1] + i].im += y->data[k].im * factor;
		}
	}
}

//DCT-II变换，复数DCT变换相当于对实部复部分别变换，注意公式中n，k是1~m，这里是0~m-1
void dct(ARRAY_creal* gEst, ARRAY_creal* temphEst)
{
	int n, m,i ,loop_ub, k;
	struct_creal mid;
	double factor;
	n = temphEst->size[0];
	m = temphEst->size[1];

	i = gEst->size[0] * gEst->size[1];
	gEst->size[0] = n;
	loop_ub = m;
	gEst->size[1] = loop_ub;
	EnsureCapacity_creal(gEst, i);
	factor = RT_PI / 2 / ((double)m);
	for (k = 0; k < loop_ub; k++) 
	{
		mid.re = 0;
		mid.im = 0;
		for (i = 0; i < loop_ub; i++)
		{
			if (k == 0) 
			{
				mid.re += 1 / sqrt(2) * temphEst->data[i].re * cos(factor * (double)((2 * i + 1) * k));
				mid.im += 1 / sqrt(2) * temphEst->data[i].im * cos(factor * (double)((2 * i + 1) * k));
			}
			else
			{
				mid.re += temphEst->data[i].re * cos(factor * (double)((2 * i + 1) * k));
				mid.im += temphEst->data[i].im * cos(factor * (double)((2 * i + 1) * k));
			}
		}
		gEst->data[k].re = sqrt(2 / ((double)m)) * mid.re;
		gEst->data[k].im = sqrt(2 / ((double)m)) * mid.im;
	}
}
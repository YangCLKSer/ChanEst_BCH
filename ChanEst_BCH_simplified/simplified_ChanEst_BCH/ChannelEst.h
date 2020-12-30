#pragma once

#include <stddef.h>
#include <stdlib.h>
#include "typedefs.h"
#include <math.h>

extern void ChannelEst(const ARRAY_creal* rcvSigFreq, double mmseA1, double mmseB1,
	double mmseA2, double mmseB2, const struct_ENB* ENB, ARRAY_creal* TempChan);

extern void ch_esti(const ARRAY_creal* RxDataBCE, double enb_NDLRB, const
    char enb_CyclicPrefix[6], double enb_NCellID, double
    enb_NSubframe, double enb_CellRefP, ARRAY_creal* hEst);

/* Function Declarations */
static double rt_atan2d_snf(double u0, double u1);
static double rt_hypotd_snf(double u0, double u1);
static double rt_roundd_snf(double u);

extern void ch_esti_dct(ARRAY_creal* hEst, const ARRAY_real
    * locOFDMWithRS, const ARRAY_real* locRS);

extern void ch_esti_time_intp(ARRAY_creal* hEst, const ARRAY_real
    * locOFDMWithRS);

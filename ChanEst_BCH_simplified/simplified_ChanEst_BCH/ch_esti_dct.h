#pragma once
#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"

void ch_esti_eidct(ARRAY_complex* hEst, ARRAY_complex* y, int m, int row);
void dct(ARRAY_complex* gEst, ARRAY_complex* temphEst);
void ch_esti_dct(ARRAY_complex* hEst, ARRAY_int32* locOFDMWithRS, ARRAY_int32* locRS, double Pc);
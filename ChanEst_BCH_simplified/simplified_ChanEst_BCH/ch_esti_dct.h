#pragma once
#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"

void ch_esti_eidct(ARRAY_creal* hEst, ARRAY_creal* y, int m, int row);
void dct(ARRAY_creal* gEst, ARRAY_creal* temphEst);
void ch_esti_dct(ARRAY_creal* hEst, ARRAY_int32* locOFDMWithRS, ARRAY_int32* locRS, double Pc);
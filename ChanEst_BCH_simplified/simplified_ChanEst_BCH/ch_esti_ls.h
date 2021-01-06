#pragma once
#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"

void ch_esti_ls(ARRAY_creal* hEst, ARRAY_creal* RxDataBCE, ARRAY_int32* locOFDMWithRS,
	ARRAY_int32* locRS, ARRAY_creal* valRS);
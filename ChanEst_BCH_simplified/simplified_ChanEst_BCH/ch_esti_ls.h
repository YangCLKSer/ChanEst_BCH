#pragma once
#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"

void ch_esti_ls(ARRAY_complex* hEst, ARRAY_complex* RxDataBCE, ARRAY_int32* locOFDMWithRS,
	ARRAY_int32* locRS, ARRAY_complex* valRS);
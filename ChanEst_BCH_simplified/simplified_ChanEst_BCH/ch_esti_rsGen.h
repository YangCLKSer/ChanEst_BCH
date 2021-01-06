#pragma once
#include "typedefs.h"
#include "datadefs.h"
#include "Array_Ctrl.h"
#include "ch_esti_prseqGen.h"
#include <math.h>

extern void ch_esti_rsGen(ARRAY_int32* locRS, ARRAY_creal* valRS, int idxSlot, int idxSym, int NID, int idxAntPort,
    int numRBDL, int numSymDL, int NCP);
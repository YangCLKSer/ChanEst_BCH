#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"
#include <math.h>
#include "ch_esti.h"

extern void ChannelEst(ARRAY_complex* TempChan, ARRAY_complex* rcvSigFreq, double mmseA1, \
    double mmseB1, double mmseA2, double mmseB2, struct_ENB* ENB);

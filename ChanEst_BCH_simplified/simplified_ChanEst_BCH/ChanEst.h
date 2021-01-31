#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"
#include <math.h>
#include "ch_esti.h"

extern void ChannelEst(ARRAY_creal* TempChan, ARRAY_creal* rcvSigFreq, double mmseA1, \
    double mmseB1, double mmseA2, double mmseB2, struct_ENB* ENB);
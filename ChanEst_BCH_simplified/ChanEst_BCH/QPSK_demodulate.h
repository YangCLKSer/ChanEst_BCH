#pragma once

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"
#include <math.h>


/* Function Declarations */
extern void QPSK_demodulate(double outBits[480], const struct_complex inSymbols[240], const double inPower[240]);

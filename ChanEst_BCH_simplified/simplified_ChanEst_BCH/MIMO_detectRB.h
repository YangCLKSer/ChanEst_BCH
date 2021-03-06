#pragma once

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static void b_mld2sfbc(const double Received[120], const double mimoCH[240],
    double Detected[120], double ampd[120]);
static void mld2sfbc(const struct_complex Received[240], const struct_complex mimoCH[960],
    double Detected[240], double ampd[240]);
static void mld4sfbcfstd(const struct_complex Received[240], const struct_complex mimoCH[960],
    double Detected[240], double ampd[240]);
extern void MIMO_detectRB(struct_complex Detected_RB[240], double ampd[240], const struct_complex RxData[240], const struct_complex equCH[960], double
    methodDetect);

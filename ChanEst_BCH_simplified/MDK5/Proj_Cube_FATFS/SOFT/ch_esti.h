#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"
#include <math.h>
#include "ch_esti_rsGen.h"
#include "ch_esti_ls.h"
#include "ch_esti_dct.h"
#include "ch_esti_time_intp.h"

extern void ch_esti(ARRAY_complex* hEst, ARRAY_complex* RxDataBCE, struct_ENB* enb);

#pragma once

#include <stddef.h>
#include <stdlib.h>
#if ON_PC
#include <stdio.h>
#endif // ON_PC
#include <string.h>
#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"
#include <math.h>

extern void BCHIndices(ARRAY_int32* index_data, struct_ENB* enb);

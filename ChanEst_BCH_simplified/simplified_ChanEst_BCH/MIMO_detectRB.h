/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"

/* Function Declarations */
extern void MIMO_detectRB(const struct_creal RxData[240], const struct_creal equCH[960],
  double methodDetect, struct_creal Detected_RB[240], double ampd[240]);

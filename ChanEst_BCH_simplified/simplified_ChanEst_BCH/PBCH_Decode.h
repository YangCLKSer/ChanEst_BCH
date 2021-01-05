/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "typedefs.h"
#include "datadefs.h"

/* Function Declarations */
extern void PBCH_Decode(const struct_creal sym[240], const struct_creal chanEst[960],
  const struct_ENB *enb, struct_creal bits[480], struct_creal symbols[240], double *nfmod4,
  double trblk[24], double *cellrefp);

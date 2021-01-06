/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "typedefs.h"
#include "datadefs.h"
#include "Array_Ctrl.h"

/* Function Declarations */
extern void bch_decode(const double demod[480], double NID, struct_creal sbits[480],
	double* nfmod4);
extern void PBCH_Decode(const struct_creal sym[240], const struct_creal chanEst[960],
  const struct_ENB *enb, struct_creal bits[480], struct_creal symbols[240], double *nfmod4,
  double trblk[24], double *cellrefp);

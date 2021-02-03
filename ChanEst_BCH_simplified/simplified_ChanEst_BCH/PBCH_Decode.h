/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "typedefs.h"
#include "datadefs.h"
#include "Array_Ctrl.h"
#include "MIMO_detectRB.h"
#include "QPSK_demodulate.h"
#include "cc_decode.h"

/* Function Declarations */
extern void bch_decode(double sbits[480], int* nfmod4, 
	int trblk[24],int* sucess, double demod[480], int NID, int c);
extern void PBCH_Decode(double bits[480], struct_creal symbols[240], 
	int* nfmod4, int trblk[24], int* cellrefp, struct_creal sym[240], 
	struct_creal chanEst[960], struct_ENB* enb);


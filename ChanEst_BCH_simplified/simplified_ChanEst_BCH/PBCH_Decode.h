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
extern void PBCH_Decode(double bits[480], struct_complex symbols[240], 
	int* nfmod4, int trblk[24], int* cellrefp, struct_complex sym[240], 
	struct_complex chanEst[960], struct_ENB* enb);
extern void Sync_PRsqn_genr(int PRsqnC[1920], int Cinit);


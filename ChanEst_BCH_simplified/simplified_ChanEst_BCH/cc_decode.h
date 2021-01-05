#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

//#define BCH_OUT_LEN 1920

extern void sub_deinterleave(double *input_bit, double *output_bit, int input_length, int origin_length);
extern void deinterleave(double *decode_in_data, int info_len, int complete_block_num, int last_block_len, int decode_in_data_save_interval);
extern void ACS(int time_index, int butterfly_index, double *bm, int *sp, double *pm_old, double *pm_new, int *max_pm_flag, double *max_pm);
extern void calbm(int time_index, const double *decode_in_data, double *bm, int decode_in_data_save_interval);
extern void traceback(int *flag, int start_point, int stop_point, int tb_len, int info_len, char *table, char *decode_out, int output_en);
extern void wava_decode(double *decode_in_data, char *decode_out, int info_len);
extern int CRC_Check(char *seq_in, int seq_in_len, int crc_len, int crc_type);
extern int cc_decode(int *decode_out, double *decode_in, int msg_len, int decode_in_len,
	int channel_type, int trans_ant_num, int ue_ant_type, int as_type, int rnti_type);

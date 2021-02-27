#pragma once

#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"

#include <stdlib.h>
#include <string.h>
//#include <conio.h>

//#define BCH_OUT_LEN 1920

extern void sub_deinterleave(double* input_bit, double* output_bit, int input_length, int origin_length);
extern void deinterleave(double* decode_in_data, int info_len, int decode_in_data_len, int complete_block_num, int last_block_len, int decode_in_data_save_interval);
extern void ACS(int time_index, int butterfly_index, double* bm, int* sp, double* pm_old, double* pm_new, int* max_pm_flag, double* max_pm);
extern void calbm(int time_index, const double* decode_in_data, double* bm, int decode_in_data_save_interval);
extern void traceback(int* flag, int start_point, int stop_point, int tb_len, int info_len, int* table, int* decode_out, int output_en);
extern void wava_decode(double* decode_in_data, int* decode_out, int info_len, int decode_in_data_save_interval);
extern int CRC_Check(int* seq_in, int seq_in_len, int crc_len, int crc_type);
extern int cc_decode(double* d_decode_out, double* decode_in, double d_msg_len, double d_decode_in_len,
	double d_channel_type, double d_trans_ant_num, double d_ue_ant_type, double d_as_type, double d_rnti_type);

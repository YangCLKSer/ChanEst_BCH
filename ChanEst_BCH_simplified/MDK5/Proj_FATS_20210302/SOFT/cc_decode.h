#pragma once

#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"

#include <stdlib.h>
#include <string.h>
//#include <conio.h>

//#define BCH_OUT_LEN 1920

extern void sub_deinterleave(FLOAT_TYPE* input_bit, FLOAT_TYPE* output_bit, int input_length, int origin_length);
extern void deinterleave(FLOAT_TYPE* decode_in_data, int info_len, int decode_in_data_len, int complete_block_num, int last_block_len, int decode_in_data_save_interval);
extern void ACS(int time_index, int butterfly_index, FLOAT_TYPE* bm, int* sp, FLOAT_TYPE* pm_old, FLOAT_TYPE* pm_new, int* max_pm_flag, FLOAT_TYPE* max_pm);
extern void calbm(int time_index, const FLOAT_TYPE* decode_in_data, FLOAT_TYPE* bm, int decode_in_data_save_interval);
extern void traceback(int* flag, int start_point, int stop_point, int tb_len, int info_len, int* table, int* decode_out, int output_en);
extern void wava_decode(FLOAT_TYPE* decode_in_data, int* decode_out, int info_len, int decode_in_data_save_interval);
extern int CRC_Check(int* seq_in, int seq_in_len, int crc_len, int crc_type);
extern int cc_decode(FLOAT_TYPE* d_decode_out, FLOAT_TYPE* decode_in, FLOAT_TYPE d_msg_len, FLOAT_TYPE d_decode_in_len,
	FLOAT_TYPE d_channel_type, FLOAT_TYPE d_trans_ant_num, FLOAT_TYPE d_ue_ant_type, FLOAT_TYPE d_as_type, FLOAT_TYPE d_rnti_type);

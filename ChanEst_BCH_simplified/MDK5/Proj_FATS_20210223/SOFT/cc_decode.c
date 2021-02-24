#pragma once
/*
********************************************************************************
* Copyright (c) 2009, NCRL.
* All rights reserved.
* 
* ANSI C source code
* 
* MODULE NAME ：cc_decode.c
*	
* ABSTRACT： cc deocde
* 
* VERSION：	V1.0
* AUTHOR: 	IceSword 2009-4-17	
* 
* REVISION HISTORY:
* 		V0.2	IceSword	2009-4-3	Created
* 		V1.0	IceSword	2009-4-17	Revised
********************************************************************************
*/

/*
********************************************************************************
*			INCLUDE FILES
********************************************************************************
*/
#include "cc_decode.h"

//#define BCH_OUT_LEN 1920

//void sub_deinterleave(double *input_bit, double *output_bit, int input_length, int origin_length);
//void deinterleave(double *decode_in_data, int info_len, int complete_block_num, int last_block_len, int decode_in_data_save_interval);
//void ACS(int time_index, int butterfly_index, double *bm, int *sp, double *pm_old, double *pm_new, int *max_pm_flag, double *max_pm);
//void calbm(int time_index, const double *decode_in_data, double *bm, int decode_in_data_save_interval);
//void traceback(int *flag, int start_point, int stop_point, int tb_len, int info_len, char *table, char *decode_out, int output_en);
//void wava_decode(double *decode_in_data, char *decode_out, int info_len);
//int CRC_Check(char *seq_in, int seq_in_len, int crc_len, int crc_type);
//int cc_decode(int *decode_out, double *decode_in, int msg_len, int decode_in_len, 
//int channel_type, int trans_ant_num, int ue_ant_type, int as_type, int rnti_type);

/*
********************************************************************************
* NAME:		sub_deinterleave
* PURPOSE:	子块解交织主函数
* AUTHOR: 	Icesword 2009-4-5	
********************************************************************************
*/
void sub_deinterleave(double *input_bit, double *output_bit, int input_length, int origin_length)
{
	int C = 32;				//row number of interleave matrix
	int R;						//column number of interleave matrix
	int F0;						//number of null bits added
	int get_addr;			//get address
	int origin_addr;		//origin address
	int r_index = 0;		//row index
	int c_index = 0;		//column index
	//inter-column permutation pattern
	const int P[32] = {1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31,0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30};

	//compute row number & null bits number
	R = (int)(origin_length/C);
	if (origin_length % C != 0)
	{
		R = R + 1;
	}
	F0 = R*C - origin_length;

	get_addr=0; 
	while(get_addr<input_length)
	{
		origin_addr = C*r_index + P[c_index];
		//decide if it is a null bit
		if (origin_addr >= F0)
		{
			output_bit[origin_addr-F0] = input_bit[get_addr];
			get_addr++;
		}

		if (r_index < R-1)
		{
			r_index++;
		}
		else
		{
			r_index = 0;
			c_index++;
		}
	}

}

/*
********************************************************************************
* NAME:		deinterleave
* PURPOSE:	解交织，用于解cc速率匹配中的子块交织
* AUTHOR: 	Icesword 2009-4-5	
********************************************************************************
*/
void deinterleave(double *decode_in_data, int info_len, int decode_in_data_len, 
				  int complete_block_num, int last_block_len, int decode_in_data_save_interval)
{
	int i,j;
	double *de;
	double *ch;
	double *temp;

	if (NULL == (de = (double *)malloc(sizeof(double)*decode_in_data_len)))
	{
		printf("can't malloc memory, quit \n");
		//_getch();
		//exit(0);
	}
	if (NULL == (ch = (double *)malloc(sizeof(double)*info_len)))
	{
		printf("can't malloc memory, quit \n");
		//_getch();
		//exit(0);
	}
	if (NULL == (temp = (double *)malloc(sizeof(double)*info_len)))
	{
		printf("can't malloc memory, quit \n");
		//_getch();
		//exit(0);
	}

	//deinterleave each block
	for (i=0; i<complete_block_num; i++)
	{
		for (j=0; j<info_len; j++)
		{
			ch[j] = decode_in_data[info_len*i+j];
		}
		memset(temp, 0, sizeof(double)*info_len);
		sub_deinterleave(ch, temp, info_len, info_len);
		for (j=0; j<info_len; j++)
		{
			de[i+decode_in_data_save_interval*j] = temp[j];
		}
	}

	if (3 != complete_block_num && last_block_len != 0)
	{
		for (j=0; j<last_block_len; j++)
		{
			ch[j] = decode_in_data[info_len*complete_block_num+j];
		}
		memset(temp, 0, sizeof(double)*info_len);
		sub_deinterleave(ch, temp, last_block_len, info_len);
		for (j=0; j<info_len; j++)
		{
			de[complete_block_num+decode_in_data_save_interval*j] = temp[j];
		}
	}

	memcpy(decode_in_data, de, sizeof(double)*decode_in_data_len);

	free(ch);
	free(de);
	free(temp);
}

/*
********************************************************************************
* NAME:		ACS
* PURPOSE:	cc译码采用的蝶形加比选单元
* AUTHOR: 	Icesword 2009-4-5	
********************************************************************************
*/
void ACS(int time_index, int butterfly_index, double *bm, int *sp, double *pm_old, double *pm_new, int *max_pm_flag, double *max_pm)
{
	const int trellis[32] = {0, 3, 7, 4, 6, 5, 1, 2, 1, 2, 6, 5, 7, 4, 0, 3, 4, 7, 3, 0, 2, 1, 5, 6, 5, 6, 2, 1, 3, 0, 4, 7};//record trellis
	int tar_s_l = 2*butterfly_index;					//target low state in one butterfly
	int tar_s_h = 2*butterfly_index+1;			//target high state in one butterfly
	int src_s_l = butterfly_index;						//source low state in one butterfly
	int src_s_h = butterfly_index+32;				//source high state in one butterfly
	double pm_l_l = pm_old[src_s_l] + bm[trellis[butterfly_index]];			//low-low path metric in one butterfly
	double pm_l_h = pm_old[src_s_l] - bm[trellis[butterfly_index]];			//low-high path metric in one butterfly
	double pm_h_l = pm_old[src_s_h] - bm[trellis[butterfly_index]];			//high-low path metric in one butterfly
	double pm_h_h = pm_old[src_s_h] + bm[trellis[butterfly_index]];		//high-high path metric in one butterfly

	/*for tar_s_l*/
	if (pm_h_l > pm_l_l)
	{
		sp[time_index*64+tar_s_l] = 1;
		pm_new[tar_s_l] = pm_h_l;
	}
	else
	{
		sp[time_index*64+tar_s_l] = 0;
		pm_new[tar_s_l] = pm_l_l;
	}

	/*for tar_s_h*/
	if (pm_h_h > pm_l_h)
	{
		sp[time_index*64+tar_s_h] = 1;
		pm_new[tar_s_h] = pm_h_h;
	}
	else
	{
		sp[time_index*64+tar_s_h] = 0;
		pm_new[tar_s_h] = pm_l_h;
	}

	//record maximum pm
	if (pm_new[tar_s_h] > *max_pm)
	{
		*max_pm = pm_new[tar_s_h];
		*max_pm_flag = tar_s_h;
	}
	if (pm_new[tar_s_l] > *max_pm)
	{
		*max_pm = pm_new[tar_s_l];
		*max_pm_flag = tar_s_l;
	}

}

/*
********************************************************************************
* NAME:		cal_bm
* PURPOSE:	计算分支度量
* AUTHOR: 	Icesword 2009-4-5	
********************************************************************************
*/
void calbm(int time_index, const double *decode_in_data, double *bm, int decode_in_data_save_interval)
{
	double parity[3];

	if (decode_in_data_save_interval == 3)
	{
		parity[0] = decode_in_data[3*time_index];
		parity[1] = decode_in_data[3*time_index+1];
		parity[2] = decode_in_data[3*time_index+2];
	}
	else
	{
		parity[0] = decode_in_data[2*time_index];
		parity[1] = decode_in_data[2*time_index+1];
		parity[2] = 0;
	}

	bm[0] =  parity[0]   +   parity[1]    +    parity[2];
	bm[1] =  parity[0]   +   parity[1]     -    parity[2];
	bm[2] =  parity[0]   -    parity[1]    +    parity[2];
	bm[3] =  parity[0]   -    parity[1]     -    parity[2];
	bm[4] = -parity[0]  +   parity[1]    +    parity[2];
	bm[5] = -parity[0]  +   parity[1]     -    parity[2];
	bm[6] = -parity[0]   -   parity[1]    +    parity[2];
	bm[7] = -parity[0]   -   parity[1]     -     parity[2];

}

/*
********************************************************************************
* NAME:		traceback
* PURPOSE:	回溯单元
* AUTHOR: 	Icesword 2009-4-5	
********************************************************************************
*/
void traceback(int *flag, int start_point, int stop_point, int tb_len, int info_len, int *table, int *decode_out, int output_en)
{
	int i,j;
	int state;

	state = *flag;

	for (j=start_point; j>stop_point; j--)
	{
		i = j%tb_len;
		if (output_en)
		{
			decode_out[j%info_len] = state & 1;
		}
		state = (state>>1) + 32 * table[i*64+state];
	}
	*flag = state;

}

/*
********************************************************************************
* NAME:		wava_decode
* PURPOSE:	wava译码算法
* AUTHOR: 	Icesword 2009-4-5	
********************************************************************************
*/
void wava_decode(double *decode_in_data, int *decode_out, int info_len, int decode_in_data_save_interval)
{
	int sp[40*64] = {0};					//survive path ram
	double bm[8];							//branch metric
	double pm0[64] = {0};				//path metric ram 0
	double pm1[64] = {0};				//path metric ram 1
	double *pm_old = pm0;			//pointer to path metric of last time
	double *pm_new = pm1;		//pointer to path metric of this time
	double *temp;							//temp pointer
	int max_pm_flag;						//flag of maximum path metric
	double max_pm;						//maximum path metric
	int it = 0;									//iteration number
	int time_index;							//time index
	int butterfly_index;					//butterfly index
	int start_state;							//start state in one path
	int end_state;							//end state in one path
	int flag;										//current state
	int pm_sort[64];						//path metric sort, from biggest to smallest
	int search_tail_num = 63;		//number of searching tail-bitting path
	int temp_int;								//temp int value
	double temp_double;						//temp double value
	int i,j;

	while (it < 2)
	{
		for (i=0; i<64; i++)
		{
			pm_sort[i] = i;
		}
		for (time_index=0; time_index<info_len; time_index++)
		{
			max_pm_flag = -1;
			max_pm = -10000;
			calbm(time_index, decode_in_data, bm, decode_in_data_save_interval);
			for (butterfly_index=0; butterfly_index<32; butterfly_index++)
			{
				ACS(time_index, butterfly_index, bm, sp, pm_old, pm_new, &max_pm_flag, &max_pm);
			}
			temp = pm_old;
			pm_old = pm_new;
			pm_new = temp;

		}

		it++;
		end_state = max_pm_flag;
		start_state = max_pm_flag;
		traceback(&start_state, info_len-1, -1, info_len, info_len, sp, decode_out, 0);
		if (start_state == end_state)//start to output decoded data
		{
			flag = max_pm_flag;
			traceback(&flag, info_len-1, -1, info_len, info_len, sp, decode_out, 1);
			break;
		}
	}

	if (start_state != end_state && it == 2)
	{
		//bubble sort and record
		for (i=0; i<search_tail_num+1; i++)
		{
			for (j=63; j>i; j--)
			{
				if (pm_old[j] > pm_old[j-1])
				{
					temp_double = pm_old[j];
					pm_old[j] = pm_old[j-1];
					pm_old[j-1] = temp_double;

					temp_int = pm_sort[j];
					pm_sort[j] = pm_sort[j-1];
					pm_sort[j-1] = temp_int;
				}
			}
		}
		//search tail-bitting path
		for (i=0; i<search_tail_num; i++)
		{
			end_state = pm_sort[i+1];
			start_state = pm_sort[i+1];
			traceback(&start_state, info_len-1, -1, info_len, info_len, sp, decode_out, 0);
			if (start_state == end_state)
			{
				max_pm_flag = end_state;
				break;
			}
		}
		flag = max_pm_flag;
		traceback(&flag, info_len-1, -1, info_len, info_len, sp, decode_out, 1);
	}

}

/*
********************************************************************************
* NAME:		crc_check
* PURPOSE:	crc校验程序，可匹配4中不同长度crc，若正确输出0，错误输出1
* AUTHOR: 	Icesword 2009-4-5	
********************************************************************************
*/
int CRC_Check(int *seq_in, int seq_in_len, int crc_len, int crc_type)
{
	int i, j;
	int temp;
	int sum;
	int reg[24] = {0};
	int G[25] = {0};

	//-	gCRC24A(D) = [D24 + D23 + D18 + D17 + D14 + D11 + D10 + D7 + D6 + D5 + D4 + D3 + D + 1] and;
	//-	gCRC24B(D) = [D24 + D23 + D6 + D5 + D + 1] for a CRC length L = 24 and;
	//-	gCRC16(D) = [D16 + D12 + D5 + 1] for a CRC length L = 16.
	//	-	gCRC8(D) = [D8 + D7 + D4 + D3 + D + 1] for a CRC length of L = 8.
	switch(crc_len)
	{
	case 8: G[0]=G[1]=G[3]=G[4]=G[7]=G[8]=1; break;
	case 16:G[0]=G[5]=G[12]=G[16]=1; break;
	case 24: 
		if (!crc_type)
		{
			G[0]=G[1]=G[3]=G[4]=G[5]=G[6]=G[7]=G[10]=G[11]=G[14]=G[17]=G[18]=G[23]=G[24]=1;
		}
		else
		{
			G[0]=G[1]=G[5]=G[6]=G[23]=G[24]=1;
		}
		break;
	default: break;
	}


	if (crc_len != 0)
	{
		for(i = 0; i < seq_in_len ; i++)
		{
			temp = reg[crc_len-1];
			for(j = crc_len-1; j >0; j--)
			{
				reg[j] = reg[j-1] ^ (G[j] && temp);
			}
			reg[0] = temp ^ seq_in[i];
		}

		sum = 0;
		for(i = 0; i < crc_len; i++)
		{
			sum += reg[i];
		}

		if(sum != 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}

}


/*
********************************************************************************
* NAME:		cc_decode
* PURPOSE:	cc译码程序
*
* ENTRY:	
									decode_in:		译码数据输入，对BCH，为480长的double型数据, 即4段发送数据中的一段
									decode_in_len:	译码数据输入长度，对BCH为480
									channel_type:	若为BCH，设置为0， 若为DCI，设置为1
									trans_ant_num:	用于BCH，发送天线数，用于加扰、解扰选择
									ue_ant_type:		用于DCI
									as_type:				用于DCI
									rnti_type:			用于DCI
									decode_out:		译码结果输出，对BCH，为24长的int型数据
									msg_len:			译码结果输出长度，对BCH为24
*
* EXIT:		
									crc_result:			crc校验结果，1表示正确，0表示错误
*
* FUNCTION REFERENCE : no
* 
* VALUE AFFECTED:	no
*					
* AUTHOR: 	Icesword 2009-4-5	
* Revised:  Icesword 2009-4-5
*  
********************************************************************************
*/
int cc_decode(double *d_decode_out, double *decode_in, double d_msg_len, double d_decode_in_len, 
			   double d_channel_type, double d_trans_ant_num, double d_ue_ant_type, double d_as_type, double d_rnti_type)
{
	double *decode_in_data;
	int *decode_out_crc;
	int x_ant1[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//PBCH crc mask
	int x_ant2[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	int x_ant4[16] = {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
	int x_as0[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//DCI antenna selection mask
	int x_as1[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
	int x_rnti0[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0};//DCI RNTI
	int x_rnti1[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	int x_rnti2[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int crc_len = 16;											//crc length
	int info_len;													//information length
	int trans_block_num;									//number of block transmitted
	int decode_in_data_len;								//data length input into decoder
	int complete_block_num;							//number of complete block input into decoder
	int last_block_len;										//length of last block
	int decode_in_data_save_interval;			//decode in data save interval
	int crc_result;												//crc result , 1->crc right, 0->crc wrong
	int i;															//used for loop index

	//匹配matlab
	int *decode_out;
	int msg_len = (int)d_msg_len;
	int decode_in_len = (int)d_decode_in_len;
	int channel_type = (int)d_channel_type;
	int trans_ant_num = (int)d_trans_ant_num;
	int ue_ant_type = (int)d_ue_ant_type;
	int as_type = (int)d_as_type;
	int rnti_type = (int)d_rnti_type;
		   
			   
	info_len = crc_len+msg_len;																			
	trans_block_num = (decode_in_len%info_len==0) ? (int)(decode_in_len/info_len) : (int)(decode_in_len/info_len)+1;
	decode_in_data_len = (trans_block_num>2) ? 3*info_len : 2*info_len;		
	complete_block_num =((int)(decode_in_len/info_len) >= 3) ? 3 : (int)(decode_in_len/info_len);		
	last_block_len = (complete_block_num == 3) ? 0 : (decode_in_len - complete_block_num*info_len);
	decode_in_data_save_interval = (complete_block_num == 3) ? 3 : (complete_block_num+((last_block_len>0)? 1 : 0));

	if (NULL == (decode_in_data = (double *)malloc(sizeof(double)*decode_in_data_len)))
	{
		printf("can't malloc memory, quit \n");
		//_getch();
		//exit(0);
	}
	if (NULL == (decode_out_crc = (int *)malloc(sizeof(int)*info_len)))
	{
		printf("can't malloc memory, quit \n");
		//_getch();
		//exit(0);
	}
	//匹配matlab
	if (NULL == (decode_out = (int *)malloc(sizeof(int)*msg_len)))
	{
		printf("can't malloc memory, quit \n");
		//_getch();
		//exit(0);
	}

	//combine data that transmit repeatedly
	memset(decode_in_data, 0, sizeof(double)*decode_in_data_len);
	for (i=0; i<decode_in_len; i++)
	{
		decode_in_data[i%decode_in_data_len] += decode_in[i];
	}

	//deinterleave
	deinterleave(decode_in_data, info_len, decode_in_data_len, complete_block_num, last_block_len, decode_in_data_save_interval);

	//decode by wava
	wava_decode(decode_in_data, decode_out_crc, info_len, decode_in_data_save_interval);

	//scramble for bch
	if (!channel_type)
	{
		switch (trans_ant_num)
		{
		case 1:	for (i=0; i<16; i++)
				{
					decode_out_crc[msg_len+i] = decode_out_crc[msg_len+i] ^ x_ant1[i];
				}
				break;
		case 2:	for (i=0; i<16; i++)
				{
					decode_out_crc[msg_len+i] = decode_out_crc[msg_len+i] ^ x_ant2[i];
				}
				break;
		case 4:	for (i=0; i<16; i++)
				{
					decode_out_crc[msg_len+i] = decode_out_crc[msg_len+i] ^ x_ant4[i];
				}
				break;
		default: printf("error transmit antenna ports number. quit \n");
				//_getch();exit(0);
				break;
		}
	}
	else	//scramble for dci
	{
//unfinished
	}

	//crc check
	crc_result = !CRC_Check(decode_out_crc, info_len, crc_len, 0);
	memcpy(decode_out, decode_out_crc, sizeof(int)*msg_len);
	
	//匹配matlab
	for (i=0; i<msg_len; i++)
	{
		d_decode_out[i] = (double)decode_out[i];
	}
	
	free(decode_in_data);
	free(decode_out_crc);
	free(decode_out);

	return crc_result;
}
/*void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	int i;
    double *BCHin;
    double *BCHout;
    double *Sucess;
	double NumAnt;
    double DataLen;
    int BurstLength;
	double *Temp;
    double *TempAnt;

	if (nrhs != 3) 
		mexErrMsgTxt("Two inputs required.");
    if (nlhs != 2) 
		mexErrMsgTxt("Two output required.");

	**************************** receive data ********************************
	Temp = mxGetPr(prhs[0]);
	BurstLength = mxGetN(prhs[0]);

	if((BCHin=(double*)mxMalloc(BurstLength*sizeof(double))) == NULL)
	{
		printf("\nMemory allocation error of BCHin in cc_encode");
		exit(0);
	}
	for (i=0; i<BurstLength; i++)
	{
		BCHin[i] = Temp[i];
	}

	TempAnt = mxGetPr(prhs[1]);
	NumAnt = *TempAnt;
    TempAnt = mxGetPr(prhs[2]);
	DataLen = *TempAnt;


	***************************** output data *****************************
	plhs[0] = mxCreateDoubleMatrix(1,24, mxREAL);
	BCHout = mxGetPr(plhs[0]);
    plhs[1] = mxCreateDoubleMatrix(1,1, mxREAL);
	Sucess = mxGetPr(plhs[1]);
    *Sucess = cc_decode(BCHout, BCHin, 24, DataLen, 0,NumAnt,0,0,0 );

	**** free the sapce *****
	mxFree(BCHin);
}
*/


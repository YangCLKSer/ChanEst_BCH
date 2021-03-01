#pragma once

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "typedefs.h"
#include "datadefs.h"
#include "Array_Ctrl.h"
#include "MIMO_detectRB.h"
#include "QPSK_demodulate.h"
#include "cc_decode.h"

/* Function Declarations */

/*******************************************************************************
*功能：bch解码
*参数：
* demod：FLOAT_TYPE，QPSK解调符号
* NID：int，小区号
* NumTxAnt：int，天线数
* sbits：FLOAT_TYPE，解调软bit
* nfmod4：int指针，子帧号
* trblk：int，MIB信息
* sucess：int指针，解码成功标志
*返回值：无
*说明：无
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.0
*更改历史
	日期          内容          更改者
	2021.02.20   V1.0          杨昌林
********************************************************************************/
extern void bch_decode(FLOAT_TYPE sbits[480], int* nfmod4, 
	int trblk[24],int* sucess, FLOAT_TYPE demod[480], int NID, int c);

/*******************************************************************************
*功能：PBCH解码
*参数：
* sym：复数，接收信号
* chanEst：复数，PBCH信道估计
* enb：小区信息
* bits：FLOAT_TYPE，输出软bit
* symbols：复数，检出符号
* nfmod4：int指针，子帧号
* trblk：int，MIB信息
* cellrefp：int，天线数
*返回值：无
*说明：无
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.0
*更改历史
	日期          内容          更改者
	2021.02.20   V1.0          杨昌林
********************************************************************************/
extern void PBCH_Decode(FLOAT_TYPE bits[480], struct_complex symbols[240], 
	int* nfmod4, int trblk[24], int* cellrefp, struct_complex sym[240], 
	struct_complex chanEst[960], struct_ENB* enb);

/*******************************************************************************
*功能：1920比特长Pseudo-Random序列产生
*参数：
* PRsqnC：int，PR序列
* Cinit：int，初始化种子
*返回值：无
*说明：无
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.0
*更改历史
	日期          内容          更改者
	2021.02.20   V1.0          杨昌林
********************************************************************************/
extern void Sync_PRsqn_genr(int PRsqnC[1920], int Cinit);


#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"
#include <math.h>
#include "ch_esti_rsGen.h"
#include "ch_esti_ls.h"
#include "ch_esti_dct.h"
#include "ch_esti_time_intp.h"


/*******************************************************************************
*功能：对参考信号进行信道估计，并输出时频插值后的信道估计
*参数：
* hEst：复数矩阵，维度0为接收与发射天线数之积，维度1为ofdm符号数，输出信道估计
* RxDataBCE：复数矩阵，维度0为发射天线数，维度1为numRBDL*12*numOFDM，接收数据
* enb：小区信息
*返回值：无
*说明：
* Pc：DCT插值截止窗口系数
* NID：小区号
* numTxAnt：发射天线数，这里为1
* numRBDL：下行资源块数
* numSymDL：下行ofdm符号数
* NCP：循环前缀种类。1-normal，0-extended
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.1
*更改历史
*	日期          更改者		内容
*	2021.02.20   杨昌林		V1.0
*	2021.03.02   杨昌林		V1.1：宏定义浮点数类型
********************************************************************************/
extern void ch_esti(ARRAY_complex* hEst, ARRAY_complex* RxDataBCE, struct_ENB* enb);

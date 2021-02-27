#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"
#include <math.h>
#include "ch_esti.h"

/*******************************************************************************
*功能：OFDM信道估计
*参数：
* TempChan：复数矩阵，维度0为接收天线数（此处固定为1），维度1为接收与发射天线数之积，维度2为ofdm符号数，输出信道估计
* rcvSigFreq：复数矩阵，维度0为发射天线数，维度1为numRBDL*12*numOFDM，接收数据
* ENB：小区信息
* mmseA1，mmseA2，mmseB1，mmseB2：未使用此参数
*返回值：无
*说明：无
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.0
*更改历史
    日期          内容          更改者
    2021.02.20   V1.0          杨昌林
********************************************************************************/
extern void ChannelEst(ARRAY_complex* TempChan, ARRAY_complex* rcvSigFreq, double mmseA1, \
    double mmseB1, double mmseA2, double mmseB2, struct_ENB* ENB);

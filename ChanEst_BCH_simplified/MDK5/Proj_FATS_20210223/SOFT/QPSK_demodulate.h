#pragma once

/* Include Files */
#include "typedefs.h"
#include <math.h>



/*******************************************************************************
*功能：QPSK解调
*参数：
* outBit：解调软bit信息
* inSymbols:复数，接收符号
* inPower：double，幅度信息
*返回值：无
*说明：无
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.0
*更改历史
    日期          内容          更改者
    2021.02.20   V1.0          杨昌林
********************************************************************************/
extern void QPSK_demodulate(double outBits[480], const struct_complex inSymbols[240], const double inPower[240]);

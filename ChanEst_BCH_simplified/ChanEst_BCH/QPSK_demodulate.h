#pragma once

/* Include Files */
#include "typedefs.h"
#include <math.h>



/*******************************************************************************
*功能：QPSK解调
*参数：
* outBit：解调软bit信息
* inSymbols:复数，接收符号
* inPower：FLOAT_TYPE，幅度信息
*返回值：无
*说明：无
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.1
*更改历史
*	日期          更改者		内容
*	2021.02.20   杨昌林		V1.0
*	2021.03.02   杨昌林		V1.1：宏定义浮点数类型
********************************************************************************/
extern void QPSK_demodulate(FLOAT_TYPE outBits[480], const struct_complex inSymbols[240], const FLOAT_TYPE inPower[240]);

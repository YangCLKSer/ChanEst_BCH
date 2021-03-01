#pragma once
#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"
#include <math.h>


/*******************************************************************************
*功能：线性时域插值
*参数：
* hEst：复数矩阵，信道估计结果，对其进行时域插值。参考信号位置的数据作为输入。
* locOFDMWithRS：int矩阵，参考信号位置。
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
void ch_esti_time_intp(ARRAY_complex* hEst, const  ARRAY_int32 * locOFDMWithRS);

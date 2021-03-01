#pragma once

#include <stddef.h>
#include <stdlib.h>
#if ON_PC
#include <stdio.h>
#endif // ON_PC
#include <string.h>
#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"
#include <math.h>


/*******************************************************************************
*功能：BCH符号位置
*参数：
* enb：小区信息
* index_data：int矩阵，维度0为1，维度1为BCH符号数量，输出符号位置
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
extern void BCHIndices(ARRAY_int32* index_data, struct_ENB* enb);

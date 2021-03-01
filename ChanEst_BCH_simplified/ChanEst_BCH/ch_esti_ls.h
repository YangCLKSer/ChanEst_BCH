#pragma once
#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"

/*******************************************************************************
*功能：参考信号位置信道参数最小二乘估计
*参数：
* hEst：复数矩阵，信道估计
* RxDataBCE：复数矩阵，输入数据
* locOFDMWithRS：int矩阵，维度0为1，维度1为参考信号所占子载波数，指示RS所在子载波序号，从1开始
* locRS：int矩阵，维度0为参考信号所占子载波数，维度1为参考信号所占时域符号数，指示对应子载波上RS的时域符号位置，从1开始
* valRS：复数矩阵，结构同locRS，为参考信号的值
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
void ch_esti_ls(ARRAY_complex* hEst, ARRAY_complex* RxDataBCE, ARRAY_int32* locOFDMWithRS,
	ARRAY_int32* locRS, ARRAY_complex* valRS);

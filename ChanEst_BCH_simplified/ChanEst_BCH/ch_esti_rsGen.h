#pragma once
#include "typedefs.h"
#include "datadefs.h"
#include "Array_Ctrl.h"
#include "ch_esti_prseqGen.h"
#include <math.h>


/*******************************************************************************
*功能：参考信号生成，主要分为PR序列产生与参考信号位置生成
*参数：
* locRS：int矩阵，0维长度1，1维长度维2*numRBDL，生成的参考信号位置
* valRS：复数矩阵，0维长度1，1维长度维2*numRBDL，生成的参考信号的值
* idxSlot：int，index of slot, 0, 1, ..., 19
* idSym：int，index of symbol, 0, 1, 2, 3, 4, 5, 6
* NID：int，小区号
* idxAntPort：int，天线端口序号
* numRBDL：int，下行资源块数量
* numSymDL：int
* NCP：int
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
extern void ch_esti_rsGen(ARRAY_int32* locRS, ARRAY_complex* valRS, int idxSlot, int idxSym, int NID, int idxAntPort,
    int numRBDL, int numSymDL, int NCP);

#pragma once
#include "typedefs.h"
#include "datadefs.h"
#include "Array_Ctrl.h"
#include "ch_esti_prseqGen.h"
#include <math.h>


/*******************************************************************************
*功能：
*参数：
*返回值：无
*说明：无
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.0
*更改历史
    日期          内容          更改者
    2021.02.20   V1.0          杨昌林
********************************************************************************/
extern void ch_esti_rsGen(ARRAY_int32* locRS, ARRAY_complex* valRS, int idxSlot, int idxSym, int NID, int idxAntPort,
    int numRBDL, int numSymDL, int NCP);

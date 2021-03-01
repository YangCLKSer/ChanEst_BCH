#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "typedefs.h"
//#include "rtwtypes.h"
//#include "BCHIndices_types.h"

/* Function Declarations */


/*******************************************************************************
*功能：矩阵的数据内存申请
*参数：
* oldNumel：int，矩阵的原数据元素数量
* emxArray：ARRAY_**类型矩阵的指针
*返回值：无
*说明：**：complex-复数；real-FLOAT_TYPE；int32-int；uint32-unsigned int
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.0
*更改历史
    日期          内容          更改者
    2021.02.20   V1.0          杨昌林
********************************************************************************/
extern void EnsureCapacity_complex(ARRAY_complex* emxArray, int oldNumel);
extern void EnsureCapacity_int32(ARRAY_int32* emxArray, int oldNumel);
extern void EnsureCapacity_real(ARRAY_real* emxArray, int oldNumel);
extern void EnsureCapacity_uint32(ARRAY_uint32* emxArray, int oldNumel);

/*******************************************************************************
*功能：矩阵与其数据空间释放
*参数：
* pEmxArray：ARRAY_**类型矩阵的指针的指针，用于在完成数据释放后，对使用指针存储的矩阵变量本身的释放
*返回值：无
*说明：**：complex-复数；real-FLOAT_TYPE；int32-int；uint32-unsigned int
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.0
*更改历史
    日期          内容          更改者
    2021.02.20   V1.0          杨昌林
********************************************************************************/
extern void Free_complex(ARRAY_complex** pEmxArray);
extern void Free_int32(ARRAY_int32** pEmxArray);
extern void Free_real(ARRAY_real** pEmxArray);
extern void Free_uint32(ARRAY_uint32** pEmxArray);

/*******************************************************************************
*功能：矩阵参数的申请与初始化
*参数：
* pEmxArray：ARRAY_**类型矩阵的指针的指针，用于对使用指针存储的矩阵变量本身的申请
*返回值：无
*说明：**：complex-复数；real-FLOAT_TYPE；int32-int；uint32-unsigned int
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.0
*更改历史
    日期          内容          更改者
    2021.02.20   V1.0          杨昌林
********************************************************************************/
extern void Init_complex(ARRAY_complex** pEmxArray, int numDimensions);
extern void Init_int32(ARRAY_int32** pEmxArray, int numDimensions);
extern void Init_real(ARRAY_real** pEmxArray, int numDimensions);
extern void Init_uint32(ARRAY_uint32** pEmxArray, int numDimensions);

/*******************************************************************************
*功能：矩阵数据显示输出
*参数：
* pEmxArray：ARRAY_**类型矩阵的指针
*返回值：无
*说明：**：complex-复数；real-FLOAT_TYPE；int32-int；uint32-unsigned int
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.0
*更改历史
    日期          内容          更改者
    2021.02.20   V1.0          杨昌林
********************************************************************************/
extern void Print_complex(ARRAY_complex* pEmxArray);
extern void Print_int32(ARRAY_int32* pEmxArray);
extern void Print_uint32(ARRAY_uint32* pEmxArray);
extern void Print_real(ARRAY_real* pEmxArray);

/*******************************************************************************
*功能：矩阵数据赋零初始化
*参数：
* pEmxArray：ARRAY_**类型矩阵的指针
*返回值：无
*说明：**：complex-复数；real-FLOAT_TYPE；int32-int；uint32-unsigned int
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.0
*更改历史
    日期          内容          更改者
    2021.02.20   V1.0          杨昌林
********************************************************************************/
extern void Zeros_complex(ARRAY_complex* pEmxArray);
extern void Zeros_real(ARRAY_real* pEmxArray);
extern void Zeros_int32(ARRAY_int32* pEmxArray);
extern void Zeros_uint32(ARRAY_uint32* pEmxArray);


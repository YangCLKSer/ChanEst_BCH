#pragma once

//控制关于控制台头文件的编译与否
#define ON_PC 1
#ifdef ON_PC
#include <stdio.h>
#endif // ON_PC
#include "datadefs.h"
#include <math.h>

#define boolean_T unsigned char
#define FLOAT_TYPE float
typedef struct
{
	int* data;
	int* size;
	int allocatedSize;
	int numDimensions;
	boolean_T canFreeData;
} ARRAY_int32;

typedef struct
{
	FLOAT_TYPE* data;
	int* size;
	int allocatedSize;
	int numDimensions;
	boolean_T canFreeData;
} ARRAY_real;

typedef struct
{
	unsigned int* data;
	int* size;
	int allocatedSize;
	int numDimensions;
	boolean_T canFreeData;
} ARRAY_uint32;

typedef struct {
	FLOAT_TYPE re, im;
} struct_complex;

typedef struct {
	int NDLRB;
	char DuplexMode[20];
	char CyclicPrefix[20];
	int NCellID;
	int NSubframe;
	int TDDConfig;
	int SSC;
	int CellRefP;
	char PHICHDuration[20];
	char Ng[20];
	int NFrame;
	int CFI;
} struct_ENB;

typedef struct {
	struct_complex* data;
	int* size;
	int allocatedSize;
	int numDimensions;
	boolean_T canFreeData;
} ARRAY_complex;


/*******************************************************************************
*功能：复数加法
*参数：
* a：复数
* b：复数
*返回值：复数，a+b
*说明：无
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.1
*更改历史
*	日期          更改者		内容
*	2021.02.20   杨昌林		V1.0
*	2021.03.02   杨昌林		V1.1：宏定义浮点数类型
********************************************************************************/
extern struct_complex complexAdd(struct_complex a, struct_complex b);

/*******************************************************************************
*功能：复数减法
*参数：
* a：复数
* b：复数
*返回值：复数，a-b
*说明：无
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.1
*更改历史
*	日期          更改者		内容
*	2021.02.20   杨昌林		V1.0
*	2021.03.02   杨昌林		V1.1：宏定义浮点数类型
********************************************************************************/
extern struct_complex complexMinus(struct_complex a, struct_complex b);

/*******************************************************************************
*功能：复数乘法
*参数：
* a：复数
* b：复数
*返回值：复数，a*b
*说明：无
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.1
*更改历史
*	日期          更改者		内容
*	2021.02.20   杨昌林		V1.0
*	2021.03.02   杨昌林		V1.1：宏定义浮点数类型
********************************************************************************/
extern struct_complex complexMulti(struct_complex a, struct_complex b);

/*******************************************************************************
*功能：复数除法
*参数：
* a：复数
* b：复数
*返回值：复数，a/b
*说明：无
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.1
*更改历史
*	日期          更改者		内容
*	2021.02.20   杨昌林		V1.0
*	2021.03.02   杨昌林		V1.1：宏定义浮点数类型
********************************************************************************/
extern struct_complex complexDiv(struct_complex a, struct_complex b);

/*******************************************************************************
*功能：复数e指数
*参数：
* a：复数
*返回值：复数，e^a
*说明：无
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.1
*更改历史
*	日期          更改者		内容
*	2021.02.20   杨昌林		V1.0
*	2021.03.02   杨昌林		V1.1：宏定义浮点数类型
********************************************************************************/
extern struct_complex complexExp(struct_complex a);

/*******************************************************************************
*功能：复数的模
*参数：
* a：复数
*返回值：FLOAT_TYPE，a的模
*说明：无
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.1
*更改历史
*	日期          更改者		内容
*	2021.02.20   杨昌林		V1.0
*	2021.03.02   杨昌林		V1.1：宏定义浮点数类型
********************************************************************************/
extern FLOAT_TYPE complexNorm(struct_complex a);

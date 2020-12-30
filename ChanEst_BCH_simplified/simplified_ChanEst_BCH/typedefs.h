#pragma once

#include "datadefs.h"

#define  MAX_int32_T     ((int)(2147483647))    /* 2147483647  */
#define  MIN_int32_T     ((int)(-2147483647-1)) /* -2147483648 */
#define  MAX_uint32_T    ((unsigned int)(0xFFFFFFFFU))  /* 4294967295  */
#define  MIN_uint32_T    ((unsigned int)(0))

#define false 0
#define true 1

#define boolean_T unsigned char
#define real_T double

typedef struct {
	struct_creal* data;
	int* size;
	int allocatedSize;
	int numDimensions;
	boolean_T canFreeData;
} ARRAY_creal;

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
	double* data;
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
	real_T re, im;
} struct_creal;

typedef struct {
	double NDLRB;
	char DuplexMode[20];
	char CyclicPrefix[20];
	double NCellID;
	double NSubframe;
	double TDDConfig;
	double SSC;
	double CellRefP;
	char PHICHDuration[20];
	char Ng[20];
	double NFrame;
	double CFI;
} struct_ENB;
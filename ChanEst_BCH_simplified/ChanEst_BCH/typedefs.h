#pragma once

//控制关于控制台头文件的编译与否
#define ON_PC 1
#ifdef ON_PC
#include <stdio.h>
#endif // ON_PC
#include "datadefs.h"
#include <math.h>

#define boolean_T unsigned char
#define real_T double
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
	double re, im;
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



extern struct_complex complexAdd(struct_complex a, struct_complex b);

extern struct_complex complexMinus(struct_complex a, struct_complex b);

extern struct_complex complexMulti(struct_complex a, struct_complex b);

extern struct_complex complexDiv(struct_complex a, struct_complex b);

extern struct_complex complexExp(struct_complex a);

extern double complexNorm(struct_complex a);

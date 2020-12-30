#pragma once

#include "datadefs.h"
#include <math.h>

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

extern struct_creal crealAdd(struct_creal a, struct_creal b);

extern struct_creal crealMinus(struct_creal a, struct_creal b);

extern struct_creal crealMulti(struct_creal a, struct_creal b);

extern struct_creal crealDiv(struct_creal a, struct_creal b);

extern struct_creal crealExp(struct_creal a);
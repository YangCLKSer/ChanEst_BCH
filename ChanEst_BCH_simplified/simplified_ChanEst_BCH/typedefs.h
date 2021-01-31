#pragma once

/*#include <complex.h>
#include <complex>*/

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
	real_T re, im;
} struct_creal;

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
	struct_creal* data;
	int* size;
	int allocatedSize;
	int numDimensions;
	boolean_T canFreeData;
} ARRAY_creal;



extern struct_creal crealAdd(struct_creal a, struct_creal b);

extern struct_creal crealMinus(struct_creal a, struct_creal b);

extern struct_creal crealMulti(struct_creal a, struct_creal b);

extern struct_creal crealDiv(struct_creal a, struct_creal b);

extern struct_creal crealExp(struct_creal a);

extern double crealNorm(struct_creal a);
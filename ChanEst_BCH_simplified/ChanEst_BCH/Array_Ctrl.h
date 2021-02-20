#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "typedefs.h"
//#include "rtwtypes.h"
//#include "BCHIndices_types.h"

/* Function Declarations */
extern void EnsureCapacity_complex(ARRAY_complex* emxArray, int oldNumel);
extern void EnsureCapacity_int32(ARRAY_int32* emxArray, int oldNumel);
extern void EnsureCapacity_real(ARRAY_real* emxArray, int oldNumel);
extern void EnsureCapacity_uint32(ARRAY_uint32* emxArray, int oldNumel);
extern void Free_complex(ARRAY_complex** pEmxArray);
extern void Free_int32(ARRAY_int32** pEmxArray);
extern void Free_real(ARRAY_real** pEmxArray);
extern void Free_uint32(ARRAY_uint32** pEmxArray);
extern void Init_complex(ARRAY_complex** pEmxArray, int numDimensions);
extern void Init_int32(ARRAY_int32** pEmxArray, int numDimensions);
extern void Init_real(ARRAY_real** pEmxArray, int numDimensions);
extern void Init_uint32(ARRAY_uint32** pEmxArray, int numDimensions);

extern void Print_complex(ARRAY_complex* pEmxArray);
extern void Print_int32(ARRAY_int32* pEmxArray);
extern void Print_uint32(ARRAY_uint32* pEmxArray);
extern void Print_real(ARRAY_real* pEmxArray);

extern void Zeros_complex(ARRAY_complex* pEmxArray);
extern void Zeros_real(ARRAY_real* pEmxArray);
extern void Zeros_int32(ARRAY_int32* pEmxArray);
extern void Zeros_uint32(ARRAY_uint32* pEmxArray);


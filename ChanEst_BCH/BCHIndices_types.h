/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: BCHIndices_types.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 26-Dec-2020 21:05:29
 */

#ifndef BCHINDICES_TYPES_H
#define BCHINDICES_TYPES_H

/* Include Files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef struct_emxArray_char_T_1x3
#define struct_emxArray_char_T_1x3

struct emxArray_char_T_1x3
{
  char data[3];
  int size[2];
};

#endif                                 /*struct_emxArray_char_T_1x3*/

#ifndef typedef_emxArray_char_T_1x3
#define typedef_emxArray_char_T_1x3

typedef struct emxArray_char_T_1x3 emxArray_char_T_1x3;

#endif                                 /*typedef_emxArray_char_T_1x3*/

#ifndef struct_emxArray_char_T_1x6
#define struct_emxArray_char_T_1x6

struct emxArray_char_T_1x6
{
  char data[6];
  int size[2];
};

#endif                                 /*struct_emxArray_char_T_1x6*/

#ifndef typedef_emxArray_char_T_1x6
#define typedef_emxArray_char_T_1x6

typedef struct emxArray_char_T_1x6 emxArray_char_T_1x6;

#endif                                 /*typedef_emxArray_char_T_1x6*/

#ifndef typedef_emxArray_creal_T
#define typedef_emxArray_creal_T

typedef struct {
  creal_T *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
} emxArray_creal_T;

#endif                                 /*typedef_emxArray_creal_T*/

#ifndef struct_emxArray_int32_T
#define struct_emxArray_int32_T

struct emxArray_int32_T
{
  int *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_int32_T*/

#ifndef typedef_emxArray_int32_T
#define typedef_emxArray_int32_T

typedef struct emxArray_int32_T emxArray_int32_T;

#endif                                 /*typedef_emxArray_int32_T*/

#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  double *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_real_T*/

#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T

typedef struct emxArray_real_T emxArray_real_T;

#endif                                 /*typedef_emxArray_real_T*/

#ifndef struct_emxArray_uint32_T
#define struct_emxArray_uint32_T

struct emxArray_uint32_T
{
  unsigned int *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_uint32_T*/

#ifndef typedef_emxArray_uint32_T
#define typedef_emxArray_uint32_T

typedef struct emxArray_uint32_T emxArray_uint32_T;

#endif                                 /*typedef_emxArray_uint32_T*/

#ifndef typedef_struct1_T
#define typedef_struct1_T

typedef struct {
  double NDLRB;
  char DuplexMode[3];
  char CyclicPrefix[6];
  double NCellID;
  double NSubframe;
  double TDDConfig;
  double SSC;
  double CellRefP;
  emxArray_char_T_1x6 PHICHDuration;
  emxArray_char_T_1x3 Ng;
  double NFrame;
  double CFI;
} struct1_T;

#endif                                 /*typedef_struct1_T*/

#ifndef typedef_struct0_T
#define typedef_struct0_T

typedef struct {
  double NDLRB;
  char DuplexMode[3];
  char CyclicPrefix[6];
  double NCellID;
  double NSubframe;
  double TDDConfig;
  double SSC;
  double CellRefP;
  double NFrame;
  double CFI;
} struct0_T;

#endif                                 /*typedef_struct0_T*/
#endif

/*
 * File trailer for BCHIndices_types.h
 *
 * [EOF]
 */

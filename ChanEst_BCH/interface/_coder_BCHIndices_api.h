/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_BCHIndices_api.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 26-Dec-2020 21:05:29
 */

#ifndef _CODER_BCHINDICES_API_H
#define _CODER_BCHINDICES_API_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"

/* Type Definitions */
#ifndef struct_emxArray_char_T_1x3
#define struct_emxArray_char_T_1x3

struct emxArray_char_T_1x3
{
  char_T data[3];
  int32_T size[2];
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
  char_T data[6];
  int32_T size[2];
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
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
} emxArray_creal_T;

#endif                                 /*typedef_emxArray_creal_T*/

#ifndef typedef_struct1_T
#define typedef_struct1_T

typedef struct {
  real_T NDLRB;
  char_T DuplexMode[3];
  char_T CyclicPrefix[6];
  real_T NCellID;
  real_T NSubframe;
  real_T TDDConfig;
  real_T SSC;
  real_T CellRefP;
  emxArray_char_T_1x6 PHICHDuration;
  emxArray_char_T_1x3 Ng;
  real_T NFrame;
  real_T CFI;
} struct1_T;

#endif                                 /*typedef_struct1_T*/

#ifndef typedef_struct0_T
#define typedef_struct0_T

typedef struct {
  real_T NDLRB;
  char_T DuplexMode[3];
  char_T CyclicPrefix[6];
  real_T NCellID;
  real_T NSubframe;
  real_T TDDConfig;
  real_T SSC;
  real_T CellRefP;
  real_T NFrame;
  real_T CFI;
} struct0_T;

#endif                                 /*typedef_struct0_T*/

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void BCHIndices(struct0_T *enb, real_T index_data[], int32_T index_size[2]);
extern void BCHIndices_api(const mxArray * const prhs[1], int32_T nlhs, const
  mxArray *plhs[1]);
extern void BCHIndices_atexit(void);
extern void BCHIndices_initialize(void);
extern void BCHIndices_terminate(void);
extern void BCHIndices_xil_shutdown(void);
extern void BCHIndices_xil_terminate(void);
extern void ChannelEst(emxArray_creal_T *rcvSigFreq, real_T mmseA1, real_T
  mmseB1, real_T mmseA2, real_T mmseB2, struct1_T *ENB, emxArray_creal_T
  *TempChan);
extern void ChannelEst_api(const mxArray * const prhs[6], int32_T nlhs, const
  mxArray *plhs[1]);

#endif

/*
 * File trailer for _coder_BCHIndices_api.h
 *
 * [EOF]
 */

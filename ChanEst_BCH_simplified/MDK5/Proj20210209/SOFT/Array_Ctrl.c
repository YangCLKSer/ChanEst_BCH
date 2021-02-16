#pragma once
/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: BCHIndices_emxutil.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 26-Dec-2020 21:05:29
 */

 /* Include Files */
#include "Array_Ctrl.h"


/* Function Definitions */

/*
 * Arguments    : emxArray_complex_T *emxArray
 *                int oldNumel
 * Return Type  : void
 */
void EnsureCapacity_complex(ARRAY_complex* emxArray, int oldNumel)
{
    int newNumel;
    int i;
    void* newData;
    if (oldNumel < 0) {
        oldNumel = 0;
    }

    newNumel = 1;
    for (i = 0; i < emxArray->numDimensions; i++) {
        newNumel *= emxArray->size[i];
    }

    if (newNumel > emxArray->allocatedSize) {
        i = emxArray->allocatedSize;
        if (i < 16) {
            i = 16;
        }

        while (i < newNumel) {
            if (i > 1073741823) {
                i = MAX_int32_T;
            }
            else {
                i *= 2;
            }
        }

        newData = calloc((unsigned int)i, sizeof(struct_complex));
        if (emxArray->data != NULL) {
            memcpy(newData, emxArray->data, sizeof(struct_complex) * oldNumel);
            if (emxArray->canFreeData) {
                free(emxArray->data);
            }
        }

        emxArray->data = (struct_complex*)newData;
        emxArray->allocatedSize = i;
        emxArray->canFreeData = 1;
    }
}

/*
 * Arguments    : emxArray_int32_T *emxArray
 *                int oldNumel
 * Return Type  : void
 */
void EnsureCapacity_int32(ARRAY_int32* emxArray, int oldNumel)
{
    int newNumel;
    int i;
    void* newData;
    if (oldNumel < 0) {
        oldNumel = 0;
    }

    newNumel = 1;
    for (i = 0; i < emxArray->numDimensions; i++) {
        newNumel *= emxArray->size[i];
    }

    if (newNumel > emxArray->allocatedSize) {
        i = emxArray->allocatedSize;
        if (i < 16) {
            i = 16;
        }

        while (i < newNumel) {
            if (i > 1073741823) {
                i = MAX_int32_T;
            }
            else {
                i *= 2;
            }
        }

        newData = calloc((unsigned int)i, sizeof(int));
        if (emxArray->data != NULL) {
            memcpy(newData, emxArray->data, sizeof(int) * oldNumel);
            if (emxArray->canFreeData) {
                free(emxArray->data);
            }
        }

        emxArray->data = (int*)newData;
        emxArray->allocatedSize = i;
        emxArray->canFreeData = 1;
    }
}

/*
 * Arguments    : emxArray_real_T *emxArray
 *                int oldNumel
 * Return Type  : void
 */
void EnsureCapacity_real(ARRAY_real* emxArray, int oldNumel)
{
    int newNumel;
    int i;
    void* newData;
    if (oldNumel < 0) {
        oldNumel = 0;
    }

    newNumel = 1;
    for (i = 0; i < emxArray->numDimensions; i++) {
        newNumel *= emxArray->size[i];
    }

    if (newNumel > emxArray->allocatedSize) {
        i = emxArray->allocatedSize;
        if (i < 16) {
            i = 16;
        }

        while (i < newNumel) {
            if (i > 1073741823) {
                i = MAX_int32_T;
            }
            else {
                i *= 2;
            }
        }

        newData = calloc((unsigned int)i, sizeof(double));
        if (emxArray->data != NULL) {
            memcpy(newData, emxArray->data, sizeof(double) * oldNumel);
            if (emxArray->canFreeData) {
                free(emxArray->data);
            }
        }

        emxArray->data = (double*)newData;
        emxArray->allocatedSize = i;
        emxArray->canFreeData = 1;
    }
}

/*
 * Arguments    : emxArray_uint32_T *emxArray
 *                int oldNumel
 * Return Type  : void
 */
void EnsureCapacity_uint32(ARRAY_uint32* emxArray, int oldNumel)
{
    int newNumel;
    int i;
    void* newData;
    if (oldNumel < 0) {
        oldNumel = 0;
    }

    newNumel = 1;
    for (i = 0; i < emxArray->numDimensions; i++) {
        newNumel *= emxArray->size[i];
    }

    if (newNumel > emxArray->allocatedSize) {
        i = emxArray->allocatedSize;
        if (i < 16) {
            i = 16;
        }

        while (i < newNumel) {
            if (i > 1073741823) {
                i = MAX_int32_T;
            }
            else {
                i *= 2;
            }
        }

        newData = calloc((unsigned int)i, sizeof(unsigned int));
        if (emxArray->data != NULL) {
            memcpy(newData, emxArray->data, sizeof(unsigned int) * oldNumel);
            if (emxArray->canFreeData) {
                free(emxArray->data);
            }
        }

        emxArray->data = (unsigned int*)newData;
        emxArray->allocatedSize = i;
        emxArray->canFreeData = 1;
    }
}

/*
 * Arguments    : emxArray_complex_T **pEmxArray
 * Return Type  : void
 */
void Free_complex(ARRAY_complex** pEmxArray)
{
    if (*pEmxArray != (ARRAY_complex*)NULL) {
        if (((*pEmxArray)->data != (struct_complex*)NULL) && (*pEmxArray)->canFreeData) {
            free((*pEmxArray)->data);
        }

        free((*pEmxArray)->size);
        free(*pEmxArray);
        *pEmxArray = (ARRAY_complex*)NULL;
    }
}

/*
 * Arguments    : emxArray_int32_T **pEmxArray
 * Return Type  : void
 */
void Free_int32(ARRAY_int32** pEmxArray)
{
    if (*pEmxArray != (ARRAY_int32*)NULL) {
        if (((*pEmxArray)->data != (int*)NULL) && (*pEmxArray)->canFreeData) {
            free((*pEmxArray)->data);
        }

        free((*pEmxArray)->size);
        free(*pEmxArray);
        *pEmxArray = (ARRAY_int32*)NULL;
    }
}

/*
 * Arguments    : emxArray_real_T **pEmxArray
 * Return Type  : void
 */
void Free_real(ARRAY_real** pEmxArray)
{
    if (*pEmxArray != (ARRAY_real*)NULL) {
        if (((*pEmxArray)->data != (double*)NULL) && (*pEmxArray)->canFreeData) {
            free((*pEmxArray)->data);
        }

        free((*pEmxArray)->size);
        free(*pEmxArray);
        *pEmxArray = (ARRAY_real*)NULL;
    }
}

/*
 * Arguments    : emxArray_uint32_T **pEmxArray
 * Return Type  : void
 */
void Free_uint32(ARRAY_uint32** pEmxArray)
{
    if (*pEmxArray != (ARRAY_uint32*)NULL) {
        if (((*pEmxArray)->data != (unsigned int*)NULL) && (*pEmxArray)
            ->canFreeData) {
            free((*pEmxArray)->data);
        }

        free((*pEmxArray)->size);
        free(*pEmxArray);
        *pEmxArray = (ARRAY_uint32*)NULL;
    }
}

/*
 * Arguments    : emxArray_complex_T **pEmxArray
 *                int numDimensions
 * Return Type  : void
 */
void Init_complex(ARRAY_complex** pEmxArray, int numDimensions)
{
    ARRAY_complex* emxArray;
    int i;
    *pEmxArray = (ARRAY_complex*)malloc(sizeof(ARRAY_complex));
    emxArray = *pEmxArray;
    emxArray->data = (struct_complex*)NULL;
    emxArray->numDimensions = numDimensions;
    emxArray->size = (int*)malloc(sizeof(int) * numDimensions);
    emxArray->allocatedSize = 0;
    emxArray->canFreeData = 1;
    for (i = 0; i < numDimensions; i++) {
        emxArray->size[i] = 0;
    }
}

/*
 * Arguments    : emxArray_int32_T **pEmxArray
 *                int numDimensions
 * Return Type  : void
 */
void Init_int32(ARRAY_int32** pEmxArray, int numDimensions)
{
    ARRAY_int32* emxArray;
    int i;
    *pEmxArray = (ARRAY_int32*)malloc(sizeof(ARRAY_int32));
    emxArray = *pEmxArray;
    emxArray->data = (int*)NULL;
    emxArray->numDimensions = numDimensions;
    emxArray->size = (int*)malloc(sizeof(int) * numDimensions);
    emxArray->allocatedSize = 0;
    emxArray->canFreeData = 1;
    for (i = 0; i < numDimensions; i++) {
        emxArray->size[i] = 0;
    }
}

/*
 * Arguments    : emxArray_real_T **pEmxArray
 *                int numDimensions
 * Return Type  : void
 */
void Init_real(ARRAY_real** pEmxArray, int numDimensions)
{
    ARRAY_real* emxArray;
    int i;
    *pEmxArray = (ARRAY_real*)malloc(sizeof(ARRAY_real));
    emxArray = *pEmxArray;
    emxArray->data = (double*)NULL;
    emxArray->numDimensions = numDimensions;
    emxArray->size = (int*)malloc(sizeof(int) * numDimensions);
    emxArray->allocatedSize = 0;
    emxArray->canFreeData = 1;
    for (i = 0; i < numDimensions; i++) {
        emxArray->size[i] = 0;
    }
}

/*
 * Arguments    : emxArray_uint32_T **pEmxArray
 *                int numDimensions
 * Return Type  : void
 */
void Init_uint32(ARRAY_uint32** pEmxArray, int numDimensions)
{
    ARRAY_uint32* emxArray;
    int i;
    *pEmxArray = (ARRAY_uint32*)malloc(sizeof(ARRAY_uint32));
    emxArray = *pEmxArray;
    emxArray->data = (unsigned int*)NULL;
    emxArray->numDimensions = numDimensions;
    emxArray->size = (int*)malloc(sizeof(int) * numDimensions);
    emxArray->allocatedSize = 0;
    emxArray->canFreeData = 1;
    for (i = 0; i < numDimensions; i++) {
        emxArray->size[i] = 0;
    }
}

void Print_complex(ARRAY_complex* pEmxArray)
{
    int i, j;
    int d1, d2;
    if (pEmxArray->numDimensions > 2)
    {
        d1 = 1;
        d2 = 2;
    }
    else
    {
        d1 = 0;
        d2 = 1;
    }
    printf("\n");
    for (i = 0; i < (pEmxArray)->size[d1]; i++)
    {
        printf("%d行: ", i);
        for (j = 0; j < (pEmxArray)->size[d2]; j++)
        {
            if (pEmxArray->data[i * pEmxArray->size[d2] + j].im < 0)
                printf("%.4e-%.4e*i, ", pEmxArray->data[i * pEmxArray->size[d2] + j].re, \
                    - pEmxArray->data[i * pEmxArray->size[d2] + j].im);
            else
                printf("%.4e+%.4e*i, ", pEmxArray->data[i * pEmxArray->size[d2] + j].re, \
                    pEmxArray->data[i * pEmxArray->size[d2] + j].im);
            
        }
        printf("\n\n");
    }
    printf("\n");
}
void Print_int32(ARRAY_int32* pEmxArray)
{
    int i, j;
    printf("\n");
    for (i = 0; i < (pEmxArray)->size[0]; i++)
    {
        printf("%d行: ", i);
        for (j = 0; j < (pEmxArray)->size[1]; j++)
        {
            printf("%d, ", pEmxArray->data[i * pEmxArray->size[1] + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void Print_uint32(ARRAY_uint32* pEmxArray)
{
    int i, j;
    printf("\n");
    for (i = 0; i < (pEmxArray)->size[0]; i++)
    {
        printf("%d行: ", i);
        for (j = 0; j < (pEmxArray)->size[1]; j++)
        {
            printf("%u, ", pEmxArray->data[i * pEmxArray->size[1] + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void Print_real(ARRAY_real* pEmxArray)
{
    int i, j;
    printf("\n");
    for (i = 0; i < (pEmxArray)->size[0]; i++)
    {
        printf("%d行: ", i);
        for (j = 0; j < (pEmxArray)->size[1]; j++)
        {
            printf("%.4e, ", pEmxArray->data[i * pEmxArray->size[1] + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void Zeros_complex(ARRAY_complex* pEmxArray)
{
    int loop_ub, i;
    loop_ub = 1;
    for (i = 0; i < pEmxArray->numDimensions; i++)
    {
        loop_ub *= pEmxArray->size[i];
    }
    //EnsureCapacity_complex(pEmxArray, oldnum);
    for (i = 0; i < loop_ub; i++)
    {
        pEmxArray->data[i].im = 0.0;
        pEmxArray->data[i].re = 0.0;
    }
}

void Zeros_real(ARRAY_real* pEmxArray)
{
    int loop_ub, i;
    loop_ub = 1;
    for (i = 0; i < pEmxArray->numDimensions; i++)
    {
        loop_ub *= pEmxArray->size[i];
    }
    //EnsureCapacity_real(pEmxArray, oldnum);
    for (i = 0; i < loop_ub; i++)
    {
        pEmxArray->data[i] = 0.0;
    }
}

void Zeros_int32(ARRAY_int32* pEmxArray)
{
    int loop_ub, i;
    loop_ub = 1;
    for (i = 0; i < pEmxArray->numDimensions; i++)
    {
        loop_ub *= pEmxArray->size[i];
    }
    //EnsureCapacity_int32(pEmxArray, oldnum);
    for (i = 0; i < loop_ub; i++)
    {
        pEmxArray->data[i] = 0;
    }
}

void Zeros_uint32(ARRAY_uint32* pEmxArray)
{
    int loop_ub, i;
    loop_ub = 1;
    for (i = 0; i < pEmxArray->numDimensions; i++)
    {
        loop_ub *= pEmxArray->size[i];
    }
    //EnsureCapacity_uint32(pEmxArray, oldnum);
    for (i = 0; i < loop_ub; i++)
    {
        pEmxArray->data[i] = 0;
    }
}

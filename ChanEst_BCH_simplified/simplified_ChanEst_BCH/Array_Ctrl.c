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
#include "typedefs.h"
#include <stdlib.h>
#include <string.h>
//#include "rt_nonfinite.h"


/* Function Definitions */

/*
 * Arguments    : emxArray_creal_T *emxArray
 *                int oldNumel
 * Return Type  : void
 */
void EnsureCapacity_creal(ARRAY_creal* emxArray, int oldNumel)
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

        newData = calloc((unsigned int)i, sizeof(struct_creal));
        if (emxArray->data != NULL) {
            memcpy(newData, emxArray->data, sizeof(struct_creal) * oldNumel);
            if (emxArray->canFreeData) {
                free(emxArray->data);
            }
        }

        emxArray->data = (struct_creal*)newData;
        emxArray->allocatedSize = i;
        emxArray->canFreeData = true;
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
        emxArray->canFreeData = true;
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
        emxArray->canFreeData = true;
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
        emxArray->canFreeData = true;
    }
}

/*
 * Arguments    : emxArray_creal_T **pEmxArray
 * Return Type  : void
 */
void Free_creal(ARRAY_creal** pEmxArray)
{
    if (*pEmxArray != (ARRAY_creal*)NULL) {
        if (((*pEmxArray)->data != (struct_creal*)NULL) && (*pEmxArray)->canFreeData) {
            free((*pEmxArray)->data);
        }

        free((*pEmxArray)->size);
        free(*pEmxArray);
        *pEmxArray = (ARRAY_creal*)NULL;
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
 * Arguments    : emxArray_creal_T **pEmxArray
 *                int numDimensions
 * Return Type  : void
 */
void Init_creal(ARRAY_creal** pEmxArray, int numDimensions)
{
    ARRAY_creal* emxArray;
    int i;
    *pEmxArray = (ARRAY_creal*)malloc(sizeof(ARRAY_creal));
    emxArray = *pEmxArray;
    emxArray->data = (struct_creal*)NULL;
    emxArray->numDimensions = numDimensions;
    emxArray->size = (int*)malloc(sizeof(int) * numDimensions);
    emxArray->allocatedSize = 0;
    emxArray->canFreeData = true;
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
    emxArray->canFreeData = true;
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
    emxArray->canFreeData = true;
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
    emxArray->canFreeData = true;
    for (i = 0; i < numDimensions; i++) {
        emxArray->size[i] = 0;
    }
}


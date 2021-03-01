#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "typedefs.h"
//#include "rtwtypes.h"
//#include "BCHIndices_types.h"

/* Function Declarations */


/*******************************************************************************
*���ܣ�����������ڴ�����
*������
* oldNumel��int�������ԭ����Ԫ������
* emxArray��ARRAY_**���;����ָ��
*����ֵ����
*˵����**��complex-������real-FLOAT_TYPE��int32-int��uint32-unsigned int
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
    ����          ����          ������
    2021.02.20   V1.0          �����
********************************************************************************/
extern void EnsureCapacity_complex(ARRAY_complex* emxArray, int oldNumel);
extern void EnsureCapacity_int32(ARRAY_int32* emxArray, int oldNumel);
extern void EnsureCapacity_real(ARRAY_real* emxArray, int oldNumel);
extern void EnsureCapacity_uint32(ARRAY_uint32* emxArray, int oldNumel);

/*******************************************************************************
*���ܣ������������ݿռ��ͷ�
*������
* pEmxArray��ARRAY_**���;����ָ���ָ�룬��������������ͷź󣬶�ʹ��ָ��洢�ľ������������ͷ�
*����ֵ����
*˵����**��complex-������real-FLOAT_TYPE��int32-int��uint32-unsigned int
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
    ����          ����          ������
    2021.02.20   V1.0          �����
********************************************************************************/
extern void Free_complex(ARRAY_complex** pEmxArray);
extern void Free_int32(ARRAY_int32** pEmxArray);
extern void Free_real(ARRAY_real** pEmxArray);
extern void Free_uint32(ARRAY_uint32** pEmxArray);

/*******************************************************************************
*���ܣ�����������������ʼ��
*������
* pEmxArray��ARRAY_**���;����ָ���ָ�룬���ڶ�ʹ��ָ��洢�ľ���������������
*����ֵ����
*˵����**��complex-������real-FLOAT_TYPE��int32-int��uint32-unsigned int
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
    ����          ����          ������
    2021.02.20   V1.0          �����
********************************************************************************/
extern void Init_complex(ARRAY_complex** pEmxArray, int numDimensions);
extern void Init_int32(ARRAY_int32** pEmxArray, int numDimensions);
extern void Init_real(ARRAY_real** pEmxArray, int numDimensions);
extern void Init_uint32(ARRAY_uint32** pEmxArray, int numDimensions);

/*******************************************************************************
*���ܣ�����������ʾ���
*������
* pEmxArray��ARRAY_**���;����ָ��
*����ֵ����
*˵����**��complex-������real-FLOAT_TYPE��int32-int��uint32-unsigned int
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
    ����          ����          ������
    2021.02.20   V1.0          �����
********************************************************************************/
extern void Print_complex(ARRAY_complex* pEmxArray);
extern void Print_int32(ARRAY_int32* pEmxArray);
extern void Print_uint32(ARRAY_uint32* pEmxArray);
extern void Print_real(ARRAY_real* pEmxArray);

/*******************************************************************************
*���ܣ��������ݸ����ʼ��
*������
* pEmxArray��ARRAY_**���;����ָ��
*����ֵ����
*˵����**��complex-������real-FLOAT_TYPE��int32-int��uint32-unsigned int
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
    ����          ����          ������
    2021.02.20   V1.0          �����
********************************************************************************/
extern void Zeros_complex(ARRAY_complex* pEmxArray);
extern void Zeros_real(ARRAY_real* pEmxArray);
extern void Zeros_int32(ARRAY_int32* pEmxArray);
extern void Zeros_uint32(ARRAY_uint32* pEmxArray);


#pragma once

//���ƹ��ڿ���̨ͷ�ļ��ı������
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


/*******************************************************************************
*���ܣ������ӷ�
*������
* a������
* b������
*����ֵ��������a+b
*˵������
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
	����          ����          ������
	2021.02.20   V1.0          �����
********************************************************************************/
extern struct_complex complexAdd(struct_complex a, struct_complex b);

/*******************************************************************************
*���ܣ���������
*������
* a������
* b������
*����ֵ��������a-b
*˵������
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
	����          ����          ������
	2021.02.20   V1.0          �����
********************************************************************************/
extern struct_complex complexMinus(struct_complex a, struct_complex b);

/*******************************************************************************
*���ܣ������˷�
*������
* a������
* b������
*����ֵ��������a*b
*˵������
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
	����          ����          ������
	2021.02.20   V1.0          �����
********************************************************************************/
extern struct_complex complexMulti(struct_complex a, struct_complex b);

/*******************************************************************************
*���ܣ���������
*������
* a������
* b������
*����ֵ��������a/b
*˵������
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
	����          ����          ������
	2021.02.20   V1.0          �����
********************************************************************************/
extern struct_complex complexDiv(struct_complex a, struct_complex b);

/*******************************************************************************
*���ܣ�����eָ��
*������
* a������
*����ֵ��������e^a
*˵������
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
	����          ����          ������
	2021.02.20   V1.0          �����
********************************************************************************/
extern struct_complex complexExp(struct_complex a);

/*******************************************************************************
*���ܣ�������ģ
*������
* a������
*����ֵ��double��a��ģ
*˵������
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
	����          ����          ������
	2021.02.20   V1.0          �����
********************************************************************************/
extern double complexNorm(struct_complex a);

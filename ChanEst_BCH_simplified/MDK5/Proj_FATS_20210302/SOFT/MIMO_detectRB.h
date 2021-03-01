#pragma once

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "typedefs.h"
#include "Array_Ctrl.h"
#include <math.h>
#include <string.h>

/*******************************************************************************
*���ܣ����Ⱥϲ�����ƥ���˲�
*������
* Received�������������ź�
* mimoCH���������ŵ�����
* Detected��FLOAT_TYPE��������
* ampd��FLOAT_TYPE���ŵ�������Ϣ
*����ֵ����
*˵������
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
    ����          ����          ������
    2021.02.20   V1.0          �����
********************************************************************************/
extern void mrc(struct_complex Detected_RB[240], FLOAT_TYPE ampd[240], const struct_complex RxData[240], const struct_complex equCH[960]);

/*******************************************************************************
*���ܣ�SFBC�����Ȼ���
*������
* Received��FLOAT_TYPE�������ź�
* mimoCH��FLOAT_TYPE���ŵ�����
* Detected��FLOAT_TYPE��������
* ampd��FLOAT_TYPE���ŵ�������Ϣ
*����ֵ����
*˵�����ڲ����ð�
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
    ����          ����          ������
    2021.02.20   V1.0          �����
********************************************************************************/
extern void b_mld2sfbc(const FLOAT_TYPE Received[120], const FLOAT_TYPE mimoCH[240],
    FLOAT_TYPE Detected[120], FLOAT_TYPE ampd[120]);

/*******************************************************************************
*���ܣ�SFBC�����Ȼ���
*������
* Received�������������ź�
* mimoCH���������ŵ�����
* Detected��FLOAT_TYPE��������
* ampd��FLOAT_TYPE���ŵ�������Ϣ
*����ֵ����
*˵������
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
    ����          ����          ������
    2021.02.20   V1.0          �����
********************************************************************************/
extern void mld2sfbc(const struct_complex Received[240], const struct_complex mimoCH[960],
    FLOAT_TYPE Detected[240], FLOAT_TYPE ampd[240]);

/*******************************************************************************
*���ܣ�SFBC-FSTD�����Ȼ���
*������
* Received�������������ź�
* mimoCH���������ŵ�����
* Detected��FLOAT_TYPE��������
* ampd��FLOAT_TYPE���ŵ�������Ϣ
*����ֵ����
*˵������
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
    ����          ����          ������
    2021.02.20   V1.0          �����
********************************************************************************/
extern void mld4sfbcfstd(const struct_complex Received[240], const struct_complex mimoCH[960],
    FLOAT_TYPE Detected[240], FLOAT_TYPE ampd[240]);


/*******************************************************************************
*���ܣ���ͬ��֡��ͬƵ�ʵ�������Դ�������ݵļ��
*������
* RxData: ��������Դ��ӳ���Ľ�������
* equCH: ��������Ч�ŵ�
* methodDetect: int������㷨ѡ��, ������0, 1, 2��0�����Ⱥϲ�����ƥ���˲���1��SFBC�ļ�⣻2��SFBC-FSTD�ļ��
* mod_mode: int�����Ʒ�ʽ, 4, 16, 64
* Detected_RB: �����������ķ���
* ampd: FLOAT_TYPE��������Ϣ������QAM���
*����ֵ����
*˵���������㷨���ޣ�δʹ��mod_mode��ʡ�ԡ�methodDetectΪ1��2ʱδ�������ԡ�
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
    ����          ����          ������
    2021.02.20   V1.0          �����
********************************************************************************/
extern void MIMO_detectRB(struct_complex Detected_RB[240], FLOAT_TYPE ampd[240], const struct_complex RxData[240], const struct_complex equCH[960], int
    methodDetect);
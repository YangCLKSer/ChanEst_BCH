#pragma once

#include <stddef.h>
#include <stdlib.h>
#if ON_PC
#include <stdio.h>
#endif // ON_PC
#include <string.h>
#include "typedefs.h"
#include "Array_Ctrl.h"
#include "datadefs.h"
#include <math.h>


/*******************************************************************************
*���ܣ�BCH����λ��
*������
* enb��С����Ϣ
* index_data��int����ά��0Ϊ1��ά��1ΪBCH�����������������λ��
*����ֵ����
*˵������
*��д�ߣ������
*��д���ڣ�2021.02.20
*�汾�ţ�V1.0
*������ʷ
    ����          ����          ������
    2021.02.20   V1.0          �����
********************************************************************************/
extern void BCHIndices(ARRAY_int32* index_data, struct_ENB* enb);

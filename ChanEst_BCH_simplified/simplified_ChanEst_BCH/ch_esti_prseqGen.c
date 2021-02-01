#include "ch_esti_prseqGen.h"

void ch_esti_prseqGen(ARRAY_int32* C, int lenC, int Cinit)
{
	int Nc = 1600;
    int loop_ub,i,n,lenGold;
    ARRAY_int32* x1, *x2;

    i = C->size[0] * C->size[1];
    C->size[0] = 1;
    loop_ub = (int)(lenC);
    C->size[1] = loop_ub;
    EnsureCapacity_int32(C, i);
    for (i = 0; i < loop_ub; i++) {
        C->data[i] = 0.0;
    }

    lenGold = Nc + lenC;

    Init_int32(&x1, 2);
    i = x1->size[0] * x1->size[1];
    x1->size[0] = 1;
    loop_ub = (int)(lenGold);
    x1->size[1] = loop_ub;
    EnsureCapacity_int32(x1, i);
    for (i = 0; i < loop_ub; i++) {
        x1->data[i] = 0.0;
    }
    x1->data[0] = 1;

    Init_int32(&x2, 2);
    i = x2->size[0] * x2->size[1];
    x2->size[0] = 1;
    loop_ub = (int)(lenGold);
    x2->size[1] = loop_ub;
    EnsureCapacity_int32(x2, i);
    for (i = 0; i < loop_ub; i++) {
        x2->data[i] = 0.0;
        
    }

    de2bi(x2, Cinit, 31, 0);
    for (i = 0; i < lenGold - 31; i++)
    {
        x1->data[i + 31] = (x1->data[i + 3] + x1->data[i]) % 2;
        x2->data[i + 31] = (x2->data[i + 3] + x2->data[i + 2] + x2->data[i + 1] + x2->data[i]) % 2;
        //printf("%d:%d, %d\n", i + 1, x1->data[i], x2->data[i]);
    }
    
    for (i = 0; i < lenC; i++)
    {
        C->data[i] = (x1->data[i + Nc] + x2->data[i + Nc]) % 2;
    }
}

void de2bi(ARRAY_int32* out, int input, int bit_num, int type)
{
    int mid, loop_ub, i;

    mid = input;
    /*i = out->size[0] * out->size[1];
    EnsureCapacity_int32(out, i);
    for (i = 0; i < loop_ub; i++) {
        out->data[i] = 0;
    }*/
    
    loop_ub = bit_num;
    for (i = 0; i < loop_ub; i++)
    {
        if (mid >= (int)(1 << (loop_ub - i - 1)))
        {
            out->data[loop_ub - i - 1] = 1;
            mid = mid - (int)(1 << (loop_ub - i - 1));
        }
    }
    /*if (type)
        {

        }*/

}
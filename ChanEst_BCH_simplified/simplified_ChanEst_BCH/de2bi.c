#include "typedefs.h"
#include "datadefs.h"

/*type为1：left-msb，即结果进行翻转，否则无需翻转*/
void de2bi(ARRAY_int32* out ,int input, int bit_num, int type)
{
	int mid,loop_ub,i;

	mid = input;
    i = out->size[0] * out->size[1];
    EnsureCapacity_real(out, i);
    for (i = 0; i < loop_ub; i++) {
        out->data[i] = 0;
    }

    for (i = 0; i < loop_ub; i++)
    {
        if (mid >= (int)2 << (loop_ub - i - 1))
        {
            out->data[loop_ub - i] = 1;
            mid = mid - (int)1 << (loop_ub - i - 1);
        }
    }
/*if (type)
    {

    }*/
    
}
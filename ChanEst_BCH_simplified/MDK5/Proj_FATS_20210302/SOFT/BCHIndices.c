#pragma once
#include "BCHIndices.h"

void BCHIndices(ARRAY_int32* index_data, struct_ENB* enb)
{
    int Bshift, counter, out_c, b_counter, res, trueCount, k, i;
    FLOAT_TYPE RSindex[24];
    signed char out[72];
    boolean_T exitg1, y, exitg2;
    signed char BCHindex01_data[72];

    Bshift = enb->NCellID % 3;

    for (counter = 0; counter < 24; counter++) {
        RSindex[counter] = 3.0 * (FLOAT_TYPE)counter + Bshift;
    }

    out_c = 0;
    for (b_counter = 0; b_counter < 72; b_counter++) {
        out[b_counter] = (signed char)b_counter;
    }

    for (counter = 0; counter < 72; counter++) {
        res = 0;
        b_counter = 1;
        exitg1 = 0;
        while ((!exitg1) && (b_counter - 1 < 24)) {
            trueCount = 0;
            if (b_counter == counter) {
                trueCount = 1;
            }

            y = (trueCount != 0);
            if (y) {
                k = 0;
                exitg2 = 0;
                while ((!exitg2) && (k <= trueCount - 1)) {
                    if (RSindex[0] == 0.0) {
                        y = 0;
                        exitg2 = 1;
                    }
                    else {
                        k = 1;
                    }
                }
            }

            if (y) {
                res = 1;
                exitg1 = 1;
            }
            else {
                b_counter++;
            }
        }

        if (res == 0) {
            out[out_c] = (signed char)counter;
            out_c++;
        }
    }

    if (1 > out_c) {
        counter = 0;
    }
    else {
        counter = out_c;
    }

    for (b_counter = 0; b_counter < counter; b_counter++) {
        BCHindex01_data[b_counter] = (signed char)(out[b_counter] + 1);
    }


    //Init_int32(&index_data, 2);
    i = index_data->size[0] * index_data->size[1];
    index_data->size[0] = 1;
    index_data->size[1] = (counter + counter) + 144;
    EnsureCapacity_int32(index_data, i);
    for (b_counter = 0; b_counter < counter; b_counter++) {
        index_data->data[b_counter] = (FLOAT_TYPE)BCHindex01_data[b_counter] + 504.0;
    }

    for (b_counter = 0; b_counter < counter; b_counter++) {
        index_data->data[b_counter + counter] = (FLOAT_TYPE)BCHindex01_data[b_counter] + 576.0;
    }

    for (b_counter = 0; b_counter < 72; b_counter++) {
        res = (b_counter + counter) + counter;
        index_data->data[res] = (FLOAT_TYPE)b_counter + 649.0;
        index_data->data[res + 72] = (FLOAT_TYPE)b_counter + 721.0;
    }
}

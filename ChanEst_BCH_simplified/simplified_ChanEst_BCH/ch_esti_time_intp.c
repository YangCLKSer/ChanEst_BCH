/* Include Files */
#include "ch_esti_time_intp.h"

/* Function Definitions */

/*
 * Time interpolation of channel frequency response on ofdms without RS
 *  Abstract:
 *    This function is linear time interpolation for ofdms without rs.
 *  Input:
 *    hEst, the estimation results after ls sampling.
 *    locOFDMWithRS, locations of ofdms with rs.
 *  Output:
 *    hEst, channel freq estimation output after linear time interpolation.
 *  Reference:
 *    NULL.
 *  Author:
 *    Yang Yushan     2009-04-09
 *  Version History:
 *    Yang Yushan     2009-04-09      Created
 * ------------------------------------------------------------------------------%
 */
void ch_esti_time_intp( ARRAY_complex * hEst, const  ARRAY_int32 
    * locOFDMWithRS)
{
    int locN, i_ofdm, i;//后RS序列位置，当前时域位置，当前频域位置
    int numOFDM, numSym, end;
    double factor,factor2;
    numOFDM = hEst->size[0];
    numSym = hEst->size[1];
    end = locOFDMWithRS->size[1]-1;

    locN = 0;
    //i_ofdm小于
    for (i_ofdm = 0; i_ofdm < numOFDM; i_ofdm++)
    {
        if (i_ofdm < locOFDMWithRS->data[locN]-1)
        {
            if (locN <= 0 && locOFDMWithRS->data[0]!=1)
            {
                factor = ((double)locOFDMWithRS->data[0] - 1.0 - (double)i_ofdm)\
                    / ((double)locOFDMWithRS->data[1] - (double)locOFDMWithRS->data[0]);
                for (i = 0; i < numSym; i++)
                {
                    hEst->data[i_ofdm * numSym + i].re = \
                        hEst->data[(locOFDMWithRS->data[0] - 1) * numSym + i].re\
                        + (hEst->data[(locOFDMWithRS->data[0] - 1) * numSym + i].re\
                            - hEst->data[(locOFDMWithRS->data[1] - 1) * numSym + i].re)\
                        * factor;
                    hEst->data[i_ofdm * numSym + i].im = \
                        hEst->data[(locOFDMWithRS->data[0] - 1) * numSym + i].im\
                        + (hEst->data[(locOFDMWithRS->data[0] - 1) * numSym + i].im\
                            - hEst->data[(locOFDMWithRS->data[1] - 1) * numSym + i].im)\
                        * factor;
                }
            }
            else if (locN > 0)
            {
                factor = ((double)locOFDMWithRS->data[locN] - 1.0 - (double)i_ofdm)\
                    / ((double)locOFDMWithRS->data[locN] - (double)locOFDMWithRS->data[locN - 1]);
                factor2 = ((double)i_ofdm + 1.0 - (double)locOFDMWithRS->data[locN - 1])\
                    / ((double)locOFDMWithRS->data[locN] - (double)locOFDMWithRS->data[locN - 1]);
                for (i = 0; i < numSym; i++)
                {
                    hEst->data[i_ofdm * numSym + i].re = \
                        hEst->data[(locOFDMWithRS->data[locN - 1] - 1) * numSym + i].re * factor\
                        + hEst->data[(locOFDMWithRS->data[locN] - 1) * numSym + i].re * factor2;
                    hEst->data[i_ofdm * numSym + i].im = \
                        hEst->data[(locOFDMWithRS->data[locN - 1] - 1) * numSym + i].im * factor\
                        + hEst->data[(locOFDMWithRS->data[locN] - 1) * numSym + i].im * factor2;
                }
            }
            
        }
        else if (i_ofdm > locOFDMWithRS->data[locN]-1)
        {
            //locN为end+1代表超出
            if (locN <= end)
            {
                locN++;
            }
            if (locN > end)
            {
                factor = ((double)i_ofdm - (double)locOFDMWithRS->data[end] + 1.0)\
                    / ((double)locOFDMWithRS->data[end] - (double)locOFDMWithRS->data[end - 1]);
                for (i = 0; i < numSym; i++)
                {
                    hEst->data[i_ofdm * numSym + i].re = \
                        hEst->data[(locOFDMWithRS->data[end] - 1) * numSym + i].re\
                        + (hEst->data[(locOFDMWithRS->data[end] - 1) * numSym + i].re\
                            - hEst->data[(locOFDMWithRS->data[end - 1] - 1) * numSym + i].re)\
                        * factor;
                    hEst->data[i_ofdm * numSym + i].im = \
                        hEst->data[(locOFDMWithRS->data[end] - 1) * numSym + i].im\
                        + (hEst->data[(locOFDMWithRS->data[end] - 1) * numSym + i].im\
                            - hEst->data[(locOFDMWithRS->data[end - 1] - 1) * numSym + i].im)\
                        * factor;
                }
            }
            else
            {
                factor = ((double)locOFDMWithRS->data[locN] - 1.0 - (double)i_ofdm)\
                    / ((double)locOFDMWithRS->data[locN] - (double)locOFDMWithRS->data[locN - 1]);
                factor2 = ((double)i_ofdm + 1.0 - (double)locOFDMWithRS->data[locN - 1])\
                    / ((double)locOFDMWithRS->data[locN] - (double)locOFDMWithRS->data[locN - 1]);
                for (i = 0; i < numSym; i++)
                {
                    hEst->data[i_ofdm * numSym + i].re = \
                        hEst->data[(locOFDMWithRS->data[locN - 1] - 1) * numSym + i].re * factor\
                        + hEst->data[(locOFDMWithRS->data[locN] - 1) * numSym + i].re * factor2;
                    hEst->data[i_ofdm * numSym + i].im = \
                        hEst->data[(locOFDMWithRS->data[locN - 1] - 1) * numSym + i].im * factor\
                        + hEst->data[(locOFDMWithRS->data[locN] - 1) * numSym + i].im * factor2;
                }
            }
        }
        
    }

}
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
void ch_esti_time_intp( ARRAY_creal * hEst, const  ARRAY_real 
    * locOFDMWithRS)
{
    ARRAY_creal * v,* b_v,* r,* r2;
    ARRAY_real * y,*x;
    ARRAY_int32 * r1;
    boolean_T  do_trans, exitg1;
    boolean_T  guard1 = 0;
    int numOFDM, i, md2, loop_ub, i1, n, nm1d2, i2, m, j, xcnt;
    double ndbl, apnd, cdiff, gap, a, i_ofdm;
    
    numOFDM = hEst->size[0];

    /*  estimation on ofdms without rs */
    /* ---- Linear Interpolation */
    /* -- interpolation */
    Init_creal (&v, 2);
    if (1 == locOFDMWithRS->size[0]) {
        i = hEst->size[0];
        for (md2 = 0; md2 < i; md2++) {
            if ((double)md2 + 1.0 != locOFDMWithRS->data[0]) {
                loop_ub = hEst->size[1] - 1;
                i1 = v->size[0] * v->size[1];
                v->size[0] = 1;
                v->size[1] = loop_ub + 1;
                 EnsureCapacity_creal (v, i1);
                for (i1 = 0; i1 <= loop_ub; i1++) {
                    v->data[i1] = hEst->data[((int)locOFDMWithRS->data[0] + hEst->size[0] *
                        i1) - 1];
                }

                loop_ub = v->size[1];
                for (i1 = 0; i1 < loop_ub; i1++) {
                    hEst->data[md2 + hEst->size[0] * i1] = v->data[i1];
                }
            }
        }
    }
    else {
        if (locOFDMWithRS->data[0] > locOFDMWithRS->data[locOFDMWithRS->size[0] - 1])
        {
            i = 1;
        }
        else {
            i = (int)locOFDMWithRS->data[0];
        }

         Init_real (&y, 2);
        if (locOFDMWithRS->data[locOFDMWithRS->size[0] - 1] <
            locOFDMWithRS->data[0]) {
            y->size[0] = 1;
            y->size[1] = 0;
        }
        else if (floor(locOFDMWithRS->data[0]) == locOFDMWithRS->data[0]) {
            i1 = y->size[0] * y->size[1];
            y->size[0] = 1;
            y->size[1] = (int)floor(locOFDMWithRS->data[locOFDMWithRS->size[0] - 1] -
                locOFDMWithRS->data[0]) + 1;
             EnsureCapacity_real (y, i1);
            loop_ub = (int)floor(locOFDMWithRS->data[locOFDMWithRS->size[0] - 1] -
                locOFDMWithRS->data[0]);
            for (i1 = 0; i1 <= loop_ub; i1++) {
                y->data[i1] = locOFDMWithRS->data[0] + (double)i1;
            }
        }
        else {
            ndbl = floor((locOFDMWithRS->data[locOFDMWithRS->size[0] - 1] -
                locOFDMWithRS->data[0]) + 0.5);
            apnd = locOFDMWithRS->data[0] + ndbl;
            cdiff = apnd - locOFDMWithRS->data[locOFDMWithRS->size[0] - 1];
            if (fabs(cdiff) < 4.4408920985006262E-16 * fmax(fabs(locOFDMWithRS->data[0]),
                fabs(locOFDMWithRS->data[locOFDMWithRS->size[0] - 1]))) {
                ndbl++;
                apnd = locOFDMWithRS->data[locOFDMWithRS->size[0] - 1];
            }
            else if (cdiff > 0.0) {
                apnd = locOFDMWithRS->data[0] + (ndbl - 1.0);
            }
            else {
                ndbl++;
            }

            if (ndbl >= 0.0) {
                n = (int)ndbl;
            }
            else {
                n = 0;
            }

            i1 = y->size[0] * y->size[1];
            y->size[0] = 1;
            y->size[1] = n;
             EnsureCapacity_real (y, i1);
            if (n > 0) {
                y->data[0] = locOFDMWithRS->data[0];
                if (n > 1) {
                    y->data[n - 1] = apnd;
                    nm1d2 = (n - 1) / 2;
                    for (md2 = 0; md2 <= nm1d2 - 2; md2++) {
                        m = md2 + 1;
                        y->data[md2 + 1] = locOFDMWithRS->data[0] + (double)m;
                        y->data[(n - md2) - 2] = apnd - (double)m;
                    }

                    if (nm1d2 << 1 == n - 1) {
                        y->data[nm1d2] = (locOFDMWithRS->data[0] + apnd) / 2.0;
                    }
                    else {
                        y->data[nm1d2] = locOFDMWithRS->data[0] + (double)nm1d2;
                        y->data[nm1d2 + 1] = apnd - (double)nm1d2;
                    }
                }
            }
        }

         Init_real (&x, 1);
        i1 = x->size[0];
        x->size[0] = locOFDMWithRS->size[0];
         EnsureCapacity_real (x, i1);
        loop_ub = locOFDMWithRS->size[0];
        for (i1 = 0; i1 < loop_ub; i1++) {
            x->data[i1] = locOFDMWithRS->data[i1];
        }

         Init_creal (&b_v, 2);
        loop_ub = hEst->size[1];
        i1 = b_v->size[0] * b_v->size[1];
        b_v->size[0] = locOFDMWithRS->size[0];
        b_v->size[1] = loop_ub;
         EnsureCapacity_creal (b_v, i1);
        for (i1 = 0; i1 < loop_ub; i1++) {
            nm1d2 = locOFDMWithRS->size[0];
            for (i2 = 0; i2 < nm1d2; i2++) {
                b_v->data[i2 + b_v->size[0] * i1] = hEst->data[((int)locOFDMWithRS->
                    data[i2] + hEst->size[0] * i1) - 1];
            }
        }

        /* 假设其为大小顺序的，那么从大到小和从小到大就只需要判断首尾,然后flipud。 */
        do_trans = (y->data[0] > y->data[y->size[1] - 1]);
        if (do_trans) {
            m = locOFDMWithRS->size[0] - 1;
            i1 = hEst->size[1] - 1;
            md2 = locOFDMWithRS->size[0] >> 1;
            for (j = 0; j <= i1; j++) {
                for (loop_ub = 0; loop_ub < md2; loop_ub++) {
                    ndbl = b_v->data[loop_ub + b_v->size[0] * j].re;
                    apnd = b_v->data[loop_ub + b_v->size[0] * j].im;
                    nm1d2 = m - loop_ub;
                    b_v->data[loop_ub + b_v->size[0] * j] = b_v->data[nm1d2 + b_v->size[0]
                        * j];
                    b_v->data[nm1d2 + b_v->size[0] * j].re = ndbl;
                    b_v->data[nm1d2 + b_v->size[0] * j].im = apnd;
                }
            }

            m = locOFDMWithRS->size[0] - 1;
            md2 = locOFDMWithRS->size[0] >> 1;
            for (loop_ub = 0; loop_ub < md2; loop_ub++) {
                ndbl = x->data[loop_ub];
                nm1d2 = m - loop_ub;
                x->data[loop_ub] = x->data[nm1d2];
                x->data[nm1d2] = ndbl;
            }

            n = y->size[1] - 1;
            nm1d2 = y->size[1] >> 1;
            for (md2 = 0; md2 < nm1d2; md2++) {
                m = n - md2;
                ndbl = y->data[md2];
                y->data[md2] = y->data[m];
                y->data[m] = ndbl;
            }
        }

         Init_creal (&r, 2);
        i1 = r->size[0] * r->size[1];
        r->size[0] = y->size[1];
        r->size[1] = b_v->size[1];
         EnsureCapacity_creal (r, i1);
        loop_ub = y->size[1] * b_v->size[1];
        for (i1 = 0; i1 < loop_ub; i1++) {
            r->data[i1].re = 0.0;
            r->data[i1].im = 0.0;
        }

        if (x->size[0] > 1) {
            nm1d2 = 1;
            i1 = x->size[0];
            for (xcnt = 0; xcnt < i1; xcnt++) {
                apnd = x->data[xcnt];
                cdiff = y->data[y->size[1] - 1];
                gap = y->data[y->size[1] - 1] - y->data[0];
                if (xcnt + 1 < x->size[0]) {
                    cdiff = x->data[xcnt + 1];
                    gap = cdiff - x->data[xcnt];
                }

                m = nm1d2;
                i2 = y->size[1] - nm1d2;
                md2 = 0;
                exitg1 = 0;
                while ((!exitg1) && (md2 <= i2)) {
                    n = m + md2;
                    j = n - 1;
                    ndbl = y->data[j];
                    guard1 = 0;
                    if (ndbl == apnd) {
                        loop_ub = b_v->size[1];
                        for (nm1d2 = 0; nm1d2 < loop_ub; nm1d2++) {
                            r->data[j + r->size[0] * nm1d2] = b_v->data[xcnt + b_v->size[0] *
                                nm1d2];
                        }

                        guard1 = 1;
                    }
                    else if (ndbl > apnd) {
                        if (ndbl < cdiff) {
                            a = (cdiff - ndbl) / gap;
                            ndbl = (ndbl - apnd) / gap;
                            loop_ub = b_v->size[1];
                            for (nm1d2 = 0; nm1d2 < loop_ub; nm1d2++) {
                                r->data[j + r->size[0] * nm1d2].re = a * b_v->data[xcnt +
                                    b_v->size[0] * nm1d2].re + ndbl * b_v->data[(xcnt + b_v->size
                                        [0] * nm1d2) + 1].re;
                                r->data[j + r->size[0] * nm1d2].im = a * b_v->data[xcnt +
                                    b_v->size[0] * nm1d2].im + ndbl * b_v->data[(xcnt + b_v->size
                                        [0] * nm1d2) + 1].im;
                            }

                            guard1 = 1;
                        }
                        else {
                            exitg1 = 1;
                        }
                    }
                    else {
                        guard1 = 1;
                    }

                    if (guard1) {
                        nm1d2 = n;
                        md2++;
                    }
                }
            }
        }
        else {
             Init_int32 (&r1, 1);
            i1 = r1->size[0];
            r1->size[0] = y->size[1];
             EnsureCapacity_int32 (r1, i1);
            loop_ub = y->size[1];
            for (i1 = 0; i1 < loop_ub; i1++) {
                r1->data[i1] = (int)y->data[i1] - 1;
            }

             Init_creal (&r2, 1);
            i1 = r2->size[0];
            r2->size[0] = y->size[1];
             EnsureCapacity_creal (r2, i1);
            loop_ub = y->size[1];
            for (i1 = 0; i1 < loop_ub; i1++) {
                r2->data[i1].re = 1.0;
                r2->data[i1].im = 0.0;
            }

            loop_ub = b_v->size[1];
            i1 = v->size[0] * v->size[1];
            v->size[0] = x->size[0];
            v->size[1] = b_v->size[1];
             EnsureCapacity_creal (v, i1);
            for (i1 = 0; i1 < loop_ub; i1++) {
                nm1d2 = x->size[0];
                for (i2 = 0; i2 < nm1d2; i2++) {
                    v->data[i2 + i1] = b_v->data[((int)x->data[i2] + b_v->size[0] * i1) -
                        1];
                }
            }

            loop_ub = v->size[1];
            for (i1 = 0; i1 < loop_ub; i1++) {
                nm1d2 = r2->size[0];
                for (i2 = 0; i2 < nm1d2; i2++) {
                    r->data[r1->data[i2] + r->size[0] * i1].re = r2->data[i2].re * v->
                        data[i1].re - r2->data[i2].im * v->data[i1].im;
                    r->data[r1->data[i2] + r->size[0] * i1].im = r2->data[i2].re * v->
                        data[i1].im + r2->data[i2].im * v->data[i1].re;
                }
            }

             Free_creal (&r2);
             Free_int32 (&r1);
        }

         Free_creal (&b_v);
         Free_real (&x);
         Free_real (&y);
        if (do_trans) {
            m = r->size[0] - 1;
            n = r->size[1];
            md2 = r->size[0] >> 1;
            for (j = 0; j < n; j++) {
                for (loop_ub = 0; loop_ub < md2; loop_ub++) {
                    ndbl = r->data[loop_ub + r->size[0] * j].re;
                    apnd = r->data[loop_ub + r->size[0] * j].im;
                    i1 = m - loop_ub;
                    r->data[loop_ub + r->size[0] * j] = r->data[i1 + r->size[0] * j];
                    r->data[i1 + r->size[0] * j].re = ndbl;
                    r->data[i1 + r->size[0] * j].im = apnd;
                }
            }
        }

        loop_ub = r->size[1];
        for (i1 = 0; i1 < loop_ub; i1++) {
            nm1d2 = r->size[0];
            for (i2 = 0; i2 < nm1d2; i2++) {
                hEst->data[((i + i2) + hEst->size[0] * i1) - 1] = r->data[i2 + r->size[0]
                    * i1];
            }
        }

         Free_creal (&r);
        if (1.0 != locOFDMWithRS->data[0]) {
            i = (int)(locOFDMWithRS->data[0] - 1.0);
            for (md2 = 0; md2 < i; md2++) {
                gap = locOFDMWithRS->data[0] - ((double)md2 + 1.0);
                a = locOFDMWithRS->data[1] - locOFDMWithRS->data[0];
                loop_ub = hEst->size[1] - 1;
                i1 = v->size[0] * v->size[1];
                v->size[0] = 1;
                v->size[1] = loop_ub + 1;
                 EnsureCapacity_creal (v, i1);
                for (i1 = 0; i1 <= loop_ub; i1++) {
                    nm1d2 = (int)locOFDMWithRS->data[0] - 1;
                    m = (int)locOFDMWithRS->data[1] - 1;
                    ndbl = (hEst->data[nm1d2 + hEst->size[0] * i1].re - hEst->data[m +
                        hEst->size[0] * i1].re) * gap;
                    cdiff = (hEst->data[nm1d2 + hEst->size[0] * i1].im - hEst->data[m +
                        hEst->size[0] * i1].im) * gap;
                    if (cdiff == 0.0) {
                        apnd = ndbl / a;
                        ndbl = 0.0;
                    }
                    else if (ndbl == 0.0) {
                        apnd = 0.0;
                        ndbl = cdiff / a;
                    }
                    else {
                        apnd = ndbl / a;
                        ndbl = cdiff / a;
                    }

                    v->data[i1].re = hEst->data[nm1d2 + hEst->size[0] * i1].re + apnd;
                    v->data[i1].im = hEst->data[nm1d2 + hEst->size[0] * i1].im + ndbl;
                }

                loop_ub = v->size[1];
                for (i1 = 0; i1 < loop_ub; i1++) {
                    hEst->data[md2 + hEst->size[0] * i1] = v->data[i1];
                }
            }
        }

        if (numOFDM != locOFDMWithRS->data[locOFDMWithRS->size[0] - 1]) {
            i = (int)((double)numOFDM + (1.0 - (locOFDMWithRS->data
                [locOFDMWithRS->size[0] - 1] + 1.0)));
            for (md2 = 0; md2 < i; md2++) {
                i_ofdm = (locOFDMWithRS->data[locOFDMWithRS->size[0] - 1] + 1.0) +
                    (double)md2;
                gap = i_ofdm - locOFDMWithRS->data[locOFDMWithRS->size[0] - 1];
                a = locOFDMWithRS->data[locOFDMWithRS->size[0] - 1] -
                    locOFDMWithRS->data[locOFDMWithRS->size[0] - 2];
                loop_ub = hEst->size[1] - 1;
                i1 = v->size[0] * v->size[1];
                v->size[0] = 1;
                v->size[1] = loop_ub + 1;
                 EnsureCapacity_creal (v, i1);
                for (i1 = 0; i1 <= loop_ub; i1++) {
                    ndbl = (hEst->data[((int)locOFDMWithRS->data[locOFDMWithRS->size[0] -
                        1] + hEst->size[0] * i1) - 1].re - hEst->data
                        [((int)locOFDMWithRS->data[locOFDMWithRS->size[0] - 2] +
                            hEst->size[0] * i1) - 1].re) * gap;
                    cdiff = (hEst->data[((int)locOFDMWithRS->data[locOFDMWithRS->size[0] -
                        1] + hEst->size[0] * i1) - 1].im - hEst->data
                        [((int)locOFDMWithRS->data[locOFDMWithRS->size[0] - 2] +
                            hEst->size[0] * i1) - 1].im) * gap;
                    if (cdiff == 0.0) {
                        apnd = ndbl / a;
                        ndbl = 0.0;
                    }
                    else if (ndbl == 0.0) {
                        apnd = 0.0;
                        ndbl = cdiff / a;
                    }
                    else {
                        apnd = ndbl / a;
                        ndbl = cdiff / a;
                    }

                    v->data[i1].re = hEst->data[((int)locOFDMWithRS->data
                        [locOFDMWithRS->size[0] - 1] + hEst->size[0] * i1) - 1].re + apnd;
                    v->data[i1].im = hEst->data[((int)locOFDMWithRS->data
                        [locOFDMWithRS->size[0] - 1] + hEst->size[0] * i1) - 1].im + ndbl;
                }

                loop_ub = v->size[1];
                for (i1 = 0; i1 < loop_ub; i1++) {
                    hEst->data[((int)i_ofdm + hEst->size[0] * i1) - 1] = v->data[i1];
                }
            }
        }
    }

     Free_creal (&v);
}
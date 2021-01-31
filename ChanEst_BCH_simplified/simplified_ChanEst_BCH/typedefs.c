#include "datadefs.h"
#include "typedefs.h"
#include <math.h>

/*
�����ļ�鹤����
	1.�������������ݴ������Ķ�Ӧ
	2.���ݽṹ�Ƿ����Ҫ��
	3.�Ƿ��м����Free
	4.׫дTest_Main���Ժ������ܣ�����ͨ��txt�ı����������ķ�ʽ������
*/

struct_creal crealAdd(struct_creal a, struct_creal b)
{
	struct_creal result;
	result.re = a.re + b.re;
	result.im = a.im + b.im;

	return result;
}

struct_creal crealMinus(struct_creal a, struct_creal b)
{
	struct_creal result;
	result.re = a.re - b.re;
	result.im = a.im - b.im;

	return result;
}

struct_creal crealMulti(struct_creal a, struct_creal b)
{
	struct_creal result;
	result.re = a.re * b.re - a.im * b.im;
	result.im = a.re * b.im + a.im * b.re;

	return result;
}

struct_creal crealDiv(struct_creal a, struct_creal b)
{
	struct_creal result;
	if (b.re * b.re + b.im * b.im != 0)
	{
		result.re = (a.re * b.re + a.im * b.im) / (b.re * b.re + b.im * b.im);
		result.im = (a.im * b.re - a.re * b.im) / (b.re * b.re + b.im * b.im);
	}
	else
	{
		result.re = 0;
		result.im = 0;
	}


	return result;
}

struct_creal crealExp(struct_creal a)
{
	struct_creal result;
	result.re = exp(a.re) * cos(a.im);
	result.im = exp(a.re) * sin(a.im);

	return result;
}

double crealNorm(struct_creal a)
{
	double out;
	out = sqrt(pow(a.re, 2) + pow(a.im, 2));
	
	return out;
}
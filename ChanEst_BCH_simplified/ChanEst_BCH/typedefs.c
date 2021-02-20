#pragma once
#include "typedefs.h"

/*
�����ļ�鹤����
	1.�������������ݴ������Ķ�Ӧ
	2.���ݽṹ�Ƿ����Ҫ��
	3.�Ƿ��м����Free
	4.׫дTest_Main���Ժ������ܣ�����ͨ��txt�ı����������ķ�ʽ������
*/
/*
�����ӷ�
���룺��������
��������������������
*/
struct_complex complexAdd(struct_complex a, struct_complex b)
{
	struct_complex result;
	result.re = a.re + b.re;
	result.im = a.im + b.im;

	return result;
}

/*
��������
���룺��������
�������������һ���������ڶ�������
*/
struct_complex complexMinus(struct_complex a, struct_complex b)
{
	struct_complex result;
	result.re = a.re - b.re;
	result.im = a.im - b.im;

	return result;
}

/*
�����˷�
���룺��������
������������������˷�
*/
struct_complex complexMulti(struct_complex a, struct_complex b)
{
	struct_complex result;
	result.re = a.re * b.re - a.im * b.im;
	result.im = a.re * b.im + a.im * b.re;

	return result;
}

/*
��������
���룺��������
�������������һ���������Եڶ�������
*/
struct_complex complexDiv(struct_complex a, struct_complex b)
{
	struct_complex result;
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

/*
������eָ��
���룺����
�����������������eָ��
*/
struct_complex complexExp(struct_complex a)
{
	struct_complex result;
	result.re = exp(a.re) * cos(a.im);
	result.im = exp(a.re) * sin(a.im);

	return result;
}

/*
����ģ
���룺����
�����double��������ģ
*/
double complexNorm(struct_complex a)
{
	double out;
	out = sqrt(pow(a.re, 2) + pow(a.im, 2));
	
	return out;
}

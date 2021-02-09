#include "datadefs.h"
#include "typedefs.h"
#include <math.h>

/*
后续的检查工作：
	1.数据类型与数据处理函数的对应
	2.数据结构是否符合要求
	3.是否将中间变量Free
	4.撰写Test_Main测试函数功能，可以通过txt文本或者其他的方式逐层测试
*/

struct_complex complexAdd(struct_complex a, struct_complex b)
{
	struct_complex result;
	result.re = a.re + b.re;
	result.im = a.im + b.im;

	return result;
}

struct_complex complexMinus(struct_complex a, struct_complex b)
{
	struct_complex result;
	result.re = a.re - b.re;
	result.im = a.im - b.im;

	return result;
}

struct_complex complexMulti(struct_complex a, struct_complex b)
{
	struct_complex result;
	result.re = a.re * b.re - a.im * b.im;
	result.im = a.re * b.im + a.im * b.re;

	return result;
}

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

struct_complex complexExp(struct_complex a)
{
	struct_complex result;
	result.re = exp(a.re) * cos(a.im);
	result.im = exp(a.re) * sin(a.im);

	return result;
}

double complexNorm(struct_complex a)
{
	double out;
	out = sqrt(pow(a.re, 2) + pow(a.im, 2));
	
	return out;
}
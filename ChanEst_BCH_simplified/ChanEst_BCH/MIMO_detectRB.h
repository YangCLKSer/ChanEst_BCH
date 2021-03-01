#pragma once

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "typedefs.h"
#include "Array_Ctrl.h"
#include <math.h>
#include <string.h>

/*******************************************************************************
*功能：最大比合并或者匹配滤波
*参数：
* Received：复数，接收信号
* mimoCH：复数，信道参数
* Detected：FLOAT_TYPE，检测符号
* ampd：FLOAT_TYPE，信道幅度信息
*返回值：无
*说明：无
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.1
*更改历史
*	日期          更改者		内容
*	2021.02.20   杨昌林		V1.0
*	2021.03.02   杨昌林		V1.1：宏定义浮点数类型
********************************************************************************/
extern void mrc(struct_complex Detected_RB[240], FLOAT_TYPE ampd[240], const struct_complex RxData[240], const struct_complex equCH[960]);

/*******************************************************************************
*功能：SFBC最大似然检测
*参数：
* Received：FLOAT_TYPE，接收信号
* mimoCH：FLOAT_TYPE，信道参数
* Detected：FLOAT_TYPE，检测符号
* ampd：FLOAT_TYPE，信道幅度信息
*返回值：无
*说明：内部调用版
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.1
*更改历史
*	日期          更改者		内容
*	2021.02.20   杨昌林		V1.0
*	2021.03.02   杨昌林		V1.1：宏定义浮点数类型
********************************************************************************/
extern void b_mld2sfbc(const FLOAT_TYPE Received[120], const FLOAT_TYPE mimoCH[240],
    FLOAT_TYPE Detected[120], FLOAT_TYPE ampd[120]);

/*******************************************************************************
*功能：SFBC最大似然检测
*参数：
* Received：复数，接收信号
* mimoCH：复数，信道参数
* Detected：FLOAT_TYPE，检测符号
* ampd：FLOAT_TYPE，信道幅度信息
*返回值：无
*说明：无
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.1
*更改历史
*	日期          更改者		内容
*	2021.02.20   杨昌林		V1.0
*	2021.03.02   杨昌林		V1.1：宏定义浮点数类型
********************************************************************************/
extern void mld2sfbc(const struct_complex Received[240], const struct_complex mimoCH[960],
    FLOAT_TYPE Detected[240], FLOAT_TYPE ampd[240]);

/*******************************************************************************
*功能：SFBC-FSTD最大似然检测
*参数：
* Received：复数，接收信号
* mimoCH：复数，信道参数
* Detected：FLOAT_TYPE，检测符号
* ampd：FLOAT_TYPE，信道幅度信息
*返回值：无
*说明：无
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.1
*更改历史
*	日期          更改者		内容
*	2021.02.20   杨昌林		V1.0
*	2021.03.02   杨昌林		V1.1：宏定义浮点数类型
********************************************************************************/
extern void mld4sfbcfstd(const struct_complex Received[240], const struct_complex mimoCH[960],
    FLOAT_TYPE Detected[240], FLOAT_TYPE ampd[240]);


/*******************************************************************************
*功能：相同子帧相同频率的两个资源块内数据的检测
*参数：
* RxData: 复数，资源解映射后的接收数据
* equCH: 复数，等效信道
* methodDetect: int，检测算法选择, 整数，0, 1, 2。0：最大比合并或者匹配滤波；1：SFBC的检测；2：SFBC-FSTD的检测
* mod_mode: int，调制方式, 4, 16, 64
* Detected_RB: 复数，检测出的符号
* ampd: FLOAT_TYPE，幅度信息，用于QAM解调
*返回值：无
*说明：由于算法有限，未使用mod_mode，省略。methodDetect为1，2时未经过测试。
*编写者：杨昌林
*编写日期：2021.02.20
*版本号：V1.1
*更改历史
*	日期          更改者		内容
*	2021.02.20   杨昌林		V1.0
*	2021.03.02   杨昌林		V1.1：宏定义浮点数类型
********************************************************************************/
extern void MIMO_detectRB(struct_complex Detected_RB[240], FLOAT_TYPE ampd[240], const struct_complex RxData[240], const struct_complex equCH[960], int
    methodDetect);

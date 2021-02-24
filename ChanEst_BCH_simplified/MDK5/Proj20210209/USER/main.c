//#include "main.h"
#include "stm32f4xx_hal.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"

#include <stdlib.h>
#include "ChanEst.h"
#include "BCHIndices.h"
#include "PBCH_Decode.h"

void CellSearch(void);

/************************************************
 ALIENTEK 阿波罗STM32F429开发板实验14
 LTDC LCD实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com  
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

int main(void)
{
	u8 x=0,len;
	u16 count=0,times=0;
	u8 lcd_id[12],test_num[20];
	//ARRAY_complex* rxSigFreq;
	
	HAL_Init();											//初始化HAL库
	Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
	delay_init(180);                //初始化延时函数
	uart_init(115200);              //初始化USART
	LED_Init();                     //初始化LED 
	KEY_Init();                     //初始化按键
	SDRAM_Init();                   //SDRAM初始化
	LCD_Init();                     //LCD初始化
	
	POINT_COLOR=RED; 
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。	
	while(1)
	{
		LED0=!LED0;	 
		delay_ms(10);	
		count++;
		x = 1;
		switch(x)
		{
			case 0:LCD_Clear(WHITE);break;
			case 1:LCD_Clear(BLACK);break;
			default:LCD_Clear(WHITE);
		}
		POINT_COLOR=BLACK;	  
		LCD_ShowString(10,40,260,32,32,"Apollo STM32F4/F7"); 	
		sprintf((char*)test_num,"ChanEst TEST:%d",count);
		LCD_ShowString(10,80,240,24,24,test_num);
		LCD_ShowString(10,110,240,16,16,"ATOM@ALIENTEK");
		LCD_ShowString(10,130,240,16,16,lcd_id);		//显示LCD ID	      					 
		LCD_ShowString(10,150,240,12,12,"2021/2/16");
		printf("\r\n%s\r\n",test_num);
		
		
		
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("\r\n您发送的消息为:\r\n");
			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART_RX_BUF,len,1000);	//发送接收到的数据
			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结束
			printf("\r\n\r\n");//插入换行
			USART_RX_STA=0;
		}else
		{
			times++;
			if(times%5000==0)
			{
				printf("\r\nALIENTEK 阿波罗STM32F429开发板 串口实验\r\n");
				printf("正点原子@ALIENTEK\r\n\r\n\r\n");
			}
			if(times%200==0)printf("请输入数据,以回车键结束\r\n");  
			if(times%30==0)LED0=!LED0;//闪烁LED,提示系统正在运行.
			delay_ms(10);   
		} 
		
		CellSearch();
		
	}
}


void CellSearch(void)
{
	int i,j,loop_ub;
	int a,b;
	struct_ENB ENB;
	ARRAY_complex* chanEst, * rxSigFreq;
	ARRAY_int32* bch_ind;
	double bch_bits[480];
	struct_complex pbch_symbols[240];
	int sfmod4;
	int MIB[24];
	struct_complex bchRx[240];
	struct_complex bchHest[960];
	
	LCD_ShowString(10,165,300,12,12,"Start Up.");
	printf("Start Up.\r\n");
	delay_ms(1000);
	Init_complex(&rxSigFreq, 2);
	i = rxSigFreq->size[0] * rxSigFreq->size[1];
	rxSigFreq->size[0] = 72;
	rxSigFreq->size[1] = 14;
	loop_ub = rxSigFreq->size[0];
	EnsureCapacity_complex(rxSigFreq, i);
	for (i = 0; i < loop_ub; i++) {
		rxSigFreq->data[i].re = 0.0;
		rxSigFreq->data[i].im = 0.0;
	}

	ENB.NDLRB = 6;
	strcpy(ENB.DuplexMode, "TDD");
	strcpy(ENB.CyclicPrefix, "Normal");
	ENB.NCellID = 85;
	ENB.NSubframe = 0;
	ENB.TDDConfig = 0;
	ENB.SSC = 0;
	ENB.CellRefP = 4;
	strcpy(ENB.PHICHDuration, "");
	strcpy(ENB.Ng, "");
	ENB.NFrame = 0;
	ENB.CFI = 0;

	/*
	FILE* fp;
	if ((fp = fopen("C:\\1D\\1SEU\\2020.09_USRP_E310\\C\\C_proj\\ChanEst_BCH_simplified\\simplified_ChanEst_BCH\\data\\rxSigFreq.txt", "r")) == NULL)
	{
		//printf("??????\r\n");
		//exit(0);
	}
	else
	{
		//printf("??????\r\n");

		for (i = 0; i < 72 * 2; i++)
		{
			for (j = 0; j < 14; j++)
			{
				if (i < 72)
				{
					//fscanf(fp, "%lf\r\n", &rxSigFreq->data[rxSigFreq->size[1] * i + j].re);
					//printf("%.6lf,\r\n", rxSigFreq->data[rxSigFreq->size[1] * i + j].re);
				}
				else
				{
					//fscanf(fp, "%lf\r\n", &rxSigFreq->data[rxSigFreq->size[1] * (i - 72) + j].im);
					//printf("%.6lf,\r\n", rxSigFreq->data[rxSigFreq->size[1] * (i - 72) + j].im);
				}
			}
		}
	}
	fclose(fp);*/
	
	
	Init_complex(&chanEst, 3);
	ChannelEst(chanEst, rxSigFreq, 0, 0, 0, 0, &ENB);
	LCD_ShowString(10,180,300,12,12,"ChannelEst Done.");
	printf("ChannelEst Done.\r\n");
	
	delay_ms(10);
	//printf("chanEst\r\n");
	//Print_complex(chanEst);
	Init_int32(&bch_ind, 2);
	BCHIndices(bch_ind, &ENB);
	LCD_ShowString(10,210,300,12,12,"BCHIndices Done.");
	printf("BCHIndices Done.\r\n");
	
	delay_ms(10);
	//printf("bch_ind\r\n");
	//Print_int32(bch_ind);
	
	for (i = 0; i < chanEst->size[1]; i++) {
		for (j = 0; j < 240; j++)
		{
			bchHest[240 * i + j] = chanEst->data[chanEst->size[2] * i + bch_ind->data[j] - 1];
		}
	}

	for (i = 0; i < bch_ind->size[1]; i++)
	{
		a = ((bch_ind->data[i] - 1) / rxSigFreq->size[0]);
		b = ((bch_ind->data[i] - 1) % rxSigFreq->size[0]);
		bchRx[i] = rxSigFreq->data[b * rxSigFreq->size[1] + a];
	}
	LCD_ShowString(10,230,240,12,12,"PBCH_Decode Start.");	
	printf("PBCH_Decode Start.\r\n");
	PBCH_Decode(bch_bits, pbch_symbols, &sfmod4, MIB, &(ENB.CellRefP), bchRx, bchHest, &ENB);
	LCD_ShowString(10,250,240,12,12,"PBCH_Decode Done.");	
	printf("PBCH_Decode Done.\r\n");
	printf("\r\nMIB:");
	for(i=0;i<24;i++)
	{
		printf("%d ",MIB[i]);
	}
	printf("\r\n");
	
	Free_complex(&rxSigFreq);
	Free_complex(&chanEst);
	Free_int32(&bch_ind);
}

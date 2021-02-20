#include "main.h"
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
	u8 x=0;
	u16 count=0;
	u8 lcd_id[12],test_num[20];
	struct_ENB ENB;
	int i, j, loop_ub;
	int a, b;
	ARRAY_complex* chanEst, * rxSigFreq;
	ARRAY_int32* bch_ind;
	double bch_bits[480];
	struct_complex pbch_symbols[240];
	int sfmod4;
	int MIB[24];
	struct_complex bchRx[240];
	struct_complex bchHest[960];
	
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
		count++;
		x = 12;
		switch(x)
		{
			case 0:LCD_Clear(WHITE);break;
			case 1:LCD_Clear(BLACK);break;
			case 2:LCD_Clear(BLUE);break;
			case 3:LCD_Clear(RED);break;
			case 4:LCD_Clear(MAGENTA);break;
			case 5:LCD_Clear(GREEN);break;
			case 6:LCD_Clear(CYAN);break; 
			case 7:LCD_Clear(YELLOW);break;
			case 8:LCD_Clear(BRRED);break;
			case 9:LCD_Clear(GRAY);break;
			case 10:LCD_Clear(LGRAY);break;
			case 11:LCD_Clear(BROWN);break;
			default:LCD_Clear(WHITE);
		}
		POINT_COLOR=RED;	  
		LCD_ShowString(10,40,260,32,32,"Apollo STM32F4/F7"); 	
		sprintf((char*)test_num,"ChanEst TEST:%d",count);
		LCD_ShowString(10,80,240,24,24,test_num);
		LCD_ShowString(10,110,240,16,16,"ATOM@ALIENTEK");
		LCD_ShowString(10,130,240,16,16,lcd_id);		//显示LCD ID	      					 
		LCD_ShowString(10,150,240,12,12,"2021/2/16");	      					 
		x++;
		if(x==12)
			x=0;      
		LED0=!LED0;	 
		delay_ms(1000);	
		
		LCD_ShowString(10,165,300,12,12,"Start Up.");
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
			//printf("??????\n");
			//exit(0);
		}
		else
		{
			//printf("??????\n");

			for (i = 0; i < 72 * 2; i++)
			{
				for (j = 0; j < 14; j++)
				{
					if (i < 72)
					{
						//fscanf(fp, "%lf\n", &rxSigFreq->data[rxSigFreq->size[1] * i + j].re);
						//printf("%.6lf,\n", rxSigFreq->data[rxSigFreq->size[1] * i + j].re);
					}
					else
					{
						//fscanf(fp, "%lf\n", &rxSigFreq->data[rxSigFreq->size[1] * (i - 72) + j].im);
						//printf("%.6lf,\n", rxSigFreq->data[rxSigFreq->size[1] * (i - 72) + j].im);
					}
				}
			}
		}
		fclose(fp);*/
		
		
		Init_complex(&chanEst, 3);
		ChannelEst(chanEst, rxSigFreq, 0, 0, 0, 0, &ENB);
		LCD_ShowString(10,180,300,12,12,"ChannelEst Done.");
		delay_ms(10);
		//printf("chanEst\n");
		//Print_complex(chanEst);
		Init_int32(&bch_ind, 2);
		BCHIndices(bch_ind, &ENB);
		LCD_ShowString(10,210,300,12,12,"BCHIndices Done.");
		delay_ms(10);
		//printf("bch_ind\n");
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
		PBCH_Decode(bch_bits, pbch_symbols, &sfmod4, MIB, &(ENB.CellRefP), bchRx, bchHest, &ENB);
		LCD_ShowString(10,250,240,12,12,"PBCH_Decode Done.");	
		
		
		Free_complex(&rxSigFreq);
		Free_complex(&chanEst);
		Free_int32(&bch_ind);
	}
}

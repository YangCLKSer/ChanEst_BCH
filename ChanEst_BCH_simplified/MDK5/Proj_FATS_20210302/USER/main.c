#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "string.h"
#include "sdram.h"
#include "malloc.h"
#include "w25qxx.h"
#include "ff.h"
#include "exfuns.h"
#include "string.h"
#include "usmart.h"
#include "sdio_sdcard.h"
#include "ftl.h" 

#include <stdlib.h>
//#include <stdio.h>
#include "ChanEst.h"
#include "BCHIndices.h"
#include "PBCH_Decode.h"

void CellSearch(ARRAY_complex* rxSigFreq);
void data_read(ARRAY_complex* rxSigFreq);
/************************************************
 ALIENTEK 阿波罗STM32F429开发板实验41
 FATFS实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com  
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
    
int main(void)
{
 	u32 total,free;
	u32 t=0;	
	u8 res=0;
	u16 i,j,loop_ub;
	u8 test_num[20],lcd_id[12];
	ARRAY_complex* rxSigFreq;
	
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
	
	HAL_Init();                     //初始化HAL库   
	Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
	delay_init(180);                //初始化延时函数
	uart_init(115200);              //初始化USART
	usmart_dev.init(90);
	LED_Init();                     //初始化LED 
	KEY_Init();                     //初始化按键
	SDRAM_Init();                   //SDRAM初始化
	LCD_Init();                     //LCD初始化
	W25QXX_Init();				    //初始化W25Q256
	my_mem_init(SRAMIN);		    //初始化内部内存池
	my_mem_init(SRAMEX);		    //初始化外部内存池
	my_mem_init(SRAMCCM);		    //初始化CCM内存池 
	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7"); 
	LCD_ShowString(30,70,200,16,16,"FATFS TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/1/16");	 	 
	LCD_ShowString(30,130,200,16,16,"Use USMART for test");	      
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);
 	while(SD_Init())//检测不到SD卡
	{
		LCD_ShowString(30,150,200,16,16,"SD Card Error!");
		delay_ms(500);					
		LCD_ShowString(30,150,200,16,16,"Please Check! ");
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
	}
	FTL_Init();
 	exfuns_init();							//为fatfs相关变量申请内存				 
	f_mount(fs[0],"0:",1); 					//挂载SD卡 
 	res=f_mount(fs[1],"1:",1); 				//挂载FLASH.	
	if(res==0X0D)//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
	{
		LCD_ShowString(30,150,200,16,16,"Flash Disk Formatting...");	//格式化FLASH
		res=f_mkfs("1:",1,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:ALIENTEK");	//设置Flash磁盘的名字为：ALIENTEK
			LCD_ShowString(30,150,200,16,16,"Flash Disk Format Finish");	//格式化完成
		}else LCD_ShowString(30,150,200,16,16,"Flash Disk Format Error ");	//格式化失败
		delay_ms(1000);
	}	
	res=f_mount(fs[2],"2:",1); 				//挂载NAND FLASH.	
	if(res==0X0D)//NAND FLASH磁盘,FAT文件系统错误,重新格式化NAND FLASH
	{
		LCD_ShowString(30,150,200,16,16,"NAND Disk Formatting...");//格式化NAND
		res=f_mkfs("2:",1,4096);//格式化FLASH,2,盘符;1,不需要引导区,8个扇区为1个簇
		if(res==0)
		{
			f_setlabel((const TCHAR *)"2:NANDDISK");	//设置Flash磁盘的名字为：NANDDISK
			LCD_ShowString(30,150,200,16,16,"NAND Disk Format Finish");		//格式化完成
		}else LCD_ShowString(30,150,200,16,16,"NAND Disk Format Error ");	//格式化失败
		delay_ms(1000);
	}    
	LCD_Fill(30,150,240,150+16,WHITE);		//清除显示			  
	while(exf_getfree("0:",&total,&free))	//得到SD卡的总容量和剩余容量
	{
		LCD_ShowString(30,150,200,16,16,"SD Card Fatfs Error!");
		delay_ms(200);
		LCD_Fill(30,150,240,150+16,WHITE);	//清除显示			  
		delay_ms(200);
		LED0=!LED0;//DS0闪烁
	}													  			    
 	POINT_COLOR=BLUE;//设置字体为蓝色	   
	LCD_ShowString(30,150,200,16,16,"FATFS OK!");	 
	LCD_ShowString(30,170,200,16,16,"SD Total Size:     MB");	 
	LCD_ShowString(30,190,200,16,16,"SD  Free Size:     MB"); 	    
 	LCD_ShowNum(30+8*14,170,total>>10,5,16);	//显示SD卡总容量 MB
 	LCD_ShowNum(30+8*14,190,free>>10,5,16);     //显示SD卡剩余容量 MB			    
	
	FRESULT file_result;
	FIL * fp;
	file_result = mf_open("rxSigFreq.txt", FA_READ);
	if (file_result)//0为成功
	{
		printf("File Open Failed\r\n");
		//exit(0);
	}
	else
	{
		printf("File Opened\r\n");
		data_read(rxSigFreq);
	}
	mf_close();
	printf("File Closed.\r\n");
	switch(1)
	{
		case 0:LCD_Clear(WHITE);break;
		case 1:LCD_Clear(BLACK);break;
		default:LCD_Clear(WHITE);
	}
	while(1)
	{
		t++; 
		//delay_ms(200);		 			   
		LED0=!LED0;
		
		POINT_COLOR=BLACK;
		LCD_Fill(10,10,309,100,WHITE);
		sprintf((char*)test_num,"ChanEst TEST:%d",t);
		LCD_ShowString(10,10,300,24,24,test_num);
		printf("\r\n%s\r\n",test_num);
		
		CellSearch(rxSigFreq);
	} 
	
	Free_complex(&rxSigFreq);
}

void data_read(ARRAY_complex* rxSigFreq)
{
	int count=0,len=0,i;
	char str[1000],*p_str,*p_r,num[20];
	FLOAT_TYPE mid;
	p_str = str;
	len = 0;
	while(count<2010)
	{
		//将缓存中字符串复制到待处理字符串尾
		mf_read(512);
		strcpy(str+len,fatbuf);
		len += 512;
		//这时p_str指向字符串头，即改为数字头指针
		p_str = str;
		//处理到待处理字符串长度小于20，则再读取
		while(len>20 && count<2010)
		{
			
			//找到数字末尾，并将当前数字复制到num中进行转换
			p_r = strchr(p_str,'\r');
			for(i=0;i<p_r-p_str;i++)
			{
				num[i] = p_str[i];
			}
			num[p_r-p_str]='\0';
			//字符串结构为”数字\r\n数字“因此\r指针+2后指向下一个数字
			len -= p_r-p_str+2;
			p_str = p_r+2;
			
			//矩阵赋值
			mid = (FLOAT_TYPE)atof(num);
			//printf("%e\r\n",mid);
			if (count<1008)
			{
				rxSigFreq->data[count].re = mid;
			}
			else
			{
				rxSigFreq->data[count-1008].im = mid;
			}
			count++;
		}
		//将未处理字符移动到str前端
		strcpy(str,p_str);
	}
	
	//处理最后的字符串
	mf_read(512);
	strcpy(str+len,fatbuf);
	len += 512;
	//这时p_str指向字符串头，即改为数字头指针
	p_str = str;
	while(count<2016)
	{
		//这时p_str指向字符串头，即改为数字头指针
		//找到数字末尾，并将当前数字复制到num中进行转换
		p_r = strchr(p_str,'\r');
		for(i=0;i<p_r-p_str;i++)
		{
			num[i] = p_str[i];
		}
		num[p_r-p_str]='\0';
		//字符串结构为”数字\r\n数字“因此\r指针+2后指向下一个数字
		len -= p_r-p_str+2;
		p_str = p_r+2;
		
		//矩阵赋值
		mid = (FLOAT_TYPE)atof(num);
		//printf("%e\r\n",mid);
		if (count<1008)
		{
			rxSigFreq->data[count].re = mid;
		}
		else
		{
			rxSigFreq->data[count-1008].im = mid;
		}
		count++;
	}
	//Print_complex(rxSigFreq);
}

void CellSearch(ARRAY_complex* rxSigFreq)
{
	int i,j,loop_ub;
	int a,b;
	struct_ENB ENB;
	ARRAY_complex* chanEst;
	ARRAY_int32* bch_ind;
	FLOAT_TYPE bch_bits[480];
	struct_complex pbch_symbols[240];
	int sfmod4;
	int MIB[24];
	struct_complex bchRx[240];
	struct_complex bchHest[960];
	u8 str[50],num_str[10];
	
	LCD_ShowString(10,40,300,12,12,"Start Up.");
	printf("Start Up.\r\n");
	//delay_ms(1000);

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
	
	
	Init_complex(&chanEst, 3);
	ChannelEst(chanEst, rxSigFreq, 0, 0, 0, 0, &ENB);
	LCD_ShowString(10,55,300,12,12,"ChannelEst Done.");
	printf("ChannelEst Done.\r\n");
	
	//delay_ms(10);
	//printf("chanEst\r\n");
	//Print_complex(chanEst);
	Init_int32(&bch_ind, 2);
	BCHIndices(bch_ind, &ENB);
	LCD_ShowString(10,70,300,12,12,"BCHIndices Done.");
	printf("BCHIndices Done.\r\n");
	
	//delay_ms(10);
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
	PBCH_Decode(bch_bits, pbch_symbols, &sfmod4, MIB, &(ENB.CellRefP), bchRx, bchHest, &ENB);
	LCD_ShowString(10,85,300,12,12,"PBCH_Decode Done.");	
	printf("PBCH_Decode Done.\r\n");
	printf("\r\nMIB:");
	sprintf(str,"MIB:");
	for(i=0;i<24;i++)
	{
		printf("%d ",MIB[i]);
		sprintf(num_str,"%d ",MIB[i]);
		strcat(str,num_str);
	}
	LCD_ShowString(10,100,300,12,12,str);
	printf("\r\n");
	
	
	Free_complex(&chanEst);
	Free_int32(&bch_ind);
}

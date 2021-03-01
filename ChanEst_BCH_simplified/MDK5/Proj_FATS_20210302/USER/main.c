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
 ALIENTEK ������STM32F429������ʵ��41
 FATFSʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
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
	
	HAL_Init();                     //��ʼ��HAL��   
	Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
	delay_init(180);                //��ʼ����ʱ����
	uart_init(115200);              //��ʼ��USART
	usmart_dev.init(90);
	LED_Init();                     //��ʼ��LED 
	KEY_Init();                     //��ʼ������
	SDRAM_Init();                   //SDRAM��ʼ��
	LCD_Init();                     //LCD��ʼ��
	W25QXX_Init();				    //��ʼ��W25Q256
	my_mem_init(SRAMIN);		    //��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		    //��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);		    //��ʼ��CCM�ڴ�� 
	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7"); 
	LCD_ShowString(30,70,200,16,16,"FATFS TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/1/16");	 	 
	LCD_ShowString(30,130,200,16,16,"Use USMART for test");	      
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);
 	while(SD_Init())//��ⲻ��SD��
	{
		LCD_ShowString(30,150,200,16,16,"SD Card Error!");
		delay_ms(500);					
		LCD_ShowString(30,150,200,16,16,"Please Check! ");
		delay_ms(500);
		LED0=!LED0;//DS0��˸
	}
	FTL_Init();
 	exfuns_init();							//Ϊfatfs��ر��������ڴ�				 
	f_mount(fs[0],"0:",1); 					//����SD�� 
 	res=f_mount(fs[1],"1:",1); 				//����FLASH.	
	if(res==0X0D)//FLASH����,FAT�ļ�ϵͳ����,���¸�ʽ��FLASH
	{
		LCD_ShowString(30,150,200,16,16,"Flash Disk Formatting...");	//��ʽ��FLASH
		res=f_mkfs("1:",1,4096);//��ʽ��FLASH,1,�̷�;1,����Ҫ������,8������Ϊ1����
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:ALIENTEK");	//����Flash���̵�����Ϊ��ALIENTEK
			LCD_ShowString(30,150,200,16,16,"Flash Disk Format Finish");	//��ʽ�����
		}else LCD_ShowString(30,150,200,16,16,"Flash Disk Format Error ");	//��ʽ��ʧ��
		delay_ms(1000);
	}	
	res=f_mount(fs[2],"2:",1); 				//����NAND FLASH.	
	if(res==0X0D)//NAND FLASH����,FAT�ļ�ϵͳ����,���¸�ʽ��NAND FLASH
	{
		LCD_ShowString(30,150,200,16,16,"NAND Disk Formatting...");//��ʽ��NAND
		res=f_mkfs("2:",1,4096);//��ʽ��FLASH,2,�̷�;1,����Ҫ������,8������Ϊ1����
		if(res==0)
		{
			f_setlabel((const TCHAR *)"2:NANDDISK");	//����Flash���̵�����Ϊ��NANDDISK
			LCD_ShowString(30,150,200,16,16,"NAND Disk Format Finish");		//��ʽ�����
		}else LCD_ShowString(30,150,200,16,16,"NAND Disk Format Error ");	//��ʽ��ʧ��
		delay_ms(1000);
	}    
	LCD_Fill(30,150,240,150+16,WHITE);		//�����ʾ			  
	while(exf_getfree("0:",&total,&free))	//�õ�SD������������ʣ������
	{
		LCD_ShowString(30,150,200,16,16,"SD Card Fatfs Error!");
		delay_ms(200);
		LCD_Fill(30,150,240,150+16,WHITE);	//�����ʾ			  
		delay_ms(200);
		LED0=!LED0;//DS0��˸
	}													  			    
 	POINT_COLOR=BLUE;//��������Ϊ��ɫ	   
	LCD_ShowString(30,150,200,16,16,"FATFS OK!");	 
	LCD_ShowString(30,170,200,16,16,"SD Total Size:     MB");	 
	LCD_ShowString(30,190,200,16,16,"SD  Free Size:     MB"); 	    
 	LCD_ShowNum(30+8*14,170,total>>10,5,16);	//��ʾSD�������� MB
 	LCD_ShowNum(30+8*14,190,free>>10,5,16);     //��ʾSD��ʣ������ MB			    
	
	FRESULT file_result;
	FIL * fp;
	file_result = mf_open("rxSigFreq.txt", FA_READ);
	if (file_result)//0Ϊ�ɹ�
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
		//���������ַ������Ƶ��������ַ���β
		mf_read(512);
		strcpy(str+len,fatbuf);
		len += 512;
		//��ʱp_strָ���ַ���ͷ������Ϊ����ͷָ��
		p_str = str;
		//�����������ַ�������С��20�����ٶ�ȡ
		while(len>20 && count<2010)
		{
			
			//�ҵ�����ĩβ��������ǰ���ָ��Ƶ�num�н���ת��
			p_r = strchr(p_str,'\r');
			for(i=0;i<p_r-p_str;i++)
			{
				num[i] = p_str[i];
			}
			num[p_r-p_str]='\0';
			//�ַ����ṹΪ������\r\n���֡����\rָ��+2��ָ����һ������
			len -= p_r-p_str+2;
			p_str = p_r+2;
			
			//����ֵ
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
		//��δ�����ַ��ƶ���strǰ��
		strcpy(str,p_str);
	}
	
	//���������ַ���
	mf_read(512);
	strcpy(str+len,fatbuf);
	len += 512;
	//��ʱp_strָ���ַ���ͷ������Ϊ����ͷָ��
	p_str = str;
	while(count<2016)
	{
		//��ʱp_strָ���ַ���ͷ������Ϊ����ͷָ��
		//�ҵ�����ĩβ��������ǰ���ָ��Ƶ�num�н���ת��
		p_r = strchr(p_str,'\r');
		for(i=0;i<p_r-p_str;i++)
		{
			num[i] = p_str[i];
		}
		num[p_r-p_str]='\0';
		//�ַ����ṹΪ������\r\n���֡����\rָ��+2��ָ����һ������
		len -= p_r-p_str+2;
		p_str = p_r+2;
		
		//����ֵ
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

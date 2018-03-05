#ifndef __MYCT_IIC_H
#define __MYCT_IIC_H

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//���ݴ�����-IIC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/30
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//********************************************************************************
//��
////////////////////////////////////////////////////////////////////////////////// 	

#include <stdint.h>
#include "cpu_peri_gpio.h"
//IO��������
#define CT_SDA_IN()  {GPIOI->MODER&=~(3<<(3*2));GPIOI->MODER|=0<<3*2;}	//PI3����ģʽ
#define CT_SDA_OUT() {GPIOI->MODER&=~(3<<(3*2));GPIOI->MODER|=1<<3*2;} 	//PI3���ģʽ
//IO��������	 
#define CT_IIC_SCL(n) (n?GPIO_SettoHigh(GPIO_H,PIN6):GPIO_SettoLow(GPIO_H,PIN6))//SCL
#define CT_IIC_SDA(n) (n?GPIO_SettoHigh(GPIO_I,PIN3):GPIO_SettoLow(GPIO_I,PIN3))//SDA
#define CT_READ_SDA   (GPIO_GetData(GPIO_I)&PIN3)//����SDA
 
#define FT_RST(n)  (n?GPIO_SettoHigh(GPIO_I,PIN8):GPIO_SettoLow(GPIO_I,PIN8))
#define FT_INT      (GPIO_GetData(GPIO_H)&PIN7)

//IIC���в�������
void CT_IIC_Init(void);                	//��ʼ��IIC��IO��				 
void CT_IIC_Start(void);				//����IIC��ʼ�ź�
void CT_IIC_Stop(void);	  				//����IICֹͣ�ź�
void CT_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 CT_IIC_Read_Byte(unsigned char ack);	//IIC��ȡһ���ֽ�
u8 CT_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void CT_IIC_Ack(void);					//IIC����ACK�ź�
void CT_IIC_NAck(void);					//IIC������ACK�ź�

#endif








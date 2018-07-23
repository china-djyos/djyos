// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��AD7616.c
// ģ������: AD7616�ɼ�ģ��ĵײ�������������SPI�ӿ�ͨ�ź�����ȡAD�ɼ�ֵ
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 09/04.2015
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "endian.h"
#include "djyos.h"
#include "spibus.h"
#include "cpu_peri.h"
//#include "iodefine.h"

static const Pin ADRST[] = {
    {PIO_PB5, PIOB, ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT},//ADRST
};

static const Pin ADSH[] = {
    {PIO_PB4, PIOB, ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT},//ADSH
};

static const Pin ADBUSY[] = {
    {PIO_PA16, PIOA, ID_PIOA, PIO_INPUT, PIO_DEFAULT},//ADBUSY
};
static const Pin ADCS[] = {
        {PIO_PA31, PIOA, ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT},//ADCS
};
// =============================================================================
#define AD7616_SSP_BAUD     (20000000)      //AD7616��SSP�����ٶ�
#define AD7616_SSP_TIMEOUT  (5*1000)        //AD7616 ��ʱ����Ϊ5mS
#define AD7616_CHANNEL_NUM  (16)            //AD7616ͨ������

struct SPI_Device *s_ptAD7616_Dev;

// =============================================================================
// ���ܣ�AD7616��CPU�ӿں���������SPIͨ�ţ�Ӳ��ע����������Ƭѡ
// ������wBuf,д������
//      wLen,д���ݳ��ȣ��ֽڵ�λ
//      rBuf,��������
//      rLen,���ֽ���
// ���أ���
// =============================================================================
void __AD7616_SpiInterfalce(u8* wBuf,u16 wLen,u8* rBuf,u16 rLen)
{
    struct SPI_DataFrame frame;

    frame.RecvBuf = rBuf;
    frame.RecvLen = rLen;
    frame.RecvOff = wLen;
    frame.SendBuf = wBuf;
    frame.SendLen = wLen;

    PIO_Clear(ADCS);     // ����Ƭѡ�ź�
    SPI_Transfer(s_ptAD7616_Dev,&frame,1,AD7616_SSP_TIMEOUT);
    PIO_Set(ADCS);        // ����Ƭѡ�ź�
}
// =============================================================================
// ���ܣ���ʼ��AD7616�������������úͳ�ʼ�����ã��ӿڵĳ�ʼ����
// ��������
// ���أ���
// =============================================================================
bool_t AD7616_Init(void)
{
    PMC_EnablePeripheral(ID_PIOA);
    PMC_EnablePeripheral(ID_PIOB);

    //��PB4 ��PB5����Ϊͨ��IO
    MATRIX->CCFG_SYSIO |= CCFG_SYSIO_SYSIO4 | CCFG_SYSIO_SYSIO5;

    PIO_Configure(ADRST,PIO_LISTSIZE(ADRST));
    PIO_Configure(ADSH,PIO_LISTSIZE(ADSH));
    PIO_Configure(ADBUSY,PIO_LISTSIZE(ADBUSY));

    PIO_Clear(ADRST);

    Djy_DelayUs(2*mS);
    PIO_Clear(ADSH);                //����=0������ADת��
    PIO_Set(ADRST);                 //��ʼ������RST״̬
    Djy_DelayUs(16*mS);             //FULL RESET >= 15mS
    // ���³�ʼ����SSP���
    //CS��ʼ��
    PIO_Configure(ADCS,1);
    PIO_Set(ADCS);

    //SSP��������ʼ��
    if(s_ptAD7616_Dev = SPI_DevAdd("SPI0","AD7616",CN_SPINCS1,8,SPI_MODE_3,SPI_SHIFT_MSB,AD7616_SSP_BAUD,false))
    {
        SPI_BusCtrl(s_ptAD7616_Dev,CN_SPI_SET_POLL,0,0);
        return true;
    }
    else
    {
        printf("\r\n: error : device : AD7616 init failed.\n\r");
        return false;
    }
}

// =============================================================================
// ���ܣ�ֹͣADת����ͨ������IO��ƽ�ķ�ʽ����
// ��������
// ���أ���
// =============================================================================
//void __AD7616_Stop(void)
//{
//    PIO_Clear(ADSH);
//}

// =============================================================================
// ���ܣ�����AD��ʼ������ͨ������IO���ŵĸߵ͵�ƽ�����ж�BUSYλ
// ��������
// ���أ���
// =============================================================================
void AD7616_Start(void)
{
    u32 i = 0;
    PIO_Set(ADSH);
//    while(!PIO_Get(ADBUSY));
    for(i = 0; i < 0x2; i++)    //AD7616�����⣬����ʱ100nsʱ�䣬����ֹͣCONV
    {

    }
    PIO_Clear(ADSH);
//  __AD7616_Stop();
}

// =============================================================================
// ���ܣ��ж�AD7616�Ƿ��Ѿ�ת��������AD7616�����ù�������ʱ��Ϊ4us��64�����Ĺ��������ʱ��
//      Ϊ315us
// ��������
// ���أ���
// =============================================================================
static bool_t _AD7616_ConvFinished(void)
{
    u32 timeout = 10;

//  Djy_DelayUs(2);         //��������������ʱ����315uS

    while(timeout)
    {
        if(!PIO_Get(ADBUSY))
            break;
        Djy_DelayUs(1);
        timeout--;
    }
    if(timeout)
        return true;
    else
        return false;
}

// =============================================================================
// ���ܣ���ȡAD7616�Ĳɼ�ֵ������������ת����ת���������ٶ�ȡADת����ֵ
// ��������
// ���أ���
// =============================================================================
bool_t AD7616_GetValue(uint16_t *AinBuf)
{
    bool_t result = false;
    u16 i;

//  __AD7616_Start();
    if(_AD7616_ConvFinished())
    {
//          __AD7616_Stop();
        __AD7616_SpiInterfalce(NULL,0,(u8*)AinBuf,AD7616_CHANNEL_NUM*2);
        result = true;
#if(CN_CFG_BYTE_ORDER == CN_CFG_LITTLE_ENDIAN)   //С����Ҫת��
        for(i = 0; i < AD7616_CHANNEL_NUM; i ++)
        {
            AinBuf[i] = swaps(AinBuf[i]);
        }
#endif
        return true;
    }
    else
        return false;
}


// =============================================================================
// ���ܣ���ȡAD7616�Ĳɼ�ֵ����������ת����ת���������ٶ�ȡADת����ֵ
// ��������
// ���أ���
// =============================================================================
bool_t AD7616_Convert(uint16_t *AinBuf)
{
    bool_t result = false;
    u16 i;

  __AD7616_Start();
    if(_AD7616_ConvFinished())
    {
//          __AD7616_Stop();
        __AD7616_SpiInterfalce(NULL,0,(u8*)AinBuf,AD7616_CHANNEL_NUM*2);
        result = true;
#if(CN_CFG_BYTE_ORDER == CN_CFG_LITTLE_ENDIAN)   //С����Ҫת��

        for(i = 0; i < AD7616_CHANNEL_NUM; i ++)
        {
            AinBuf[i] = swaps(AinBuf[i]);
        }
#endif
        return true;
    }
    else
        return false;
}


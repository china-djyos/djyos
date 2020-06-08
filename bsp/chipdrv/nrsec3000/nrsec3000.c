//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// =============================================================================

// �ļ���     ��nrsec3000.c
// ģ������: ����оƬnrsec3000ģ������
// ģ��汾: V1.10
// ������Ա:
// ����ʱ��:
// =============================================================================

#include <stdlib.h>
#include "stdint.h"
#include "stdio.h"
#include "object.h"
#include "lock.h"
#include "stdlib.h"
#include <device.h>
#include "string.h"
#include "djybus.h"
#include "spibus.h"
#include "systime.h"
#include "djyos.h"
#include "nrsec3000.h"
#include "cpu_peri.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t NRSEC3000_Init(void);
//    NRSEC3000_Init();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"encrypt 2 nrsec300"//spi�ӿڵĹ���2оƬ
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","spi bus","cpu onchip spi"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_ENCRYPT_2_NRSEC300 == false )
//#warning  " encrypt_2_nrsec300  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_ENCRYPT_2_NRSEC300    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,500000,50000000,
#define CFG_NRSEC3000_SPI_SPEED      (20000*1000)           //"SPI�����ٶ�"����������ͨ���ٶ�
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#define CFG_NRSEC3000_BUS_NAME              ""              //"��������",NRSEC3000���ڵ�SPI��������
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

static u32 s_NRSEC3000_Timeout = CN_TIMEOUT_FOREVER;
static struct SPI_Device *s_ptNRSEC3000_Dev;
//#define CFG_NRSEC3000_SPI_SPEED      (20000*1000)



static const u8 crc7_table[256]=
{
    0x00,0x09,0x12,0x1b,0x24,0x2d,0x36,0x3f,
    0x48,0x41,0x5a,0x53,0x6c,0x65,0x7e,0x77,
    0x19,0x10,0x0b,0x02,0x3d,0x34,0x2f,0x26,
    0x51,0x58,0x43,0x4a,0x75,0x7c,0x67,0x6e,
    0x32,0x3b,0x20,0x29,0x16,0x1f,0x04,0x0d,
    0x7a,0x73,0x68,0x61,0x5e,0x57,0x4c,0x45,
    0x2b,0x22,0x39,0x30,0x0f,0x06,0x1d,0x14,
    0x63,0x6a,0x71,0x78,0x47,0x4e,0x55,0x5c,
    0x64,0x6d,0x76,0x7f,0x40,0x49,0x52,0x5b,
    0x2c,0x25,0x3e,0x37,0x08,0x01,0x1a,0x13,
    0x7d,0x74,0x6f,0x66,0x59,0x50,0x4b,0x42,
    0x35,0x3c,0x27,0x2e,0x11,0x18,0x03,0x0a,
    0x56,0x5f,0x44,0x4d,0x72,0x7b,0x60,0x69,
    0x1e,0x17,0x0c,0x05,0x3a,0x33,0x28,0x21,
    0x4f,0x46,0x5d,0x54,0x6b,0x62,0x79,0x70,
    0x07,0x0e,0x15,0x1c,0x23,0x2a,0x31,0x38,
    0x41,0x48,0x53,0x5a,0x65,0x6c,0x77,0x7e,
    0x09,0x00,0x1b,0x12,0x2d,0x24,0x3f,0x36,
    0x58,0x51,0x4a,0x43,0x7c,0x75,0x6e,0x67,
    0x10,0x19,0x02,0x0b,0x34,0x3d,0x26,0x2f,
    0x73,0x7a,0x61,0x68,0x57,0x5e,0x45,0x4c,
    0x3b,0x32,0x29,0x20,0x1f,0x16,0x0d,0x04,
    0x6a,0x63,0x78,0x71,0x4e,0x47,0x5c,0x55,
    0x22,0x2b,0x30,0x39,0x06,0x0f,0x14,0x1d,
    0x25,0x2c,0x37,0x3e,0x01,0x08,0x13,0x1a,
    0x6d,0x64,0x7f,0x76,0x49,0x40,0x5b,0x52,
    0x3c,0x35,0x2e,0x27,0x18,0x11,0x0a,0x03,
    0x74,0x7d,0x66,0x6f,0x50,0x59,0x42,0x4b,
    0x17,0x1e,0x05,0x0c,0x33,0x3a,0x21,0x28,
    0x5f,0x56,0x4d,0x44,0x7b,0x72,0x69,0x60,
    0x0e,0x07,0x1c,0x15,0x2a,0x23,0x38,0x31,
    0x46,0x4f,0x54,0x5d,0x62,0x6b,0x70,0x79
};

tagEncryptionCtrl g_tEncryptionCtrl;
tagEncryptionCtrl *ptagEncryptionCtrl=&g_tEncryptionCtrl;

void Cmd_DelayMs(u8 cmd)
{

    if(cmd==0xb3)
        DJY_EventDelay(1300);
    if(cmd==0xb4)
        DJY_EventDelay(2300);
    else
        DJY_EventDelay(30);
}

/* =============================================================================
 ���ܣ�SPI������Ҫ�õ�������������Ƭѡ�������ߺͷ��ͽ���
 ������NRSEC3000_TxRx�Ĳ���Ҫע��
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
static void NRSEC3000_CsActive(void)
{
    SPI_CsActive(s_ptNRSEC3000_Dev,s_NRSEC3000_Timeout);
}
static void NRSEC3000_CsInActive(void)
{
    SPI_CsInactive(s_ptNRSEC3000_Dev);
}
static u32 NRSEC3000_TxRx(u8* sdata,u32 slen,u8* rdata, u32 rlen,u32 RecvOff)
{
    struct SPI_DataFrame data;
    s32 result;

    data.RecvBuf = rdata;
    data.RecvLen = rlen;
    data.RecvOff = RecvOff;
    data.SendBuf = sdata;
    data.SendLen = slen;

    result = SPI_Transfer(s_ptNRSEC3000_Dev,&data,true,s_NRSEC3000_Timeout);
    if(result != CN_SPI_EXIT_NOERR)
        return 0;
    return 1;
}

/* =============================================================================
 ���ܣ���ȡcrc7
 ��������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
u8 get_crc7(const u8* buff, u16 len)
{
    uint32_t j;
    u8 crc7_accum=0;

    for(j=0;j<len;j++)
      {
        crc7_accum=crc7_table[(crc7_accum<<1)^buff[j]];
        }
    return crc7_accum;
}

/* =============================================================================
 ���ܣ�����INS������շ���ֵ
 ������Ԥ�ڽ��յ���INS����ֵ
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
bool_t Receive_Response(u8 cmd)
{
    uint32_t i;


    for (i=0;i<500;i++)
    {
        ptagEncryptionCtrl->RxBuffer[i]=0x00;
    }

     for(i=0;i<500;i++)
    {

        NRSEC3000_CsActive();//Ƭѡ
//      for (byTmp1=0;byTmp1<200;byTmp1++) ; //��ʱ
//      DJY_EventDelay(10);
        DJY_DelayUs(100);
        NRSEC3000_TxRx(ptagEncryptionCtrl->TxBuffer1+i,1,ptagEncryptionCtrl->RxBuffer+i,1,0);

        NRSEC3000_CsInActive();//��Ƭѡ
//      for (byTmp1=0;byTmp1<200;byTmp1++) ; //��ʱ
//      DJY_EventDelay(10);
        DJY_DelayUs(100);



        if((cmd==ptagEncryptionCtrl->RxBuffer[i])&&(i>5))
        {
          return 1;
        }

        if(i>=499)
        {
         printf("!!!Error,����ģ��δ��Ӧ\r\n");
         return 0;
        }
    }
    return 0;
}


/* =============================================================================
 ���ܣ��������ݣ����շ���ֵ
 ������Ԥ�ڽ��յ�������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
bool_t Receive_Data(u8 cmd, u16 length,u16 *len)
{
   uint32_t i;


   for(i=0;i<5000;i++)
   {
       NRSEC3000_CsActive();//Ƭѡ
//     for (byTmp1=0;byTmp1<200;byTmp1++) ; //��ʱ
//     DJY_EventDelay(10);
       DJY_DelayUs(100);

       NRSEC3000_TxRx(ptagEncryptionCtrl->TxBuffer2+i,1,ptagEncryptionCtrl->RxBuffer+i,1,0);


       NRSEC3000_CsInActive();//��Ƭѡ
//     for (byTmp1=0;byTmp1<200;byTmp1++) ; //��ʱ
//     DJY_EventDelay(10);
       DJY_DelayUs(100);
      if((200==i)&&(0xe0!=cmd))
      Cmd_DelayMs(cmd);

      if((0x90==ptagEncryptionCtrl->RxBuffer[i-1])&&(0x00==ptagEncryptionCtrl->RxBuffer[i])&&(0xe0!=cmd))
      {
        break;
      }
   }

    for(i=0;i<5000;i++)
    {
       if((ptagEncryptionCtrl->RxBuffer[i]==cmd)&&(ptagEncryptionCtrl->RxBuffer[i+length+3]==0x90)&&(ptagEncryptionCtrl->RxBuffer[i+length+4]==0x00))
       {
            *len=i;
            return 1;
        }

       if((0x90==ptagEncryptionCtrl->RxBuffer[i-1])&&(0x00==ptagEncryptionCtrl->RxBuffer[i])&&(0xe0!=cmd))
       {
            return 1;
       }
     }

    printf("\r\nδ�յ�����%d\r\n",i);
    return 0;

}



bool_t New_Receive_Data(u8 cmd, u16 length,u16 *len,u16 num)
{

    uint32_t i;

      for(i=0;i<num;i++)
      {
         NRSEC3000_CsActive();//Ƭѡ
         DJY_DelayUs(100);
         NRSEC3000_TxRx(ptagEncryptionCtrl->TxBuffer2+i,1,ptagEncryptionCtrl->RxBuffer+i,1,0);
         NRSEC3000_CsInActive();//��Ƭѡ
         DJY_DelayUs(100);
      }
     DJY_EventDelay(1000);
      for(i=num;;i++)
      {
         NRSEC3000_CsActive();//Ƭѡ
         DJY_DelayUs(1000);
         NRSEC3000_TxRx(ptagEncryptionCtrl->TxBuffer2+num,1,ptagEncryptionCtrl->RxBuffer+i,1,0);
         NRSEC3000_CsInActive();//��Ƭѡ
         DJY_DelayUs(1000);

        if( (0x90==ptagEncryptionCtrl->RxBuffer[i-1]) && (0x00==ptagEncryptionCtrl->RxBuffer[i]) )
                     break;
         if(i==5000)
         {
            return 0;
         }
      }

        for(i=0;i<5000;i++)
        {
           if((ptagEncryptionCtrl->RxBuffer[i]==cmd)&&(ptagEncryptionCtrl->RxBuffer[i+length+3]==0x90)&&(ptagEncryptionCtrl->RxBuffer[i+length+4]==0x00))
           {
                ptagEncryptionCtrl->len=i;
                return 1;
            }

         if( (0x90==ptagEncryptionCtrl->RxBuffer[i-1]) && (0x00==ptagEncryptionCtrl->RxBuffer[i]) )
         {
             return 1;
         }

        }

        return 0;
}












/* =============================================================================
 ���ܣ�SM1���ܵ���Գ���Կkey
 ��������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
bool_t Import_SM1_Key()
{
    u32 i=0,j=0;

//=============================================================================
//��䷢�ͻ�����1������ݣ��ֲ��еĹ���1������2����������ͷ�͵ȴ�����
    ptagEncryptionCtrl->TxBuffer1[0] = 0x80;
    ptagEncryptionCtrl->TxBuffer1[1] = 0xd4;
    ptagEncryptionCtrl->TxBuffer1[2] = 0x01;
    ptagEncryptionCtrl->TxBuffer1[3] = 0x00;
    ptagEncryptionCtrl->TxBuffer1[4] = 0x10;

    for(i=5;i<200;i++)
    {
      ptagEncryptionCtrl->TxBuffer1[i]=0xaa;
    }

//=============================================================================
//��䷢�ͻ�����2������ݣ��ֲ��еĹ���3������4�������������ݱ�־��key��crc
    ptagEncryptionCtrl->TxBuffer2[0]=0x55;
    for(j=1;j<=16;j++)
    {
        ptagEncryptionCtrl->TxBuffer2[j]= ptagEncryptionCtrl->SM1_Key[j-1];
    }
    ptagEncryptionCtrl->TxBuffer2[17]=get_crc7(ptagEncryptionCtrl->TxBuffer2+1,16);

//=============================================================================
//��䷢�ͻ�����2һֱ����aaֱ������״̬��90 00��ȷ
    for(i=18;i<5000;i++)
    {
        ptagEncryptionCtrl->TxBuffer2[i] = 0xaa;
    }

     for(i=0; i<5000; i++)
     {
         ptagEncryptionCtrl->RxBuffer[i]=0;//���ջ��������
     }

//=============================================================================
//�����ͻ�����1�������ͳ�ȥ���ȴ����յ�������д��d4
     if(Receive_Response(0xd4))
     {
         DJY_EventDelay(1000);
           Receive_Data(0xd4, 0x80,&ptagEncryptionCtrl->len);
       }




//=============================================================================
//����6��ʼ��ָ�����

     ptagEncryptionCtrl->TxBuffer1[0] = 0x80;
     ptagEncryptionCtrl->TxBuffer1[1] = 0xd4;
     ptagEncryptionCtrl->TxBuffer1[2] = 0x02;
     ptagEncryptionCtrl->TxBuffer1[3] = 0x00;
     ptagEncryptionCtrl->TxBuffer1[4] = 0x10;
     for(i=5;i<200;i++)
     {
         ptagEncryptionCtrl->TxBuffer1[i]=0xaa;
     }

//=============================================================================
//����8���������
     ptagEncryptionCtrl->TxBuffer2[0]=0x55;
     for(j=1;j<=16;j++)
     {
         ptagEncryptionCtrl->TxBuffer2[j]= 0x00;
     }
      ptagEncryptionCtrl->TxBuffer2[17]=get_crc7(ptagEncryptionCtrl->TxBuffer2+1,16);
      for(i=18;i<5000;i++)
      {
          ptagEncryptionCtrl->TxBuffer2[i] = 0xaa;
      }


       for(i=0; i<5000; i++)
       {
           ptagEncryptionCtrl->RxBuffer[i]=0;

       }
       if(Receive_Response(0xd4))
       {
           DJY_EventDelay(1000);
             if(Receive_Data(0xd4, 0x80,&ptagEncryptionCtrl->len))
             return 1;
         }

        return 0;

}




/* =============================================================================
 ���ܣ�SM1���ܵ����ʼ����IV
 ��������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
bool_t Import_IV()
{
    uint32_t i,j;

    ptagEncryptionCtrl->TxBuffer1[0] = 0x80;
    ptagEncryptionCtrl->TxBuffer1[1] = 0xd4;
    ptagEncryptionCtrl->TxBuffer1[2] = 0x04;
    ptagEncryptionCtrl->TxBuffer1[3] = 0x00;
    ptagEncryptionCtrl->TxBuffer1[4] = 0x10;
    for(i=5;i<200;i++)
    {
      ptagEncryptionCtrl->TxBuffer1[i]=0xaa;
    }

//=============================================================================
//����3��4���������,16�ֽڵ�IV��Ϊʲô��ʮ����1����
    ptagEncryptionCtrl->TxBuffer2[0]=0x55;
    for(j=1;j<=16;j++)
    {
        //ptagEncryptionCtrl->TxBuffer2[j]= ptagEncryptionCtrl->SM1_IV[j-1];
        ptagEncryptionCtrl->TxBuffer2[j]= 0x01;
    }
     ptagEncryptionCtrl->TxBuffer2[17]=get_crc7(ptagEncryptionCtrl->TxBuffer2+1,16);
     for(i=18;i<5000;i++)
     {
         ptagEncryptionCtrl->TxBuffer2[i] = 0xaa;
     }

//=============================================================================
//����������
      for(i=0; i<5000; i++)
      {
          ptagEncryptionCtrl->RxBuffer[i]=0;
      }
//=============================================================================
//����ָ�������
      if(Receive_Response(0xd4))
      {
          DJY_EventDelay(1000);
        if(Receive_Data(0xd4, 0x80,&ptagEncryptionCtrl->len))
                return 1;
        }
       return 0;
}


/* =============================================================================
 ���ܣ�SM1����
 ������Ҫ���ܵ����ݺ����ݳ���
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
bool_t SM1_Encrypt(u8 *Txbuffer,u16 InLenth)
{
      uint32_t i;

      ptagEncryptionCtrl->TxBuffer1[0] = 0xa0;
      ptagEncryptionCtrl->TxBuffer1[1] = 0xe0;
      ptagEncryptionCtrl->TxBuffer1[2] = 0x80;
      ptagEncryptionCtrl->TxBuffer1[3] = (InLenth>>8);
      ptagEncryptionCtrl->TxBuffer1[4] = (InLenth);
      for(i=5;i<200;i++)
      {
      ptagEncryptionCtrl->TxBuffer1[i] = 0xaa;
      }

      ptagEncryptionCtrl->TxBuffer2[0] = 0x55;
      for(i=1;i<=InLenth;i++)
      {
       ptagEncryptionCtrl->TxBuffer2[i]=Txbuffer[i-1];
      }

      ptagEncryptionCtrl->TxBuffer2[InLenth+1]=get_crc7(ptagEncryptionCtrl->TxBuffer2+1,InLenth);

      for(i=InLenth+2;i<5000;i++)
      {
        ptagEncryptionCtrl->TxBuffer2[i]= 0xaa;
      }
//���ͻ�����2��������0x55+inlenth������+crc+ʣ�¶���0xaa
//=============================================================================

      for(i=0; i<5000; i++)
      {

        ptagEncryptionCtrl->RxBuffer[i]=0;

      }

      if(Receive_Response(0xe0))
      {
          DJY_EventDelay(1000);
        if(Receive_Data(0xe0, InLenth+1,&ptagEncryptionCtrl->len))
        {
            memcpy(ptagEncryptionCtrl->SM1_EncryptNumber,ptagEncryptionCtrl->RxBuffer+ptagEncryptionCtrl->len+3,InLenth);
            return 1;
        }
     }
return 0;
}


/* =============================================================================
 ���ܣ�SM1����
 ������Ҫ���ܵ����ݺ����ݳ���
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
bool_t SM1_Decrypt(u8 *Txbuffer,u16 InLenth)
{
    uint32_t i;

    ptagEncryptionCtrl->TxBuffer1[0] = 0xa0;
    ptagEncryptionCtrl->TxBuffer1[1] = 0xe0;
    ptagEncryptionCtrl->TxBuffer1[2] = 0x81;
    ptagEncryptionCtrl->TxBuffer1[3] = InLenth>>8;
    ptagEncryptionCtrl->TxBuffer1[4] = InLenth;
    for(i=5;i<200;i++)
    {
    ptagEncryptionCtrl->TxBuffer1[i] = 0xaa;
    }
//���ͻ�����1��������CMDͷ+ʣ�¶���0xaa
//=============================================================================
    ptagEncryptionCtrl->TxBuffer2[0] = 0x55;
    for(i=1;i<=InLenth;i++)
    {
     ptagEncryptionCtrl->TxBuffer2[i]=Txbuffer[i-1];
    }

    ptagEncryptionCtrl->TxBuffer2[InLenth+1]=get_crc7(ptagEncryptionCtrl->TxBuffer2+1,InLenth);

    for(i=InLenth+2;i<5000;i++)
    {
      ptagEncryptionCtrl->TxBuffer2[i]= 0xaa;
    }
//���ͻ�����2��������0x55+inlenth������+crc+ʣ�¶���0xaa
//=============================================================================

    for(i=0; i<5000; i++)
    {
      ptagEncryptionCtrl->RxBuffer[i]=0;
    }

    if(Receive_Response(0xe0))
    {
        DJY_EventDelay(1000);
      if(Receive_Data(0xe0, InLenth+1,&ptagEncryptionCtrl->len))
      {
          memcpy(ptagEncryptionCtrl->SM1_TrueNumber,ptagEncryptionCtrl->RxBuffer+ptagEncryptionCtrl->len+3,InLenth);
          return 1;
      }

    }
    return 0;
}
//=============================================================================
//=============================================================================
//=============================================================================

/* =============================================================================
 ���ܣ�����SM2��Կ�ԣ���Ϊָ�������ݷ��ͽ��յĸ�ʽ��SM1��̫һ����������Լ���дһ�·��ͺ���
 ������P2=0x00
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
bool_t Key_Product()
{
    u8 spiTxBuf[500];
    u8 spiRxBuf[500];

    uint32_t i;


    spiTxBuf[0] = 0x80;
    spiTxBuf[1] = 0xb2;
    spiTxBuf[2] = 0x00;
    spiTxBuf[3] = 0x00;
    spiTxBuf[4] = 0x00;

    for (i=5;i<500;i++)
    {
        spiTxBuf[i]=0xaa;
    }
    for (i=0;i<500;i++)
    {
        spiRxBuf[i]=0;
    }

    for (i=0;i<500;i++)
    {
        NRSEC3000_CsActive();//Ƭѡ
//      for (byTmp1=0;byTmp1<30;byTmp1++) ; //��ʱ
        DJY_EventDelay(10);
//      DJY_DelayUs(100);
        NRSEC3000_TxRx(spiTxBuf+i,1,spiRxBuf+i,1,0);

        NRSEC3000_CsInActive();//��Ƭѡ
//      for (byTmp1=0;byTmp1<30;byTmp1++) ; //��ʱ
        DJY_EventDelay(10);
//      DJY_DelayUs(100);
        if ((spiRxBuf[i]==0x00)&&(spiRxBuf[i-1]==0x90))
        {
            return 1;
        }
    }
    return 0;
}

/* =============================================================================
 ���ܣ�����SM2��Կ
 ������
      1. ����ͷ CMD �� p2 Ϊ��Կ ������ ���μ� ���� SM2 SM2��Կ�� �е�˵��
      2. ����ͷ CMD �� p3 Ϊ �������յĹ�Կ���ݳ��ȣ�Ϊ �������յĹ�Կ���ݳ��ȣ�Ϊ �������յĹ�Կ���ݳ��ȣ�64 �ֽڣ�
      3. ���ճ��� LEN Ϊ 1�ֽ� ��ֵӦΪ 65 ��64 �ֽڵĹ�Կ �ֽڵĹ�Կ +1 �ֽڵ� CRC CRC��
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
bool_t Export_SM2_PublicKey(u8 p2)
{

   u8 spiTxBuf[500];
   u8 spiRxBuf[500];
   uint32_t i,j;

   uint8_t byTmp1;

   spiTxBuf[0] = 0x80;
   spiTxBuf[1] = 0xb8;
   spiTxBuf[2] = 0x01;
   spiTxBuf[3] = p2;
   spiTxBuf[4] = 0x40;

   for(i=5;i<500;i++)
   {
       spiTxBuf[i] = 0xaa;
   }
//���뻺����ΪCMDͷ+һ��0xaa
   for(i=0; i<500; i++)
   {
       spiRxBuf[i]=0;
   }

    for(i=0;i<500;i++)
    {
        NRSEC3000_CsActive();//Ƭѡ
        for (byTmp1=0;byTmp1<200;byTmp1++) ; //��ʱ
//      DJY_EventDelay(1000);
        NRSEC3000_TxRx(spiTxBuf+i,1,spiRxBuf+i,1,0);

        NRSEC3000_CsInActive();//��Ƭѡ
        for (byTmp1=0;byTmp1<200;byTmp1++) ; //��ʱ
//      DJY_EventDelay(1000);
    }

    for(i=5;i<500;i++)
    {
       if((spiRxBuf[i]==0xb8)&&(spiRxBuf[i+64+3]==0x90))
       {
           if(p2==0)
           {
             for(j=0; j<64; j++)
             {
                ptagEncryptionCtrl->SM2_00_PublicKey[j]=spiRxBuf[i+2];
                i++;
             }
           }
           if(p2==1)
           {
             for(j=0; j<64; j++)
             {
                ptagEncryptionCtrl->SM2_01_PublicKey[j]=spiRxBuf[i+2];
                i++;
             }
           }
        return 1;
        }
     }

    printf("\r\nδ��ȡ��װ�ù�Կ%d\r\n",i);
    return 0;
}

/* =============================================================================
 ���ܣ�����SM2˽Կ
 ������
      1. ����ͷ CMD �� p2 Ϊ��Կ�����ţ��μ� Ϊ��Կ�����ţ��μ� ���� SM2 SM2��Կ�� ��
      2. ����ͷ CMD �� p3 Ϊ�������յ� Ϊ�������յ� ˽Կ���ݵĳ��ȣ� Կ���ݵĳ��ȣ� ˽Կ���ݳ���Ϊ 32 �ֽڣ�
      3. ���ճ��� LEN Ϊ 1�ֽ� ��ֵӦΪ 33 ��32 �ֽڵ�˽Կ �ֽڵ�˽Կ +1 �ֽڵ� CRC CRC��
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
bool_t Export_SM2_PrivateKey(u8 p2)
{
     u8 spiTxBuf[500];
     u8 spiRxBuf[500];
     uint32_t i,j;
     //uint8_t byTmp1;



     spiTxBuf[0] = 0x80;
     spiTxBuf[1] = 0xb8;
     spiTxBuf[2] = 0x02;
     spiTxBuf[3] = p2;
     spiTxBuf[4] = 0x20;

     for(i=5;i<500;i++)
     {
         spiTxBuf[i] = 0xaa;
     }
//���뻺������䣺CMDͷ+һ��0xaa
     for(i=0; i<500; i++)
     {
         spiRxBuf[i]=0;
     }



//���з��ͺ�����һ��������Ƭѡ�����÷��ͽ���
          for(i=0;i<5;i++)
          {
             NRSEC3000_CsActive();//Ƭѡ
             DJY_DelayUs(100);
             NRSEC3000_TxRx(spiTxBuf+i,1,spiRxBuf+i,1,0);
             NRSEC3000_CsInActive();//��Ƭѡ
             DJY_DelayUs(100);
          }
         DJY_EventDelay(1000);
          for(i=5;;i++)
          {
             NRSEC3000_CsActive();//Ƭѡ
             DJY_DelayUs(1000);
             NRSEC3000_TxRx(spiTxBuf+5,1,spiRxBuf+i,1,0);
             NRSEC3000_CsInActive();//��Ƭѡ
             DJY_DelayUs(1000);

            if( (0x90==spiRxBuf[i-1]) && (0x00==spiRxBuf[i]) )
                         break;
             if(i==500)
             {
                return 0;
             }
          }

//���ݴ�������
      for(i=5;i<500;i++)
      {
         if((spiRxBuf[i]==0xb8)&&(spiRxBuf[i+32+3]==0x90))
         {
           for(j=0; j<32; j++)
           {
              ptagEncryptionCtrl->Client_PrivateKey[j]=spiRxBuf[i+2];
              i++;
           }
          return 1;
          }
       }
      printf("\r\nδ��ȡ��װ��˽Կ%d\r\n",i);
      return 0;
}

/* =============================================================================
 ���ܣ�����SM2��Կ
 ������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
bool_t Import_SM2_PublicKey(u8 p2)
{
    uint32_t i,j;

    //����SM2��Կ��64�ֽ�
    ptagEncryptionCtrl->TxBuffer1[0] = 0x80;
    ptagEncryptionCtrl->TxBuffer1[1] = 0xba;
    ptagEncryptionCtrl->TxBuffer1[2] = 0x01;
    ptagEncryptionCtrl->TxBuffer1[3] = p2;
    ptagEncryptionCtrl->TxBuffer1[4] = 0x40;

    for(i=5;i<150;i++)
    {
      ptagEncryptionCtrl->TxBuffer1[i]=0xaa;
    }
//���ͻ�����1��������CMDͷ��һ��0xaa
//���ͻ�����2�������˱�־λ0x55+64�ֽڹ�Կ+crc+һ��0xaa
    ptagEncryptionCtrl->TxBuffer2[0]=0x55;

    if(0==p2)
    {
        for(j=1;j<=64;j++)
        {
            ptagEncryptionCtrl->TxBuffer2[j] = ptagEncryptionCtrl->SM2_00_PublicKey[j-1];
        }
    }

    if(1==p2)
    {
        for(j=1;j<=64;j++)
        {
            ptagEncryptionCtrl->TxBuffer2[j] = ptagEncryptionCtrl->SM2_01_PublicKey[j-1];
        }
    }

    ptagEncryptionCtrl->TxBuffer2[65]=get_crc7(ptagEncryptionCtrl->TxBuffer2+1,64);

    for(i=66;i<5000;i++)
    {
        ptagEncryptionCtrl->TxBuffer2[i] = 0xaa;
    }


    for(i=0; i<5000; i++)
    {
        ptagEncryptionCtrl->RxBuffer[i]=0;
    }


    if(Receive_Response(0xba))
    {
         DJY_EventDelay(1000);
        if(Receive_Data(0xba, 0x80,&ptagEncryptionCtrl->len))
            return 1;
    }
        return 0;
}



/* =============================================================================
 ���ܣ�����SM2˽Կ
 ������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
bool_t Import_SM2_PrivateKey(u8 p2)
{
    uint32_t i,j;
    u8 pResult;

    ptagEncryptionCtrl->TxBuffer1[0] = 0x80;
    ptagEncryptionCtrl->TxBuffer1[1] = 0xba;
    ptagEncryptionCtrl->TxBuffer1[2] = 0x02;
    ptagEncryptionCtrl->TxBuffer1[3] = p2;
    ptagEncryptionCtrl->TxBuffer1[4] = 0x20;

    for(i=5;i<200;i++)
    {
      ptagEncryptionCtrl->TxBuffer1[i]=0xaa;
    }


    ptagEncryptionCtrl->TxBuffer2[0]=0x55;
    for(j=1;j<=32;j++)
    {
        ptagEncryptionCtrl->TxBuffer2[j]= ptagEncryptionCtrl->Client_PrivateKey[j-1];
     }

     ptagEncryptionCtrl->TxBuffer2[33]=get_crc7(ptagEncryptionCtrl->TxBuffer2+1,32);

     for(i=34;i<5000;i++)
     {
         ptagEncryptionCtrl->TxBuffer2[i] = 0xaa;
     }


      for(i=0; i<5000; i++)
      {
          ptagEncryptionCtrl->RxBuffer[i]=0;
      }

      pResult=Receive_Response(0xba);
      if(pResult)
      {
          DJY_EventDelay(1000);
          if(Receive_Data(0xba, 0x80,&ptagEncryptionCtrl->len))
              return 1;
     }
       return 0;
}

/* =============================================================================
 ���ܣ�SM3 hash
 �����������ǲ���Ҫ�ѽṹ�����Hash_Number[32];���и�ֵ
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
int NRSEC3000_sm3(u8 *Txbuffer,u16 Lenth, u8 *Rxbuffer)
{
  static unsigned char spiTxBuf[3000];
  static unsigned char spiRxBuf[3000];
  u8 *p,*q;
  uint32_t i,j;
  u16 len;

  //uint8_t byTmp1;

  p=Txbuffer;
  q=Rxbuffer;

   spiTxBuf[0] = 0x80;
   spiTxBuf[1] = 0xb5;
   spiTxBuf[2] = 0x00;
   spiTxBuf[3] = (Lenth>>8);
   spiTxBuf[4] = Lenth;

   for(i=5;i<150;i++)
   {
    spiTxBuf[i]=0xaa;
   }

   for(i=0; i<150; i++)

   {
       spiRxBuf[i]=0;
   }

   for(i=0;i<150;i++)
   {
       NRSEC3000_CsActive();//Ƭѡ
       DJY_DelayUs(1000); //��ʱ

       NRSEC3000_TxRx(spiTxBuf+i,1,spiRxBuf+i,1,0);

       NRSEC3000_CsInActive();//��Ƭѡ
       DJY_DelayUs(1000) ; //��ʱ

       if((0xb5==spiRxBuf[i])&&(i>5))
       {
           break;
       }

        if(i>=199)
        {
         printf("!!!Error,����ģ��δ��Ӧ\r\n");
         return 0;
        }
}

   spiTxBuf[0]=0x55;
   for(i=1;i<=Lenth;i++)
   {
      spiTxBuf[i]=*(p+i-1);//���ݽ�����Ҫ���͵�ǰlenth���ȵ����ݷ��뻺����
   }
   spiTxBuf[Lenth+1]=get_crc7(spiTxBuf+1,Lenth);//�ټ�һ��crc

   for(i=Lenth+2;i<3000;i++)
   {
      spiTxBuf[i]=0xaa;//�������0xaa
   }

      for(i=0; i<3000; i++)
     {
          spiRxBuf[i]=0;
     }


   for(i=0;i<3000;i++)
     {
       NRSEC3000_CsActive();//Ƭѡ
       DJY_DelayUs(100); //��ʱ

       NRSEC3000_TxRx(spiTxBuf+i,1,spiRxBuf+i,1,0);

       NRSEC3000_CsInActive();//��Ƭѡ
       DJY_DelayUs(100); //��ʱ

        if((0x90==spiRxBuf[i-1])&&(0x00==spiRxBuf[i]))
        {
          break;
        }
     }

   for(i=Lenth+2;i<3000;i++)
      {
         if((spiRxBuf[i]==0xb5))
         {
              len=i;
              for(j=0;j<32;j++)
              {
                    *(q+j)=spiRxBuf[j+len+2];//���յ������ݴ��ݻ�ȥ
              }
              return 0;
          }
       }

      printf("\r\nδ�յ�����%d\r\n",i);
      return 1;

}

/******************************������SM3 Hash ����***********************************************/
//������
//in: [����]����Ҫ����hash ��������ݵ��׵�ַ
//inl: [����]�����ݵĳ��ȣ��ֽ�����
//out: [���]������Hash ������ڴ��׵�ַ��sm3 �������Ϊ32 �ֽڣ�
//pubkey: [����]����Կ���ݣ�64 �ֽڣ�
//pucID: [����]���û���ʶ
//idl: [����]���û���ʶ�ĳ���
//���ؽ����  0���ɹ�
//            ������ʧ��
/*********************************************************************************************/
int sm3(unsigned char *in,int inl,unsigned char *out, unsigned char *pubkey, unsigned char
*pucID, int idl)
{
    int nRet,l;
    int i;
    unsigned char *Z = NULL;
       // unsigned char *Z1 = NULL;
    int entl = 0;
    unsigned char tmpm[32];
        unsigned char tmpm1[32];
    //unsigned char temp[1500];
    //unsigned char temp1[210];
    unsigned char abxy[32 * 4] =
    { 0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, /* a*/
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFC,
    0x28,0xE9,0xFA,0x9E,0x9D,0x9F,0x5E,0x34,0x4D,0x5A, /* b */
    0x9E,0x4B,0xCF,0x65,0x09,0xA7,0xF3,0x97,0x89,0xF5,
    0x15,0xAB,0x8F,0x92,0xDD,0xBC,0xBD,0x41,0x4D,0x94,
    0x0E,0x93,
    0x32,0xC4,0xAE,0x2C,0x1F,0x19,0x81,0x19,0x5F,0x99, /* x */
    0x04,0x46,0x6A,0x39,0xC9,0x94,0x8F,0xE3,0x0B,0xBF,
    0xF2,0x66,0x0B,0xE1,0x71,0x5A,0x45,0x89,0x33,0x4C,
    0x74,0xC7,
    0xBC,0x37,0x36,0xA2,0xF4,0xF6,0x77,0x9C,0x59,0xBD, /* y */
    0xCE,0xE3,0x6B,0x69,0x21,0x53,0xD0,0xA9,0x87,0x7C,
    0xC6,0x2A,0x47,0x40,0x02,0xDF,0x32,0xE5,0x21,0x39,
    0xF0,0xA0
    };
    l = 2 + idl + 32 * 6;
    Z = (unsigned char *)malloc(l);//temp1;
    if (!Z)
    return -1;
    entl = idl * 8;
    memset(Z + 1, entl & 0xFF, 1);
    entl >>= 8;
    memset(Z, entl & 0xFF, 1);
    memcpy(Z + 2, pucID, idl);
    memcpy(Z + 2 + idl, abxy, 32 *4);
    memcpy(Z + 2 + idl + 4 * 32, pubkey, 32);
    memcpy(Z + 2 + idl + 5 * 32, pubkey+32, 32);
    nRet = NRSEC3000_sm3(Z,l,tmpm);
    //free(Z);
    if (nRet != 0)
    goto quit;
    free(Z);

    //Z=temp;
    l = inl + 32;
    Z = (unsigned char *)malloc(l);//temp;
    if (!Z) {
    nRet = -1;
    goto quit;
    }
    memcpy(Z,tmpm,32);
    memcpy(Z+32, in, inl);
    nRet = NRSEC3000_sm3(Z,l,tmpm1);
    for(i=0; i<32; i++)
    {
        *(out+i)=tmpm1[i];
    }
    //quit:
   // Z=NULL;
    quit:
    if (Z)
    free(Z);

    return 0;
}


/* =============================================================================
 ���ܣ�SM2 ǩ��
 ���������ɵ�64�ֽ�ǩ�����ڽ��ջ�����û�д���
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
bool_t SM2_Sign(u8 p2)
{
  uint32_t i;

   ptagEncryptionCtrl->TxBuffer1[0] = 0x80;
   ptagEncryptionCtrl->TxBuffer1[1] = 0xb4;
   ptagEncryptionCtrl->TxBuffer1[2] = 0x00;
   ptagEncryptionCtrl->TxBuffer1[3] = p2;
   ptagEncryptionCtrl->TxBuffer1[4] = 0x20;

   for(i=5;i<200;i++)
   {
   ptagEncryptionCtrl->TxBuffer1[i] = 0xaa;
   }

   ptagEncryptionCtrl->TxBuffer2[0] = 0x55;

   for(i=0;i<32;i++)
   {
    ptagEncryptionCtrl->TxBuffer2[1+i]=ptagEncryptionCtrl->Hash_Number[i];
   }

   ptagEncryptionCtrl->TxBuffer2[33]=get_crc7(ptagEncryptionCtrl->TxBuffer2+1,32);

   for(i=34;i<5000;i++)
   {
      ptagEncryptionCtrl->TxBuffer2[i]= 0xaa;
   }


   for(i=0; i<5000; i++)
   {

        ptagEncryptionCtrl->RxBuffer[i]=0;

   }
   if(Receive_Response(0xb4))
   {
       DJY_EventDelay(1000);
        if(New_Receive_Data(0xb4, 0x40,&ptagEncryptionCtrl->len,35))
        {
            memcpy(ptagEncryptionCtrl->sign,ptagEncryptionCtrl->RxBuffer+ptagEncryptionCtrl->len+2,64);
            return 1;
        }

    }

   return 0;
}

/* =============================================================================
 ���ܣ�SM2 ��ǩ
 ������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
bool_t SM2_Sign_Verif(u8 p2)
{
    uint32_t i,j;
    u8 pResult;

    ptagEncryptionCtrl->TxBuffer1[0] = 0x80;
    ptagEncryptionCtrl->TxBuffer1[1] = 0xb6;
    ptagEncryptionCtrl->TxBuffer1[2] = 0x00;
    ptagEncryptionCtrl->TxBuffer1[3] = p2;
    ptagEncryptionCtrl->TxBuffer1[4] = 0x60;

    for(i=5;i<200;i++)
    {
      ptagEncryptionCtrl->TxBuffer1[i]=0xaa;
    }


    ptagEncryptionCtrl->TxBuffer2[0]=0x55;
    for(j=1;j<=32;j++)
    {
        ptagEncryptionCtrl->TxBuffer2[j]= ptagEncryptionCtrl->Hash_Number[j-1];
     }
    for(j=33;j<=96;j++)
    {
        ptagEncryptionCtrl->TxBuffer2[j]= ptagEncryptionCtrl->sign[j-33];
     }

     ptagEncryptionCtrl->TxBuffer2[97]=get_crc7(ptagEncryptionCtrl->TxBuffer2+1,96);

     for(i=98;i<5000;i++)
     {
         ptagEncryptionCtrl->TxBuffer2[i] = 0xaa;
     }


      for(i=0; i<5000; i++)
      {
          ptagEncryptionCtrl->RxBuffer[i]=0;
      }

      pResult=Receive_Response(0xb6);
      if(pResult)
      {
          DJY_EventDelay(1000);
          if(New_Receive_Data(0xba, 0x80,&ptagEncryptionCtrl->len,99))
              return 1;
     }
       return 0;
}

/* =============================================================================
 ���ܣ�SM2 ����
 ������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
bool_t SM2_Encrypt(u8 p2)
{
  uint32_t i,j;

   ptagEncryptionCtrl->TxBuffer1[0] = 0x80;
   ptagEncryptionCtrl->TxBuffer1[1] = 0xb3;
   ptagEncryptionCtrl->TxBuffer1[2] = 0x01;
   ptagEncryptionCtrl->TxBuffer1[3] = p2;
   ptagEncryptionCtrl->TxBuffer1[4] = 0x20;
   for(i=5;i<200;i++)
   {
     ptagEncryptionCtrl->TxBuffer1[i] = 0xaa;
   }

   ptagEncryptionCtrl->TxBuffer2[0]=0x55;
   for(j=1;j<=16;j++)
   {
     ptagEncryptionCtrl->TxBuffer2[j]=0;
     ptagEncryptionCtrl->TxBuffer2[j+16] = ptagEncryptionCtrl->SM1_IV[j-1];

    // ptagEncryptionCtrl->TxBuffer2[j]=ptagEncryptionCtrl->SM2_TrueNumber[j-1];
//     ptagEncryptionCtrl->TxBuffer2[j+16] = ptagEncryptionCtrl->IV[j-1];
   }

   ptagEncryptionCtrl->TxBuffer2[33]=get_crc7(ptagEncryptionCtrl->TxBuffer2+1,32);
   for(i=34;i<5000;i++)
   {
     ptagEncryptionCtrl->TxBuffer2[i] = 0xaa;
   }


   for(i=0; i<5000; i++)
   {
        ptagEncryptionCtrl->RxBuffer[i]=0;
   }
   if(Receive_Response(0xb3))
   {
       //DJY_EventDelay(1000);
        if(New_Receive_Data(0xb3, 0x80,&ptagEncryptionCtrl->len,35))
        {
            memcpy(ptagEncryptionCtrl->SM2_EncryptNumber,ptagEncryptionCtrl->RxBuffer+ptagEncryptionCtrl->len+2,128);
             return 1;
        }

    }

   return 0;

}

/* =============================================================================
 ���ܣ�SM2 ����
 ������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
bool_t SM2_Decrypt(u8 p2)
{

 uint32_t i;

  ptagEncryptionCtrl->TxBuffer1[0] = 0x80;
  ptagEncryptionCtrl->TxBuffer1[1] = 0xb3;
  ptagEncryptionCtrl->TxBuffer1[2] = 0x81;
  ptagEncryptionCtrl->TxBuffer1[3] = p2;
  ptagEncryptionCtrl->TxBuffer1[4] = 0x80;
  for(i=5;i<200;i++)
  {
  ptagEncryptionCtrl->TxBuffer1[i] = 0xaa;
  }

  ptagEncryptionCtrl->TxBuffer2[0] = 0x55;
  for(i=1;i<=128;i++)
  {
   ptagEncryptionCtrl->TxBuffer2[i]=ptagEncryptionCtrl->SM2_EncryptNumber[i-1];
  }

  ptagEncryptionCtrl->TxBuffer2[129]=get_crc7(ptagEncryptionCtrl->TxBuffer2+1,128);

  for(i=130;i<5000;i++)
  {
     ptagEncryptionCtrl->TxBuffer2[i]= 0xaa;
  }

  for(i=0; i<5000; i++)
  {
       ptagEncryptionCtrl->RxBuffer[i]=0;
  }
  if(Receive_Response(0xb3))
  {
      DJY_EventDelay(1000);
       if(New_Receive_Data(0xb3, 0x20,&ptagEncryptionCtrl->len,131))
       {
        memcpy(ptagEncryptionCtrl->SM2_TrueNumber,ptagEncryptionCtrl->RxBuffer+ptagEncryptionCtrl->len+2,32);
         return 1;
       }
    }

return 0;
}

/* =============================================================================
 ���ܣ�SM2 ����֤������
 ������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
bool_t Cer_Request(char *byP3Info,u8 p3)
{
    uint32_t i,j;

    u16 Packet_Len;
//    char   byP3Info[50]="C=CN,ST=jiangsu,L=nanjing,O=GDD,OU=nari,CN=test001";



    ptagEncryptionCtrl->TxBuffer1[0] = 0x80;
    ptagEncryptionCtrl->TxBuffer1[1] = 0xb7;
    ptagEncryptionCtrl->TxBuffer1[2] = 0x01;
    ptagEncryptionCtrl->TxBuffer1[3] = 0x00;
    ptagEncryptionCtrl->TxBuffer1[4] = p3;
    for(i=5;i<200;i++)
    {
    ptagEncryptionCtrl->TxBuffer1[i] = 0xaa;
    }

    ptagEncryptionCtrl->TxBuffer2[0] = 0x55;
    for(i=1;i<=p3;i++)
    {
     ptagEncryptionCtrl->TxBuffer2[i]=byP3Info[i-1];
    }

    ptagEncryptionCtrl->TxBuffer2[p3+1]=get_crc7(ptagEncryptionCtrl->TxBuffer2+1,p3);

    for(i=(p3+2);i<5000;i++)
    {
      ptagEncryptionCtrl->TxBuffer2[i]= 0xaa;
    }


    for(i=0; i<5000; i++)
    {

      ptagEncryptionCtrl->RxBuffer[i]=0;

    }

    if(Receive_Response(0xb7))
    {
        DJY_EventDelay(1000);
        for(i=0;i<5000;i++)
        {
           NRSEC3000_CsActive();//Ƭѡ
          DJY_EventDelay(10);
           NRSEC3000_TxRx(ptagEncryptionCtrl->TxBuffer2+i,1,ptagEncryptionCtrl->RxBuffer+i,1,0);

           NRSEC3000_CsInActive();//��Ƭѡ
          DJY_EventDelay(10);
            if(70==i)
                DJY_EventDelay(100);

            if((0x90==ptagEncryptionCtrl->RxBuffer[i-1])&&(0x00==ptagEncryptionCtrl->RxBuffer[i]))
            {
                break;
            }
        }

        for(i=(p3+2);i<5000;i++)
        {
            if(ptagEncryptionCtrl->RxBuffer[i]==0xb7)
            {
                Packet_Len=ptagEncryptionCtrl->RxBuffer[i+1];
                Packet_Len=(Packet_Len<<8)+ptagEncryptionCtrl->RxBuffer[i+2];
                ptagEncryptionCtrl->len = i;
                printf("%x",Packet_Len);
                //*Rxbuff=(spiRxBuf+i+2);
                for(j=0;j<Packet_Len;j++)
                {
                    ptagEncryptionCtrl->SM2_Certificate[j]=ptagEncryptionCtrl->RxBuffer[j+i+3];
                    printf("%x",ptagEncryptionCtrl->RxBuffer[j+i+3]);
                }

                return 1;
            }
        }
    }
    return 0;
}

/* =============================================================================
 ���ܣ����������
 ������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
bool_t Random_Numer(u8 p3)
{
   u8 spiTxBuf[500];
   u8 spiRxBuf[500];
   uint32_t i,j;

  volatile uint8_t byTmp1;

   spiTxBuf[0] = 0x00;
   spiTxBuf[1] = 0x84;
   spiTxBuf[2] = 0x00;
   spiTxBuf[3] = 0x00;
   spiTxBuf[4] = p3;


   for(i=5;i<500;i++)
   {
     spiTxBuf[i] = 0xaa;
   }

   for(i=0; i<500; i++)
   {
        spiRxBuf[i]=0;
   }

    for(i=0;i<500;i++)
    {
       NRSEC3000_CsActive();//Ƭѡ
       for (byTmp1=0;byTmp1<200;byTmp1++) ; //��ʱ

       NRSEC3000_TxRx(spiTxBuf+i,1,spiRxBuf+i,1,0);

       NRSEC3000_CsInActive();//��Ƭѡ
       for (byTmp1=0;byTmp1<200;byTmp1++) ; //��ʱ

    }

    for(i=5;i<500;i++)
    {
       if((spiRxBuf[i]==0x84)&&(spiRxBuf[i+p3+3]==0x90))
       {
           for(j=0;j<p3;j++)
           {
               ptagEncryptionCtrl->Random[j]=(spiRxBuf[i+2+j]);
           }

        return 1;
        }
     }

    printf("\r\nδ�յ������%d\r\n",i);
    return 0;

}

/* =============================================================================
 ���ܣ���ȡ�汾��Ϣ
 ������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
u8 NRSEC3000_RdVersion()
{
    uint32_t i,j;


    ptagEncryptionCtrl->TxBuffer1[0] = 0x00;
    ptagEncryptionCtrl->TxBuffer1[1] = 0x5b;
    ptagEncryptionCtrl->TxBuffer1[2] = 0x00;
    ptagEncryptionCtrl->TxBuffer1[3] = 0x00;
    ptagEncryptionCtrl->TxBuffer1[4] = 0x40;

    for (i=5;i<150;i++)
    {
        ptagEncryptionCtrl->TxBuffer1[i]=0xaa;
    }

        for (i=0;i<150;i++)
        {
            NRSEC3000_CsActive();//Ƭѡ
            DJY_DelayUs(100);
            NRSEC3000_TxRx(ptagEncryptionCtrl->TxBuffer1+i,1,ptagEncryptionCtrl->RxBuffer+i,1,0);
            NRSEC3000_CsInActive();//��Ƭѡ
            DJY_DelayUs(100);

            if((0x90==ptagEncryptionCtrl->RxBuffer[i-1])&&(0x00==ptagEncryptionCtrl->RxBuffer[i])&&(i>=5))
            {
              break;
            }
        }

    for (i=5;i<150;i++)
    {
        if (ptagEncryptionCtrl->RxBuffer[i]==0x5b)
        {
            break;
        }
    }

    if (i==150)
    {
        return 0;
    }

    for (j=0;j<64;j++)
    {
        ptagEncryptionCtrl->Version[j]=ptagEncryptionCtrl->RxBuffer[i+2+j];
    }
    return 1;
}

/* =============================================================================
 ���ܣ���ȫ��֤
 ������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
bool_t Safety_Cert()
{
 uint32_t i;

  ptagEncryptionCtrl->TxBuffer1[0] = 0x80;
  ptagEncryptionCtrl->TxBuffer1[1] = 0xb3;
  ptagEncryptionCtrl->TxBuffer1[2] = 0x01;
  ptagEncryptionCtrl->TxBuffer1[3] = 0x04;
  ptagEncryptionCtrl->TxBuffer1[4] = 0x20;
  for(i=5;i<200;i++)
  {
  ptagEncryptionCtrl->TxBuffer1[i] = 0xaa;
  }

  ptagEncryptionCtrl->TxBuffer2[0] = 0x55;
  for(i=1;i<=32;i++)
  {
   ptagEncryptionCtrl->TxBuffer2[i]=ptagEncryptionCtrl->Auth[i-1];
  }

  ptagEncryptionCtrl->TxBuffer2[33]=get_crc7(ptagEncryptionCtrl->Auth,32);

  for(i=34;i<5000;i++)
  {
     ptagEncryptionCtrl->TxBuffer2[i]= 0xaa;
  }


  for(i=0; i<5000; i++)
  {

       ptagEncryptionCtrl->RxBuffer[i]=0;

  }
  if(Receive_Response(0xb3))
  {
      DJY_EventDelay(1000);

       if(New_Receive_Data(0xb3, 0x92,&ptagEncryptionCtrl->len,35))
       {
           memcpy(ptagEncryptionCtrl->Auth_Number,ptagEncryptionCtrl->RxBuffer+ptagEncryptionCtrl->len+2,146);
           return 1;
       }

    }
  return 0;
}

/* =============================================================================
 ���ܣ�nrsec3000оƬ��ʼ������ʼ���ͼ����豸����Ӧ��spi����.
 ��������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/

bool_t NRSEC3000_Init(void)
{
    s_ptNRSEC3000_Dev = SPI_DevAdd(CFG_NRSEC3000_BUS_NAME,"NRSEC3000",0,8,SPI_MODE_3,
                            SPI_SHIFT_MSB,CFG_NRSEC3000_SPI_SPEED,false);
    if(NULL != s_ptNRSEC3000_Dev)
    {
        SPI_BusCtrl(s_ptNRSEC3000_Dev,CN_SPI_SET_POLL,0,0);
        //SPI_BusCtrl(s_ptNRSEC3000_Dev,CN_SPI_SET_CLK,CFG_NRSEC3000_SPI_SPEED,0);
        return true;
    }
    return false;
}


void test(void)
{
//     u8 *pDest;
      // u8 Txbuffer[32];
       u8 sdata[10];
       u8 i;

       NRSEC3000_Init( );

//     //�ǲ���Ҫ��һ����Ԥ�ȡ��Ľ׶Σ�ÿ�����϶������⣬ָ����մ���Ȼ��֪����ô�ľ�ͻȻ����
//     //ʱ������⣿�ǲ��ǲ��β��ԣ�
       for(i=0;i<1;i++)
       {
           sdata[i]=0xff;
           NRSEC3000_TxRx(sdata+i,1,sdata+5+i,1,0);
       }



//     sm3(unsigned char *in,int inl,unsigned char *out, unsigned char *pubkey, unsigned char *pucID, int idl);





//��ע��Ƭѡ�������ʱ�����Բɼ�����Ӱ��ܴ󣬵�����ʱ֮���յ����ݵ��ٶȿ���,����������µ����ݽ��պ���
//     ָ������յ�����������������ݣ�֮ǰ���Ǻúõġ��������Export_SM2_PrivateKey(1)һ��������ʲô��û��ͻȻ���ֺ��ˡ���
//     ��ʱ�����ݵĽ���Ӱ��ܴ�
//     �������ͽ��ջ����������̫��ᵼ���쳣����ջ���

       //���²��Թ��̺����ע�Ͷ����ʼд�ģ��������ԣ��������ܹ��Ϻõ�ʵ�ֹ���

       //������Ҫ�����ǰѼ���оƬ�ṹ������Ĳ�������Ū�����㣬����Import_IV��SM2_Encrypt���ͻ�������ֵĸ�ֵ

       //Ψһһ���޷������������ǡ�����˽Կ�����ܣ��ʾ���֭Ҳ�벻����Ϊʲô����


//     //======================================���뵼��˽Կ����======================================
//         Random_Numer(32);//����������һ������������������ʵ��
//         for(i=0;i<32;i++)
//         {
//          ptagEncryptionCtrl->Client_PrivateKey[i]=ptagEncryptionCtrl->Random[i];
//         }
//         Import_SM2_PrivateKey(1);//���˰˰ٶ��0xaa�������յ�SW 0x90 0x00����Ȼ��ʽ�Եģ��о����Ǻܺ�
//
//         Export_SM2_PrivateKey(1);//ʧ�ܣ���ָ�û����ȷ��Ӧ���ڶ�ȡʱð��0x6a��0x81����������0x6e 0x00XXXXXXXXXXXXXXXXXXXXXXXXXX
//     //==========================================================================================
//     //==============================���뵼����Կ����==============================
//         Random_Numer(64);//����������һ������������������ʵ��
//         for(i=0;i<64;i++)
//         {
//          ptagEncryptionCtrl->SM2_01_PublicKey[i]=ptagEncryptionCtrl->Random[i];
//         }
//
//         Import_SM2_PublicKey(1);//����һǧ���0xaa�������յ�SW 0x90 0x00����Ȼ��ʽ�Եģ��о����Ǻܺ�
//
//         Export_SM2_PublicKey(1);//Ӧ�����ݸ�ʽ��ȷ�����Բ��ԾͲ�֪����
//     //==========================================================================
//     //=========================================SM1���ܽ��ܲ���=========================================
//         Random_Numer(16);//����������һ������������������ʵ��
//         for(i=0;i<16;i++)
//         {
//          ptagEncryptionCtrl->SM1_Key[i]=ptagEncryptionCtrl->Random[i];
//         }
//         Import_SM1_Key();//ok ָ��ͺܺã���������ʱ����յ�������0xaa���յ�SW 0x90 0x00
//         Import_IV();//ok ָ��ͺܺã���������ʱ����յ�������0xaa���յ�SW 0x90 0x00
//         Random_Numer(32);//����������һ������������������ʵ��
//         for(i=0;i<32;i++)
//         {
//          ptagEncryptionCtrl->SM1_TrueNumber[i]=ptagEncryptionCtrl->Random[i];
//         }
//
//         SM1_Encrypt(ptagEncryptionCtrl->SM1_TrueNumber,32);//Ӧ�����ݸ�ʽ��ȷ�����Բ��ԾͲ�֪����
//
//         SM1_Decrypt(ptagEncryptionCtrl->SM1_EncryptNumber,32);//Ӧ�����ݸ�ʽ��ȷ�����Բ��ԾͲ�֪����
//     //================================================================================================
//     //==========================�������ܲ���=============================================================
//     char   byP3Info[50]="C=CN,ST=jiangsu,L=nanjing,O=GDD,OU=nari,CN=test001";
//         Cer_Request(byP3Info,50);//����ʱ���򻻳���Djy_EventDelay���յ���289��Ӧ�����ݣ���Ȼ��forѭ����ʱ���ղ�������
//         Random_Numer(16);//����������һ������������������ʵ��
//         NRSEC3000_RdVersion();//��Ȼ������9��0xaa�Ŷ������ݣ���������Ӧ��û����
//     //==================================================================================================
//     //====================================SM2���ܲ���===============================================
//         SM2_Encrypt(0);//ָ��ͳ�ȥ�ˣ�������û�У��᲻������Ϊȫ0��Ե�ʣ��Ȳ���Ӧ������
//                        //����3000���0xaa���з�Ӧ����ʽ��ȷ������������Ϊ����ʱ�й�
//         SM2_Decrypt(0);//ָ��ͳ�ȥ�ˣ�������û�У��᲻������Ϊȫ0��Ե��
//                         //����3000���0xaa���з�Ӧ����ʽ��ȷ������������Ϊ����ʱ�й�
//     //================================================================================================
//     //=========================================SM2���ܲ���============================================
//         Key_Product();//���������ܺ���ʱ�йأ�Djy_EventDelay���ԣ�����Ҳ���˺þã�30��0xaa�����յ�SW 0x90 0x00��Djy_DelayUs��forѭ�����ղ�����ȷ��ֵ��
//         Random_Numer(32);//����������һ������������������ʵ��
//
//         for(i=0;i<32;i++)
//         {
//             ptagEncryptionCtrl->SM2_TrueNumber[i]=ptagEncryptionCtrl->Random[i];
//         }
//         SM2_Encrypt(0);//ָ��ͳ�ȥ�ˣ�������û�У��᲻������Ϊȫ0��Ե�ʣ��Ȳ���Ӧ������
//
//         SM2_Decrypt(0);//ָ��ͳ�ȥ�ˣ�������û�У��᲻������Ϊȫ0��Ե��
//                        //��ʽ��ȷ
//     //================================================================================================
//     //====================================��ȫ��֤����=============================================
//
//         Random_Numer(32);//����������һ������������������ʵ��
//
//         for(i=0;i<32;i++)
//         {
//          ptagEncryptionCtrl->Auth[i]=ptagEncryptionCtrl->Random[i];
//         }
//
//         Safety_Cert();//ָ������յ����������ݿ����ǰ�ȫ��֤����ֵû�е���ʧ��
//     //=============================================================================================
//     //====================================SM2ǩ����ǩ����=============================================
//         Random_Numer(32);
//         for(i=0;i<32;i++)
//         {
//             Txbuffer[i]=ptagEncryptionCtrl->Random[i];
//         }
//         NRSEC3000_sm3(Txbuffer,32,ptagEncryptionCtrl->Hash_Number);//sm3��hash���ܸ�ʽ��ȷ
//
//         SM2_Sign(0);//ָ������յ����������ݿ�����Hash_Numberֵû�е���ʧ��
//                     //������һ��hash,����2600���0xaa���з�Ӧ����ʽ��ȷ
//
//         SM2_Sign_Verif(0);//ָ������յ����������ݿ�����Hash_Numberֵû�е���ʧ��
//                           //������һ��hash,����5900���0xaa���з�Ӧ����ʽ��ȷ
//     //==============================================================================================
}


























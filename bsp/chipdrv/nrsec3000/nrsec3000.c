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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// =============================================================================

// 文件名     ：nrsec3000.c
// 模块描述: 加密芯片nrsec3000模块驱动
// 模块版本: V1.10
// 创建人员:
// 创建时间:
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
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t NRSEC3000_Init(void);
//    NRSEC3000_Init();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"encrypt 2 nrsec300"//spi接口的国密2芯片
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"lock","spi bus","cpu onchip spi"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_ENCRYPT_2_NRSEC300 == false )
//#warning  " encrypt_2_nrsec300  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_ENCRYPT_2_NRSEC300    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,500000,50000000,
#define CFG_NRSEC3000_SPI_SPEED      (20000*1000)           //"SPI总线速度"，配置总线通信速度
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#define CFG_NRSEC3000_BUS_NAME              ""              //"总线名称",NRSEC3000所在的SPI总线名称
#endif
//%$#@end configue  ****参数配置结束
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
 功能：SPI传输需要用到的三个函数，片选拉低拉高和发送接收
 参数：NRSEC3000_TxRx的参数要注意
 返回：true,成功;false,失败
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
 功能：获取crc7
 参数：无
 返回：true,成功;false,失败
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
 功能：发送INS命令，接收返回值
 参数：预期接收到的INS命令值
 返回：true,成功;false,失败
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

        NRSEC3000_CsActive();//片选
//      for (byTmp1=0;byTmp1<200;byTmp1++) ; //延时
//      DJY_EventDelay(10);
        DJY_DelayUs(100);
        NRSEC3000_TxRx(ptagEncryptionCtrl->TxBuffer1+i,1,ptagEncryptionCtrl->RxBuffer+i,1,0);

        NRSEC3000_CsInActive();//非片选
//      for (byTmp1=0;byTmp1<200;byTmp1++) ; //延时
//      DJY_EventDelay(10);
        DJY_DelayUs(100);



        if((cmd==ptagEncryptionCtrl->RxBuffer[i])&&(i>5))
        {
          return 1;
        }

        if(i>=499)
        {
         printf("!!!Error,加密模块未响应\r\n");
         return 0;
        }
    }
    return 0;
}


/* =============================================================================
 功能：发送数据，接收返回值
 参数：预期接收到的数据
 返回：true,成功;false,失败
 =============================================================================*/
bool_t Receive_Data(u8 cmd, u16 length,u16 *len)
{
   uint32_t i;


   for(i=0;i<5000;i++)
   {
       NRSEC3000_CsActive();//片选
//     for (byTmp1=0;byTmp1<200;byTmp1++) ; //延时
//     DJY_EventDelay(10);
       DJY_DelayUs(100);

       NRSEC3000_TxRx(ptagEncryptionCtrl->TxBuffer2+i,1,ptagEncryptionCtrl->RxBuffer+i,1,0);


       NRSEC3000_CsInActive();//非片选
//     for (byTmp1=0;byTmp1<200;byTmp1++) ; //延时
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

    printf("\r\n未收到数据%d\r\n",i);
    return 0;

}



bool_t New_Receive_Data(u8 cmd, u16 length,u16 *len,u16 num)
{

    uint32_t i;

      for(i=0;i<num;i++)
      {
         NRSEC3000_CsActive();//片选
         DJY_DelayUs(100);
         NRSEC3000_TxRx(ptagEncryptionCtrl->TxBuffer2+i,1,ptagEncryptionCtrl->RxBuffer+i,1,0);
         NRSEC3000_CsInActive();//非片选
         DJY_DelayUs(100);
      }
     DJY_EventDelay(1000);
      for(i=num;;i++)
      {
         NRSEC3000_CsActive();//片选
         DJY_DelayUs(1000);
         NRSEC3000_TxRx(ptagEncryptionCtrl->TxBuffer2+num,1,ptagEncryptionCtrl->RxBuffer+i,1,0);
         NRSEC3000_CsInActive();//非片选
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
 功能：SM1功能导入对称密钥key
 参数：无
 返回：true,成功;false,失败
 =============================================================================*/
bool_t Import_SM1_Key()
{
    u32 i=0,j=0;

//=============================================================================
//填充发送缓冲区1里的内容，手册中的过程1、过程2，发送命令头和等待接收
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
//填充发送缓冲区2里的内容，手册中的过程3、过程4，发送命令数据标志、key和crc
    ptagEncryptionCtrl->TxBuffer2[0]=0x55;
    for(j=1;j<=16;j++)
    {
        ptagEncryptionCtrl->TxBuffer2[j]= ptagEncryptionCtrl->SM1_Key[j-1];
    }
    ptagEncryptionCtrl->TxBuffer2[17]=get_crc7(ptagEncryptionCtrl->TxBuffer2+1,16);

//=============================================================================
//填充发送缓冲区2一直发送aa直到接收状态字90 00正确
    for(i=18;i<5000;i++)
    {
        ptagEncryptionCtrl->TxBuffer2[i] = 0xaa;
    }

     for(i=0; i<5000; i++)
     {
         ptagEncryptionCtrl->RxBuffer[i]=0;//接收缓冲区清空
     }

//=============================================================================
//将发送缓冲区1的内容送出去，等待接收到括号里写的d4
     if(Receive_Response(0xd4))
     {
         DJY_EventDelay(1000);
           Receive_Data(0xd4, 0x80,&ptagEncryptionCtrl->len);
       }




//=============================================================================
//过程6开始，指令填充

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
//过程8，数据填充
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
 功能：SM1功能导入初始向量IV
 参数：无
 返回：true,成功;false,失败
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
//过程3、4，数据填充,16字节的IV（为什么是十六个1？）
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
//缓冲区清零
      for(i=0; i<5000; i++)
      {
          ptagEncryptionCtrl->RxBuffer[i]=0;
      }
//=============================================================================
//发送指令和数据
      if(Receive_Response(0xd4))
      {
          DJY_EventDelay(1000);
        if(Receive_Data(0xd4, 0x80,&ptagEncryptionCtrl->len))
                return 1;
        }
       return 0;
}


/* =============================================================================
 功能：SM1加密
 参数：要加密的数据和数据长度
 返回：true,成功;false,失败
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
//发送缓冲区2：包含了0x55+inlenth的数据+crc+剩下都是0xaa
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
 功能：SM1解密
 参数：要加密的数据和数据长度
 返回：true,成功;false,失败
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
//发送缓冲区1：包含了CMD头+剩下都是0xaa
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
//发送缓冲区2：包含了0x55+inlenth的数据+crc+剩下都是0xaa
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
 功能：产生SM2密钥对，因为指令与数据发送接收的格式与SM1不太一样，这里就自己再写一下发送函数
 参数：P2=0x00
 返回：true,成功;false,失败
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
        NRSEC3000_CsActive();//片选
//      for (byTmp1=0;byTmp1<30;byTmp1++) ; //延时
        DJY_EventDelay(10);
//      DJY_DelayUs(100);
        NRSEC3000_TxRx(spiTxBuf+i,1,spiRxBuf+i,1,0);

        NRSEC3000_CsInActive();//非片选
//      for (byTmp1=0;byTmp1<30;byTmp1++) ; //延时
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
 功能：导出SM2公钥
 参数：
      1. 命令头 CMD 的 p2 为密钥 索引号 ，参见 产生 SM2 SM2密钥对 中的说明
      2. 命令头 CMD 的 p3 为 期望接收的公钥内容长度（为 期望接收的公钥内容长度（为 期望接收的公钥内容长度（64 字节）
      3. 接收长度 LEN 为 1字节 ，值应为 65 （64 字节的公钥 字节的公钥 +1 字节的 CRC CRC）
 返回：true,成功;false,失败
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
//输入缓冲区为CMD头+一堆0xaa
   for(i=0; i<500; i++)
   {
       spiRxBuf[i]=0;
   }

    for(i=0;i<500;i++)
    {
        NRSEC3000_CsActive();//片选
        for (byTmp1=0;byTmp1<200;byTmp1++) ; //延时
//      DJY_EventDelay(1000);
        NRSEC3000_TxRx(spiTxBuf+i,1,spiRxBuf+i,1,0);

        NRSEC3000_CsInActive();//非片选
        for (byTmp1=0;byTmp1<200;byTmp1++) ; //延时
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

    printf("\r\n未读取到装置公钥%d\r\n",i);
    return 0;
}

/* =============================================================================
 功能：导出SM2私钥
 参数：
      1. 命令头 CMD 的 p2 为密钥索引号，参见 为密钥索引号，参见 产生 SM2 SM2密钥对 中
      2. 命令头 CMD 的 p3 为期望接收的 为期望接收的 私钥内容的长度（ 钥内容的长度（ 私钥内容长度为 32 字节）
      3. 接收长度 LEN 为 1字节 ，值应为 33 （32 字节的私钥 字节的私钥 +1 字节的 CRC CRC）
 返回：true,成功;false,失败
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
//输入缓冲区填充：CMD头+一堆0xaa
     for(i=0; i<500; i++)
     {
         spiRxBuf[i]=0;
     }



//所有发送函数都一个样，拉片选，调用发送接收
          for(i=0;i<5;i++)
          {
             NRSEC3000_CsActive();//片选
             DJY_DelayUs(100);
             NRSEC3000_TxRx(spiTxBuf+i,1,spiRxBuf+i,1,0);
             NRSEC3000_CsInActive();//非片选
             DJY_DelayUs(100);
          }
         DJY_EventDelay(1000);
          for(i=5;;i++)
          {
             NRSEC3000_CsActive();//片选
             DJY_DelayUs(1000);
             NRSEC3000_TxRx(spiTxBuf+5,1,spiRxBuf+i,1,0);
             NRSEC3000_CsInActive();//非片选
             DJY_DelayUs(1000);

            if( (0x90==spiRxBuf[i-1]) && (0x00==spiRxBuf[i]) )
                         break;
             if(i==500)
             {
                return 0;
             }
          }

//数据处理，保存
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
      printf("\r\n未读取到装置私钥%d\r\n",i);
      return 0;
}

/* =============================================================================
 功能：导入SM2公钥
 参数：
 返回：true,成功;false,失败
 =============================================================================*/
bool_t Import_SM2_PublicKey(u8 p2)
{
    uint32_t i,j;

    //导入SM2公钥，64字节
    ptagEncryptionCtrl->TxBuffer1[0] = 0x80;
    ptagEncryptionCtrl->TxBuffer1[1] = 0xba;
    ptagEncryptionCtrl->TxBuffer1[2] = 0x01;
    ptagEncryptionCtrl->TxBuffer1[3] = p2;
    ptagEncryptionCtrl->TxBuffer1[4] = 0x40;

    for(i=5;i<150;i++)
    {
      ptagEncryptionCtrl->TxBuffer1[i]=0xaa;
    }
//发送缓冲区1：包含了CMD头和一堆0xaa
//发送缓冲区2：包含了标志位0x55+64字节公钥+crc+一堆0xaa
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
 功能：导入SM2私钥
 参数：
 返回：true,成功;false,失败
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
 功能：SM3 hash
 参数：这里是不是要把结构体里的Hash_Number[32];进行赋值
 返回：true,成功;false,失败
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
       NRSEC3000_CsActive();//片选
       DJY_DelayUs(1000); //延时

       NRSEC3000_TxRx(spiTxBuf+i,1,spiRxBuf+i,1,0);

       NRSEC3000_CsInActive();//非片选
       DJY_DelayUs(1000) ; //延时

       if((0xb5==spiRxBuf[i])&&(i>5))
       {
           break;
       }

        if(i>=199)
        {
         printf("!!!Error,加密模块未响应\r\n");
         return 0;
        }
}

   spiTxBuf[0]=0x55;
   for(i=1;i<=Lenth;i++)
   {
      spiTxBuf[i]=*(p+i-1);//传递进来的要发送的前lenth长度的数据放入缓冲区
   }
   spiTxBuf[Lenth+1]=get_crc7(spiTxBuf+1,Lenth);//再加一个crc

   for(i=Lenth+2;i<3000;i++)
   {
      spiTxBuf[i]=0xaa;//后面填充0xaa
   }

      for(i=0; i<3000; i++)
     {
          spiRxBuf[i]=0;
     }


   for(i=0;i<3000;i++)
     {
       NRSEC3000_CsActive();//片选
       DJY_DelayUs(100); //延时

       NRSEC3000_TxRx(spiTxBuf+i,1,spiRxBuf+i,1,0);

       NRSEC3000_CsInActive();//非片选
       DJY_DelayUs(100); //延时

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
                    *(q+j)=spiRxBuf[j+len+2];//把收到的数据传递回去
              }
              return 0;
          }
       }

      printf("\r\n未收到数据%d\r\n",i);
      return 1;

}

/******************************描述：SM3 Hash 操作***********************************************/
//参数：
//in: [输入]，需要进行hash 运算的数据的首地址
//inl: [输入]，数据的长度（字节数）
//out: [输出]，接收Hash 结果的内存首地址（sm3 结果长度为32 字节）
//pubkey: [输入]，公钥内容（64 字节）
//pucID: [输入]，用户标识
//idl: [输入]，用户标识的长度
//返回结果：  0：成功
//            其他：失败
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
 功能：SM2 签名
 参数：生成的64字节签名还在接收缓冲区没有处理
 返回：true,成功;false,失败
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
 功能：SM2 验签
 参数：
 返回：true,成功;false,失败
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
 功能：SM2 加密
 参数：
 返回：true,成功;false,失败
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
 功能：SM2 解密
 参数：
 返回：true,成功;false,失败
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
 功能：SM2 生成证书请求
 参数：
 返回：true,成功;false,失败
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
           NRSEC3000_CsActive();//片选
          DJY_EventDelay(10);
           NRSEC3000_TxRx(ptagEncryptionCtrl->TxBuffer2+i,1,ptagEncryptionCtrl->RxBuffer+i,1,0);

           NRSEC3000_CsInActive();//非片选
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
 功能：产生随机数
 参数：
 返回：true,成功;false,失败
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
       NRSEC3000_CsActive();//片选
       for (byTmp1=0;byTmp1<200;byTmp1++) ; //延时

       NRSEC3000_TxRx(spiTxBuf+i,1,spiRxBuf+i,1,0);

       NRSEC3000_CsInActive();//非片选
       for (byTmp1=0;byTmp1<200;byTmp1++) ; //延时

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

    printf("\r\n未收到随机数%d\r\n",i);
    return 0;

}

/* =============================================================================
 功能：获取版本信息
 参数：
 返回：true,成功;false,失败
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
            NRSEC3000_CsActive();//片选
            DJY_DelayUs(100);
            NRSEC3000_TxRx(ptagEncryptionCtrl->TxBuffer1+i,1,ptagEncryptionCtrl->RxBuffer+i,1,0);
            NRSEC3000_CsInActive();//非片选
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
 功能：安全认证
 参数：
 返回：true,成功;false,失败
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
 功能：nrsec3000芯片初始化，初始化和加载设备到对应的spi总线.
 参数：无
 返回：true,成功;false,失败
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

//     //是不是要有一个“预热”的阶段？每天早上都出问题，指令接收错误，然后不知道怎么的就突然好了
//     //时序的问题？是不是波形不对？
       for(i=0;i<1;i++)
       {
           sdata[i]=0xff;
           NRSEC3000_TxRx(sdata+i,1,sdata+5+i,1,0);
       }



//     sm3(unsigned char *in,int inl,unsigned char *out, unsigned char *pubkey, unsigned char *pucID, int idl);





//备注：片选下面的延时函数对采集数据影响很大，调高延时之后收到数据的速度快了,所以设计了新的数据接收函数
//     指令后面收到了两个无意义的数据，之前还是好好的……情况跟Export_SM2_PrivateKey(1)一样，但是什么都没动突然间又好了……
//     延时对数据的接收影响很大？
//     变量发送接收缓冲区定义的太大会导致异常，堆栈溢出

       //以下测试过程后面的注释都是最开始写的，经过调试，基本都能够较好的实现功能

       //接下来要做的是把加密芯片结构体里面的参数名字弄清晰点，还有Import_IV和SM2_Encrypt发送缓冲区奇怪的赋值

       //唯一一个无法正常工作的是“导出私钥”功能，绞尽脑汁也想不出来为什么……


//     //======================================导入导出私钥测试======================================
//         Random_Numer(32);//测试了生成一个、五个随机数，可以实现
//         for(i=0;i<32;i++)
//         {
//          ptagEncryptionCtrl->Client_PrivateKey[i]=ptagEncryptionCtrl->Random[i];
//         }
//         Import_SM2_PrivateKey(1);//花了八百多个0xaa才最终收到SW 0x90 0x00，虽然格式对的，感觉不是很好
//
//         Export_SM2_PrivateKey(1);//失败，连指令都没有正确反应，在读取时冒出0x6a和0x81两个数或者0x6e 0x00XXXXXXXXXXXXXXXXXXXXXXXXXX
//     //==========================================================================================
//     //==============================导入导出公钥测试==============================
//         Random_Numer(64);//测试了生成一个、五个随机数，可以实现
//         for(i=0;i<64;i++)
//         {
//          ptagEncryptionCtrl->SM2_01_PublicKey[i]=ptagEncryptionCtrl->Random[i];
//         }
//
//         Import_SM2_PublicKey(1);//花了一千多个0xaa才最终收到SW 0x90 0x00，虽然格式对的，感觉不是很好
//
//         Export_SM2_PublicKey(1);//应答数据格式正确，但对不对就不知道了
//     //==========================================================================
//     //=========================================SM1加密解密测试=========================================
//         Random_Numer(16);//测试了生成一个、五个随机数，可以实现
//         for(i=0;i<16;i++)
//         {
//          ptagEncryptionCtrl->SM1_Key[i]=ptagEncryptionCtrl->Random[i];
//         }
//         Import_SM1_Key();//ok 指令发送很好，发送数据时最后收到了两个0xaa才收到SW 0x90 0x00
//         Import_IV();//ok 指令发送很好，发送数据时最后收到了两个0xaa才收到SW 0x90 0x00
//         Random_Numer(32);//测试了生成一个、五个随机数，可以实现
//         for(i=0;i<32;i++)
//         {
//          ptagEncryptionCtrl->SM1_TrueNumber[i]=ptagEncryptionCtrl->Random[i];
//         }
//
//         SM1_Encrypt(ptagEncryptionCtrl->SM1_TrueNumber,32);//应答数据格式正确，但对不对就不知道了
//
//         SM1_Decrypt(ptagEncryptionCtrl->SM1_EncryptNumber,32);//应答数据格式正确，但对不对就不知道了
//     //================================================================================================
//     //==========================其他功能测试=============================================================
//     char   byP3Info[50]="C=CN,ST=jiangsu,L=nanjing,O=GDD,OU=nari,CN=test001";
//         Cer_Request(byP3Info,50);//把延时程序换成了Djy_EventDelay，收到了289个应答数据，不然用for循环延时就收不到数据
//         Random_Numer(16);//测试了生成一个、五个随机数，可以实现
//         NRSEC3000_RdVersion();//虽然经过了9个0xaa才读到数据，但是数据应该没问题
//     //==================================================================================================
//     //====================================SM2加密测试===============================================
//         SM2_Encrypt(0);//指令发送出去了，但数据没有，会不会是因为全0的缘故，等不到应答数据
//                        //经过3000多个0xaa才有反应，格式正确，基本可以认为与延时有关
//         SM2_Decrypt(0);//指令发送出去了，但数据没有，会不会是因为全0的缘故
//                         //经过3000多个0xaa才有反应，格式正确，基本可以认为与延时有关
//     //================================================================================================
//     //=========================================SM2加密测试============================================
//         Key_Product();//不明，可能和延时有关，Djy_EventDelay可以，但是也等了好久（30个0xaa）才收到SW 0x90 0x00，Djy_DelayUs和for循环就收不到正确的值。
//         Random_Numer(32);//测试了生成一个、五个随机数，可以实现
//
//         for(i=0;i<32;i++)
//         {
//             ptagEncryptionCtrl->SM2_TrueNumber[i]=ptagEncryptionCtrl->Random[i];
//         }
//         SM2_Encrypt(0);//指令发送出去了，但数据没有，会不会是因为全0的缘故，等不到应答数据
//
//         SM2_Decrypt(0);//指令发送出去了，但数据没有，会不会是因为全0的缘故
//                        //格式正确
//     //================================================================================================
//     //====================================安全认证测试=============================================
//
//         Random_Numer(32);//测试了生成一个、五个随机数，可以实现
//
//         for(i=0;i<32;i++)
//         {
//          ptagEncryptionCtrl->Auth[i]=ptagEncryptionCtrl->Random[i];
//         }
//
//         Safety_Cert();//指令可以收到，但是数据可能是安全认证因子值没有导致失败
//     //=============================================================================================
//     //====================================SM2签名验签测试=============================================
//         Random_Numer(32);
//         for(i=0;i<32;i++)
//         {
//             Txbuffer[i]=ptagEncryptionCtrl->Random[i];
//         }
//         NRSEC3000_sm3(Txbuffer,32,ptagEncryptionCtrl->Hash_Number);//sm3的hash功能格式正确
//
//         SM2_Sign(0);//指令可以收到，但是数据可能是Hash_Number值没有导致失败
//                     //随便给了一组hash,经过2600多个0xaa才有反应，格式正确
//
//         SM2_Sign_Verif(0);//指令可以收到，但是数据可能是Hash_Number值没有导致失败
//                           //随便给了一组hash,经过5900多个0xaa才有反应，格式正确
//     //==============================================================================================
}


























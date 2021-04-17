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
//------------------------------------------------------------------------------
// =============================================================================
//------------------------------------------------------------------------------
// �ļ���     ��ioiicbus.c
// ģ������:ģ��������GPIOģ��IICʱ�򣬲�����IIC��ӵ�IIC�����ϡ�
// ģ��汾: V1.00
// ������Ա: czz
// ����ʱ��: 2018.07.17
// =============================================================================
#include "IoIicBus.h"
#include "djyos.h"
#include "stdint.h"
#include "stdio.h"
#include "iicbus.h"
#include "endian.h"
#include "lock.h"
#include "int.h"
#include "stddef.h"
#include "stdlib.h"


#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"io analog iic bus"//��д�����������
//parent:"iicbus"    //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"iicbus"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_IO_ANALOG_IIC_BUS == false )
//#warning  " io_analog_iic_bus  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header      //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_IO_ANALOG_IIC_BUS    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,
//%$#@string,1,32,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


struct IIC_Io_CB
{
   u32            delay_us;  //����Ƶ������ĸߵ͵�ƽʱ��������ʱ
   IIC_IoCtrl     IOCtrl ;   //gpio���ƻص�����ָ��
   u32            tag;       //�û����ӵı�ǩ
};

// =============================================================================
// ���ܣ�����IIC��ʼ�ź�
// ������IOģ���IIC���ƿ�ṹ��ָ��
// ���أ�
// =============================================================================
static void IO_IIC_Start(struct IIC_Io_CB * ICB)
{
    ICB->IOCtrl(sda_set_out,ICB->tag);//sda�����
    ICB->IOCtrl(scl_set_High,ICB->tag);//ʱ�����øߵ�ƽ
    DJY_DelayUs(ICB->delay_us);
    ICB->IOCtrl(sda_set_Low,ICB->tag);//START:when CLK is high,DATA change form high to low
    DJY_DelayUs(ICB->delay_us);
}

// =============================================================================
// ���ܣ�����IICֹͣ�ź�
// ������IOģ���IIC���ƿ�ṹ��ָ��
// ���أ�
// =============================================================================
static void IO_IIC_Stop(struct IIC_Io_CB * ICB)
{
    ICB->IOCtrl(sda_set_out,ICB->tag);//sda�����
    ICB->IOCtrl(scl_set_Low,ICB->tag);
    ICB->IOCtrl(sda_set_Low,ICB->tag);//STOP:when CLK is high DATA change form low to high
    DJY_DelayUs(ICB->delay_us);
    ICB->IOCtrl(scl_set_High,ICB->tag);
    DJY_DelayUs(ICB->delay_us);
    ICB->IOCtrl(sda_set_High,ICB->tag);//����I2C���߽����ź�
}

// =============================================================================
// ���ܣ��ȴ�������Ӧ��
// �ȴ�Ӧ���źŵ���
// ����ֵ��false������Ӧ��ʧ��
//      true������Ӧ��ɹ�
// =============================================================================
static bool_t IO_IIC_WaitAck(struct IIC_Io_CB * ICB)
{
    u8 ucErrTime=0;
    ICB->IOCtrl(sda_set_in,ICB->tag);      //SDA����Ϊ����
    ICB->IOCtrl(sda_set_Low,ICB->tag);      //SDA����Ϊ����
    ICB->IOCtrl(scl_set_High,ICB->tag);
    DJY_DelayUs(ICB->delay_us);
    while(ICB->IOCtrl(sda_get,ICB->tag))
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            return false;
        }
    }
    ICB->IOCtrl(scl_set_Low,ICB->tag);//ʱ�����0
    ICB->IOCtrl(sda_set_out,ICB->tag);
    return true;
}

// =============================================================================
// ���ܣ�����IIC Ӧ���ź�
// ������IOģ���IIC���ƿ�ṹ��ָ��
// ���أ�
// =============================================================================
static void IO_IIC_Ack(struct IIC_Io_CB * ICB)
{
    ICB->IOCtrl(scl_set_Low,ICB->tag);
    ICB->IOCtrl(sda_set_out,ICB->tag);
    ICB->IOCtrl(sda_set_Low,ICB->tag);
    DJY_DelayUs(ICB->delay_us);
    ICB->IOCtrl(scl_set_High,ICB->tag);
    DJY_DelayUs(ICB->delay_us);
    ICB->IOCtrl(scl_set_Low,ICB->tag);
}

// =============================================================================
// ���ܣ�����NACKӦ��
// ������IOģ���IIC���ƿ�ṹ��ָ��
// ���أ�
// =============================================================================
static void IO_IIC_NAck(struct IIC_Io_CB * ICB)
{
    ICB->IOCtrl(scl_set_Low,ICB->tag);
    ICB->IOCtrl(sda_set_out,ICB->tag);
    ICB->IOCtrl(sda_set_Low,ICB->tag);
    DJY_DelayUs(ICB->delay_us);
    ICB->IOCtrl(sda_set_High,ICB->tag);
    DJY_DelayUs(ICB->delay_us);
    ICB->IOCtrl(scl_set_High,ICB->tag);
    DJY_DelayUs(ICB->delay_us);
    ICB->IOCtrl(scl_set_Low,ICB->tag);
    DJY_DelayUs(ICB->delay_us);
    ICB->IOCtrl(sda_set_Low,ICB->tag);
}

// =============================================================================
// ���ܣ�IIC����һ���ֽ�
// ������IOģ���IIC���ƿ�ṹ��ָ��
// ���أ�
// =============================================================================
static void IO_IIC_SendByte(struct IIC_Io_CB * ICB,u8 txd)
{
    u8 t;
    ICB->IOCtrl(sda_set_out,ICB->tag);
    ICB->IOCtrl(scl_set_Low,ICB->tag);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {
        if((txd&0x80))
            ICB->IOCtrl(sda_set_High,ICB->tag);
        else
            ICB->IOCtrl(sda_set_Low,ICB->tag);
        txd<<=1;
        DJY_DelayUs(ICB->delay_us);
        ICB->IOCtrl(scl_set_High,ICB->tag);
        DJY_DelayUs(ICB->delay_us);
        ICB->IOCtrl(scl_set_Low,ICB->tag);
    }
}

// =============================================================================
// ���ܣ�IIC��1���ֽ�
// ������ICB��IOģ���IIC���ƿ�ṹ��ָ��
//     ack��ack=0������nACK ����ֵ����ACK
// ���أ���ȡ���ֽ�
// =============================================================================
static u8 IO_IIC_ReadByte(struct IIC_Io_CB * ICB,u8 ack)
{
    unsigned char i,receive=0;
    ICB->IOCtrl(sda_set_in,ICB->tag);//SDA����Ϊ����
    for(i=0;i<8;i++ )
    {
        ICB->IOCtrl(scl_set_Low,ICB->tag);
        DJY_DelayUs(ICB->delay_us);
        ICB->IOCtrl(scl_set_High,ICB->tag);
        receive<<=1;
        if(ICB->IOCtrl(sda_get,ICB->tag))
            receive++;
        DJY_DelayUs(ICB->delay_us);
    }
    if (!ack)
        IO_IIC_NAck(ICB);//����nACK
    else
        IO_IIC_Ack(ICB); //����ACK
    return receive;
}


// =============================================================================
// ���ܣ�IIC���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�IICʱ��Ƶ������
// ������ICB,���Ա�ǣ���ģ���ڼ� IOģ���IIC���ƿ�ṹ��ָ��
//       cmd,����
//       data1,data2,���ݣ�������������
// ���أ���
// =============================================================================
static s32 IO_IIC_BusCtrl(struct IIC_Io_CB *ICB,u32 cmd,u32 data1,u32 data2)
{

    switch(cmd)
    {
        case CN_IIC_SET_CLK: //����ʱ��Ƶ��
            ICB->delay_us = (1000000/(2*data1));
            break;
        case CN_IIC_DMA_USED://ʹ��dma����
            break;
        case CN_IIC_DMA_UNUSED://��ֹdma����
            break;
        case CN_IIC_SET_POLL:           //ʹ����ѯ��ʽ���ͽ���
            break;
        default:     return 0;
    }
    return 1;
}
// =============================================================================
// ���ܣ���ѯ��ʽ��IIC���豸������
// ������ICB,���Ա�ǣ���ģ���ڼ� IOģ���IIC���ƿ�ṹ��ָ��
//     devaddr,���������ַ�����һbit���ֶ�д
//     memaddr//�Ĵ�����ַ
//     maddrlen�ļĴ�����ַ�ĳ���
//     addr,���ʵļĴ�����ַ
//     buf, �洢������
//     len, �洢�������ĳ���
// ���أ�len,��ȡ�ֽ���;-1,��ȡʧ��
// =============================================================================
static s32 IO_IIC_ReadPoll(struct IIC_Io_CB * ICB,u8 devaddr,u32 memaddr,
                        u8 maddrlen, u8 *buf, u32 len)
{

    u8 mem_addr_buf[4];
    //����ַ����С�˱任
    u8 i;
    for(i=0;i<maddrlen;i++)
    {
        mem_addr_buf[(maddrlen-1)-i]=((memaddr>>(8*i))&0xff);
    }

    IO_IIC_Start( ICB);//������ʼλ

    IO_IIC_SendByte(ICB, ((devaddr<<1))); // ����д������ַ
    if(false ==IO_IIC_WaitAck( ICB))
        return (-1);

    for(i=0;i<maddrlen;i++)//���������ڲ���ַ
    {
      IO_IIC_SendByte(ICB, mem_addr_buf[i]);
      if(false ==IO_IIC_WaitAck( ICB))
          return (-1);
    }
    IO_IIC_Stop( ICB); //������ʼλ
    IO_IIC_Start( ICB); //������ʼλ
    IO_IIC_SendByte(ICB, ((devaddr<<1)|0x01));//����������ַ��
    if(false ==IO_IIC_WaitAck( ICB))
        return (-1);

    for(i=0;i<len;i++)
    {
        buf[i]=IO_IIC_ReadByte(ICB,i==(len-1)?0:1); //������
    }
    IO_IIC_Stop(ICB);

    return i;
}

// =============================================================================
// ���ܣ���ѯ��ʽ��IIC���豸��д����
// ������    ICB,���Ա�ǣ���ģ���ڼ� IOģ���IIC���ƿ�ṹ��ָ��
//       devaddr,�豸��ַ
//       memaddr,�豸�ڲ���ַ
//       maddrlen,�豸�ڲ���ַ����
//       buf,�洢����
//       len,���ݳ��ȣ��ֽ�
// ���أ�len,��ȡ�ɹ�;-1,��ȡʧ��
// =============================================================================
static s32 IO_IIC_WritePoll(struct IIC_Io_CB * ICB,u8 devaddr,u32 memaddr,
                        u8 maddrlen, u8 *buf, u32 len)
{
    u8 mem_addr_buf[4];
    u8 i;
    for(i=0;i<maddrlen;i++)
    {
        mem_addr_buf[(maddrlen-1)-i]=((memaddr>>(8*i))&0xff);
    }

    IO_IIC_Start(ICB);

    IO_IIC_SendByte(ICB,(devaddr<<1));    //����������ַ
    if(false ==IO_IIC_WaitAck( ICB))
             return 0;

    for(i=0;i<maddrlen;i++)
    {
        IO_IIC_SendByte(ICB,mem_addr_buf[i]);    //���������ڲ���ַ
        if(false ==IO_IIC_WaitAck( ICB))
                 return 0;
    }

    for(i=0;i<len;i++)
    {
        IO_IIC_SendByte(ICB,buf[i]);   //������
        if(false == IO_IIC_WaitAck(ICB))
            break;
    }
    IO_IIC_Stop(ICB);                  //����һ��ֹͣ����
    return i;
}

// =============================================================================
// ���ܣ���ѯ��ʽ��дIIC�豸
// ������    ICB,���Ա�ǣ���ģ���ڼ� IOģ���IIC���ƿ�ṹ��ָ��
//       DevAddr,�豸��ַ
//       MemAddr,�豸�ڲ���ַ
//       MemAddrLen,�豸�ڲ���ַ����
//       Buf,�洢����
//       Length,���ݳ��ȣ��ֽ�
//       WrRdFlag,��д��ǣ�Ϊ0ʱд��1ʱΪ��
// ���أ�
// =============================================================================
static bool_t IO_IIC_WriteReadPoll( struct IIC_Io_CB * ICB,u8 DevAddr,u32 MemAddr,\
                                u8 MemAddrLen,u8* Buf, u32 Length,u8 WrRdFlag)
{
    static bool_t flag;


    if(WrRdFlag == CN_IIC_WRITE_FLAG)   //д
    {
        if(Length == IO_IIC_WritePoll(ICB,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            flag = true;
        else
            flag = false;
    }
    else                                //��
    {
        if(Length == IO_IIC_ReadPoll(ICB,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            flag = true;
        else
            flag = false;
    }
    return  flag ;
}

// =============================================================================
// ���ܣ����IOģ���IIC����
// ������ init,�Ĵ�����ַ
//       *BusName      //�������ƣ���IIC1
//       tag           //�û��Լ��ı��
//       IICBufLen     //���߻�������С���ֽ�
//       Clk           //����Ƶ�ʵ�λHZ
//       IIC_IoCtrl    //��������enum IIc_Io ���û��Լ��ı�ǩ����IO��״̬
// ���أ�true/false
// =============================================================================
bool_t ModuleInstall_IO_IICBus(struct IO_IIC_Init * init)
{
    struct IIC_Io_CB * IicIoCb;
    struct IIC_Param IIC_Config;

    IicIoCb = (struct IIC_Io_CB *)M_Malloc(sizeof(struct IIC_Io_CB),0);
    if(IicIoCb == NULL)
    {
        free(IicIoCb);
        return false;
    }

    IicIoCb->delay_us              = (u32)(5);//Ĭ��ֵоƬ���ʱ������
    IicIoCb->IOCtrl                = init->IIC_IoCtrl ;
    IicIoCb->tag                   = init->tag ;
//    IicIoCb->IOCtrl(sda_set_out,IicIoCb->tag);
//    IicIoCb->IOCtrl(scl_set_out,IicIoCb->tag);
//    IicIoCb->IOCtrl(sda_set_High,IicIoCb->tag);
//    IicIoCb->IOCtrl(scl_set_High,IicIoCb->tag);

    IIC_Config.BusName             = init->BusName;
    IIC_Config.IICBuf              = NULL;
    IIC_Config.IICBufLen           = 0;
    IIC_Config.SpecificFlag        = (ptu32_t)IicIoCb;
    IIC_Config.pGenerateWriteStart = (WriteStartFunc)NULL;
    IIC_Config.pGenerateReadStart  = (ReadStartFunc)NULL;
    IIC_Config.pGenerateEnd        = (GenerateEndFunc)NULL;
    IIC_Config.pBusCtrl            = (IICBusCtrlFunc)IO_IIC_BusCtrl;
    IIC_Config.pWriteReadPoll      = (WriteReadPoll)IO_IIC_WriteReadPoll;

    if(NULL != IIC_BusAdd(&IIC_Config))
        return true;
    return false;

}




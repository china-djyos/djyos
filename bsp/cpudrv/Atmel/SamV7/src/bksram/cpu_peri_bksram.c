//-----------------------------------------------------------------------------

// �ļ���     ��cpu_peri_busram.c
// ģ������: CPUƬ��backup 1K SRAM��д����
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 09/11.2015
// =============================================================================

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "os.h"
#include "cpu_peri.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip Backup area sram"//��ر��ֵ�4KƬ��ram��д
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                        //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                     //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                       //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                       //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                       //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                       //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                       //����������������������г����á�,���ָ�
//weakdependence:"none"                //��������������������������none����ʾ�������������
                                       //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                       //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                       //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_BACKUP_AREA_SRAM == false )
//#warning  " cpu_onchip_Backup_area_sram  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_BACKUP_AREA_SRAM    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
#define BACKUPSRAM_ADDR     (0x40074000)        //Ƭ�ڵ�1K Backup SRAM��ʼ��ַ
#define BACKUPSRAM_SIZE     (0x400)             //1K�ֽڵ�����

// =============================================================================
// ���ܣ���Ƭ�ڵ�1K Bytes��ַ�ռ�����ݣ����������ݵ��籣��
// ������rBuffer���洢���ݵĻ�����
//      offset����Դ洢��ʼλ��ƫ�ƣ�32λ����
//      len���洢�����ݳ���
// ���أ�д�������DWORD��4�ֽڣ���
// =============================================================================
u32 BackUpSram_Read(u8 *rBuffer,u32 offset,u32 len)
{
    u32 i,addr,dw;
    if( (NULL == rBuffer) || (offset + len >= BACKUPSRAM_SIZE) || (offset%4 != 0))
        return 0;

    addr = BACKUPSRAM_ADDR + offset;
    for(i = 0; i < len/4; i++)      //copy ��4�����Ĳ���
    {
        dw = *((u32*)(addr + i*4));
        rBuffer[4*i] = dw&0xff;
        rBuffer[4*i+1] = (dw>>8)&0xff;
        rBuffer[4*i+2] = (dw>>16)&0xff;
        rBuffer[4*i+3] = (dw>>24)&0xff;
    }

    dw = *((u32*)(addr + i*4));
    for(i = i*4; i < len; i++)      //copyʣ�ಿ��
    {
        rBuffer[i] = dw & 0xff;
        dw >>= 8;
    }
    return i;
}

// =============================================================================
// ���ܣ�дƬ�ڵ�1K Bytes��ַ�ռ�����ݣ����������ݵ��籣��
// ������rBuffer���洢���ݵĻ�����
//      offset����Դ洢��ʼλ��ƫ�ƣ�32λ����
//      len���洢�����ݳ���
// ���أ�д�������DWORD��4�ֽڣ���
// =============================================================================
u32 BackUpSram_Write(u8 *wBuffer,u32 offset,u32 len)
{
    u32 i,addr,dw;
    if( (NULL == wBuffer) || (offset + len >= BACKUPSRAM_SIZE) || (offset%4 != 0))
        return 0;

    addr = BACKUPSRAM_ADDR + offset;
    for(i = 0; i < len/4; i++)      //copy ��4�����Ĳ���
    {
        dw =  wBuffer[4*i]
            + (wBuffer[4*i+1]<<8)
            + (wBuffer[4*i+2]<<16)
            + (wBuffer[4*i+3]<<24);
        *(u32*)(addr+4*i) = dw;
    }

    for(i = i*4; i < len; i++)      //copyʣ�ಿ��
    {
        dw = wBuffer[i]<<(i*8);
    }
    *(u32*)(addr+(i&~0x3)) = dw;
    return i;
}

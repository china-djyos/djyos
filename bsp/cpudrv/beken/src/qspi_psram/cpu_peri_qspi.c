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
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// =============================================================================
#include <typedef.h>
#include <stdlib.h>
#include <dbug.h>
#include "qspi/qspi_pub.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern int ModuleInstall_QSPI_PSRAM(void);
//    ModuleInstall_QSPI_PSRAM();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"qspi"//qspi�ӿڵ�psram
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","component heap"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_QSPI_RAM_SIZE           //****�������Ƿ��Ѿ����ú�
#warning   qspi�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,8388608,
#define CFG_QSPI_RAM_SIZE                   8388608       //RAM�ĳߴ磬Ĭ��8M��
#define CFG_QSPI_RAM_PAGE_SIZE              1024          //RAM��ҳ�ߴ磬Ĭ��1K��
#define CFG_QSPI_RAM_ALIGNMENT              0             //Ҫ��Ķ���ߴ磬0��ʾ��ϵͳ�Ķ���ߴ�
//%$#@enum,true,false,
#define CFG_QSPI_RAM_DEDICATED              false         //true��ʾר��heap��false��ʾͨ��heap��
//%$#@string,1,10,
#define CFG_QSPI_RAM_HEAP_NAME              "PSRAM"      //�½���heap���֡�
//%$#@select
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure						
	

// =============================================================================
// ���ܣ���ʼ��QSPIʹ��psramģ��
// ��������
// ���أ�1=�ɹ���0=ʧ��
// =============================================================================
int ModuleInstall_QSPI_PSRAM(void)
{
    qspi_dcache_drv_desc qspi_cfg;

    qspi_cfg.mode = 0;        // 0: 1 line mode    3: 4 line mode
    qspi_cfg.clk_set = 0x0;
    qspi_cfg.wr_command = 0x02;        //write
    qspi_cfg.rd_command = 0x03;        //read
    qspi_cfg.wr_dummy_size = 0;
    qspi_cfg.rd_dummy_size = 0x00;

    bk_qspi_mode_start(1, 0);
    if(bk_qspi_dcache_configure(&qspi_cfg))
    {
        info_printf("QSPI","QSPI init fail.\r\n");
        return 0;
    }

    if(Heap_Add(QSPI_DCACHE_BASE, CFG_QSPI_RAM_SIZE, CFG_QSPI_RAM_PAGE_SIZE, CFG_QSPI_RAM_ALIGNMENT,
            CFG_QSPI_RAM_DEDICATED, CFG_QSPI_RAM_HEAP_NAME) == 0)       //��QSPI�ӵ�RAM���ӵ�heap����
    {
        info_printf("QSPI","QSPI add heap fail.\r\n");
        return 0;
    }

    return 1;
}


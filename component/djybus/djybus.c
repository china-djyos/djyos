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
// �ļ���     ��djybus.c
// ģ������: djybusģ����Ҫ��DJYOS���߹��������������߷��࣬�γɽڵ�ֱ�ҵ�������
//           "DjyBus"���棬ÿ�����Ϳ����ж������ߣ���ÿ�����߿���Ҳ�ж���������γ�
//           ���������ߡ��������͡���DjyBus������Ҷ����������������
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 17/06.2014
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "object.h"
#include "dbug.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_DjyBus(void);
//    ModuleInstall_DjyBus ( );
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"djybus"       //djybus���
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock"             //������������������������none����ʾ�������������
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure


static struct obj *s_ptDjybusDir;                //���徲̬����DjyBus�ĸ����
// =============================================================================
// ���ܣ���������ʼ��DjyBus���߸��ڵ㣬�����������ͽڵ�ĸ����
// ������para,��ʵ������
// ���أ�true=�ɹ�, false=ʧ��.
// =============================================================================
bool_t ModuleInstall_DjyBus (void)
{
    //����Դ�����н���һ������㣬���н��������߽�㶼���ڸý���ϡ�
    s_ptDjybusDir = obj_newchild(objsys_root(), (fnObjOps)-1, 0, 0,"DjyBus");
    if(s_ptDjybusDir)
    {
        info_printf("module","bus installed.");
        return true;
    }

        error_printf("module","bus installation failed.");
    return false;
}

// =============================================================================
// ���ܣ���DjyBus����������������µ��������ͽ�㣬�����������ͽ����DjyBus���ӽ��,
//       ʹ���ڴ�ط���ķ�ʽ�����ڴ�
// ������NewBusTypeName,������������
// ���أ����ؽ�������Դ���ָ�룬ʧ��ʱ����NULL
// =============================================================================
struct obj * DjyBus_BusTypeAdd (const char* NewBusTypeName)
{
    struct obj * NewBusType;

    //�����ظ�����ͬ������������
    if(NULL != obj_search_child(s_ptDjybusDir, NewBusTypeName))
        return NULL;

    NewBusType = obj_newchild(s_ptDjybusDir, (fnObjOps)-1, 0, 0, NewBusTypeName);

    return NewBusType;
}

// =============================================================================
// ���ܣ�ɾ��DjyBus���߸�����ϵ�һ���������ͽ�㣬���ͷŶ�̬������ڴ�飬,�����ͽ�
//       �㲻�����ӽ��
// ������DelBusType,��ɾ�����������ͽ��
// ���أ�TRUE,ɾ���ɹ�;false,ɾ��ʧ��
// =============================================================================
bool_t DjyBus_BusTypeDelete(struct obj * DelBusType)
{
    bool_t result;

    if(obj_del(DelBusType))
    {
        result = false;
    }
    else
    {
        result = true;
    }
    return result;
}

// =============================================================================
// ���ܣ����������������Ʋ����������ͽڵ㣬���ز��ҵ��Ľ��ָ��
// ������BusTypeName,�����ҵ��������ͽ������
// ���أ����ָ�룬NULLʱ����ʧ��
// =============================================================================
struct obj * DjyBus_BusTypeFind(const char * BusTypeName)
{
    return obj_search_child(s_ptDjybusDir,BusTypeName);
}

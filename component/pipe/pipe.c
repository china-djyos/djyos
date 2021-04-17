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
//����ģ��: ��ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: �ܵ�ģ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2010-10-01
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"

#if 0
//�ܵ����ƿ�
//�����ܵ�ʱ������ܵ���û���㹻�������������������ź����У�д������ʱʹ�ܵ��е�
//����������level���������ͷ��ź�����
//д�ܵ�ʱ�����д���ˣ����������ź����У����ܵ�ʹ�ùܵ��е�����������levelʱ��
//���ͷ��ź�����
//�ܵ��ǵ��߳������

//�ر�ע��:�ܵ���������������ԱӦ���Լ�ע�ⲻҪ������������������Ϊ:
//1������ӹܵ����������������ܵ����ȣ��ܵ�д������������Ϊ��������һֱ�ò���
//   ���㣬��������������ȥ��
//2������ģʽ�£����������������(�ܵ�����-���һ������д��������)�������ֺ�1һ
//   ����Ч����
 struct PipePCB
{
    struct RingBuf pipe_buf;               //���η��ͻ�����.
    struct SemaphoreLCB *pipe_write_semp;  //д��������
    struct SemaphoreLCB *pipe_read_semp;   //����������
    u32    write_level;                     //д����ˮƽ
    u32    read_level;                      //������ˮƽ
    bool_t whole_mode;          //����ģʽ��true=ȷ��ÿ��д��������¼������Ҫд
                                //��10���ֽڣ���pipe_buf��ֻ��9����λʱ��������
                                //false=��д��9����Ȼ������
};


static struct DjyDevice * s_pPipeParentDevice;
#if(CN_CFG_PIPES_LIMIT != 0)
static struct PipePCB s_tPipeMemBlock[CN_CFG_PIPES_LIMIT];
#endif
static struct MemCellPool *s_ptPipePool;  //�ź����ṹ�ڴ��ͷָ��

//----��ʼ��pipeģ��ģ��-------------------------------------------------------
//���ܣ���"dev"�豸�´���"pipe"�豸����Ϊ��������pipe�豸�ĸ��豸������һ����
//      �豸�����κξ���Ĺ��ܡ�
//������para�������ߴ���Ĳ�������ģ��δʹ��
//���أ�1=�ɹ���0=ʧ��
//-----------------------------------------------------------------------------
ptu32_t Pipe_ModuleInit(ptu32_t para)
{
    static struct MemCellPool pipe_pool;

    para = para;        //�����������澯
    s_pPipeParentDevice = Driver_DevAddDevice( Driver_DevGetRootDevice(),"pipe",
                                 NULL,NULL,      //���ź�������
                                 (devWriteFunc) DJY_NullFunc ,
                                 (devReadFunc ) DJY_NullFunc,
                                 (devCtrlFunc ) DJY_NullFunc ,
                                 (devWriteFunc ) DJY_NullFunc ,
                                 (devReadFunc  ) DJY_NullFunc ,
                                 (devCtrlFunc  ) DJY_NullFunc,
                                 (ptu32_t)NULL   //key�豸˽������
                                 );
    if(s_pPipeParentDevice == NULL)
        return 0;

    //��ʼ��pipe���ƿ��ڴ��
#if(CN_CFG_PIPES_LIMIT != 0)
    s_ptPipePool = Mb_CreatePool_r( &pipe_pool,
                                    (void*)s_tPipeMemBlock,
                                    CN_CFG_PIPES_LIMIT,
                                    sizeof(struct PipePCB),
                                    0,0,
                                    "pipe���ƿ��");
#else
    s_ptPipePool = Mb_CreatePool_r( &pipe_pool,
                                    NULL,
                                    CN_CFG_PIPES_LIMIT,
                                    sizeof(struct PipePCB),
                                    0,0,
                                    "pipe���ƿ��");
#endif
    return 1;
}


//----����һ���ܵ�-----------------------------------------------------------
//���ܣ�����һ���ܵ�
//������buf,�ܵ�ʹ�õĻ�����������ʹ���߷����ṩ��
//      size����������С
//      whole��true=һ���Բ�������Ҫ���д����һ������ɣ�false=��һ���Բ�����
//          д��ʱ���ж��ٿ�λд����٣���ȡʱ���ܵ����ж������ݶ�ȡ����
//      name���ܵ������֣���ָ����ַ����ڴ��������Ǿֲ�����
//���أ��½������ź���ָ��
//-----------------------------------------------------------------------------
struct DjyDevice * Pipe_Create(u8* buf,u32 size,bool_t whole,
                             u32 write_level,u32 read_level,char *name)
{
    struct PipePCB *pipe;
    struct SemaphoreLCB *read_dev_semp,*write_dev_semp;
    struct DjyDevice * pipe_device;

    pipe = (struct PipePCB *)Mb_Malloc(s_ptPipePool, 0);
    if(pipe == NULL)
        goto exit_from_malloc_PCB;
    //��ʼ�������ź�״̬����������ͷŲŴ������ź�״̬
    pipe->pipe_write_semp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,name);
    if(pipe->pipe_write_semp == NULL)
        goto exit_from_pipe_write_semp;
    //��ʼ�������ź�״̬����д�����ͷŲŴ������ź�״̬
    pipe->pipe_read_semp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,name);
    if(pipe->pipe_read_semp == NULL)
        goto exit_from_pipe_read_semp;
    read_dev_semp = Lock_SempCreate(1,1,CN_BLOCK_FIFO,NULL);
    if(read_dev_semp == NULL)
        goto exit_from_dev_read_semp;
    write_dev_semp = Lock_SempCreate(1,1,CN_BLOCK_FIFO,NULL);
    if(write_dev_semp == NULL)
        goto exit_from_dev_write_semp;
    pipe_device = Driver_DevAddDevice(s_pPipeParentDevice,
                                 name,
                                 read_dev_semp,write_dev_semp,
                                 NULL,
                                 Pipe_DevRead,
                                 Pipe_DevCtrl,
                                 Pipe_DevWrite,
                                 NULL,
                                 Pipe_DevCtrl,
                                 (ptu32_t)pipe       );
    if(pipe_device == NULL)
        goto exit_from_create_device;

    Ring_Init(&pipe->pipe_buf,buf,size);
    pipe->write_level = write_level;
    pipe->read_level = read_level;
    pipe->whole_mode = whole;
    return pipe_device;

exit_from_create_device:
    Lock_SempDelete(write_dev_semp);
exit_from_dev_write_semp:
    Lock_SempDelete(read_dev_semp);
exit_from_dev_read_semp:
    Lock_SempDelete(pipe->pipe_read_semp);
exit_from_pipe_read_semp:
    Lock_SempDelete(pipe->pipe_write_semp);
exit_from_pipe_write_semp:
    Mb_Free(s_ptPipePool,pipe);
exit_from_malloc_PCB:
    return false;
}

//----���ܵ�-------------------------------------------------------------------
//����: �ӹܵ��ж�����
//����: pipe_dev��Ŀ��ܵ��豸
//      res����������
//      dst_buf���������ݴ�ŵ�ַ
//      len����������
//����: ���Ƿ���len
//-----------------------------------------------------------------------------
ptu32_t Pipe_DevRead(struct DjyDevice * pipe_dev,ptu32_t dst_buf,
                                        ptu32_t len,u32 timeout)
{
    struct PipePCB *pipe;
    u32 completed=0,temp;
    pipe = (struct PipePCB *)Driver_DevGetMyTag(pipe_dev);
    if(len > Ring_Capacity(&pipe->pipe_buf))    //һ�ζ�ȡ���ݲ��ܳ����ܵ�����
        return 0;
    temp = Ring_Check(&pipe->pipe_buf);
    while(1)
    {
        completed += Ring_Read(&pipe->pipe_buf,(u8*)(dst_buf+completed),
                               (u32)len-completed);
        if(completed < (u32)len)    //�ܵ��е�����������
        {
            Lock_SempPend(pipe->pipe_read_semp,timeout);
        }else
        {
            break;
        }
    }
    //�����֮ǰ����д���ż��Ҷ�֮�����д���ż���post�ź���
    if( (temp >= pipe->write_level)
          &&((temp - completed) < pipe->write_level))
    {
        Lock_SempPost(pipe->pipe_write_semp);
    }
    return len;
}

//----д�ܵ�-------------------------------------------------------------------
//����: ������д��ܵ���
//����: pipe_dev��Ŀ��ܵ��豸
//      src_buf��д�����ݴ�ŵ�ַ
//      res����������
//      len��д�볤��
//����: ʵ��д��������
//-----------------------------------------------------------------------------
u32 Pipe_DevWrite(struct DjyDevice * pipe_dev,ptu32_t src_buf,
                            ptu32_t len,u32 timeout)
{
    struct PipePCB *pipe;
    u32 completed = 0, temp;
    pipe = (struct PipePCB *)Driver_DevGetMyTag(pipe_dev);
    temp = Ring_Check(&pipe->pipe_buf);
    while(1)
    {
        if(pipe->whole_mode == true)        //����ģʽ
        {
            //�ܵ��е�ʣ��ռ䲻��
            if((Ring_Capacity(&pipe->pipe_buf)-Ring_Check(&pipe->pipe_buf))
                        < (u32)len)
            {
                Lock_SempPend(pipe->pipe_write_semp,timeout);
            }else
            {
                Ring_Write(&pipe->pipe_buf,(u8*)src_buf,(u32)len);
                break;
            }
        }else
        {
            completed +=Ring_Write(&pipe->pipe_buf,(u8*)src_buf,len -completed);
            if(completed >= len)
                break;
        }
    }

    //���д֮ǰ������ȡ�ż��Ҷ�֮�󳬹�д���ż���post�ź���
    if( (temp < pipe->read_level)
          &&((temp + completed) >= pipe->read_level))
    {
        Lock_SempPost(pipe->pipe_read_semp);
    }
    return completed;
}

//----���ƹܵ�-----------------------------------------------------------------
//����: �պ���
//-----------------------------------------------------------------------------
ptu32_t Pipe_DevCtrl(struct DjyDevice * pipe_dev,u32 cmd,
                        u32 data1,u32 data2)
{
    return 0;
}
//----ɾ��һ���ܵ�-----------------------------------------------------------
//���ܣ�ɾ��һ���ܵ�
//������mutex����ɾ���Ĺܵ��豸
//���أ���
//-----------------------------------------------------------------------------
bool_t Pipe_Delete(struct DjyDevice * pipe_dev)
{
    struct PipePCB *pipe;
    struct SemaphoreLCB *pipe_left_semp,*pipe_right_semp;
    pipe = (struct PipePCB *)Driver_DevGetMyTag(pipe_dev);
    pipe_left_semp = pipe_dev->left_semp;
    pipe_right_semp = pipe_dev->right_semp;
    if(Driver_DevDeleteDevice(pipe_dev))
    {
        Lock_SempDelete(pipe_left_semp);
        Lock_SempDelete(pipe_right_semp);
        Lock_SempDelete(pipe->pipe_read_semp);
        Lock_SempDelete(pipe->pipe_write_semp);
        Mb_Free(s_ptPipePool,pipe);
        return true;
    }else
        return false;
}

//----��ѯ�ܵ�����-----------------------------------------------------------
//���ܣ���ѯһ���ܵ��豸��������
//������pipe_dev������ѯ�Ĺܵ����
//���أ��ܵ�����
//-----------------------------------------------------------------------------
u32 Pipe_Capacity(struct DjyDevice * pipe_dev)
{
    struct PipePCB *pipe;
    pipe = (struct PipePCB *)Driver_DevGetMyTag(pipe_dev);
    return Ring_Capacity(&pipe->pipe_buf);
}

//----��ѯ�ܵ���������---------------------------------------------------------
//���ܣ���ѯһ���ܵ��豸�е���������
//������pipe_dev������ѯ�Ĺܵ����
//���أ��ܵ��е�������
//-----------------------------------------------------------------------------
u32 Pipe_Check(struct DjyDevice * pipe_dev)
{
    struct PipePCB *pipe;
    pipe = (struct PipePCB *)Driver_DevGetMyTag(pipe_dev);
    return Ring_Check(&pipe->pipe_buf);
}

#endif

#include "stdint.h"
#include "set-cache.h"
#include "arch_feature.h"

//----����cache----------------------------------------------------------------
//����: ��ָ��cache������cacheȫ�����ϣ�����cache�е�"��"���ݲ���д�����档
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_InvalidAll(void)
{
    u32 reg = 0;
#if         defined( arm720t) \
        ||  defined( arm920t) \
        ||  defined( arm922t)\
        ||  defined( arm926ej_s)\
        ||  defined( arm1022e)\
        ||  defined( arm1026ej_s)\
        ||  defined( strong_arm)\
        ||  defined( xscale)
    __asm__(
        "mov    %0,#0 \n\t"
        "mcr    p15,0,%0,c7,c7,0 \n\t"
        :
        :"r"(reg)
        );
#elif       defined( arm940t) ||  defined( arm946e_s)
    __asm__(
        "mov    %0,#0 \n\t"
        "mcr    p15,0,%0,c7,c5,0 \n\t"
        "mcr    p15,0,%0,c7,c6,0 \n\t"
        :
        :"r"(reg)
        );
#endif
}

//----����ָ��cache------------------------------------------------------------
//����: ��ָ��cache����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_InvalidInst(void)
{
    u32 reg = 0;
#if     defined( arm920t) \
    ||  defined( arm922t)\
    ||  defined( arm926ej_s)\
    ||  defined( arm940t)\
    ||  defined( arm946e_s)\
    ||  defined( arm1022e)\
    ||  defined( arm1026ej_s)\
    ||  defined( strong_arm)\
    ||  defined( xscale)
    __asm__(
        "mov    %0,#0 \n\t"
        "mcr    p15,0,%0,c7,c5,0 \n\t"
        :
        :"r"(reg)
        );
#elif defined( arm720t) || defined( arm740t)
    __asm__(
        "mov    %0,#0 \n\t"
        "mcr    p15,0,%0,c7,c7,0 \n\t"
        :
        :"r"(reg)
        );
#endif
}

//----��������cache------------------------------------------------------------
//����: ������cacheȫ�����ϣ�����cache�е�"��"���ݲ���д�����档
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_InvalidData(void)
{
    u32 reg = 0;
#if     defined( arm920t) \
        ||  defined( arm922t)\
        ||  defined( arm926ej_s)\
        ||  defined( arm940t)\
        ||  defined( arm946e_s)\
        ||  defined( arm1022e)\
        ||  defined( arm1026ej_s)\
        ||  defined( strong_arm)\
        ||  defined( xscale)
        __asm__(
            "mov    %0,#0 \n\t"
            "mcr    p15,0,%0,c7,c6,0 \n\t"
            :
            :"r"(reg)
            );
#elif defined( arm720t) || defined( arm740t)
        __asm__(
            "mov    %0,#0 \n\t"
            "mcr    p15,0,%0,c7,c7,0 \n\t"
            :
            :"r"(reg)
            );
#endif
}

//----ʹ��cache----------------------------------------------------------------
//����: ʹ��ָ�������cache
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_EnableAll(void)
{
    u32 reg = 0;
    __asm__(
        "mrc    p15, 0, %0, c1, c0, 0 \n\t"
        "orr    %0, %0, #0x1000 \n\t"
        "orr    %0, %0, #0x4 \n\t"
        "mcr    p15,0,%0,c1,c0,0 \n\t"
        :
        :"r"(reg)
        );
}

//----ʹ��ָ��cache------------------------------------------------------------
//����: ʹ��ָ��cache
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_EnableInst(void)
{
    u32 reg = 0;
    __asm__(
        "mrc    p15, 0, %0, c1, c0, 0 \n\t"
        "orr    %0, %0, #0x1000 \n\t"
        "mcr    p15,0,%0,c1,c0,0 \n\t"
        :
        :"r"(reg)
        );
}

//----ʹ������cache------------------------------------------------------------
//����: ʹ������cache
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_EnableData(void)
{
    u32 reg = 0;
    __asm__(
        "mrc    p15, 0, %0, c1, c0, 0 \n\t"
        "orr    %0, %0, #0x4 \n\t"
        "mcr    p15,0,%0,c1,c0,0 \n\t"
        :
        :"r"(reg)
        );
}

//----��ֹcache----------------------------------------------------------------
//����: ��ָֹ�������cache
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_DisableAll(void)
{
    u32 reg = 0;
    __asm__(
        "mrc    p15, 0, %0, c1, c0, 0 \n\t"
        "orr    %0, %0, #0x1000 \n\t"
        "orr    %0, %0, #0x4 \n\t"
        "mcr    p15,0,%0,c1,c0,0 \n\t"
        :
        :"r"(reg)
        );
}

//----��ָֹ��cache------------------------------------------------------------
//����: ��ָֹ��cache
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_DisableInst(void)
{
    u32 reg = 0;
    __asm__(
        "mrc    p15, 0, %0, c1, c0, 0 \n\t"
        "orr    %0, %0, #0x1000 \n\t"
        "mcr    p15, 0, %0, c1, c0, 0 \n\t"
        :
        :"r"(reg)
        );
}

//----��ֹ����cache------------------------------------------------------------
//����: ��ֹ����cache
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_DisableData(void)
{
    u32 reg = 0;
    __asm__(
        "mrc    p15, 0, %0, c1, c0, 0 \n\t"
        "orr    %0, %0, #0x4 \n\t"
        "mcr    p15,0,%0,c1,c0,0 \n\t"
        :
        :"r"(reg)
        );
}


//----ʹ��д����---------------------------------------------------------------
//����: ʹ��д����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_EnableWriteBuf(void)
{
    u32 reg = 0;
    __asm__(
        "mrc    p15, 0, %0, c1, c0, 0 \n\t"
        "orr    %0, %0, #0x8 \n\t"
        "mcr    p15, 0, %0, c1, c0, 0 \n\t"
        :
        :"r"(reg)
        );
}

//----��ֹд����---------------------------------------------------------------
//����: ��ֹд����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_DisableWriteBuf(void)
{
    u32 reg = 0;
    __asm__(
        "mrc    p15, 0, %0, c1, c0, 0 \n\t"
        "orr    %0, %0, #0x8 \n\t"
        "mcr    p15,0,%0,c1,c0,0 \n\t"
        :
        :"r"(reg)
        );
}

//----��������cache------------------------------------------------------------
//����: ������������cache������cache�е�"��"����д�����棬�Ա���ͬ����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_CleanData(void)
{
#if defined( arm926ej_s)      ||  defined( arm1026ej_s)
    __asm__("clean: \n\t"
        "mrc p15,0,pc,c7,c10,3 \n\t"
        "bne clean \n\t");
#elif       defined( arm920t) \
        ||  defined( arm922t)\
        ||  defined( arm940t)\
        ||  defined( arm946e_s)\
        ||  defined( arm1022e)
    u32 rd,way,line;
    for(way = 0; way < CN_CACHE_WAY; way++)
    {
        //��ʽ����õ�ÿ·������
        for(line = 0;line<CN_CACHE_SIZE/CN_CACHE_WAY/CN_CACHE_LINE_SIZE; line++)
        {
            rd = (way<<CN_C7_OFFSET_WAY) + (line<<CN_C7_OFFSET_SET);
            __asm__("mcr    p15, 0, %0, c7, c10, 2 \n\t"
                    :
                    :"r"(rd)
            );
        }
    }
#elif defined( arm720t) || defined( arm740t)
//����cpuֻ��дͨ��ʽ����������
#else
#error ����ʶ��cpu����
#endif
}

void Cache_config(void)
{
    //...
}




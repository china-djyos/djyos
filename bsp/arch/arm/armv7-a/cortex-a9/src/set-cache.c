#include "stdint.h"
#include "set-cache.h"
#include "arch_feature.h"

//----作废cache----------------------------------------------------------------
//功能: 把指令cache和数据cache全部作废，数据cache中的"脏"数据并不写回主存。
//参数: 无
//返回: 无
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

//----作废指令cache------------------------------------------------------------
//功能: 把指令cache作废
//参数: 无
//返回: 无
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

//----作废数据cache------------------------------------------------------------
//功能: 把数据cache全部作废，数据cache中的"脏"数据并不写回主存。
//参数: 无
//返回: 无
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

//----使能cache----------------------------------------------------------------
//功能: 使能指令和数据cache
//参数: 无
//返回: 无
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

//----使能指令cache------------------------------------------------------------
//功能: 使能指令cache
//参数: 无
//返回: 无
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

//----使能数据cache------------------------------------------------------------
//功能: 使能数据cache
//参数: 无
//返回: 无
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

//----禁止cache----------------------------------------------------------------
//功能: 禁止指令和数据cache
//参数: 无
//返回: 无
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

//----禁止指令cache------------------------------------------------------------
//功能: 禁止指令cache
//参数: 无
//返回: 无
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

//----禁止数据cache------------------------------------------------------------
//功能: 禁止数据cache
//参数: 无
//返回: 无
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


//----使能写缓冲---------------------------------------------------------------
//功能: 使能写缓冲
//参数: 无
//返回: 无
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

//----禁止写缓冲---------------------------------------------------------------
//功能: 禁止写缓冲
//参数: 无
//返回: 无
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

//----清理数据cache------------------------------------------------------------
//功能: 清理整个数据cache，数据cache中的"脏"数据写回主存，以保持同步。
//参数: 无
//返回: 无
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
        //下式计算得到每路的行数
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
//这两cpu只有写通方式，无须清理。
#else
#error 不认识的cpu类型
#endif
}

void Cache_config(void)
{
    //...
}




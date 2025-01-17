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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块:调度器
//作者:  罗侍田.
//版本：V1.0.0
//文件描述:cache操作相关的代码。
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2011-10-21
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __CACHE_H__
#define __CACHE_H__

#ifdef __cplusplus
extern "C" {
#endif

enum _EN_CACHE_OPERATION_TYPE_
{
    EN_WRITE_TO_NO_INVALID_NO_LOCK,    //写通，不需要清除，不支持锁定
};
struct CacheTypeFlag
{
    u32 ints_cache_type:12;     //指令cache属性
    u32 data_cache_type:12;     //数据cache属性
    u32 uni_cache:1;            //1=指令和数据独立cache，0=统一cache
    u32 operation_type:4;       //cache操作类型。
};


#if defined( arm720t)
    #define CN_CACHE_SIZE       0x2000
    #define CN_CACHE_WAY        4
    #define CN_CACHE_LINE_SIZE  16
#elif defined( arm740t)     //740t cache size可变，在arch.h中定义cache尺寸
    #define CN_CACHE_WAY        4
    #define CN_CACHE_LINE_SIZE  16
#elif defined( arm920t)
    #define CN_CACHE_SIZE       0x4000
    #define CN_CACHE_WAY        64
    #define CN_CACHE_LINE_SIZE  32
    #define CN_C7_OFFSET_SET    5   //组索引在c7中的偏移位数
    #define CN_C7_OFFSET_WAY    26  //路索引在c7中的偏移位数
    #define CN_C9_OFFSET_WAY    26  //路索引在c9中的偏移位数
#elif defined( arm922t)
    #define CN_CACHE_SIZE       0x2000
    #define CN_CACHE_WAY        64
    #define CN_CACHE_LINE_SIZE  32
    #define CN_C7_OFFSET_SET    5   //组索引在c7中的偏移位数
    #define CN_C7_OFFSET_WAY    26  //路索引在c7中的偏移位数
    #define CN_C9_OFFSET_WAY    26  //路索引在c9中的偏移位数
#elif defined( arm940t)
    #define CN_CACHE_SIZE       0x1000
    #define CN_CACHE_WAY        64
    #define CN_CACHE_LINE_SIZE  16
    #define CN_C7_OFFSET_SET    4   //组索引在c7中的偏移位数
    #define CN_C7_OFFSET_WAY    26  //路索引在c7中的偏移位数
    #define CN_C9_OFFSET_WAY    0   //路索引在c9中的偏移位数
#elif defined( arm926ej_s)  //926ej_s cache size可变,在arch.h中定义cache尺寸
    #define CN_CACHE_WAY        4
    #define CN_CACHE_LINE_SIZE  32
    #define CN_C7_OFFSET_SET    4   //组索引在c7中的偏移位数
    #define CN_C7_OFFSET_WAY    30  //路索引在c7中的偏移位数
#elif defined( arm946e_s)    //946e_s cache size可变,在arch.h中定义cache尺寸
    #define CN_CACHE_WAY        4
    #define CN_CACHE_LINE_SIZE  16
    #define CN_C7_OFFSET_SET    4   //组索引在c7中的偏移位数
    #define CN_C7_OFFSET_WAY    30  //路索引在c7中的偏移位数
    #define CN_C9_OFFSET_WAY    0   //路索引在c9中的偏移位数
#elif defined( arm1022e)
    #define CN_CACHE_SIZE       0x4000
    #define CN_CACHE_WAY        64
    #define CN_CACHE_LINE_SIZE  32
    #define CN_C7_OFFSET_SET    5   //组索引在c7中的偏移位数
    #define CN_C7_OFFSET_WAY    26  //路索引在c7中的偏移位数
    #define CN_C9_OFFSET_WAY    26  //路索引在c9中的偏移位数
#elif defined( arm1026ej_s)//1026ej_s cache size可变,在arch.h中定义cache尺寸
    #define CN_CACHE_WAY        4
    #define CN_CACHE_LINE_SIZE  32
    #define CN_C7_OFFSET_SET    5   //组索引在c7中的偏移位数
    #define CN_C7_OFFSET_WAY    30  //路索引在c7中的偏移位数
#elif defined( strong_arm)
    #define CN_CACHE_SIZE       0x4000
    #define CN_CACHE_WAY        32
    #define CN_CACHE_LINE_SIZE  16
#elif defined( xscale)
    #define CN_CACHE_SIZE       0x8000
    #define CN_CACHE_WAY        32
    #define CN_CACHE_LINE_SIZE  32
#endif

void Cache_InvalidAll(void);
void Cache_InvalidInst(void);
void Cache_InvalidData(void);
void Cache_EnableAll(void);
void Cache_EnableInst(void);
void Cache_EnableData(void);
void Cache_DisableAll(void);
void Cache_DisableInst(void);
void Cache_DisableData(void);
void Cache_EnableWriteBuf(void);
void Cache_DisableWriteBuf(void);
void Cache_CleanData(void);
void Cache_config(void);


#else
//以下把cache操作函数全部置空。
#define Cache_InvalidAll()
#define Cache_InvalidInst()
#define Cache_InvalidData()
#define Cache_EnableAll()
#define Cache_EnableInst()
#define Cache_EnableData()
#define Cache_DisableAll()
#define Cache_DisableInst()
#define Cache_DisableData()
#define Cache_EnableWriteBuf()
#define Cache_DisableWriteBuf()
#define Cache_CleanData()

#endif

#ifdef __cplusplus
}

#endif /*__CACHE_H__*/


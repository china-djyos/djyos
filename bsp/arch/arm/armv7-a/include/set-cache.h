#ifndef __CACHE_H__
#define __CACHE_H__

#ifdef __cplusplus
extern "C" {
#endif

enum _EN_CACHE_OPERATION_TYPE_
{
    EN_WRITE_TO_NO_INVALID_NO_LOCK,    //дͨ������Ҫ�������֧������
};
struct CacheTypeFlag
{
    u32 ints_cache_type:12;     //ָ��cache����
    u32 data_cache_type:12;     //����cache����
    u32 uni_cache:1;            //1=ָ������ݶ���cache��0=ͳһcache
    u32 operation_type:4;       //cache�������͡�
};


#if defined( arm720t)
    #define CN_CACHE_SIZE       0x2000
    #define CN_CACHE_WAY        4
    #define CN_CACHE_LINE_SIZE  16
#elif defined( arm740t)     //740t cache size�ɱ䣬��arch.h�ж���cache�ߴ�
    #define CN_CACHE_WAY        4
    #define CN_CACHE_LINE_SIZE  16
#elif defined( arm920t)
    #define CN_CACHE_SIZE       0x4000
    #define CN_CACHE_WAY        64
    #define CN_CACHE_LINE_SIZE  32
    #define CN_C7_OFFSET_SET    5   //��������c7�е�ƫ��λ��
    #define CN_C7_OFFSET_WAY    26  //·������c7�е�ƫ��λ��
    #define CN_C9_OFFSET_WAY    26  //·������c9�е�ƫ��λ��
#elif defined( arm922t)
    #define CN_CACHE_SIZE       0x2000
    #define CN_CACHE_WAY        64
    #define CN_CACHE_LINE_SIZE  32
    #define CN_C7_OFFSET_SET    5   //��������c7�е�ƫ��λ��
    #define CN_C7_OFFSET_WAY    26  //·������c7�е�ƫ��λ��
    #define CN_C9_OFFSET_WAY    26  //·������c9�е�ƫ��λ��
#elif defined( arm940t)
    #define CN_CACHE_SIZE       0x1000
    #define CN_CACHE_WAY        64
    #define CN_CACHE_LINE_SIZE  16
    #define CN_C7_OFFSET_SET    4   //��������c7�е�ƫ��λ��
    #define CN_C7_OFFSET_WAY    26  //·������c7�е�ƫ��λ��
    #define CN_C9_OFFSET_WAY    0   //·������c9�е�ƫ��λ��
#elif defined( arm926ej_s)  //926ej_s cache size�ɱ�,��arch.h�ж���cache�ߴ�
    #define CN_CACHE_WAY        4
    #define CN_CACHE_LINE_SIZE  32
    #define CN_C7_OFFSET_SET    4   //��������c7�е�ƫ��λ��
    #define CN_C7_OFFSET_WAY    30  //·������c7�е�ƫ��λ��
#elif defined( arm946e_s)    //946e_s cache size�ɱ�,��arch.h�ж���cache�ߴ�
    #define CN_CACHE_WAY        4
    #define CN_CACHE_LINE_SIZE  16
    #define CN_C7_OFFSET_SET    4   //��������c7�е�ƫ��λ��
    #define CN_C7_OFFSET_WAY    30  //·������c7�е�ƫ��λ��
    #define CN_C9_OFFSET_WAY    0   //·������c9�е�ƫ��λ��
#elif defined( arm1022e)
    #define CN_CACHE_SIZE       0x4000
    #define CN_CACHE_WAY        64
    #define CN_CACHE_LINE_SIZE  32
    #define CN_C7_OFFSET_SET    5   //��������c7�е�ƫ��λ��
    #define CN_C7_OFFSET_WAY    26  //·������c7�е�ƫ��λ��
    #define CN_C9_OFFSET_WAY    26  //·������c9�е�ƫ��λ��
#elif defined( arm1026ej_s)//1026ej_s cache size�ɱ�,��arch.h�ж���cache�ߴ�
    #define CN_CACHE_WAY        4
    #define CN_CACHE_LINE_SIZE  32
    #define CN_C7_OFFSET_SET    5   //��������c7�е�ƫ��λ��
    #define CN_C7_OFFSET_WAY    30  //·������c7�е�ƫ��λ��
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
//���°�cache��������ȫ���ÿա�
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


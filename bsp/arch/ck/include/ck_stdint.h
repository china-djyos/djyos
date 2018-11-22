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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
#ifndef __CK_STDINT_H__
#define __CK_STDINT_H__

#ifdef __cplusplus
extern "C" {
#endif

//瀹氶暱绫诲瀷澹版槑
typedef unsigned  long long     uint64_t;
typedef signed    long long     sint64_t;
typedef unsigned  int           uint32_t;
typedef int                     sint32_t;
typedef unsigned  short         uint16_t;
typedef short int               sint16_t;
typedef unsigned  char          uint8_t;
typedef signed  char            sint8_t;

//浠ヤ笅鏄疌PU鍙互鐢ㄤ竴鏉℃寚浠ゅ鐞嗙殑鏁版嵁绫诲瀷锛屽鐢ㄤ簬闇�淇濇寔鏁版嵁瀹屾暣鎬х殑鎿嶄綔锛屼互鍙�
//闇�蹇�鐨勬搷浣溿�
//鏁版嵁瀹屾暣鎬у拰鍘熷瓙鎿嶄綔鐨勫紓鍚�
//鏁版嵁瀹屾暣鎬ф槸鎸囧湪浠讳綍鏃跺�閮借兘璇诲埌姝ｇ‘鍊肩殑鏁版嵁锛屾瘮濡備笅鍒楄鍙�
//  uint32_t    b32;
//  b32++;  //b32鐨勫師鍊兼槸0x0000ffff
//鍦�2浣峳isc鏈轰笂锛屾墽琛岃繃绋嬩负锛�
//鍙朾32鍦板潃鍒板瘎瀛樺櫒0-->鍙朾32鐨勬暟鍊煎埌瀵勫瓨鍣�-->瀵勫瓨鍣�鍔�-->鎶婂瘎瀛樺櫒1鍐欏叆b32.
//涓婅堪杩囩▼鏃犺鍦ㄥ摢閲岃涓柇鎴栬�琚珮浼樺厛绾х嚎绋嬫墦鏂紝鍦ㄤ腑鏂垨楂樹紭鍏堢骇绾跨▼涓b32锛�
//瑕佷箞寰楀埌0x0000ffff锛岃涔堝緱鍒�x00010000,杩欎袱閮藉彲浠ヨ涓烘槸姝ｇ‘鏁版嵁.
//鑰屽湪16浣嶆満涓婏紝鎵ц杩囩▼鏄�
//1銆佸彇0x0000鍒板瘎瀛樺櫒r0锛屽彇0xffff鍒板瘎瀛樺櫒r1
//2銆佹墽琛屽姞鎿嶄綔锛屼娇r0=0x0001,r1=0x0000
//3銆佹妸r0鍐欏叆b32鐨勯珮16浣嶃�
//4銆佹妸r1鍐欏叆b32鐨勪綆16浣嶃�
//濡傛灉鍦ㄧ3鍜岀4姝ラ涔嬮棿琚腑鏂墦鏂紝鍦ㄤ腑鏂噷璇籦32锛屽皢寰楀埌閿欒鐨�x00001ffff銆�
//閭ｄ箞鍘熷瓙鎿嶄綔鍛�灏辨槸鏁翠釜鎵ц杩囩▼涓嶈浠讳綍涓柇鎴栬�楂樹紭鍏堢骇绾跨▼鎵撴柇锛岃繕鐪嬩笂闈唬鐮侊紝
//濡傛灉b32琚畾涔変负鍘熷瓙鍙橀噺锛屽垯鏃犺鍦�6浣嶈繕鏄�2浣嶆満涓婏紝b32閮藉彲浠ュ緱鍒版纭殑鍊笺�鐩�
//鍓嶏紝djyos骞朵笉鏀寔鍘熷瓙鍙橀噺锛屽彧鑳戒娇鐢ㄥ師瀛愭搷浣滃疄鐜板師瀛愬彉閲忕殑鏁堟灉銆�
typedef uint32_t                ucpu_t;
typedef sint32_t                scpu_t;

//ptu32_t绫诲瀷涓巆pu鐨勫鍧�寖鍥存湁鍏筹紝涔熶笌CPU鐨勭粨鏋勬湁鍏筹紝濡傛灉璇ョ郴缁熶腑鎸囬拡闀垮害灏忎簬鎴�
//绛変簬32浣嶏紝鍒檖tu32_t琚畾涔変负32浣嶏紝鍚﹀垯涓庢寚閽堢瓑闀匡紝瀹冪殑鐢ㄩ�鏈変簩:
//1.鐢ㄤ簬鍐呭瓨鍒嗛厤鐨勬寚閽堥檺瀹�鎴栬�鐢ㄤ簬鎻忚堪鍐呭瓨闀垮害銆�
//2.鐢ㄤ簬鍙兘闇�杞崲鎴愭寚閽堜娇鐢ㄧ殑鏁村舰鏁版嵁锛屾瘮濡俿truct pan_device缁撴瀯涓殑
//  ptu32_t  private_tag
//涓�埇鏉ヨptu32_t涓巙cpu_t鐩稿悓,浣嗕篃鏈夋剰澶�灏ゅ叾鏄�浣嶆満鍜�6浣嶆満涓�渚嬪51鏈虹殑ucpu_t
//鏄�浣嶇殑,浣嗗鍧�寖鍥存槸16浣嶇殑,ptu32_t=16浣�80c296鍗曠墖鏈虹殑ucpu_t鏄�6浣嶇殑锛屽鍧�寖鍥�
//鏄�4浣嶇殑锛宲tu32_t=32浣嶃�
typedef uint32_t                ptu32_t;

//蹇�绫诲瀷锛屼竴鑸氨鏄疌PU瀛楅暱绛夐暱鐨勭被鍨嬶紝杩欑被鍙橀噺鍙互鐩存帴鍦ㄥ瘎瀛樺櫒涓搷浣�
typedef ucpu_t                  ufast_t;
typedef scpu_t                  sfast_t;

//甯冨皵绫诲瀷锛岄�鐢ㄧ浉搴擟PU涓渶蹇殑鏁版嵁绫诲瀷
//typedef ufast_t                 bool_t;

//涓嶤PU瀛楅暱鐩稿叧鐨勯厤缃�
#define CN_CPU_BITS             32  //澶勭悊鍣ㄥ瓧闀�
#define CN_PTR_BITS             32  //鎸囬拡瀛楅暱锛岀Щ妞嶆晱鎰�
#define CN_CPU_BITS_SUFFIX_ZERO 5
#define CN_BYTE_BITS            8   //瀛楄妭浣嶅

#define CN_LIMIT_PTU32          CN_LIMIT_UINT32
#define CN_MIN_PTU32            0
#define CN_LIMIT_ALIGN          CN_LIMIT_UINT64
#define CN_MIN_ALIGN            0

#define CN_LIMIT_UCPU           CN_LIMIT_UINT32
#define CN_MIN_UCPU             0
#define CN_LIMIT_SCPU           CN_LIMIT_SINT32
#define CN_MIN_SCPU             CN_MIN_SINT32
//浠ヤ笅鏄揩閫熸暟鎹被鍨嬶紝涓�埇鏉ヨ锛孋PU绫诲瀷鏄揩閫熸暟鎹被鍨嬶紝浣嗘湁浜涚郴缁熷瓨鍌ㄥ櫒瀹藉害灏忎簬
//CPU瀛楅暱锛屽鑷翠粠瀛樺偍鍣ㄨ涓�釜cpu瀛楅暱鐨勬暟闇�澶氫釜瀛樺偍鍣ㄥ懆鏈燂紝鍙﹀,16浣嶅拰32浣峜pu
//涓�瀛楄妭鎿嶄綔涔熷苟涓嶄竴瀹氭渶蹇�瀵逛簬鏈変簺瀵瑰瓧闀挎病鏈夎姹傜殑鏁版嵁绫诲瀷鏉ヨ锛屼娇鐢ㄤ笌瀛樺偍鍣�
//瀹藉害绛夐暱鐨勫彉閲忎篃璁告槸鏈�揩閫熺殑銆傞厤缃�6浣峉DRAM鐨�4b0x蹇�绫诲瀷鏄�6浣嶇殑.
#define CN_LIMIT_UFAST          CN_LIMIT_UINT32
#define CN_MIN_UFAST            (0)
#define CN_LIMIT_SFAST          CN_LIMIT_SINT32
#define CN_MIN_SFAST            CN_MIN_SINT32

//浠ヤ笅鏄痠soC 鎵�渶瑕佺殑
#undef  INT_MAX
#undef  UINT_MAX
#define INT_MAX                 0x7fffffff
#define UINT_MAX                0xffffffff

typedef uint64_t                align_type;     //arm涓紝u64鑳芥弧瓒虫墍鏈夊榻愯姹�

//typedef uint64_t                u64;
//typedef sint64_t                s64;
//typedef uint32_t                u32;
//typedef sint32_t                s32;
//typedef uint16_t                u16;
//typedef sint16_t                s16;
//typedef uint8_t                 u8;
//typedef sint8_t                 s8;
//typedef uint64_t const          uc64;
//typedef sint64_t const          sc64;
//typedef uint32_t const          uc32;
//typedef sint32_t const          sc32;
//typedef uint16_t const          uc16;
//typedef sint16_t const          sc16;
//typedef uint8_t  const          uc8;
//typedef sint8_t  const          sc8;
//typedef unsigned char           ucpu_char;
//typedef char                    scpu_char;
//
//typedef volatile ucpu_t         vucpu_t;
//typedef volatile scpu_t         vscpu_t;
//typedef volatile uint64_t       vu64;
//typedef volatile sint64_t       vs64;
//typedef volatile uint32_t       vu32;
//typedef volatile sint32_t       vs32;
//typedef volatile uint16_t       vu16;
//typedef volatile sint16_t       vs16;
//typedef volatile uint8_t        vu8;
//typedef volatile sint8_t        vs8;
//typedef volatile ucpu_t   const vuccpu_t;
//typedef volatile scpu_t   const vsccpu_t;
//typedef volatile uint64_t const vuc64;
//typedef volatile sint64_t const vsc64;
//typedef volatile uint32_t const vuc32;
//typedef volatile sint32_t const vsc32;
//typedef volatile uint16_t const vuc16;
//typedef volatile sint16_t const vsc16;
//typedef volatile uint8_t  const vuc8;
//typedef volatile sint8_t  const vsc8;
//
//#define CN_LIMIT_UINT64         0xffffffffffffffff
//#define CN_LIMIT_UINT32         0xffffffff
//#define CN_LIMIT_UINT24           0xffffff
//#define CN_LIMIT_UINT16         0xffff
//#define CN_LIMIT_UINT8          0xff
//#define CN_LIMIT_SINT64         0x7fffffffffffffff
//#define CN_LIMIT_SINT32         0x7fffffff
//#define CN_LIMIT_SINT16         0x7fff
//#define CN_LIMIT_SINT8          0x7f
//
//#define CN_MIN_UINT64           0
//#define CN_MIN_UINT32           0
//#define CN_MIN_UINT16           0
//#define CN_MIN_UINT8            0
//#define CN_MIN_SINT64           0x8000000000000000
//#define CN_MIN_SINT32           0x80000000
//#define CN_MIN_SINT16           0x8000
//#define CN_MIN_SINT8            0x80

#ifdef __cplusplus
}
#endif
#endif // __ARM_STDINT_H__




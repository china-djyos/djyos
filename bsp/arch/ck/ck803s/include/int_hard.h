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
// Copyright (c) 2014 钁椾綔鏉冪敱娣卞湷楣忕憺杞欢鏈夐檺鍏徃鎵�鏈夈�傝憲浣滄潈浜轰繚鐣欎竴鍒囨潈鍒┿��
//
// 杩欎唤鎺堟潈鏉℃锛屽湪浣跨敤鑰呯鍚堜笅鍒楁潯浠剁殑鎯呭舰涓嬶紝鎺堜簣浣跨敤鑰呬娇鐢ㄥ強鍐嶆暎鎾湰
// 杞欢鍖呰鍘熷鐮佸強浜岃繘浣嶅彲鎵ц褰㈠紡鐨勬潈鍒╋紝鏃犺姝ゅ寘瑁呮槸鍚︾粡鏀逛綔鐨嗙劧锛�
//
// 1. 瀵逛簬鏈蒋浠舵簮浠ｇ爜鐨勫啀鏁ｆ挱锛屽繀椤讳繚鐣欎笂杩扮殑鐗堟潈瀹ｅ憡銆佹湰鏉′欢鍒楄〃锛屼互
//    鍙婁笅杩扮殑鍏嶈矗澹版槑銆�
// 2. 瀵逛簬鏈浠朵簩杩涗綅鍙墽琛屽舰寮忕殑鍐嶆暎鎾紝蹇呴』杩炲甫浠ユ枃浠朵互鍙婏紡鎴栬�呭叾浠栭檮
//    浜庢暎鎾寘瑁呬腑鐨勫獟浠嬫柟寮忥紝閲嶅埗涓婅堪涔嬬増鏉冨鍛娿�佹湰鏉′欢鍒楄〃锛屼互鍙婁笅杩�
//    鐨勫厤璐ｅ０鏄庛��

// 鍏嶈矗澹版槑锛氭湰杞欢鏄湰杞欢鐗堟潈鎸佹湁浜轰互鍙婅础鐚�呬互鐜扮姸锛�"as is"锛夋彁渚涳紝
// 鏈蒋浠跺寘瑁呬笉璐熶换浣曟槑绀烘垨榛樼ず涔嬫媴淇濊矗浠伙紝鍖呮嫭浣嗕笉闄愪簬灏遍�傚敭鎬т互鍙婄壒瀹氱洰
// 鐨勭殑閫傜敤鎬т负榛樼ず鎬ф媴淇濄�傜増鏉冩寔鏈変汉鍙婃湰杞欢涔嬭础鐚�咃紝鏃犺浠讳綍鏉′欢銆�
// 鏃犺鎴愬洜鎴栦换浣曡矗浠讳富涔夈�佹棤璁烘璐ｄ换涓哄洜鍚堢害鍏崇郴銆佹棤杩囧け璐ｄ换涓讳箟鎴栧洜闈炶繚
// 绾︿箣渚垫潈锛堝寘鎷繃澶辨垨鍏朵粬鍘熷洜绛夛級鑰岃捣锛屽浜庝换浣曞洜浣跨敤鏈蒋浠跺寘瑁呮墍浜х敓鐨�
// 浠讳綍鐩存帴鎬с�侀棿鎺ユ�с�佸伓鍙戞�с�佺壒娈婃�с�佹儵缃氭�ф垨浠讳綍缁撴灉鐨勬崯瀹筹紙鍖呮嫭浣嗕笉闄�
// 浜庢浛浠ｅ晢鍝佹垨鍔冲姟涔嬭喘鐢ㄣ�佷娇鐢ㄦ崯澶便�佽祫鏂欐崯澶便�佸埄鐩婃崯澶便�佷笟鍔′腑鏂瓑绛夛級锛�
// 涓嶈礋浠讳綍璐ｄ换锛屽嵆鍦ㄨ绉嶄娇鐢ㄥ凡鑾蜂簨鍓嶅憡鐭ュ彲鑳戒細閫犳垚姝ょ被鎹熷鐨勬儏褰笅浜︾劧銆�
//-----------------------------------------------------------------------------
#ifndef __INT_HARD_H__
#define __INT_HARD_H__

#include "arch_feature.h"
#include <stddef.h>
//#include "ck_stdint.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef ucpu_t atom_high_t;
typedef ucpu_t atom_low_t;

#define cn_real_prio_default    0x80    //姒涙顓婚惃鍕杽閺冩湹鑵戦弬顓濈喘閸忓牏楠囬敍宀�鏁ら幋閿嬪Ω閺屾劒鑵戦弬顓☆啎
                                        //娑撳搫鐤勯弮鏈佃厬閺傤厽妞傞敍宀冪箹閺勵垰鐣犻惃鍕帛鐠併倓绱崗鍫㈤獓閿涘苯缍�
                                        //閻掕绱濋悽銊﹀煕閸欘垵鐨熼悽鈺t_set_prio閸戣姤鏆熸穱顔芥暭閵嗭拷

struct IntReg     //at 0xe000e100,
{
    vu32 setena[32];
    vu32 clrena[32];
    vu32 setpend[32];
    vu32 clrpend[32];
    vu32 active[32];    //CM3閻ㄥ嫮娈戞稉顓熸焽濞茶濮╅悩鑸碉拷鐎靛嫬鐡ㄩ崳顭掔礉閸︺劌顦甸悶鍡楁珤閹笛嗩攽娴滃棗鍙� ISR 閻ㄥ嫮顑囨稉锟�
                        //閺夆剝瀵氭禒銈呮倵閿涘瞼娴夋惔鏂剧秴鐞氼偆鐤� 1閿涘瞼娲块崚锟絀SR 鏉╂柨娲栭弮鑸靛绾兛娆㈠〒鍛存祩閵嗭拷
                        //閸忥拷40娴ｅ稄绱漵tm32閸欘亙濞囬悽銊ょ啊瀵拷顬婇惃锟�3娴ｏ拷
    u32 RESERVED5[32];
    vu8 pri[240];
};

extern struct IntReg volatile * const pg_int_reg;
extern u32 u32g_vect_table[];

void __Int_ClearAllLine(void);
void __Int_InitHard(void);
void __Int_EngineReal(ufast_t ufl_line);
void __Int_EngineAsynSignal(ufast_t ufl_line);
void __Int_EngineAll(ufast_t ufl_line);

typedef void (*hdl_t)(uint32_t );

#ifdef __cplusplus
}
#endif

#endif //__INT_HARD_H__

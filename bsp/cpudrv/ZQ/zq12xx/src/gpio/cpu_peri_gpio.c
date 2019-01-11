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
// Copyright (c) 2018锛岃憲浣滄潈鐢遍兘姹熷牥鎿嶄綔绯荤粺寮�婧愬紑鍙戝洟闃熸墍鏈夈�傝憲浣滄潈浜轰繚鐣欎竴鍒囨潈鍒┿��
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
// =============================================================================

// 鏂囦欢鍚�     锛歝pu_peri_gpio.h
// 妯″潡鎻忚堪: STM32F7xx鐨凣PIO椹卞姩
// 妯″潡鐗堟湰:
// 鍒涘缓浜哄憳:
// 鍒涘缓鏃堕棿: 10/28.2016
// =============================================================================
#ifdef CFG_CORTEX_M0
#include "stdint.h"
#include "silan_iomux.h"
#include "cpu_peri_isr.h"
#include "cpu_peri_gpio.h"
#include "cpu_peri.h"

#include "project_config.h"     //鏈枃浠剁敱IDE涓厤缃晫闈㈢敓鎴愶紝瀛樻斁鍦ˋPP鐨勫伐绋嬬洰褰曚腑銆�
                                //鍏佽鏄釜绌烘枃浠讹紝鎵�鏈夐厤缃皢鎸夐粯璁ゅ�奸厤缃��

//@#$%component configure   ****缁勪欢閰嶇疆寮�濮嬶紝鐢ㄤ簬 DIDE 涓浘褰㈠寲閰嶇疆鐣岄潰
//****閰嶇疆鍧楃殑璇硶鍜屼娇鐢ㄦ柟娉曪紝鍙傝婧愮爜鏍圭洰褰曚笅鐨勬枃浠讹細component_config_readme.txt****
//%$#@initcode      ****鍒濆鍖栦唬鐮佸紑濮嬶紝鐢� DIDE 鍒犻櫎鈥�//鈥濆悗copy鍒板垵濮嬪寲鏂囦欢涓�

//%$#@end initcode  ****鍒濆鍖栦唬鐮佺粨鏉�
//%$#@describe      ****缁勪欢鎻忚堪寮�濮�
//component name:"cpu_peri_gpio" //gpio鎿嶄綔鍑芥暟闆�
//parent:"none"                  //濉啓璇ョ粍浠剁殑鐖剁粍浠跺悕瀛楋紝none琛ㄧず娌℃湁鐖剁粍浠�
//attribute:bsp                  //閫夊～鈥渢hird銆乻ystem銆乥sp銆乽ser鈥濓紝鏈睘鎬х敤浜庡湪IDE涓垎缁�
//select:choosable               //閫夊～鈥渞equired銆乧hoosable銆乶one鈥濓紝鑻ュ～蹇呴�変笖闇�瑕侀厤缃弬鏁帮紝鍒橧DE瑁佸壀鐣岄潰涓粯璁ゅ嬀鍙栵紝
                                 //涓嶅彲鍙栨秷锛屽繀閫変笖涓嶉渶瑕侀厤缃弬鏁扮殑锛屾垨鏄笉鍙�夌殑锛孖DE瑁佸壀鐣岄潰涓笉鏄剧ず锛�
//init time:none                 //鍒濆鍖栨椂鏈猴紝鍙�夊�硷細early锛宮edium锛宭ater銆�
                                 //琛ㄧず鍒濆鍖栨椂闂达紝鍒嗗埆鏄棭鏈熴�佷腑鏈熴�佸悗鏈�
//dependence:none                //璇ョ粍浠剁殑渚濊禆缁勪欢鍚嶏紙鍙互鏄痭one锛岃〃绀烘棤渚濊禆缁勪欢锛夛紝
                                 //閫変腑璇ョ粍浠舵椂锛岃渚濊禆缁勪欢灏嗗己鍒堕�変腑锛�
                                 //濡傛灉渚濊禆澶氫釜缁勪欢锛屽垯渚濇鍒楀嚭
//weakdependence:"none"          //璇ョ粍浠剁殑寮变緷璧栫粍浠跺悕锛堝彲浠ユ槸none锛岃〃绀烘棤渚濊禆缁勪欢锛夛紝
                                 //閫変腑璇ョ粍浠舵椂锛岃渚濊禆缁勪欢涓嶄細琚己鍒堕�変腑锛�
                                 //濡傛灉渚濊禆澶氫釜缁勪欢锛屽垯渚濇鍒楀嚭锛岀敤鈥�,鈥濆垎闅�
//mutex:"none"                   //璇ョ粍浠剁殑渚濊禆缁勪欢鍚嶏紙鍙互鏄痭one锛岃〃绀烘棤渚濊禆缁勪欢锛夛紝
                                 //濡傛灉渚濊禆澶氫釜缁勪欢锛屽垯渚濇鍒楀嚭
//%$#@end describe  ****缁勪欢鎻忚堪缁撴潫

//%$#@configue      ****鍙傛暟閰嶇疆寮�濮�
//%$#@target = header           //header = 鐢熸垚澶存枃浠�,cmdline = 鍛戒护琛屽彉閲忥紝DJYOS鑷湁妯″潡绂佺敤
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***瀹氫箟鏃犲�肩殑瀹忥紝浠呯敤浜庣涓夋柟缁勪欢
//%$#@free,
//%$#@end configue  ****鍙傛暟閰嶇疆缁撴潫
//@#$%component end configure

/*Gpio Module*/

typedef struct
{
	volatile u32 GPIO_DATA;//0x000
}tagRegGpioData;


typedef struct 
{
   volatile u32 GPIO_DIR; //0x400
   volatile u32 GPIO_IS;  //0x404
   volatile u32 GPIO_IBE; //0x408
   volatile u32 GPIO_IEV; //0x40C
   volatile u32 GPIO_IE;  //0x410
   volatile u32 GPIO_RIS; //0x414
   volatile u32 GPIO_MIS; //0x418
   volatile u32 GPIO_IC;  //0x41C
   volatile u32 GPIOAFSEL;//0x420
   volatile u32 GPIOFILSEL;//0x424
   volatile u32 GPIODIV;  //0x428

}tagRegGpio;

#define  GPIO1_REG_BASE ((volatile tagRegGpio*)(0x41070000 + 0x400))
#define  GPIO1_REG_DATA ((volatile u32*)(0x41070000 + 0x00))

#define  GPIO2_REG_BASE (volatile tagRegGpio*)(0x41080000 + 0x400)
#define  GPIO2_REG_DATA ((volatile u32 *)(0x41080000 + 0x00))

typedef struct
{
   tagRegGpioData *pRegGpioData;//data Register
   tagRegGpio     *pRegGpioBase;
}tagRegBaseInfo;

#define CN_GPIO_GROUP_NUM   (2)

#define CN_GPIO1_MAX_NUM    22
#define CN_GPIO2_MAX_NUM    10

/*get gpio pin group location*/

#define GET_GPIO_PIN_GROUP(gpio)    (gpio / (CN_GPIO1_MAX_NUM+1))

/*get gpio pin register location*/

#define GET_GPIO_REG_OFFSET(gpio)    (gpio % (CN_GPIO1_MAX_NUM+1))

/*turn to gpio pin*/

#define IO_TO_GPIO_PIN(x)    ((x>=8) ? (x-8) : (34+x))


/*interrupt Control Register 6 and 7*/

#define REG_INT_CTRL_INT_LINE6 (0x42030000 + 0x618)
#define REG_INT_CTRL_INT_LINE7 (0x42030000 + 0x61C)

static volatile u32 *spg_RegIntEnable[2] = {
    (volatile u32*)(REG_INT_CTRL_INT_LINE6),
    (volatile u32*)(REG_INT_CTRL_INT_LINE7),
};


/*Gpio Module Register Info*/

static volatile tagRegBaseInfo s_RegGpioInfo[CN_GPIO_GROUP_NUM] =
{
    {   //Gpio Group 1
    	(volatile u32 *)GPIO1_REG_DATA,
    	(volatile tagRegGpio*)GPIO1_REG_BASE,
    },
       //Gpio Group 2
    {
    	(volatile u32 *)GPIO2_REG_DATA,
    	(volatile tagRegGpio*)GPIO2_REG_BASE,
    }
};



// =============================================================================
// Function Describe:
// Config io to input
// 
// 閰嶇疆瀵瑰簲 鐨処O涓鸿緭鍏ユā寮�
// =============================================================================

void Gpio_Input_Config(u8 io)
{
   u8 byPinRegLoc;
   u8 byPinGroupLoc;
   u8 byGpio;
   volatile tagRegGpio* GpioRegBase  = NULL;

   //turn to gpio
   byGpio = IO_TO_GPIO_PIN(io);

   //get Pin which Group 
   byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

   //get offence
   byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);
   
   GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;
   
   if(GpioRegBase != NULL)
   {
   	   GpioRegBase->GPIO_DIR &= ~(1 << byPinRegLoc);
   }

}


// =============================================================================
// Function Describe:
// Config io to output
// 
// 
// =============================================================================

void Gpio_Output_Config(u8 io)
{
   u8 byPinRegLoc;
   u8 byPinGroupLoc;
   u8 byGpio;
   volatile tagRegGpio* GpioRegBase  = NULL;

   //turn to gpio
   byGpio = IO_TO_GPIO_PIN(io);

   //get Pin which Group 
   byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

   //get offence
   byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);
   
   GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;
   
   if(GpioRegBase != NULL)
   {
   	   GpioRegBase->GPIO_DIR |= (1 << byPinRegLoc);
   }
}


// =============================================================================
// Function Describe:
// Config io input and output derection config
// arg0:io
// arg1:bDerection TRUE - output False :input
// =============================================================================

void Gpio_Derection_Config(u8 io,bool_t bDerection)
{
    if(bDerection)
    {
    	Gpio_Output_Config(io);
    }else
    {
    	Gpio_Input_Config(io);
    }
}


// =============================================================================
// Function Describe:
// Set io Level Hight
// arg0:io
// arg1:bDerection TRUE - output False :input
// =============================================================================

void Gpio_Set_Value_Hight(u8 io)
{
	u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpioData* GpioRegData  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group 
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);
   
    GpioRegData = (volatile tagRegGpioData*)s_RegGpioInfo[byPinGroupLoc].pRegGpioData;
   
    if(GpioRegData != NULL)
    {
   	    GpioRegData->GPIO_DATA |= (1 << byPinRegLoc);
    }
}


// =============================================================================
// Function Describe:
// Set io Level Low
// arg0:io
// arg1:bDerection TRUE - output False :input
// =============================================================================

void Gpio_Set_Value_Low(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpioData* GpioRegData  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group 
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);
   
    GpioRegData = (volatile tagRegGpioData*)s_RegGpioInfo[byPinGroupLoc].pRegGpioData;
   
    if(GpioRegData != NULL)
    {
        GpioRegData->GPIO_DATA &= ~(1 << byPinRegLoc);
    }
}


// =============================================================================
// Function Describe:
// Set io Level Low
// arg0:io
// arg1:bDerection TRUE - output False :input
// =============================================================================

u8 Gpio_Get_Level(u8 io)
{
    u8 byRet;
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpioData* GpioRegData  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group 
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);
   
    GpioRegData = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioData;

    if(GpioRegData != NULL)
    {
        if(GpioRegData->GPIO_DATA & (1 << byPinRegLoc))
           
           byRet = 1;
        else
           byRet = 0;

    }

    return byRet;
}

// =============================================================================
// Function Describe:
// Enable Int
// arg0:io
// arg1:
// =============================================================================

void Gpio_Enable_Int(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group 
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);
   
    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;
   
    if(GpioRegBase != NULL)
    {
        GpioRegBase->GPIO_IE |= (1 << byPinRegLoc);
        //int Ctrl Register
        *(spg_RegIntEnable[byPinRegLoc]) = 1 << byPinRegLoc;
    }

   
}


// =============================================================================
// Function Describe:
// Disable Int
// arg0:io
// arg1:
// =============================================================================

void Gpio_Disable_Int(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group 
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);
   
    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;
   
    if(GpioRegBase != NULL)
    {
        GpioRegBase->GPIO_IE &= ~(1 << byPinRegLoc);
        *(spg_RegIntEnable[byPinRegLoc]) &= ~(1 << byPinRegLoc);
    }
}



// =============================================================================
// Function Describe:
// Clear Irq
// arg0:io
// arg1:
// =============================================================================

void Gpio_Clear_Irq(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group 
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);
   
    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;
   
    if(GpioRegBase != NULL)
    {
        GpioRegBase->GPIO_IC |= (1 << byPinRegLoc);
    }
}


// =============================================================================
// Function Describe:
// 璁剧疆澶栭儴涓柇瑙﹀彂妯″紡
// io:闇�瑕侀厤缃殑io
// trugleMethod:    杈规部瑙﹀彂 杩樻槸 鐢靛钩瑙﹀彂
// edgeTrugleMethod:鍏蜂綋鏄珮銆佷綆鐢靛钩瑙﹀彂杩樻槸涓婂崌娌挎垨鑰呬笅闄嶆部瑙﹀彂
// =============================================================================

void Gpio_Set_Iqr_Mode(u8 io,u8 trugleMethod,u8 edgeTrugleMethod)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group 
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);
   
    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;
   
    switch(trugleMethod)
    {
        case EN_EDGE:   //杈规部瑙﹀彂
             GpioRegBase->GPIO_IS &= ~(1<<byPinRegLoc);
             if(edgeTrugleMethod == EN_BOTH_EDGE)           //鍙岃竟娌胯Е鍙�
             {
                GpioRegBase->GPIO_IBE |= (1<<byPinRegLoc);  //鍙岃竟娌胯Е鍙�
             }
             else if(edgeTrugleMethod == EN_RISING_EDGE)    //涓婂崌娌胯Е鍙�
             {
                 //閫夋嫨涓婂崌娌挎垨鑰呬笅闄嶆部瑙﹀彂
                 GpioRegBase->GPIO_IBE &= ~(1<<byPinRegLoc);
                 //閫夋嫨楂樼數骞虫垨鑰呬笂鍗囨部瑙﹀彂
                 GpioRegBase->GPIO_IEV |= (1<<byPinRegLoc);
             }else if(edgeTrugleMethod == EN_FALLING_EDGE)  //涓嬮檷娌胯Е鍙�
             {
                 //閫夋嫨涓婂崌娌挎垨鑰呬笅闄嶆部瑙﹀彂
                 GpioRegBase->GPIO_IBE &= ~(1<<byPinRegLoc);
                 //閫夋嫨浣庣數骞虫垨鑰呬笅闄嶆部瑙﹀彂
                 GpioRegBase->GPIO_IEV &= ~(1<<byPinRegLoc);
             }
             break;
        case EN_LEVEL:                                      //鐢靛钩瑙﹀彂
             GpioRegBase->GPIO_IS |= (1<<byPinRegLoc);

             if(edgeTrugleMethod == EN_HIGHT_LAVEL_EDGE)
             {
                GpioRegBase->GPIO_IEV |= (1<<byPinRegLoc); //楂樼數骞宠Е鍙�
             }else if(edgeTrugleMethod == EN_LOW_LEVEL_EDGE)
             {
                GpioRegBase->GPIO_IEV &= ~(1<<byPinRegLoc);//浣庣數骞宠Е鍙�
             }
             break;
    }
 
}


// =============================================================================
// Function Describe:
// 濠娿倖灏濋崚鍡涱暥闁瀚�
// 
// 
// =============================================================================

void Gpio_Io_Filter_Config_Div(u8 io,u8 div)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group 
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);
   
    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    GpioRegBase->GPIODIV |= ((div&0xf)<<1)|0x1;
  
}


// =============================================================================
// Function Describe:
// Gpio 鎵撳紑铏戞尝閰嶇疆
// 
// 
// =============================================================================


void Gpio_io_Filter_Open(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;
    u8 intLine;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group 
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);
   
    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    GpioRegBase->GPIOFILSEL |= 1<<byPinRegLoc ;

}



// =============================================================================
// Function Describe:
// 鍏抽棴 Gpio 婊ゆ尝閰嶇疆
// 
// 
// =============================================================================

void Gpio_io_Filter_Close(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group 
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);
   
    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    GpioRegBase->GPIOFILSEL &= ~(1<<byPinRegLoc) ;

}


// =============================================================================
// Function Describe:
// Gpio 澶栭儴涓柇璇锋眰顕�
// 
// 
// =============================================================================

enum EN_GPIO_INTLINE{
    EN_GPIO1_INT_LINE = 0x00,
    EN_GPIO2_INT_LINE,
};

void Register_Gpio_Irq(u8 io,fnGpioHandle_t IsrHandle)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;
    u8 intLine;      //涓柇绾�
    u8 subIntLine;   //瀛愪腑鏂嚎

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group 
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);
   
    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    //Gpio1 涓�6鍙蜂腑鏂嚎 Gpio2 涓�7鍙� 涓柇绾�
    switch(byPinGroupLoc)
    {
        case EN_GPIO1_INT_LINE:
             intLine    = 6;
             subIntLine = byPinRegLoc;
             break;
        case EN_GPIO2_INT_LINE:
             intLine = 7;
             subIntLine = byPinRegLoc;
             break;
        default:;break;
    }

    if(IsrHandle != NULL)
    {
        djybsp_isr_hdl_register(intLine, subIntLine,IsrHandle,0);
    }

}

#endif





























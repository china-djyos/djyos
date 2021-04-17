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
#include "cpu_peri.h"
#include "djyos.h"
#include "stdlib.h"
#include "stm32h7xx.h"
#include "stm32h7xx_hal_dma2d.h"
#include "stm32h7xx_hal_ltdc.h"
#include "stm32h7xx_hal_rcc_ex.h"
#include "cpu_peri_lcd.h"
#include "string.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern struct DisplayObj* ModuleInstall_LCD(const char *DisplayName,const char* HeapName);
//  ModuleInstall_LCD(CFG_DISPLAY_NAME,CFG_LCD_HEAP_NAME);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip lcd"//CPU����LCD����������
//parent:"graphical kernel"   //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"heap","graphical kernel"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:none           //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_LCD == false )
//#warning  " cpu_onchip_lcd  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_LCD    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,1,2048,
#define CFG_LCD_XSIZE                   600 //"LCDˮƽ���ؿ��",
#define CFG_LCD_YSIZE                   480 //"LCD��ֱ���ؿ��",
//%$#@enum,true,false,
//%$#@string,1,32,
#define CFG_DISPLAY_NAME        "LCD_F7"    //"��ʾ��LCD����",
#define CFG_LCD_HEAP_NAME       "extram"    //"LCDʹ�ö�����",
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������
//@#$%component end configure


#define lcd_swap(a, b){a^=b; b^=a; a^=b;}

static struct LCD_ConFig lcd;                      //����LCD LTDC����Ҫ����
static LTDC_HandleTypeDef  LTDC_Handler;       //LTDC���
static u8 *pLTDCBufferFG1 =NULL;//��������ʼλ��
//static u8 *pLTDCBufferFG2 =NULL;//��������ʼλ��
static struct DisplayObj lcd_display;
static struct MutexLCB Dma2dMutex;//Dma2D������ʡ�

//��ͬ������Щ�����ǲ�ͬ�ģ���ͬ�Ǹ�������Ҫʵ�ָú�������ɸú�������������
extern bool_t Ltdc_Lcd_Config(struct LCD_ConFig *lcd);
//----------------------------------------------------------------
//����: ���ر���
//����:  0�ر��� ���㿪����
//----------------------------------------------------------------
__attribute__((weak))   void Lcd_BackLight_OnOff(u8 onoff)
{
//  if(onoff)
//      GPIO_SettoLow(GPIO_A,PIN8);
//  else
//      GPIO_SettoHigh(GPIO_A,PIN8);
}

//----------------------------------------------------------------
//����: ����LTDC
//����:0/1
//----------------------------------------------------------------------------
void LTDC_Switch(u8 sw)
{
    if(sw==1) __HAL_LTDC_ENABLE(&LTDC_Handler);
    else if(sw==0)__HAL_LTDC_DISABLE(&LTDC_Handler);
}

//----------------------------------------------------------------
//����: ����ָ����Layer
//����:  ��layer 0,��һ��; 1,�ڶ���
//      ����sw 1/0
//����ֵ:ture/false
//----------------------------------------------------------------------------
bool_t LTDC_Layer_Switch(u8 layerx,u8 sw)
{
    if(sw==1) __HAL_LTDC_LAYER_ENABLE(&LTDC_Handler,layerx);
    else if(sw==0) __HAL_LTDC_LAYER_DISABLE(&LTDC_Handler,layerx);
    __HAL_LTDC_RELOAD_CONFIG(&LTDC_Handler);
    return true;
}

//----------------------------------------------------------------
//����: LTDCʱ�����á�
//����: pllsaidivr:LCDʱ�ӷ�Ƶϵ��,ȡֵ��Χ:0~3,��Ӧ��Ƶ2^(pllsaidivr+1)
//����ֵ:0,�ɹ�;1,ʧ�ܡ�
//��ע:CLK=(pllsain/pllsair)/(pllsaidivr)
//----------------------------------------------------------------------------
static bool_t LTDC_Clk_Set(u32 pllsain,u32 pllsair,u32 pllsaidivr)
{
    RCC_PeriphCLKInitTypeDef PeriphClkIniture;
    PeriphClkIniture.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkIniture.PLL3.PLL3M = pllsain;
    PeriphClkIniture.PLL3.PLL3N = pllsair;
    PeriphClkIniture.PLL3.PLL3P = 2;
    PeriphClkIniture.PLL3.PLL3Q = 2;
    PeriphClkIniture.PLL3.PLL3R = pllsaidivr;

    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkIniture)==HAL_OK)
    {
        return 0;   //�ɹ�
    }
    else return 1;  //ʧ��
}

//----��ʼ��DMA2DӲ��------------------------------------------------------------
//����: ��ʼ��DMA2DӲ��
//����: ��
//����: ��
//��ע:
//----------------------------------------------------------------------------
static bool_t Dma2d_Init(void)
{
    __HAL_RCC_DMA2D_CLK_ENABLE();               //ʹ��DMA2Dʱ��

    return true;
}
//----��ʼ��LTDC�豸------------------------------------------------------------
//LTDC,������������.
//ע��:�˺���,������LTDC_Layer_Window_Config֮ǰ����.
//layerx:��ֵ,0/1.
//bufaddr:����ɫ֡������ʼ��ַ
//pixformat:��ɫ��ʽ.0,ARGB8888;1,RGB888;2,RGB565;3,ARGB1555;4,ARGB4444;5,L8;6;AL44;7;AL88
//alpha:����ɫAlphaֵ,0,ȫ͸��;255,��͸��
//alpha0:Ĭ����ɫAlphaֵ,0,ȫ͸��;255,��͸��
//bfac1:���ϵ��1,4(100),�㶨��Alpha;6(101),����Alpha*�㶨Alpha
//bfac2:���ϵ��2,5(101),�㶨��Alpha;7(111),����Alpha*�㶨Alpha
//bkcolor:��Ĭ����ɫ,32λ,��24λ��Ч,RGB888��ʽ
//����ֵ:��
//-----------------------------------------------------------------------
static void LTDC_Layer_Parameter_Config(u8 layerx,u32 bufaddr,u8 pixformat,u8 alpha,u8 alpha0,u8 bfac1,u8 bfac2,u32 bkcolor)
{
    LTDC_LayerCfgTypeDef pLayerCfg;

    pLayerCfg.WindowX0=0;                       //������ʼX����
    pLayerCfg.WindowY0=0;                       //������ʼY����
    pLayerCfg.WindowX1=CN_LCD_XSIZE;          //������ֹX����
    pLayerCfg.WindowY1=CN_LCD_YSIZE;         //������ֹY����
    pLayerCfg.PixelFormat=pixformat;            //���ظ�ʽ
    pLayerCfg.Alpha=alpha;                      //Alphaֵ���ã�0~255,255Ϊ��ȫ��͸��
    pLayerCfg.Alpha0=alpha0;                    //Ĭ��Alphaֵ
    pLayerCfg.BlendingFactor1=(u32)bfac1<<8;    //���ò���ϵ��
    pLayerCfg.BlendingFactor2=(u32)bfac2<<8;    //���ò���ϵ��
    pLayerCfg.FBStartAdress=bufaddr;            //���ò���ɫ֡������ʼ��ַ
    pLayerCfg.ImageWidth=CN_LCD_XSIZE;        //������ɫ֡�������Ŀ��
    pLayerCfg.ImageHeight=CN_LCD_YSIZE;      //������ɫ֡�������ĸ߶�
    pLayerCfg.Backcolor.Red=(u8)(bkcolor&0X00FF0000)>>16;   //������ɫ��ɫ����
    pLayerCfg.Backcolor.Green=(u8)(bkcolor&0X0000FF00)>>8;  //������ɫ��ɫ����
    pLayerCfg.Backcolor.Blue=(u8)bkcolor&0X000000FF;        //������ɫ��ɫ����
    HAL_LTDC_ConfigLayer(&LTDC_Handler,&pLayerCfg,layerx);   //������ѡ�еĲ�
}

//-----------------------------------------------------------------------
//LTDC,���մ�������,������LCD�������ϵΪ��׼
//ע��:�˺���������LTDC_Layer_Parameter_Config֮��������.
//layerx:��ֵ,0/1.
//sx,sy:��ʼ����
//width,height:��Ⱥ͸߶�
//----��ʼ��LTDC�豸------------------------------------------------------------
static void LTDC_Layer_Window_Config(u8 layerx,u16 sx,u16 sy,u16 width,u16 height)
{
    HAL_LTDC_SetWindowPosition(&LTDC_Handler,sx,sy,layerx);  //���ô��ڵ�λ��
    HAL_LTDC_SetWindowSize(&LTDC_Handler,width,height,layerx);//���ô��ڴ�С
}

//-----------------------------------------------------------------------
//ѡ���
//layerx:���;0,��һ��;1,�ڶ���;
//Ŀǰֻ��һ��
//-----------------------------------------------------------------------
static bool_t LTDC_Select_Layer(u8 layerx)
{
    switch (layerx)
    {
        case 0:
            lcd.pFrameBuffe=pLTDCBufferFG1;
            return true;
        case 1:
//            lcd.pFrameBuffe=pLTDCBufferFG2;
            return true;
        default:  return false;
    }
}

//-----------------------------------------------------------------------
//LTDC������,DMA2D���
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)
//ע��:sx,ex,���ܴ���lcddev.width-1;sy,ey,���ܴ���lcddev.height-1!!!
//color:Ҫ������ɫ
//-----------------------------------------------------------------------
static bool_t LTDC_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
{
    u32 timeout=0;
    u32 addr;
    bool_t flag=true;
    u32 height,width;


    width=ex-sx+1;
    height=ey-sy+1;
    addr=((u32)lcd.pFrameBuffe+lcd.pixsize*(CN_LCD_XSIZE*sy+sx));
    if(false==Lock_MutexPend(&Dma2dMutex,lcd.Dma2dTimeOut)
            ||(ex+1>CN_LCD_XSIZE)
            ||(ey+1>CN_LCD_YSIZE))
        return false;

    DMA2D->CR&=~(1<<0);
    DMA2D->CR=3<<16;
    DMA2D->OPFCCR=lcd.Dma2dPixelFormat;
    DMA2D->OOR  = lcd_display.width-width;
    DMA2D->OMAR =  addr;
    DMA2D->NLR  = height|(width<<16);
    DMA2D->OCOLR=color;
    DMA2D->CR|=1<<0;
    while((DMA2D->ISR&(1<<1))==0)   //�ȴ��������
    {
        timeout++;
        if(timeout>0X1FFFFF)
            {
                break;  //��ʱ�˳�
                flag=false;
            }
    }
    DMA2D->IFCR|=1<<1;              //���������ɱ�־
    Lock_MutexPost(&Dma2dMutex);
    return flag;
}

//------------------------------------------------------------------
//����: LTDC ��ʼ��
//����: true/false
//����:
//-----------------------------------------------------------------------------
static void LTDC_Init(u8 *pFrameBufferFG)
{
    LTDC_Clk_Set(5,160,18);//��������ʱ��  50Mhz
    LTDC_Handler.Instance=LTDC;
    LTDC_Handler.Init.HSPolarity=lcd.LtdcHSPolarity;     //ˮƽͬ������
    LTDC_Handler.Init.VSPolarity=lcd.LtdcVSPolarity;     //��ֱͬ������
    LTDC_Handler.Init.DEPolarity= lcd.LtdcDEPolarity;    //����ʹ�ܼ���
    LTDC_Handler.Init.PCPolarity=lcd.LtdcPCPolarity;     //����ʱ�Ӽ���
    LTDC_Handler.Init.HorizontalSync=lcd.hsw-1;          //ˮƽͬ�����
    LTDC_Handler.Init.VerticalSync=lcd.vsw-1;            //��ֱͬ�����
    LTDC_Handler.Init.AccumulatedHBP=lcd.hsw+lcd.hbp-1;  //ˮƽͬ�����ؿ��
    LTDC_Handler.Init.AccumulatedVBP=lcd.vsw+lcd.vbp-1;  //��ֱͬ�����ظ߶�
    LTDC_Handler.Init.AccumulatedActiveW=lcd.hsw+lcd.hbp+lcd_display.width-1;//��Ч���
    LTDC_Handler.Init.AccumulatedActiveH=lcd.vsw+lcd.vbp+lcd_display.height-1;//��Ч�߶�
    LTDC_Handler.Init.TotalWidth=lcd.hsw+lcd.hbp+lcd_display.width+lcd.hfp-1;//�ܿ��
    LTDC_Handler.Init.TotalHeigh=lcd.vsw+lcd.vbp+lcd_display.height+lcd.vfp-1;//�ܸ߶�
    LTDC_Handler.Init.Backcolor.Red  =0;           //��Ļ�������ɫ����
    LTDC_Handler.Init.Backcolor.Green=0;         //��Ļ��������ɫ����
    LTDC_Handler.Init.Backcolor.Blue =0;          //��Ļ����ɫ��ɫ����
    LTDC_Handler.State = HAL_LTDC_STATE_RESET;
    __HAL_RCC_LTDC_CLK_ENABLE();     //ʹ��LTDCʱ��
    HAL_LTDC_Init(&LTDC_Handler);

    //������
    LTDC_Layer_Parameter_Config(0,(u32)pFrameBufferFG,lcd.LtdcPixelFormat,255,0,6,7,0X000000);//���������
    LTDC_Layer_Window_Config(0,0,0,lcd_display.width,lcd_display.height);
    LTDC_Select_Layer(0);           //ѡ���1��
    Lcd_BackLight_OnOff(1);

}
//------------------------------------------------------------------
//����: ���⿪��
//����: true/false
//����:
//-----------------------------------------------------------------------------


//----������ʾ��---------------------------------------------------------------
//����: ������driver�ṩ�̱�д�ģ�����������ʾ�����ֲ����ĺ���������Ӧ�ó����
//      ���룬�������豸���̶��塣
//����: disp����ʾ��ָ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
static bool_t __lcd_disp_ctrl(struct DisplayObj *disp)
{
    return true;
}


//----λͼ�л�����-------------------------------------------------------------
//����: ��λͼ�л�һ�����أ�ֻ����bitmap�����ظ�ʽΪcn_custom_pfʱ������Ҫ���ơ�
//      ����Կ�������֧���Զ����ʽ��������ֱ�ӷ��ء�
//����: bitmap��Ŀ��λͼ
//      limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ���
//      x��y������
//      color����ͼ�õ���ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      r2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
static bool_t __lcd_set_pixel_bm(struct RectBitmap *bitmap,
                     s32 x,s32 y,u32 color,u32 r2_code)
{
    return false;
}
//----λͼ�л���---------------------------------------------------------------
//����: ��λͼ�л�һ��ֱ�ߣ�������(x2,y2)�㣬ֻ������limit�޶��������ڵĲ��֡�
//����: bitmap��Ŀ��λͼ
//      limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ���
//      x1��y1��x2��y2������յ�����
//      color����ͼ�õ���ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=ʧ�ܣ���Ӳ�����ٻ�֧�ְ�r2_code����
//-----------------------------------------------------------------------------
static bool_t __lcd_line_bm(struct RectBitmap *bitmap,struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//----λͼ�л���(���˵�)-------------------------------------------------------
//����: ��λͼ�л�һ��ֱ�ߣ�ֻ������limit�޶��������ڵĲ��֡�
//����: bitmap��Ŀ��λͼ
//      limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ���
//      x1��y1��x2��y2������յ�����
//      color����ͼ�õ���ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=ʧ�ܣ���Ӳ�����ٻ�֧�ְ�r2_code����
//-----------------------------------------------------------------------------
static bool_t __lcd_line_bm_ie(struct RectBitmap *bitmap,struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//----λͼ��������----------------------------------------------------------------
//Bitmap�о������,֧�ֽ���ɫ
//ʲô�����Ӧ��ʵ�ֱ������Ĺ���,�μ����ṹǰ���ע��
//������ʾ������ʵ�ֱ�����,ֱ�ӷ���false����
//����:
// DstBitmap: ���Ƶ�Ŀ��λͼ
// Target: Ŀ���������,����ɫ���ʱ������������������ɫ�������յ�
// Focus: ʵ���������,gkernelȷ������Target�����ڲ�
// Color0,Color1: Target���ϽǺ����½���ɫֵ,������ǽ������,Color1��������,
//              ���ظ�ʽ��CN_SYS_PF_ERGB8888
//Mode: ���ģʽ,CN_FILLRECT_MODE_N�峣��
//����: true=�ɹ����ƣ�false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_fill_rect_bm(struct RectBitmap *dst_bitmap,
                          struct Rectangle *Target,
                          struct Rectangle *Focus,
                          u32 Color0,u32 Color1,u32 Mode)
{
    u16 pixel;
    u16 *dst;
    s32 height,width;
    u32 timeout;
    bool_t flag;

    if((Mode != CN_FILLRECT_MODE_N)  \
        ||(dst_bitmap->PixelFormat != lcd.LcdPixelFormat))
      return false;

    pixel = GK_ConvertRGB24ToPF(lcd.LcdPixelFormat,Color0);

    dst = (u16*)((u32)dst_bitmap->bm_bits+Focus->left*lcd.pixsize+\
                     Focus->top * dst_bitmap->linebytes);

    width=Focus->right - Focus->left;
    height=Focus->bottom - Focus->top;
    if(false==Lock_MutexPend(&Dma2dMutex,lcd.Dma2dTimeOut))
      return false;
    DMA2D->CR&=~(1<<0);             //��ֹͣDMA2D
    DMA2D->CR=3<<16;                //�Ĵ������洢��ģʽ
    DMA2D->OPFCCR=lcd.Dma2dPixelFormat;  //���������ɫ��ʽ
    DMA2D->OOR  = dst_bitmap->width-width;  //������ƫ��
    DMA2D->OMAR=(u32)dst;               //����洢����ַ
    DMA2D->NLR  = height|(width<<16);   //�趨�����Ĵ���
    DMA2D->OCOLR=pixel;                //�趨�����ɫ�Ĵ���
    DMA2D->CR|=1<<0;                //����DMA2D
    timeout =0;
    while((DMA2D->ISR&(1<<1))==0)   //�ȴ��������
    {
      timeout+=10;
      DJY_DelayUs(10);
      if(timeout>lcd.Dma2dTimeOut)
      {
          flag=false;
           break;  //��ʱ�˳�
      }
    }
    DMA2D->IFCR|=1<<1;              //���������ɱ�־
    Lock_MutexPost(&Dma2dMutex);
    return flag;
}
//--------------bitmap��λ�鴫��-----------------------------------------------
//����������λͼ��λ�鴫��,������γߴ粻���,��Ҫʵ�������ѹ��
//ʲô�����Ӧ��ʵ�ֱ������Ĺ���,�μ����ṹǰ���ע��
//������ʾ������ʵ�ֱ�����,ֱ�ӷ���false����
//����:
// dst_bitmap: Ŀ��λͼ
// DstRect: Ŀ�����
// src_bitmap: Դλͼ
// SrcRect: Դ����
// RopCode: ��ʾЧ��,�μ� gkernel.h�ļ��� CN_ROP_ALPHA_SRC_MSK �峣����˵��
// HyalineColor: ͸��ɫ,�������RopCode���趨�� CN_ROP_KEYCOLOR_EN,��
//                  ������ָ��͸��ɫ,CN_SYS_PF_ERGB8888��ʽ
//����: true=�ɹ�,false=ʧ��
//-----------------------------------------------------------------------------

bool_t __lcd_blt_bm_to_bm(struct RectBitmap *dst_bitmap,
                          struct Rectangle *DstRect,
                          struct RectBitmap *src_bitmap,
                          struct Rectangle *SrcRect,
                          struct RopGroup RopCode,u32 transparentcolor)
{

    u16 *src_offset,*dst_offset;    //Դλͼ���󻺳������ܲ�����!!!
    u32 width,height;
    u32 timeout=0;
    bool_t flag=true;
    struct RopGroup Rop = { 0, 0, 0, CN_R2_COPYPEN, 0, 0, 0  };

    width = SrcRect->right-SrcRect->left;
    height= SrcRect->bottom-SrcRect->top;

    if((dst_bitmap->PixelFormat != lcd.LcdPixelFormat) \
             ||(src_bitmap->PixelFormat != lcd.LcdPixelFormat)\
             ||((DstRect->bottom-DstRect->top) != height)\
             ||((DstRect->right-DstRect->left) != width)\
             ||(memcmp(&RopCode, &Rop ,sizeof(struct RopGroup))!=0)
             ||((ptu32_t)src_bitmap->bm_bits & 1))
        return false;

    src_offset = (u16*)((ptu32_t)(src_bitmap->bm_bits+\
              SrcRect->top * src_bitmap->linebytes)+\
              SrcRect->left*lcd.pixsize);

    dst_offset = (u16*)((ptu32_t)(dst_bitmap->bm_bits+\
             DstRect->top * dst_bitmap->linebytes)+\
             DstRect->left*lcd.pixsize);

    if(false==Lock_MutexPend(&Dma2dMutex,lcd.Dma2dTimeOut))
        return false;
    DMA2D->CR=0<<16;                //�洢�����洢��ģʽ
    DMA2D->FGPFCCR=lcd.Dma2dPixelFormat;   //������ɫ��ʽ
    DMA2D->FGOR=(u32)(src_bitmap->width - width); //ǰ������ƫ��Ϊ0
    DMA2D->OOR= (u32)(dst_bitmap->width - width); //������ƫ��
    DMA2D->CR&=~(1<<0);             //��ֹͣDMA2D
    DMA2D->FGMAR=(u32)src_offset;        //Դ��ַ
    DMA2D->OMAR=(u32)dst_offset;        //����洢����ַ
    DMA2D->NLR=(height)|((width)<<16);   //�趨�����Ĵ���
    DMA2D->CR|=1<<0;                //����DMA2D
    while((DMA2D->ISR&(1<<1))==0)   //�ȴ��������
    {
        timeout++;
        if(timeout>0X1FFFFF)
        {
            flag=false;
            break;  //��ʱ�˳�
        }
    }
    DMA2D->IFCR|=1<<1;              //���������ɱ�־
    Lock_MutexPost(&Dma2dMutex);
    return flag;
}

//----screen�л�����-----------------------------------------------------------
//����: ��screen�л�һ������.
//����: x��y����������
//      color����ɫ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=û�л��ƣ����������
//-----------------------------------------------------------------------------
bool_t __lcd_set_pixel_screen(s32 x,s32 y,u32 color,u32 rop2_code)
{

    u32 dest;
    u32 byteoffset;
    if(((x+1)>CN_LCD_XSIZE)||((y+1)>CN_LCD_YSIZE))
        return false;
    byteoffset = (y*CN_LCD_XSIZE + x)*lcd.pixsize;
    color = GK_ConvertRGB24ToPF(lcd.LcdPixelFormat,color);
    dest = (u32)lcd.pFrameBuffe[byteoffset];
    dest = GK_BlendRop2(dest,color,rop2_code);

    switch (lcd.pixsize)
    {
    case 2:
        *(u16 *)(&lcd.pFrameBuffe[byteoffset]) = (u16)color;
        break;
    case 3:
        lcd.pFrameBuffe[byteoffset] = (u8)color>>16;
        lcd.pFrameBuffe[byteoffset+1] = (u8)color>>8;
        lcd.pFrameBuffe[byteoffset+2] = (u8)color;
        break;
    default:            break;
    }
    return true;

}

//��screen�л�һ�����أ���frame buffer������£�������ʾgui����������������
//�������direct_screen==true������ܵ��ñ������������Ƿ���frame buffer��
//driver�������ṩ���ұ���ʵ�ֱ�����
//������ʾ������ʵ�ֱ������Ĺ���,����ֱ�ӷ���false
//����:
// PixelGroup: ������������ָ��.
// color: ��ɫֵ,CN_SYS_PF_ERGB8888��ʽ
// n: ����������
// r2_code: ��ʾЧ��,���gkernel.h�е�CN_R2_BLACK�峣������
bool_t __lcd_set_pixel_group_screen(struct PointCdn *PixelGroup,u32 color,u32 n, u32 r2_code)
{
    u32 dest,i;
    u32 offset;
    color = GK_ConvertRGB24ToPF(lcd.LcdPixelFormat,color);

    for(i=0;i<n;i++)
    {
        offset = PixelGroup[i].y*CN_LCD_XSIZE + PixelGroup[i].x;
        dest = *(u16 *)&(lcd.pFrameBuffe[offset]);
        dest = GK_BlendRop2(dest,color,r2_code);

        *(u16 *)&(lcd.pFrameBuffe[offset]) = dest;
    }


    return true;
}

//----screen�л���---------------------------------------------------------------
//����: ��screen�л�һ������ֱ�ߣ������˵㣬ֻ������limit�޶��������ڵĲ��֡���
//      ��֧�ֻ��ߵ�Ӳ�����٣�ֱ�ӷ���false��
//����: limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ���
//      x1��y1��x2��y2������յ�����
//      color����ͼ�õ���ɫ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=ʧ�ܣ���Ӳ�����ٻ�֧�ְ�r2_code����
//-----------------------------------------------------------------------------
bool_t __lcd_line_screen(struct Rectangle *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    if(y1 == y2)    //��ˮƽ��
    {
        if(x1 > x2)
            lcd_swap(x1,x2);
        if((y1 < limit->top) || (y1 >= limit->bottom)
            ||(x2 <= limit->left) || (x1 >= limit->right))
            return false;         //����ֱ����limit֮��
        if(x1 < limit->left)
            x1 = limit->left;
        if(x2 > limit->right)
            x2 = limit->right;
        color = GK_ConvertRGB24ToPF(lcd.LcdPixelFormat,color);
        return LTDC_Color_Fill( x1,y1,x2-1,y2+1,color);

    }else if(x1 == x2)   //�洹ֱ��
    {
        if(y1 > y2)
            lcd_swap(y1,y2);
        if((x1 < limit->left) || (x1 >= limit->right)
            ||(y2 <= limit->top) || (y1 >= limit->bottom))
            return false;         //����ֱ����limit֮��
        if(y1 < limit->top)
            y1 = limit->top;
        if(y2 > limit->bottom)
            y2 = limit->bottom;
        color = GK_ConvertRGB24ToPF(lcd.LcdPixelFormat,color);
        return LTDC_Color_Fill( x1,y1,x2+1,y2-1,color);
    }
    return false;
}
//----screen�л���(���˵�)------------------------------------------------------
//����: ��screen�л�һ��ֱ�ߣ�ֻ������limit�޶��������ڵĲ��֡�
//����: limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ���
//      x1��y1��x2��y2������յ�����
//      color����ͼ�õ���ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=ʧ�ܣ���Ӳ�����ٻ�֧�ְ�r2_code����
//-----------------------------------------------------------------------------
static bool_t __lcd_line_screen_ie(struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{

    return false;
}
//----screen��������-----------------------------------------------------------
//����: ��screen�еľ�����color��ɫ��䣬color�ĸ�ʽ��cn_sys_pf_e8r8g8b8��
//����: dst_rect�������ľ���
//      color�������ɫ
//����: true=�ɹ����ƣ�false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_fill_rect_screen(struct Rectangle *Target,
                              struct Rectangle *Focus,
                              u32 Color0,u32 Color1,u32 Mode)
{
    u32 pen;
    bool_t flag;
    if(Mode != CN_FILLRECT_MODE_N)
        return false;
    pen = GK_ConvertRGB24ToPF(lcd.LcdPixelFormat,Color0);
    flag=LTDC_Color_Fill(Focus->left,Focus->top,Focus->right-1,Focus->bottom-1,pen);
    return flag;
}

//----bitmap��screenλ�鴫��---------------------------------------------------
//����: ��һ��λͼ�е�һ�����������͵���һ������
//      Դ��Ŀ��λͼ������ͬ��
//����: dst_rect��Ŀ�����
//      src_bitmap��Դλͼ
//      xsrc��ysrc��Դλͼ�б����͵��������Ͻ�����(��Դλͼ���Ͻ�����Ϊԭ��)
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
static bool_t __lcd_bm_to_screen(struct Rectangle *dst_rect,
        struct RectBitmap *src_bitmap,s32 x,s32 y)
{
    return false;
}
//----��screen��ȡ����---------------------------------------------------------
//����: ��screen��ȡһ���أ���ת����cn_sys_pf_e8r8g8b8��cn_sys_pf_a8r8g8b8��ʽ��
//����: x��y������
//����: ������ɫֵ
//-----------------------------------------------------------------------------
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    u32 e,r,g,b,color;
    u32 byteoffset;
    byteoffset = (y*CN_LCD_XSIZE + x)*lcd.pixsize;

    switch (lcd.pixsize)
    {
    case 2:
        color = *(u16 *)(&lcd.pFrameBuffe[byteoffset]);
        r = (color>>11) &0x1f;
        g = (color>>5) & 0x3f;
        b = color & 0x1f;
        break;
    case 3:
        color = lcd.pFrameBuffe[byteoffset]|\
        lcd.pFrameBuffe[byteoffset+1] <<8|\
        lcd.pFrameBuffe[byteoffset+2] <<16;
        b = lcd.pFrameBuffe[byteoffset];
        g = lcd.pFrameBuffe[byteoffset+1];
        r = lcd.pFrameBuffe[byteoffset+2];
        break;
    default:            break;
    }
    e = 0;
    return ((e<<28)|(r<<20) | (g<<12) | (b<<4));
}

//----��ȡbitmap�о��ο�-------------------------------------------------------
//����: ��һ��λͼ�еľ��ζ�ȡ����һ��λͼ�С�
//����: rect������ȡ�ľ���
//      dest��������ε�λͼ���䳤�������rect��ͬ
//����: true=�ɹ���ȡ��false=ʧ�ܣ�ԭ��������ṩ�˲�֧�ֵ����ظ�ʽ
//-----------------------------------------------------------------------------
static bool_t __lcd_get_rect_screen(struct Rectangle *rect,struct RectBitmap *dest)
{
    return false;
}

//----��ʼ��lcd�豸------------------------------------------------------------
//����: ��ʼ��lcd�豸��
//����: ��
//����: ��ʾ����Դָ��
//-----------------------------------------------------------------------------
struct DisplayObj* ModuleInstall_LCD(const char *DisplayName,\
        const char* HeapName)
{
    static struct GkWinObj frame_win;
    static struct RectBitmap FrameBitmap;

    struct HeapCB *heap;
    Ltdc_Lcd_Config(&lcd);
    heap =Heap_FindHeap(HeapName);
    if(heap==NULL)
        return NULL;
    //������64�ֽ�����Դ治��64�ֽڶ�������ʱ���������������
    pLTDCBufferFG1 =M_MallocHeap(CN_LCD_XSIZE*CN_LCD_YSIZE*lcd.pixsize+0x40,heap,0);
//    pLTDCBufferFG2 =M_MallocHeap(CN_LCD_XSIZE*CN_LCD_YSIZE*lcd.pixsize,heap,0);//����һ��
    if(0x3f&(u32)pLTDCBufferFG1)
        pLTDCBufferFG1+=(0x40-(0x3f&(u32)pLTDCBufferFG1));

    FrameBitmap.bm_bits=(u8 *)pLTDCBufferFG1;
    FrameBitmap.width = CN_LCD_XSIZE;
    FrameBitmap.height = CN_LCD_YSIZE;
    FrameBitmap.PixelFormat = lcd.LcdPixelFormat;
    FrameBitmap.linebytes = CN_LCD_XSIZE*lcd.pixsize;
    FrameBitmap.ExColor = 0xffffffff;
    frame_win.wm_bitmap = &FrameBitmap;


    lcd_display.frame_buffer =&frame_win;
    lcd_display.framebuf_direct = true;

    tg_lcd_display.width_um = CFG_LCD_XSIZE_UM;
    tg_lcd_display.height_um = CFG_LCD_YSIZE_UM;
    lcd_display.width = CFG_LCD_XSIZE;
    lcd_display.height = CFG_LCD_YSIZE;
    lcd_display.pixel_format = lcd.LcdPixelFormat;

    lcd_display.draw.SetPixelToBitmap = __lcd_set_pixel_bm;
    lcd_display.draw.FillRectToBitmap = __lcd_fill_rect_bm;
    lcd_display.draw.LineToBitmap = __lcd_line_bm;
    lcd_display.draw.LineToBitmapIe = __lcd_line_bm_ie;
    lcd_display.draw.BltBitmapToBitmap = __lcd_blt_bm_to_bm;
    lcd_display.draw.SetPixelToScreen = __lcd_set_pixel_screen;
    lcd_display.draw.SetPixelGroupToScreen = __lcd_set_pixel_group_screen;
    lcd_display.draw.LineToScreen = __lcd_line_screen;
    lcd_display.draw.LineToScreenIe = __lcd_line_screen_ie;
    lcd_display.draw.FillRectToScreen = __lcd_fill_rect_screen;
    lcd_display.draw.CopyBitmapToScreen = __lcd_bm_to_screen;
    lcd_display.draw.GetPixelFromScreen = __lcd_get_pixel_screen;
    lcd_display.draw.GetRectFromScreen = __lcd_get_rect_screen;

    lcd_display.DisplayHeap =  heap;
    lcd_display.disp_ctrl = __lcd_disp_ctrl;

    Dma2d_Init();
    Lock_MutexCreate_s(&Dma2dMutex,"DMA2D");
    LTDC_Init(pLTDCBufferFG1);

    if(GK_InstallDisplay(&lcd_display,DisplayName))
        return &lcd_display;
    return NULL;
}











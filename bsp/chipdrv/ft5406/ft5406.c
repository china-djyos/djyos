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

// �ļ���     ��ft5406.c
// ģ������: ������оƬft5406������
// ģ��汾: V1.00
// ������Ա: chj
// ����ʱ��: 05/15.2018
// =============================================================================
#include "ft5406.h"
#include "iicbus.h"
#include "systime.h"
#include "Touch.h"
#include "board.h"
#include "stdio.h"

#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern bool_t ModuleInstall_FT5406(void);
//  ModuleInstall_FT5406();
//#if(CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT == true)
//    extern bool_t GDD_AddInputDev(const char *InputDevName);
//    GDD_AddInputDev(CFG_FT5406_TOUCH_NAME);
//#endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"touchscreen ft5406"//iic�ӿڵĴ�����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:later               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","iicbus","graphical decorate development","file system","graphical kernel","touch"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_TOUCHSCREEN_FT5406 == false )
//#warning  " touchscreen_ft5406  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_TOUCHSCREEN_FT5406    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0x30,0x3F,
#define CFG_FT5406_RT_I2C_ADDRESS   0x38                //"�豸��ַ",Ӳ������IIC�豸��ַ
//%$#@enum,0x38,
//%$#@enum,100000,400000
#define CFG_FT5406_RT_I2C_CLK_FRE   (100*1000)           //"�����ٶ�",��λHz

//%$#@string,1,16,
#define CFG_FT5406_BUS_NAME          "I2C0"             //"IIC��������",����оƬʹ�õ�IIC��������
#define CFG_FT5406_TOUCH_NAME        "TOUCH_FT5406"     //"����������",����оƬ������
#define CFG_FT5406_DISPLAY_NAME      "desktop"          //"��������",��������ʹ�õ����������
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

/*! @brief FT5406_RT I2C address. */
//#define CFG_FT5406_RT_I2C_ADDRESS (0x38)

/*! @brief FT5406_RT maximum number of simultaneously detected touches. */
#define FT5406_RT_MAX_TOUCHES (5U)

/*! @brief FT5406_RT register address where touch data begin. */
#define FT5406_RT_TOUCH_DATA_SUBADDR (1)

/*! @brief FT5406_RT raw touch data length. */
#define FT5406_RT_TOUCH_DATA_LEN (0x20)

//FT5406 ���ּĴ�������
#define FT_DEVIDE_MODE          0x00//FT5406ģʽ���ƼĴ���
#define FT_REG_NUM_FINGER       0x02//����״̬�Ĵ���

#define FT_TP1_REG              0X03//��һ�����������ݵ�ַ
#define FT_TP2_REG              0X09//�ڶ������������ݵ�ַ
#define FT_TP3_REG              0X0F//���������������ݵ�ַ
#define FT_TP4_REG              0X15//���ĸ����������ݵ�ַ
#define FT_TP5_REG              0X1B//��������������ݵ�ַ


#define FT_ID_G_LIB_VERSION     0xA1//�汾
#define FT_ID_G_MODE            0xA4//FT5206�ж�ģʽ���ƼĴ���
#define FT_ID_G_THGROUP         0x80//������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE    0x88//����״̬�������üĴ���

#define TOUCH_POINT_GET_EVENT(T) ((touch_event_t)((T).XH >> 6))
#define TOUCH_POINT_GET_ID(T) ((T).YH >> 4)
#define TOUCH_POINT_GET_X(T) ((((T).XH & 0x0f) << 8) | (T).XL)
#define TOUCH_POINT_GET_Y(T) ((((T).YH & 0x0f) << 8) | (T).YL)

typedef struct _ft5406_touch_point
{
    uint8_t XH;
    uint8_t XL;
    uint8_t YH;
    uint8_t YL;
    uint8_t RESERVED[2];
} ft5406_touch_point_t;

typedef struct _ft5406_touch_data
{
    uint8_t DEVIDE_MODE;
    uint8_t GEST_ID;
    uint8_t TD_STATUS;
    ft5406_touch_point_t TOUCH;
} ft5406_touch_data_t;

typedef enum _touch_event
{
    kTouch_Down = 0,    /*!< The state changed to touched. */
    kTouch_Up = 1,      /*!< The state changed to not touched. */
    kTouch_Contact = 2, /*!< There is a continuous touch being detected. */
    kTouch_Reserved = 3 /*!< No touch information available. */
} touch_event_t;

//����IICBUS�ܹ��µ�IIC�豸�ṹ
static struct IIC_Device *ps_FT5406_Dev = NULL;
static u32 s_FT5406_Timeout = CN_TIMEOUT_FOREVER;
static struct SingleTouchPrivate FT5406;
static struct ST_TouchAdjust tg_touch_adjust;

__attribute__((weak))  void Board_FT5406_RST(void)
{
    return;
}

__attribute__((weak))  void Board_FT5406_Int_Gpio(void)
{
    return;
}

// =============================================================================
// ���ܣ�FT5406оƬ��ʼ����
// ��������
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
static bool_t FT5406_Init(void)
{
    u8 data = 0;
    u8 id=0;
    s32 result=0;
    ft5406_touch_data_t touch_data;

    Board_FT5406_Int_Gpio();
    Board_FT5406_RST();

    result=IIC_Write(ps_FT5406_Dev,FT_DEVIDE_MODE,&data,1,true,s_FT5406_Timeout);//������������ģʽ
    if(result!=1)
    {
        goto FT5406_INIT_FALSE;
    }
    else
    {
        result=IIC_Read(ps_FT5406_Dev,data,(u8*)&touch_data,sizeof(touch_data),s_FT5406_Timeout);
        if(result!=sizeof(touch_data))
            goto FT5406_INIT_FALSE;

        id=TOUCH_POINT_GET_ID(touch_data.TOUCH);
        printf("Touch FT5406 Init success,id:%02X",id);
        result = *(&result);    //��������
        return true;
    }

FT5406_INIT_FALSE:
    printf("Touch FT5406 Init ERROR.FILE:%s,LINE:%d\r\n",__FILE__,__LINE__);
    return false;
}

//ɨ�败����(���ò�ѯ��ʽ)
//����:touch_data��,�洢XYZֵ
//����ֵ:�Ƿ��д���
// 1=�����ʰ��£�0 ����������
static ufast_t FT5406_Scan(struct SingleTouchMsg *touch_data)
{
    ft5406_touch_data_t data;
    touch_event_t touch_event;
    s32 result=0;
    u32 io_s=1;
    u32 touch_x=0,touch_y=0;

    io_s = GPIO_ReadPinInput(FT5406_INT_GPIO, FT5406_INT_GPIO_PIN);
    if(io_s==0)
    {
        result=IIC_Read(ps_FT5406_Dev,0x00,(u8*)&data,sizeof(data),s_FT5406_Timeout);
        if(result!=sizeof(data))
            return  0;

        touch_x = TOUCH_POINT_GET_X(data.TOUCH);
        touch_y = TOUCH_POINT_GET_Y(data.TOUCH);

        if(tg_touch_adjust.ratio_x != 0)
        {
            touch_data->y = ((touch_x << 16)- tg_touch_adjust.offset_x)
                                / tg_touch_adjust.ratio_x;
        }
        else
        {
            touch_data->y = touch_x;
        }

        if(tg_touch_adjust.ratio_y != 0)
        {
            touch_data->x = ((touch_y << 16)- tg_touch_adjust.offset_y)
                                / tg_touch_adjust.ratio_y;
        }
        else
        {
            touch_data->x = touch_y;
        }

        touch_data->z = 1;

        touch_event = TOUCH_POINT_GET_EVENT(data.TOUCH);
        if(touch_event==kTouch_Down)
            return 1;
    }
    return 0;
}

//----������ģ��У׼---------------------------------------------------------
//����: У׼������ģ�飬���ѽ��������
//����: desktop����ǰ����
//����: 0=ʧ�ܣ�1=�ɹ�
//----------------------------------------------------------------------------
bool_t Touch_Adjust(struct GkWinObj *desktop,struct ST_TouchAdjust* touch_adjust)
{
    struct SingleTouchMsg touch_xyz0,touch_xyz1;
    FILE *touch_init;
    s32 limit_left,limit_top,limit_right,limit_bottom;

    if((touch_init = fopen("/yaffs2/touch_init.dat","r")) != NULL)
    {

        fread(touch_adjust,sizeof(struct ST_TouchAdjust),1,touch_init);
    }
    else
    {
        limit_left = desktop->limit_left;
        limit_top = desktop->limit_top;
        limit_right = desktop->limit_right;
        limit_bottom = desktop->limit_bottom;

        GK_FillWin(desktop,CN_COLOR_WHITE,0);

        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
                       "����������", 21, CN_COLOR_RED, CN_R2_COPYPEN, 0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                       "��׼ȷ���ʮ�ֽ����", 21, CN_COLOR_RED, CN_R2_COPYPEN, 0);
        GK_Lineto(desktop,0,20,40,20,CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_Lineto(desktop,20,0,20,40,CN_COLOR_RED,CN_R2_COPYPEN,CN_TIMEOUT_FOREVER);
        GK_SyncShow(CN_TIMEOUT_FOREVER);

        while(FT5406_Scan(&touch_xyz0)!=1);//�ȴ�����
        printf("�ɼ�����1:(%d,%d)\n\r",touch_xyz0.x,touch_xyz0.y);

        GK_FillWin(desktop,CN_COLOR_WHITE,0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
                       "����������", 21, CN_COLOR_RED, CN_R2_COPYPEN, 0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                       "�ٴ�׼ȷ���ʮ�ֽ����", 21, CN_COLOR_RED, CN_R2_COPYPEN, 0);
        GK_Lineto(desktop,limit_right-40,limit_bottom-20,
                      limit_right,limit_bottom-20,
                      CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_Lineto(desktop,limit_right-20,limit_bottom-40,
                      limit_right-20,limit_bottom,
                      CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_SyncShow(CN_TIMEOUT_FOREVER);

        while(FT5406_Scan(&touch_xyz1)!=1);//�ȴ�����
        printf("�ɼ�����2:(%d,%d)\n\r",touch_xyz1.x,touch_xyz1.y);

        GK_FillWin(desktop,CN_COLOR_GREEN,0);
        touch_adjust->ratio_x = ((touch_xyz1.x - touch_xyz0.x)<<16)
                        /(limit_right - limit_left -40);
        touch_adjust->offset_x = (touch_xyz0.x<<16) - 20*touch_adjust->ratio_x;
        touch_adjust->ratio_y = ((touch_xyz1.y - touch_xyz0.y)<<16)
                        /(limit_bottom- limit_top-40);
        touch_adjust->offset_y= (touch_xyz0.y<<16) - 20*touch_adjust->ratio_y;
        GK_FillWin(desktop,CN_COLOR_BLUE,0);
        GK_SyncShow(CN_TIMEOUT_FOREVER);

        touch_init = fopen("/yaffs2/touch_init.dat","w+");
        if(touch_init)
            fwrite(touch_adjust,sizeof(struct ST_TouchAdjust),1,touch_init);
        return true;
    }
    return false;
}

// =============================================================================
// ���ܣ�FT5406ģ���ʼ����
// ��������
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t ModuleInstall_FT5406(void)
{
    struct GkWinObj *desktop;
    bool_t result = false;
    static struct IIC_Device* s_FT5406_Dev;

    //���FT5406��IIC����
    s_FT5406_Dev = IIC_DevAdd(CFG_FT5406_BUS_NAME,"IIC_Dev_FT5406",CFG_FT5406_RT_I2C_ADDRESS,0,8);
    if(NULL != s_FT5406_Dev)
    {
        desktop = GK_GetDesktop(CFG_FT5406_DISPLAY_NAME);
        ps_FT5406_Dev = s_FT5406_Dev;
        result=FT5406_Init();
        if(!result)
            return false;

        result=Touch_Adjust(desktop,&tg_touch_adjust);
        if(!result)
            return false;

        FT5406.read_touch = FT5406_Scan;//������������꺯��
        FT5406.touch_loc.display = GK_GetDisplay(CFG_FT5406_DISPLAY_NAME);
        result=Touch_InstallDevice(CFG_FT5406_TOUCH_NAME,&FT5406);//���������Touch
        if(!result)
            return false;

        result = true;
    }

    return result;
}


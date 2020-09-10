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
//����ģ��:��׼����--������ģ��

//------------------------------------------------------

#include "stdint.h"
#include "stdio.h"
#include "stddef.h"
#include "Touch.h"
#include "string.h"
#include "gkernel.h"
//#include "cpu_peri_iic.h"
#include "iicbus.h"
//#include "ctiic.h"
#include "board.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_GT911(void);
//    ModuleInstall_GT911( );
//#if(CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT == true)
//    extern bool_t GDD_AddInputDev(const char *InputDevName);
//    GDD_AddInputDev(CFG_GT911_TOUCH_NAME);
//#endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"touchscreen GT911"//iic�ӿڵĴ���������
//parent:"touch"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:pre-main              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"cpu onchip iic","touch"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"          //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_TOUCHSCREEN_GT911 == false )
//#warning  " touchscreen_GT911  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_TOUCHSCREEN_GT911    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@enum,true,false,
//%$#@string,1,128,
#define CFG_GT911_BUS_NAME     "IIC1"        //"IIC��������",����оƬʹ�õ�IIC��������
#define CFG_GT911_TOUCH_NAME   "gt911"       //"����������",���ô���������
#define CFG_TARGET_DISPLAY_NAME "LCD_F7"      //"����������ʾ������",���ô�����������ʾ��������
//%$#@select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


extern void Set_TpINT_ToIn(void); //��board.c��ʵ�֣�����TP��INT��Ϊ����ģʽ

#define GT911_ADDR                0xBA
#define GT911_ADDR_BITS           16

#define GT911_COMMAND             0x8040
#define GT911_CONFIG_VERSION      0x8047
#define GT911_CONFIG_CHKSUM       0x80FF
#define GT911_PRODUCT_ID          0x8140
#define GT911_SENSOR_ID           0x814A
#define GT911_READ_COOR_ADDR      0x814E

#define GT911_TP1_REG              0X8150  //��һ�����������ݵ�ַ
#define GT911_TP2_REG              0X8158  //�ڶ������������ݵ�ַ
#define GT911_TP3_REG              0X8160  //���������������ݵ�ַ
#define GT911_TP4_REG              0X8168  //���ĸ����������ݵ�ַ
#define GT911_TP5_REG              0X8170  //��������������ݵ�ַ
#define GT911_TP6_REG              0X8178  //���������������ݵ�ַ
#define GT911_TP7_REG              0X8180  //���߸����������ݵ�ַ
#define GT911_TP8_REG              0X8188  //�ڰ˸����������ݵ�ַ
#define GT911_TP9_REG              0X8190  //�ھŸ����������ݵ�ַ
#define GT911_TP10_REG             0X8198  //��ʮ�����������ݵ�ַ
//#define GT911_TP1_REG              0X814f  //��һ��������track id��ַ
//#define GT911_TP2_REG              0X8157  //�ڶ���������track id��ַ
//#define GT911_TP3_REG              0X815f  //������������track id��ַ
//#define GT911_TP4_REG              0X8167  //���ĸ�������track id��ַ
//#define GT911_TP5_REG              0X816f  //�����������track id��ַ
//#define GT911_TP6_REG              0X8177  //������������track id��ַ
//#define GT911_TP7_REG              0X817f  //���߸�������track id��ַ
//#define GT911_TP8_REG              0X8187  //�ڰ˸�������track id��ַ
//#define GT911_TP9_REG              0X818f  //�ھŸ�������track id��ַ
//#define GT911_TP10_REG             0X8197  //��ʮ��������track id��ַ

#define GT911_MAX_TOUCH           0x0A         //����CTP_CFG_GROUP5�еĵ�6��Ԫ��



const u16 GT911_TPX_TBL[]={GT911_TP1_REG, GT911_TP2_REG, GT911_TP3_REG,
        GT911_TP4_REG, GT911_TP5_REG, GT911_TP6_REG, GT911_TP7_REG, GT911_TP8_REG,
        GT911_TP9_REG, GT911_TP10_REG};

//����IICBUS�ܹ��µ�IIC�豸�ṹ
static struct IIC_Device *ps_GT911_Dev = NULL;
static u32 s_GT911_Timeout = CN_TIMEOUT_FOREVER;
static struct SingleTouchPrivate GT911;
__attribute__((weak))  void GT911_Pin_Init(void)
{
    return;
}

#if 0
/* TODO: define your config for Sensor_ID == 5 here, if needed */
#define CTP_CFG_GROUP5 {\
    0x5c,0x00,0x04,0x58,0x02,0x0A,0x3D,0x00,0x01,0x08,0x28,0x0F,0x50,0x32,0x03,0x05,\
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x17,0x19,0x1D,0x14,0x90,0x2F,0x89,0x1E,0x20,\
    0x31,0x0D,0x00,0x00,0x00,0x43,0x03,0x25,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
    0x00,0x00,0x00,0x14,0x2D,0x94,0xD5,0x02,0x07,0x00,0x00,0x04,0xAF,0x15,0x00,0x95,\
    0x19,0x00,0x80,0x1E,0x00,0x70,0x23,0x00,0x63,0x29,0x00,0x63,0x00,0x00,0x00,0x00,\
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
    0x16,0x15,0x14,0x11,0x10,0x0F,0x0E,0x0D,0x0C,0x09,0x08,0x07,0x06,0x05,0x04,0x01,\
    0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x29,0x28,\
    0x27,0x26,0x25,0x24,0x23,0x22,0x21,0x20,0x1F,0x1E,0x1C,0x1B,0x19,0x14,0x13,0x12,\
    0x11,0x10,0x0F,0x0E,0x0D,0x0C,0x0A,0x08,0x07,0x06,0x04,0x02,0x00,0xFF,0x00,0x00,\
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF3,0x01\
}
#else
#define CTP_CFG_GROUP5 {\
    0x65,0x00,0x04,0x58,0x02,0x0A,0x3D,0x00,0x01,0x08,0x28,0x0F,0x50,0x32,0x03,\
    0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x17,0x19,0x1D,0x14,0x90,0x2F,0x89,\
    0x17,0x19,0x31,0x0D,0x00,0x00,0x00,0x01,0x03,0x2D,0x00,0x00,0x00,0x00,0x00,\
    0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x37,0x94,0xD5,0x02,0x07,0x00,0x00,0x04,\
    0xA2,0x11,0x00,0x90,0x16,0x00,0x80,0x1C,0x00,0x73,0x25,0x00,0x68,0x30,0x00,\
    0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x16,0x15,0x14,0x11,0x10,0x0F,0x0E,0x0D,\
    0x0C,0x09,0x08,0x07,0x06,0x05,0x04,0x01,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,\
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x29,0x28,0x27,0x26,0x25,0x24,0x23,0x22,\
    0x21,0x20,0x1F,0x1E,0x1C,0x1B,0x19,0x14,0x13,0x12,0x11,0x10,0x0F,0x0E,0x0D,\
    0x0C,0x0A,0x08,0x07,0x06,0x04,0x02,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,\
    0x00,0x00,0x00,0x00,0x32,0x01\
}
#endif
u8 cfg_info_group5[] = CTP_CFG_GROUP5;


// =============================================================================
// ���ܣ�    ��������
// ������    reg: �Ĵ�����ַ ; buf: ���ݻ���; len: ���ݳ���
// ���أ�    ���յĳ���
// =============================================================================
static bool_t GT911_RD_Reg ( u16 reg,u8 *buf, u8 len)
{
    return IIC_Read(ps_GT911_Dev, reg&0xffff, buf, len, s_GT911_Timeout);
}
// =============================================================================
// ���ܣ�  д�Ĵ���
// ������  reg: �Ĵ�����ַ ; buf: ���ݻ���; len: ���ݳ���
// ���أ�  д�ĳ���
// =============================================================================
static  bool_t GT911_WR_Reg (u16 reg, u8 *buf,u8 len)
{
   return  IIC_Write(ps_GT911_Dev, reg, buf, len, true, s_GT911_Timeout);
}

/* =============================================================================
 ���ܣ�GT911оƬ��ʼ������ʼ���ͼ����豸����Ӧ��IIC����.
 ��������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
static bool_t GT911_Init( )
{
    u8 temp[6];
//    u8 test_buf[512];
    u8 count = 0, Val_1 = 0, Val_2 = 0;
    s8 ret = 0;
    GPIO_SettoLow(GPIO_D,PIN2);         //RST 0
    DJY_EventDelay(20000);
    GPIO_SettoLow(GPIO_D,PIN3);         //INT 0
    DJY_EventDelay(2000);
    GPIO_SettoHigh(GPIO_D,PIN2);        //RST 1
    DJY_EventDelay(6000);
    GPIO_SettoLow(GPIO_D,PIN3);         //INT 0
    DJY_EventDelay(50000);

    Set_TpINT_ToIn();

    ret = GT911_RD_Reg(GT911_PRODUCT_ID, temp, 6);
    if (ret == 6)
    {
        if(temp[3] == 0x00)
            printf("IC Version: %c%c%c_%02X%02X.\r\n",temp[0], temp[1], temp[2], temp[5], temp[4]);
        else
            printf("IC Version: %c%c%c%c_%02X%02X.\r\n",temp[0], temp[1], temp[2], temp[3], temp[5], temp[4]);

        temp[4] = 0;
        if(strcmp((char*)temp, "911") == 0)
        {
            temp[0] = 2;
            GT911_WR_Reg(GT911_COMMAND, &temp[0], 1);
            ret = GT911_RD_Reg(GT911_SENSOR_ID, &Val_1, 1);
            ret = GT911_RD_Reg(GT911_SENSOR_ID, &Val_2, 1);
            if((ret == 1) && (Val_1 == Val_2))
            {
                printf(" Sensor_ID = %d.\r\n", Val_1);
                if(Val_1 < 0x06)
                {
                    return true;

//                    ret = GT911_RD_Reg(GT911_CONFIG_VERSION, test_buf, sizeof(cfg_info_group5));
//                    ret = GT911_RD_Reg(GT911_CONFIG_VERSION, &Val_2, 1);
//                    if((ret == 1) && (Val_2 >= 90))
//                    {
//                        if(GT911_WR_Reg(GT911_CONFIG_VERSION, cfg_info_group5, sizeof(cfg_info_group5)) == sizeof(cfg_info_group5))
//                        {
//                            Djy_EventDelay(2000*1000);
//                            ret = GT911_RD_Reg(GT911_CONFIG_VERSION, test_buf, sizeof(cfg_info_group5));
//                            if((ret > 0) && (test_buf[0] != cfg_info_group5[0]))    //�����ж�������Ϣ�Ƿ����д��ȥ�ˡ�
//                                printf("config info write fail\r\n");
//                            for(count=0; count<sizeof(cfg_info_group5); count++)
//                                temp[0] += cfg_info_group5[count]; /*����У���*/
//                            temp[0] = (~temp[0]) + 1;
//                            temp[1] = 1;    //0,���������浽flash;1,�������浽flash
//                            if(GT911_WR_Reg(GT911_CONFIG_CHKSUM, temp, 2) == 2)
//                                return true;
//                        }
//                    }
                }
            }
        }

    }
    return false;
}

//ɨ�败����(���ò�ѯ��ʽ)
//mode:0,����ɨ��.
//����ֵ:��ǰ����״̬.
// 1=�����ʰ��£�0=����������
static bool_t GT911_Scan(struct SingleTouchMsg *touch_data)
{
    u8 touch_num, buf[4], mode;
    u8 i;
//    if(FT_INT == 0)
//    GT911_RD_Reg(FT_REG_NUM_FINGER,&mode,1);//��ȡ�������״̬
    for(i = 0; i < 5; i++)
    {
        if(GT911_RD_Reg(GT911_READ_COOR_ADDR, &mode, 1))
        {
            if(mode & (1 << 7))     //��ȡ0x814EһֱΪ80��ʾ̧��״̬.bit7λΪ0�Ļ�����ʾbufferû��׼��������ʱ1ms~10ms�ٶ�ȡ,�����ȡֵ��1��ʾbuffer׼��OK
            {
                touch_num = mode&0x0f;  //0~3λ����ʾtouch number������Ϊ0��ʾ̧��
                if(touch_num > 0)
                {
                    touch_data->z=0;
            //        for(i=0;i<GT911_MAX_TOUCH;i++)
            //        {
            //           GT911_RD_Reg(GT911_TPX_TBL[i],buf,4); //��ȡXY����ֵ
            //           if( ((buf[0]&0XF0)==0X80) && ((buf[2]&0XF0)!=0Xf0))
            //           {
            //               tch_x +=((s32)(buf[0]&0X0F)<<8)+buf[1];
            //               tch_y +=((s32)(buf[2]&0X0F)<<8)+buf[3];
            //               contact++;
            //           }
            //        }
            //
            //        if(contact ==0)
            //            return 0;
            //
            //        tch_x /=contact;       //ƽ���������ֵ
            //        tch_y /=contact;
                    GT911_RD_Reg(GT911_TPX_TBL[0],buf,4); //��ȡXY����ֵ,ֻ�����ص�1��
                    touch_data->x = buf[0] | (buf[1] << 8);
                    touch_data->y = buf[2] | (buf[3] << 8);;
                    touch_data->z=1;

                    touch_num = 0;    //��ȡ��Ӧ���������0x814E��0
                    GT911_WR_Reg(GT911_READ_COOR_ADDR,&touch_num,1);
                //    printf("touch_data->x = %d\r\n",touch_data->x);
                //    printf("touch_data->y = %d\r\n",touch_data->y);
                    return 1;
                }
                touch_num = 0;  //0�Ļ�ֱ�Ӷ�0x814E��0
                GT911_WR_Reg(GT911_READ_COOR_ADDR,&touch_num,1);
            }
        }
        DJY_EventDelay(6000);
    }

    return 0;
}

#if 0
//1����������   0�ɿ�������
static bool_t touch_check(void)
{
    u8 i;
    struct SingleTouchMsg touch_xyz0;
    for(i=0;i<5;i++)
    {
        Djy_DelayUs(50*mS);
        if(GT911_Scan(&touch_xyz0))//�ж�����
            i=0;
    }
    return 0;
}
//----������У׼---------------------------------------------------------------
//����: �������ĳߴ������Һ������ʾ������ȫһ�£���װҲ������ƫ���Ҫ����
//      У׼ϵ����ƫ������Ϊ��ø��߾��ȣ�ʹ�ö���С����
//����: display_name,����������Ӧ����ʾ����(��Դ��)//�þ�̬�����洢ÿ�ο���У׼һ��//
//����: ��
//-----------------------------------------------------------------------------
static bool_t touch_ratio_adjust(struct GkWinObj *desktop)
{
    struct SingleTouchMsg touch_xyz0,touch_xyz1;
    s32 limit_left,limit_top,limit_right,limit_bottom;
    FILE *touch_init = NULL;
    touch_init = fopen(CFG_TOUCH_ADJUST_FILE,"r");
    if(touch_init != NULL)
    {
        fread(&tg_touch_adjust,sizeof(struct ST_TouchAdjust),1,touch_init);
        fclose(touch_init);
    }
    else
    {
        memset(&tg_touch_adjust, 0, sizeof(struct ST_TouchAdjust));
        limit_left = desktop->limit_left;
        limit_top = desktop->limit_top;
        limit_right = desktop->limit_right;
        limit_bottom = desktop->limit_bottom;

        GK_FillWin(desktop,CN_COLOR_WHITE,0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
                       "������������Ƭ", 21, CN_COLOR_BLACK+1, CN_R2_COPYPEN, 0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                       "��׼ȷ���ʮ�ֽ����", 21, CN_COLOR_BLACK+1, CN_R2_COPYPEN, 0);
        GK_Lineto(desktop,0,20,40,20,CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_Lineto(desktop,20,0,20,40,CN_COLOR_RED,CN_R2_COPYPEN,CN_TIMEOUT_FOREVER);
        GK_SyncShow(CN_TIMEOUT_FOREVER);
        while(!GT911_Scan(&touch_xyz0));//�ȴ�����
        touch_xyz0.x = 220;
        touch_xyz0.y = 20;
        printf("�ɼ�����1:(%d,%d)\n\r",touch_xyz0.x,touch_xyz0.y);
        touch_check();


        GK_FillWin(desktop,CN_COLOR_WHITE,0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
                       "����������", 21, CN_COLOR_BLACK + 1, CN_R2_COPYPEN, 0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                       "�ٴ�׼ȷ���ʮ�ֽ����", 21, CN_COLOR_BLACK + 1, CN_R2_COPYPEN, 0);
        GK_Lineto(desktop,limit_right-40,limit_bottom-20,
                      limit_right,limit_bottom-20,
                      CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_Lineto(desktop,limit_right-20,limit_bottom-40,
                      limit_right-20,limit_bottom,
                      CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_SyncShow(CN_TIMEOUT_FOREVER);

        while(!GT911_Scan(&touch_xyz1)); //��¼�������ڶ���У��ֵ
        touch_xyz1.x = 20;
        touch_xyz1.y = 300;
        printf("�ɼ�����2:(%d,%d)\n\r",touch_xyz1.x,touch_xyz1.y);
        touch_check();

        GK_FillWin(desktop,CN_COLOR_GREEN,0);
        tg_touch_adjust.ratio_x = ((touch_xyz1.x - touch_xyz0.x)<<16)
                        /(limit_right - limit_left -40);
        tg_touch_adjust.offset_x = (touch_xyz0.x<<16) - 20*tg_touch_adjust.ratio_x;
        tg_touch_adjust.ratio_y = ((touch_xyz1.y - touch_xyz0.y)<<16)
                        /(limit_bottom- limit_top-40);
        tg_touch_adjust.offset_y= (touch_xyz0.y<<16) - 20*tg_touch_adjust.ratio_y;
        GK_FillWin(desktop,CN_COLOR_BLUE,0);
        GK_SyncShow(CN_TIMEOUT_FOREVER);
        touch_init = fopen(CFG_TOUCH_ADJUST_FILE,"w+");
        if(touch_init)
        {
            fwrite(&tg_touch_adjust,sizeof(struct ST_TouchAdjust),1,touch_init);
            fclose(touch_init);
        }
        else
        {
            return false;
        }
    }
    return true;
}
#endif
// =============================================================================
// ���ܣ�GT911ģ���ʼ����
// ��������
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t ModuleInstall_GT911(void)
{
    bool_t result = false;
    static struct IIC_Device* s_GT911_Dev;

    //���GT911��IIC����
    s_GT911_Dev = IIC_DevAdd(CFG_GT911_BUS_NAME,"IIC_Dev_GT911", GT911_ADDR >> 1, 0, GT911_ADDR_BITS);
    if(NULL != s_GT911_Dev)
    {
        GK_GetDesktop(CFG_TARGET_DISPLAY_NAME);
        IIC_BusCtrl(s_GT911_Dev,CN_IIC_SET_CLK,100*1000,0);
//        IIC_BusCtrl(s_GT911_Dev,CN_IIC_SET_POLL,0,0);
        ps_GT911_Dev = s_GT911_Dev;
        result=GT911_Init();
        if(!result)
            return false;

        GT911.read_touch = GT911_Scan;    //������������꺯��
        GT911.touch_loc.display = GK_GetDisplay(CFG_TARGET_DISPLAY_NAME);
        result=Touch_InstallDevice(CFG_GT911_TOUCH_NAME,&GT911);//���������Touch
        if(!result)
            return false;

        result = true;
    }

    return result;
}

void touch_test(void)
{
    u8 temp[6];
    u8 i,touch_num = 0, Val_1 = 0;
    u16 xy;

    while(1)
    {
        do
        {
            GT911_RD_Reg(0x814E, &Val_1, 1);
            Djy_EventDelay(10000);
        }while(!(Val_1 & (1 << 7)));
//        GT911_RD_Reg(0x814f, &Val, 1);
        if(Val_1 & (1 << 7))
        {
            touch_num = Val_1&0x0f;
            printf("touch_num = %d \r\n",touch_num);
            if(touch_num == 0)
            {
                GT911_WR_Reg(0x814E,&touch_num,1);
                printf("û�д���\r\n");
            }
            else
            {
//                for(i=0;i<touch_num;i++)
//                {
                   GT911_RD_Reg(0X814f,temp,7); //��ȡXY����ֵ
                   printf("track id = %d \r\n",temp[0]);
                   xy = temp[5] | (temp[6] << 8);
                   printf("size = %d \r\n", xy);
                   xy = temp[1] | (temp[2] << 8);
                   printf("x=%d.\r\n",xy);
                   xy = temp[3] | (temp[4] << 8);
                   printf("y=%d.\r\n",xy);
//                }
            }
//            else
//            {
//                printf("touch_num = %d ",touch_num);
//            }
        }
//        if(GT911_RD_Reg(0x814f, &temp[0], 7) == 7)
//        {
//            printf("track id = %d \r\n",temp[0]);
//            xy = temp[5] | (temp[6] << 8);
//            printf("size = %d \r\n", xy);
//            xy = temp[1] | (temp[2] << 8);
//            printf("x=%d.\r\n",xy);
//            xy = temp[3] | (temp[4] << 8);
//            printf("y=%d.\r\n",xy);
////            Djy_EventDelay(1000*1000);
//        }
        touch_num = 0;
        GT911_WR_Reg(0x814E,&touch_num,1);
    }
}

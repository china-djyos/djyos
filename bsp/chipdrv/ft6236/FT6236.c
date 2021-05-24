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
//    extern bool_t ModuleInstall_FT6236(void);
//    ModuleInstall_FT6236( );
//#if(CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT == true)
//    extern bool_t GDD_AddInputDev(const char *InputDevName);
//    GDD_AddInputDev(CFG_FT6236_TOUCH_NAME);
//#endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"touchscreen FT6236"//iic�ӿڵĴ���������
//parent:"touch"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"io analog iic bus","touch"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"          //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_TOUCHSCREEN_FT6236 == false )
//#warning  " touchscreen_FT6236  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_TOUCHSCREEN_FT6236    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@enum,24,32,
#define  CFG_TOUCH_SIZE  32     //"�����ߴ�",
//%$#@enum,true,false,
#define  CFG_TOUCH_TURN  false          //"�����귴ת",�������Ƿ���Ҫ��ת
//%$#@num,1,100,
#define CT_MAX_TOUCH  1                         //"������",Ŀǰֻ֧�ֵ��㴥��,֧�����5�㴥��
//%$#@enum,true,false,
//%$#@string,1,128,
#define CFG_TOUCH_ADJUST_FILE   "/efs/touch_init.dat"  //"�����ļ�·��",���津���������������ļ�
#define CFG_FT6236_BUS_NAME     "IoIic"        //"IIC��������",����оƬʹ�õ�IIC��������
#define CFG_FT6236_TOUCH_NAME   "FT6236"       //"����������",���ô���������
#define CFG_TARGET_DISPLAY_NAME "display"      //"����������ʾ������",���ô�����������ʾ��������
//%$#@select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

#if (CFG_TOUCH_SIZE == 24)
#define FT_CMD_WR               (0X15<<1)
#define FT_CMD_RD               (0X15<<1|1)
#elif (CFG_TOUCH_SIZE == 32)
#define FT_CMD_WR               0X70        //д����
#define FT_CMD_RD               0X71        //������
#endif
//FT6236 ���ּĴ�������
#define FT_DEVIDE_MODE          0x00//FT6236ģʽ���ƼĴ���
#define FT_REG_NUM_FINGER       0x02//����״̬�Ĵ���

#define FT_TP1_REG              0X03//��һ�����������ݵ�ַ
#define FT_TP2_REG              0X09//�ڶ������������ݵ�ַ
#define FT_TP3_REG              0X0F//���������������ݵ�ַ
#define FT_TP4_REG              0X15//���ĸ����������ݵ�ַ
#define FT_TP5_REG              0X1B//��������������ݵ�ַ


#define FT_ID_G_LIB_VERSION     0xA1//�汾
#define FT_ID_G_MODE            0xA4//FT6236�ж�ģʽ���ƼĴ���
#define FT_ID_G_THGROUP         0x80//������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE    0x88//����״̬�������üĴ���

const u16 FT6236_TPX_TBL[CT_MAX_TOUCH]={FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,\
                                        FT_TP4_REG,FT_TP5_REG};

//����IICBUS�ܹ��µ�IIC�豸�ṹ
static struct IIC_Device *ps_FT6236_Dev = NULL;
static u32 s_FT6236_Timeout = CN_TIMEOUT_FOREVER;
static struct SingleTouchPrivate FT6236;
static struct ST_TouchAdjust tg_touch_adjust;

//�Ƿ�Ϊ������־λ
static bool_t vertical_screen_flag = true;
__attribute__((weak))  void FT6236_Pin_Init(void)
{
    return;
}

// =============================================================================
// ���ܣ�    ��������
// ������    RegAddr �Ĵ�����ַ  num ���ݳ���
// ���أ����յ���
// =============================================================================
static bool_t FT6236_RD_Reg ( u16 reg,u8 *buf, u8 len)
{
    return IIC_Read(ps_FT6236_Dev,reg&0xff,buf,len,s_FT6236_Timeout);
}
// =============================================================================
// ���ܣ�д�Ĵ���
// ������DelDev,�����ṹ��ָ��
//       addr,���͵�ַ����ָ�洢��ַ
//       buf,�������ݻ�����ָ��
//       len,�������ݳ��ȣ��ֽڵ�λ
//       block_option,����ѡ��,true��ʾ�������ͣ�falseΪ����������
//       timeout,��ʱʱ�䣬us
// =============================================================================
static  bool_t FT6236_WR_Reg (u16 reg, u8 *buf,u8 len)
{
   return  IIC_Write(ps_FT6236_Dev,reg,buf,len,true,s_FT6236_Timeout);
}
/* =============================================================================
 ���ܣ�FT6236оƬ��ʼ������ʼ���ͼ����豸����Ӧ��IIC����.
 ��������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
static bool_t FT6236_Init( )
{
    u8 temp[2];
//    u16 chipid=0;
    FT6236_Pin_Init();

    FT_RST(0);    //��λ
    DJY_DelayUs(50*mS);
    FT_RST(1);   //�ͷŸ�λ
    DJY_DelayUs(100*mS);

    CT_IIC_SCL(1);
    CT_IIC_SDA(1);
    DJY_DelayUs(10*mS);

//    chipid = 0;
//    do{
//        FT6236_RD_Reg(0xa3,&chipid,2);
//         i++;
//         DJY_DelayUs(50*mS);
//         if((chipid==0x06)||(chipid==0x36)||(chipid==0x64))
//            break;
//      } while(i<20);
#if 1
    temp[0]=0;
    temp[1]=0;

    FT6236_WR_Reg(FT_DEVIDE_MODE,temp,1);   //������������ģʽ
//    FT6236_WR_Reg(FT_ID_G_MODE,temp,1);     //��ѯģʽ
//    temp[0]=22;                             //������Чֵ��22��ԽСԽ����
//    FT6236_WR_Reg(FT_ID_G_THGROUP,temp,1);  //���ô�����Чֵ
//    temp[0]=12;                             //�������ڣ�����С��12�����14
//    FT6236_WR_Reg(FT_ID_G_PERIODACTIVE,temp,1);
    //��ȡ�汾�ţ��ο�ֵ��0x3003
    FT6236_RD_Reg(0xa6,&temp[0],2);
    //�汾:0X3003/0X0001/0X0002
//    if((temp[0]==0X30&&temp[1]==0X03)||temp[1]==0X01||temp[1]==0X02)
//    {
        printf("CTP ID:%x\r\n",((u16)temp[0]<<8)+temp[1]);
        return true;
//    }
#endif
//    printf("Touch FT6236 Init ERROR.\r\n");
//    return false;
}

//ɨ�败����(���ò�ѯ��ʽ)
//mode:0,����ɨ��.
//����ֵ:��ǰ����״̬.
// 1=�����ʰ��£�0=����������
static bool_t FT6236_Scan(struct SingleTouchMsg *touch_data)
{
    u8 buf[4],i,mode;
    s32 tch_x,tch_y;
    u8  contact=0;

//    if(FT_INT == 0)
    FT6236_RD_Reg(FT_REG_NUM_FINGER,&mode,1);//��ȡ�������״̬

    if((mode==0)||mode>5)
        return 0;
    tch_x=0;    tch_y=0; touch_data->z=0;
    for(i=0;i<CT_MAX_TOUCH;i++)
    {
        FT6236_RD_Reg(FT6236_TPX_TBL[i],buf,4); //��ȡXY����ֵ
        if( ((buf[0]&0XF0)==0X80) && ((buf[2]&0XF0)!=0Xf0))
        {
#if (CFG_TOUCH_SIZE == 24)
            if(vertical_screen_flag)
            {
                tch_x +=((s32)(buf[0]&0X0F)<<8)+buf[1];
                tch_y +=((s32)(buf[2]&0X0F)<<8)+buf[3];
            }
            else
            {
                tch_y +=((s32)(buf[0]&0X0F)<<8)+buf[1];
                tch_x +=((s32)(buf[2]&0X0F)<<8)+buf[3];
            }
#elif (CFG_TOUCH_SIZE == 32)
            if(vertical_screen_flag)
            {
                tch_x +=((s32)(buf[0]&0X0F)<<8)+buf[1];
                tch_y +=((s32)(buf[2]&0X0F)<<8)+buf[3];
            }
            else
            {
                tch_y +=((s32)(buf[0]&0X0F)<<8)+buf[1];
                tch_x +=((s32)(buf[2]&0X0F)<<8)+buf[3];
            }
#endif

            contact++;
        }
    }

    if(contact ==0)
        return 0;

    tch_x /=contact;       //ƽ���������ֵ
    tch_y /=contact;

    if(tg_touch_adjust.ratio_x != 0)
    {
        touch_data->x = ((tch_x << 16)- tg_touch_adjust.offset_x)
                            / tg_touch_adjust.ratio_x;
    }
    else
    {
        touch_data->x = tch_x;
    }

    if(tg_touch_adjust.ratio_y != 0)
    {
        touch_data->y = ((tch_y << 16)- tg_touch_adjust.offset_y)
                            / tg_touch_adjust.ratio_y;
    }
    else
    {
        touch_data->y = tch_y;
    }
    touch_data->z=1;
//    printf("touch_data->x = %d\r\n",touch_data->x);
//    printf("touch_data->y = %d\r\n",touch_data->y);
    return 1;
}


//1����������   0�ɿ�������
static bool_t touch_check(void)
{
    u8 i;
    struct SingleTouchMsg touch_xyz0;
    for(i=0;i<5;i++)
    {
        DJY_DelayUs(50*mS);
        if(FT6236_Scan(&touch_xyz0))//�ж�����
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
//
//        GK_FillWin(desktop,CN_COLOR_WHITE,0);
//        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
//                       "������������Ƭ", 21, CN_COLOR_BLACK+1, CN_R2_COPYPEN, 0);
//        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
//                       "��׼ȷ���ʮ�ֽ����", 21, CN_COLOR_BLACK+1, CN_R2_COPYPEN, 0);
//        GK_Lineto(desktop,0,20,40,20,CN_COLOR_RED,CN_R2_COPYPEN,0);
//        GK_Lineto(desktop,20,0,20,40,CN_COLOR_RED,CN_R2_COPYPEN,CN_TIMEOUT_FOREVER);
//        GK_SyncShow(CN_TIMEOUT_FOREVER);
//        while(!FT6236_Scan(&touch_xyz0));//�ȴ�����
#if (CFG_TOUCH_SIZE == 24)
        if(vertical_screen_flag)
        {
            #if(CFG_TOUCH_TURN == false)
                touch_xyz0.x = 20;
                touch_xyz0.y = 20;
            #else
                touch_xyz0.x = 220;
                touch_xyz0.y = 300;
            #endif
        }
        else
        {
            #if(CFG_TOUCH_TURN == false)
                touch_xyz0.x = 300;
                touch_xyz0.y = 20;
            #else
                touch_xyz0.x = 20;
                touch_xyz0.y = 220;
            #endif
        }
#elif (CFG_TOUCH_SIZE == 32)
        if(vertical_screen_flag)
        {
            #if(CFG_TOUCH_TURN == false)
                touch_xyz0.x = 20;
                touch_xyz0.y = 20;
            #else
                touch_xyz0.x = 220;
                touch_xyz0.y = 300;
            #endif
        }
        else
        {
            #if(CFG_TOUCH_TURN == false)
                touch_xyz0.x = 300;
                touch_xyz0.y = 20;
            #else
                touch_xyz0.x = 20;
                touch_xyz0.y = 220;
            #endif
        }
#endif

        printf("�ɼ�����1:(%d,%d)\n\r",touch_xyz0.x,touch_xyz0.y);
//        touch_check();


//        GK_FillWin(desktop,CN_COLOR_WHITE,0);
//        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
//                       "����������", 21, CN_COLOR_BLACK + 1, CN_R2_COPYPEN, 0);
//        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
//                       "�ٴ�׼ȷ���ʮ�ֽ����", 21, CN_COLOR_BLACK + 1, CN_R2_COPYPEN, 0);
//        GK_Lineto(desktop,limit_right-40,limit_bottom-20,
//                      limit_right,limit_bottom-20,
//                      CN_COLOR_RED,CN_R2_COPYPEN,0);
//        GK_Lineto(desktop,limit_right-20,limit_bottom-40,
//                      limit_right-20,limit_bottom,
//                      CN_COLOR_RED,CN_R2_COPYPEN,0);
//        GK_SyncShow(CN_TIMEOUT_FOREVER);
//
//        while(!FT6236_Scan(&touch_xyz1)); //��¼�������ڶ���У��ֵ
#if (CFG_TOUCH_SIZE == 24)
        if(vertical_screen_flag)
        {
            #if(CFG_TOUCH_TURN == false)
                touch_xyz1.x = 220;
                touch_xyz1.y = 300;
            #else
                touch_xyz1.x = 20;
                touch_xyz1.y = 20;
            #endif
        }
        else
        {
            #if(CFG_TOUCH_TURN == false)
                touch_xyz1.x = 20;
                touch_xyz1.y = 220;
            #else
                touch_xyz1.x = 300;
                touch_xyz1.y = 20;
            #endif
        }
#elif (CFG_TOUCH_SIZE == 32)
        if(vertical_screen_flag)
        {
            #if(CFG_TOUCH_TURN == false)
                touch_xyz1.x = 220;
                touch_xyz1.y = 300;
            #else
                touch_xyz1.x = 20;
                touch_xyz1.y = 20;
            #endif
        }
        else
        {
            #if(CFG_TOUCH_TURN == false)
                touch_xyz1.x = 20;
                touch_xyz1.y = 220;
            #else
                touch_xyz1.x = 300;
                touch_xyz1.y = 20;
            #endif
        }
#endif

        printf("�ɼ�����2:(%d,%d)\n\r",touch_xyz1.x,touch_xyz1.y);
//        touch_check();

//        GK_FillWin(desktop,CN_COLOR_GREEN,0);
        tg_touch_adjust.ratio_x = ((touch_xyz1.x - touch_xyz0.x)<<16)
                        /(limit_right - limit_left -40);
        tg_touch_adjust.offset_x = (touch_xyz0.x<<16) - 20*tg_touch_adjust.ratio_x;
        tg_touch_adjust.ratio_y = ((touch_xyz1.y - touch_xyz0.y)<<16)
                        /(limit_bottom- limit_top-40);
        tg_touch_adjust.offset_y= (touch_xyz0.y<<16) - 20*tg_touch_adjust.ratio_y;
//        GK_FillWin(desktop,CN_COLOR_BLUE,0);
//        GK_SyncShow(CN_TIMEOUT_FOREVER);
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

// =============================================================================
// ���ܣ�FT6236ģ���ʼ����
// ��������
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t ModuleInstall_FT6236(void)
{
    struct GkWinObj *desktop;
    bool_t result = false;
    static struct IIC_Device* s_FT6236_Dev;

    //���FT6236��IIC����
    s_FT6236_Dev = IIC_DevAdd(CFG_FT6236_BUS_NAME,"IIC_Dev_FT6236",FT_CMD_WR>>1,0,8);
    if(NULL != s_FT6236_Dev)
    {
        desktop = GK_GetDesktop(CFG_TARGET_DISPLAY_NAME);

        if((desktop->right-desktop->left) < (desktop->bottom-desktop->top))
            vertical_screen_flag=true;
        else
            vertical_screen_flag=false;

        IIC_BusCtrl(s_FT6236_Dev,CN_IIC_SET_CLK,100*1000,0);
        ps_FT6236_Dev = s_FT6236_Dev;
        result=FT6236_Init();
        if(!result)
            return false;

        result=touch_ratio_adjust(desktop);
        if(!result)
            return false;

        FT6236.read_touch = FT6236_Scan;    //������������꺯��
        FT6236.touch_loc.display = GK_GetDisplay(CFG_TARGET_DISPLAY_NAME);
        result=Touch_InstallDevice(CFG_FT6236_TOUCH_NAME,&FT6236);//���������Touch
        if(!result)
            return false;

        result = true;
    }

    return result;
}



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
#include "cpu_peri_iic.h"
#include "iicbus.h"
//#include "ctiic.h"
#include "board.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_FT5X26(void);
//    ModuleInstall_FT5X26( );
//#if(CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT == true)
//    extern bool_t GDD_AddInputDev(const char *InputDevName);
//    GDD_AddInputDev(CFG_FT5X26_TOUCH_NAME);
//#endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"touchscreen FT5X26"//iic�ӿڵĴ���������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:later              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"io analog iic bus","graphical kernel"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"graphical decorate development"          //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_TOUCHSCREEN_FT5X26 == false )
//#warning  " touchscreen_FT5X26  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_TOUCHSCREEN_FT5X26    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,1,100,
#define CT_MAX_TOUCH  5                         //"������",֧�����5�㴥��
//%$#@enum,true,false,
//%$#@string,1,128,
#define CFG_TOUCH_ADJUST_FILE   "/yaf2/touch_init.dat"  //���津���������������ļ�
#define CFG_FT5X26_BUS_NAME     "IoIic"        //"IIC��������",����оƬʹ�õ�IIC��������
#define CFG_FT5X26_TOUCH_NAME   "FT5X26"       //"�����豸����",
#define CFG_FT5X26_DISPLAY_NAME "display"      //"�����������������",
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

#define FT_CMD_WR               0X70
#define FT_CMD_RD               0X71
//FT5X26 ���ּĴ�������
#define FT_DEVIDE_MODE          0x00//FT5X26ģʽ���ƼĴ���
#define FT_REG_NUM_FINGER       0x02//����״̬�Ĵ���

#define FT_TP1_REG              0X03//��һ�����������ݵ�ַ
#define FT_TP2_REG              0X09//�ڶ������������ݵ�ַ
#define FT_TP3_REG              0X0F//���������������ݵ�ַ
#define FT_TP4_REG              0X15//���ĸ����������ݵ�ַ
#define FT_TP5_REG              0X1B//��������������ݵ�ַ


#define FT_ID_G_LIB_VERSION     0xA1//�汾
#define FT_ID_G_MODE            0xA4//FT5X26�ж�ģʽ���ƼĴ���
#define FT_ID_G_THGROUP         0x80//������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE    0x88//����״̬�������üĴ���

const u16 FT5206_TPX_TBL[CT_MAX_TOUCH]={FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,\
                                        FT_TP4_REG,FT_TP5_REG};

//����IICBUS�ܹ��µ�IIC�豸�ṹ
static struct IIC_Device *ps_FT5X26_Dev = NULL;
static u32 s_FT5X26_Timeout = CN_TIMEOUT_FOREVER;
static struct SingleTouchPrivate FT5X26;
static struct ST_TouchAdjust tg_touch_adjust;

__attribute__((weak))  void FT5X26_Pin_Init(void)
{
    return;
}

void FT5X26_RST(void)
{
    FT_RST(0);    //��λ
    DJY_DelayUs(20*mS);
    FT_RST(1);   //�ͷŸ�λ
    DJY_DelayUs(50*mS);
}

// =============================================================================
// ���ܣ�    ��������
// ������    RegAddr �Ĵ�����ַ  num ���ݳ���
// ���أ����յ���
// =============================================================================
static bool_t FT5206_RD_Reg ( u16 reg,u8 *buf, u8 len)
{
    return IIC_Read(ps_FT5X26_Dev,reg&0xff,buf,len,s_FT5X26_Timeout);
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
static  bool_t FT5206_WR_Reg (u16 reg, u8 *buf,u8 len)
{
   return  IIC_Write(ps_FT5X26_Dev,reg,buf,len,true,s_FT5X26_Timeout);
}
/* =============================================================================
 ���ܣ�FT5X26оƬ��ʼ������ʼ���ͼ����豸����Ӧ��IIC����.
 ��������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/
static bool_t FT5X26_Init( )
{
    u8 temp[2];

    FT5X26_Pin_Init();
    FT5X26_RST();

    FT5206_WR_Reg(FT_DEVIDE_MODE,temp,1);   //������������ģʽ
    FT5206_WR_Reg(FT_ID_G_MODE,temp,1);     //��ѯģʽ
    temp[0]=22;                             //������Чֵ��22��ԽСԽ����
    FT5206_WR_Reg(FT_ID_G_THGROUP,temp,1);  //���ô�����Чֵ
    temp[0]=12;                             //�������ڣ�����С��12�����14
    FT5206_WR_Reg(FT_ID_G_PERIODACTIVE,temp,1);
    //��ȡ�汾�ţ��ο�ֵ��0x3003
    FT5206_RD_Reg(FT_ID_G_LIB_VERSION,&temp[0],2);
    //�汾:0X3003/0X0001/0X0002
    if((temp[0]==0X30&&temp[1]==0X03)||temp[1]==0X01||temp[1]==0X02)
    {
        printf("CTP ID:%x\r\n",((u16)temp[0]<<8)+temp[1]);
        return true;
    }
    printf("Touch FT5X26 Init ERROR.\r\n");
    return false;
}

//ɨ�败����(���ò�ѯ��ʽ)
//mode:0,����ɨ��.
//����ֵ:��ǰ����״̬.
// 1=�����ʰ��£�0=����������
static bool_t FT5X26_Scan(struct SingleTouchMsg *touch_data)
{
    u8 buf[4],i,mode;
    s32 tch_x,tch_y;
    u8  contact=0;
    FT5206_RD_Reg(FT_REG_NUM_FINGER,&mode,1);//��ȡ�������״̬

    if((mode==0)||mode>5)
        return 0;
    tch_x=0;    tch_y=0; touch_data->z=0;
    for(i=0;i<CT_MAX_TOUCH;i++)
    {
       FT5206_RD_Reg(FT5206_TPX_TBL[i],buf,4); //��ȡXY����ֵ
       if((buf[0]&0XF0)==0X80)
       {
           tch_y +=((s32)(buf[0]&0X0F)<<8)+buf[1];
           tch_x +=((s32)(buf[2]&0X0F)<<8)+buf[3];
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
        if(FT5X26_Scan(&touch_xyz0))//�ж�����
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
    FILE *touch_init;
    s32 limit_left,limit_top,limit_right,limit_bottom;

    if((touch_init = fopen(CFG_TOUCH_ADJUST_FILE,"r")) != NULL)
    {

        fread(&tg_touch_adjust,sizeof(struct ST_TouchAdjust),1,touch_init);
    }
    else
    {
        limit_left = desktop->limit_left;
        limit_top = desktop->limit_top;
        limit_right = desktop->limit_right;
        limit_bottom = desktop->limit_bottom;

        GK_FillWin(desktop,CN_COLOR_WHITE,0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
                       "����������", 21, CN_COLOR_BLACK+1, CN_R2_COPYPEN, 0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                       "��׼ȷ���ʮ�ֽ����", 21, CN_COLOR_BLACK+1, CN_R2_COPYPEN, 0);
        GK_Lineto(desktop,0,20,40,20,CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_Lineto(desktop,20,0,20,40,CN_COLOR_RED,CN_R2_COPYPEN,CN_TIMEOUT_FOREVER);
        GK_SyncShow(CN_TIMEOUT_FOREVER);
        while(!FT5X26_Scan(&touch_xyz0));//�ȴ�����
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

        while(!FT5X26_Scan(&touch_xyz1)); //��¼�������ڶ���У��ֵ
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
            fwrite(&tg_touch_adjust,sizeof(struct ST_TouchAdjust),1,touch_init);
        return true;
    }
    fclose(touch_init);
    return false;
}

// =============================================================================
// ���ܣ�FT5X26ģ���ʼ����
// ��������
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t ModuleInstall_FT5X26(void)
{
    struct GkWinObj *desktop;
    bool_t result = false;
    static struct IIC_Device* s_FT5X26_Dev;

    //���FT5X26��IIC����
    s_FT5X26_Dev = IIC_DevAdd(CFG_FT5X26_BUS_NAME,"IIC_Dev_FT5X26",FT_CMD_WR>>1,0,8);
    if(NULL != s_FT5X26_Dev)
    {
        desktop = GK_GetDesktop(CFG_FT5X26_DISPLAY_NAME);
        IIC_BusCtrl(s_FT5X26_Dev,CN_IIC_SET_CLK,100*1000,0);
        ps_FT5X26_Dev = s_FT5X26_Dev;
        result=FT5X26_Init();
        if(!result)
            return false;

        result=touch_ratio_adjust(desktop);
        if(!result)
            return false;

        FT5X26.read_touch = FT5X26_Scan;//������������꺯��
        FT5X26.touch_loc.display = GK_GetDisplay(CFG_FT5X26_DISPLAY_NAME);
        result=Touch_InstallDevice(CFG_FT5X26_TOUCH_NAME,&FT5X26);//���������Touch
        if(!result)
            return false;

        result = true;
    }

    return result;
}



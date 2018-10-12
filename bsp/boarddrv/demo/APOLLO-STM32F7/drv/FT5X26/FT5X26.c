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
// Copyright (c) 2018 ����Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
#include "ctiic.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern struct GkWinObj;
//    extern bool_t ModuleInstall_Touch_FT5206(struct GkWinObj *desktop,\
//          const char *touch_dev_name );
//    struct GkWinObj *desktop;
//    desktop = GK_GetDesktop(CFG_DESKTOP_NAME);
//    if(NULL == desktop)
//    {
//        printf("Desktop Not Exist !\r\n");
//    }
//    else
//    {
//        ModuleInstall_Touch_FT5206(desktop,CFG_TOUCH_NAME);
//    }
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"FT5X26"       //iic�ӿڵĴ���������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���             //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                  //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"cpu_peri_iic","gkernel"     //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"GDD"          //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CT_MAX_TOUCH        //****�������Ƿ��Ѿ����ú�
#warning    FT5X26�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,1,100,
#define CT_MAX_TOUCH  5                         //"������",֧�����5�㴥��
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_DESKTOP_NAME              "desktop" //"��������",���ô�����������ʾ�����������
#define CFG_TOUCH_NAME                "FT5X26"  //"����������",���ô���������
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure



//�������껺����



static struct ST_TouchAdjust tg_touch_adjust;



#define FT_CMD_WR               0X70
#define FT_CMD_RD               0X71
//FT5206 ���ּĴ�������
#define FT_DEVIDE_MODE          0x00//FT5206ģʽ���ƼĴ���
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

const u16 FT5206_TPX_TBL[CT_MAX_TOUCH]={FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,\
                                        FT_TP4_REG,FT_TP5_REG};

// =============================================================================
// ���ܣ�    ��������
// ������    RegAddr �Ĵ�����ַ  num ���ݳ���
// ���أ����յ���
// =============================================================================
static bool_t FT5206_RD_Reg ( u16 reg,u8 *buf, u8 len)
{
    u8 i;
    CT_IIC_Start();
    CT_IIC_Send_Byte(FT_CMD_WR);    //����д����
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg&0XFF);     //���͵�8λ��ַ
    CT_IIC_Wait_Ack();
    CT_IIC_Start();
    CT_IIC_Send_Byte(FT_CMD_RD);    //���Ͷ�����
    CT_IIC_Wait_Ack();
    for(i=0;i<len;i++)
    {
        buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //������
    }
    CT_IIC_Stop();//����һ��ֹͣ����
    return true;
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
    u8 i;
    u8 ret=0;
    CT_IIC_Start();
    CT_IIC_Send_Byte(FT_CMD_WR);    //����д����
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg&0XFF);     //���͵�8λ��ַ
    CT_IIC_Wait_Ack();
    for(i=0;i<len;i++)
    {
        CT_IIC_Send_Byte(buf[i]);   //������
        ret=CT_IIC_Wait_Ack();
        if(ret)break;
    }
    CT_IIC_Stop();                  //����һ��ֹͣ����
    if(ret==0)
        return true;
    return false;

}
/* =============================================================================
 ���ܣ�FT5206оƬ��ʼ������ʼ���ͼ����豸����Ӧ��IIC����.
 ��������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/

static bool_t FT5206_Init( )
{
    u8 temp[2];
    CT_IIC_Init();
    FT_RST(0);    //��λ
    Djy_DelayUs(20*mS);
    FT_RST(1);   //�ͷŸ�λ
    Djy_DelayUs(50*mS);
    temp[0]=0;
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

static bool_t FT5206_Scan(struct SingleTouchMsg *touch_data)
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
        Djy_DelayUs(50*mS);
        if(FT5206_Scan(&touch_xyz0))//�ж�����
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
static void touch_ratio_adjust(struct GkWinObj *desktop)
{
    struct SingleTouchMsg touch_xyz0,touch_xyz1;
    FILE *touch_init;
    s32 limit_left,limit_top,limit_right,limit_bottom;

    if((touch_init = fopen("/yaffs2/touch_init.dat","r")) != NULL)
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
                       "����������", 21, CN_COLOR_BLACK, CN_R2_COPYPEN, 0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                       "��׼ȷ���ʮ�ֽ����", 21, CN_COLOR_BLACK, CN_R2_COPYPEN, 0);
        GK_Lineto(desktop,0,20,40,20,CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_Lineto(desktop,20,0,20,40,CN_COLOR_RED,CN_R2_COPYPEN,CN_TIMEOUT_FOREVER);
        GK_SyncShow(CN_TIMEOUT_FOREVER);
        while(!FT5206_Scan(&touch_xyz0));//�ȴ�����
        printf("�ɼ�����1:(%d,%d)\n\r",touch_xyz0.x,touch_xyz0.y);
        touch_check();


        GK_FillWin(desktop,CN_COLOR_WHITE,0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
                       "����������", 21, CN_COLOR_BLACK, CN_R2_COPYPEN, 0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                       "�ٴ�׼ȷ���ʮ�ֽ����", 21, CN_COLOR_BLACK, CN_R2_COPYPEN, 0);
        GK_Lineto(desktop,limit_right-40,limit_bottom-20,
                      limit_right,limit_bottom-20,
                      CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_Lineto(desktop,limit_right-20,limit_bottom-40,
                      limit_right-20,limit_bottom,
                      CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_SyncShow(CN_TIMEOUT_FOREVER);

        while(!FT5206_Scan(&touch_xyz1)); //��¼�������ڶ���У��ֵ
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

        touch_init = fopen("/yaffs2/touch_init.dat","w+");
        if(touch_init)
            fwrite(&tg_touch_adjust,sizeof(struct ST_TouchAdjust),1,touch_init);
    }
    fclose(touch_init);
}

//----��ʼ��������ģ��---------------------------------------------------------
//����:
//����: display_name,����������Ӧ����ʾ����(��Դ��)
//����: true,�ɹ�;false,ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_Touch_FT5206(struct GkWinObj *desktop,\
        const char *touch_dev_name )
{
    static struct SingleTouchPrivate FT5206;

     if(FT5206_Init()==false)
          return false;
     touch_ratio_adjust(desktop);          //��ĻУ׼

     FT5206.read_touch = FT5206_Scan;//������������꺯��
     FT5206.touch_loc.display = NULL;     //NULL��ʾ��Ĭ������
     Touch_InstallDevice(touch_dev_name,&FT5206);//���������Touch
     return true;
}




#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "stdint.h"

#include "int_hard.h"
#include "int.h"
#include "cpu_peri.h"
#include "Touch.h"
#include "gkernel.h"
#include "systime.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_TOUCH_S3C2416(void);
//    ModuleInstall_TOUCH_S3C2416( );
//#if(CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT == true)
//    extern bool_t GDD_AddInputDev(const char *InputDevName);
//    GDD_AddInputDev(CFG_TOUCH_DEV_NAME);
//#endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip touch"//CPU��������������
//parent:"touch"     //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                        //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                     //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                       //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                     //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                       //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"graphical kernel","touch"//������������������������none����ʾ�������������
                                       //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                       //����������������������г����á�,���ָ�
//weakdependence:"none"                 //��������������������������none����ʾ�������������
                                       //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                       //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                       //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_TOUCH == false )
//#warning  " cpu_onchip_touch  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_TOUCH    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_TOUCH_DEV_NAME      "TOUCH_MX6Q"     //"����������",���ô���������
#define CFG_DISPLAY_NAME        "LCD_MX6Q"       //"��ʾ������",���ô�����������ʾ��������
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

/*============================================================================*/

#define ADC_MCLK    (50*1000*1000)
#define ADC_CLK     2000000 /*2MHZ*/

/*============================================================================*/

#define TS_XY_SWAP
#define TS_ORG_RD

#define TS_CAL_XOFFSET      (40)
#define TS_CAL_YOFFSET      (40)

#define TS_CAL_LU_XPOS      (TS_CAL_XOFFSET)
#define TS_CAL_LU_YPOS      (TS_CAL_YOFFSET)

#define TS_CAL_RU_XPOS      (CN_LCD_XSIZE-TS_CAL_XOFFSET)
#define TS_CAL_RU_YPOS      (TS_CAL_YOFFSET)

#define TS_CAL_LD_XPOS      (TS_CAL_XOFFSET)
#define TS_CAL_LD_YPOS      (CN_LCD_YSIZE-TS_CAL_YOFFSET)

#define TS_CAL_RD_XPOS      (CN_LCD_XSIZE-TS_CAL_XOFFSET)
#define TS_CAL_RD_YPOS      (CN_LCD_YSIZE-TS_CAL_YOFFSET)

#define TS_CAL_XSIZE        (TS_CAL_RU_XPOS-TS_CAL_LU_XPOS)
#define TS_CAL_YSIZE        (TS_CAL_RD_YPOS-TS_CAL_RU_YPOS)

typedef struct TS_CFG_DATA
{
    u16 LUAdx,LUAdy;
    u16 RUAdx,RUAdy;
    u16 RDAdx,RDAdy;
    u16 LDAdx,LDAdy;
}TS_CFG_DATA;

///////////////////

static  int ts_xsize,ts_ysize;
static  int ts_cal_ref_pos[4][2];
static  TS_CFG_DATA ts_cfg_data;   //��������

#define CN_LCD_XSIZE   ts_xsize
#define CN_LCD_YSIZE   ts_ysize

/*============================================================================*/
/*============================================================================*/

static bool_t ts_translate_data(s32 *x,s32 *y)
{
    int x_coor,y_coor,adx,ady;
    int ts_adx_min,ts_adx_max,ts_ady_min,ts_ady_max;

    if((x!=NULL)&&(y!=NULL))
    {
        adx =*x;
        ady =*y;

#ifdef  TS_ORG_LU
        ts_adx_min  =ts_cfg_data.LUAdx;
        ts_ady_min  =ts_cfg_data.LUAdy;

        ts_adx_max  =ts_cfg_data.RDAdx;
        ts_ady_max  =ts_cfg_data.RDAdy;

        if((ts_adx_min==ts_adx_max) || (ts_ady_min==ts_ady_max))
        {
            *x  =-1;
            *y  =-1;
            return  false;
        }

#ifdef  TS_XY_SWAP
        x_coor = TS_CAL_XOFFSET+(CN_LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
        y_coor = TS_CAL_YOFFSET+(CN_LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
#else
        x_coor = TS_CAL_XOFFSET+(CN_LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
        y_coor = TS_CAL_YOFFSET+(CN_LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
#endif
        *x =x_coor;
        *y =y_coor;
#endif

#ifdef  TS_ORG_RU
        ts_adx_min  =ts_cfg_data.RUAdx;
        ts_ady_min  =ts_cfg_data.RUAdy;

        ts_adx_max  =ts_cfg_data.LDAdx;
        ts_ady_max  =ts_cfg_data.LDAdy;

        if((ts_adx_min==ts_adx_max) || (ts_ady_min==ts_ady_max))
        {
            *x  =-1;
            *y  =-1;
            return  false;
        }

#ifdef  TS_XY_SWAP
        x_coor = TS_CAL_XOFFSET+(CN_LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
        y_coor = TS_CAL_YOFFSET+(CN_LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
#else
        x_coor = TS_CAL_XOFFSET+(CN_LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
        y_coor = TS_CAL_YOFFSET+(CN_LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
#endif
        *x  =CN_LCD_XSIZE-x_coor;
        *y  =y_coor;
#endif

#ifdef  TS_ORG_RD
        ts_adx_min  =ts_cfg_data.RDAdx;
        ts_ady_min  =ts_cfg_data.RDAdy;

        ts_adx_max  =ts_cfg_data.LUAdx;
        ts_ady_max  =ts_cfg_data.LUAdy;

        if((ts_adx_min==ts_adx_max) || (ts_ady_min==ts_ady_max))
        {
            *x  =-1;
            *y  =-1;
            return  false;
        }

#ifdef  TS_XY_SWAP
        x_coor = TS_CAL_XOFFSET+(CN_LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
        y_coor = TS_CAL_YOFFSET+(CN_LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
#else
        x_coor = TS_CAL_XOFFSET+(CN_LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
        y_coor = TS_CAL_YOFFSET+(CN_LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
#endif
        *x  =CN_LCD_XSIZE-x_coor;
        *y  =CN_LCD_YSIZE-y_coor;
#endif

#ifdef  TS_ORG_LD
        ts_adx_min  =ts_cfg_data.LDAdx;
        ts_ady_min  =ts_cfg_data.LDAdy;

        ts_adx_max  =ts_cfg_data.RUAdx;
        ts_ady_max  =ts_cfg_data.RUAdy;

        if((ts_adx_min==ts_adx_max) || (ts_ady_min==ts_ady_max))
        {
            *x  =-1;
            *y  =-1;
            return  false;
        }

#ifdef  TS_XY_SWAP
        x_coor = TS_CAL_XOFFSET+(CN_LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
        y_coor = TS_CAL_YOFFSET+(CN_LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
#else
        x_coor = TS_CAL_XOFFSET+(CN_LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
        y_coor = TS_CAL_YOFFSET+(CN_LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
#endif
        *x  =x_coor;
        *y  =CN_LCD_YSIZE-y_coor;

#endif
        return true;
    }
    return  false;
}

/*============================================================================*/


void touch_ratio_adjust(struct GkWinObj *desktop);
static ufast_t read_touch_data(struct SingleTouchMsg *touch_data);
static bool_t touch_hard_init(void);

//----��ʼ��������ģ��---------------------------------------------------------
//����:
//����: display_dev_name:����������Ӧ����ʾ����(��Դ��)
//      touch_dev_name:�������豸��.
//����: ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_TOUCH_S3C2416(void)
{
    struct GkWinObj *desktop;
    static struct SingleTouchPrivate touch_dev;

    desktop = GK_GetDesktop(CFG_DISPLAY_NAME);
    ts_xsize =desktop->right-desktop->left;
    ts_ysize =desktop->bottom-desktop->top;

    ts_cal_ref_pos[0][0] =TS_CAL_LU_XPOS;
    ts_cal_ref_pos[0][1] =TS_CAL_LU_YPOS;
    ts_cal_ref_pos[1][0] =TS_CAL_RU_XPOS;
    ts_cal_ref_pos[1][1] =TS_CAL_RU_YPOS;
    ts_cal_ref_pos[2][0] =TS_CAL_RD_XPOS;
    ts_cal_ref_pos[2][1] =TS_CAL_RD_YPOS;
    ts_cal_ref_pos[3][0] =TS_CAL_LD_XPOS;
    ts_cal_ref_pos[3][1] =TS_CAL_LD_YPOS;

    if(!touch_hard_init( ))
    {
        return false;
    }
    touch_ratio_adjust(desktop);
    touch_dev.read_touch = read_touch_data;
    touch_dev.touch_loc.display = GK_GetDisplay(CFG_DISPLAY_NAME);
    Touch_InstallDevice(CFG_TOUCH_DEV_NAME,&touch_dev);
    return true;
}

//----������Ӳ����ʼ��---------------------------------------------------------
//����: ������Ӳ����ʼ��
//����: ��
//����: �����豸�ţ�-1��ʾ���ռ����豸ʧ��
//-----------------------------------------------------------------------------
static bool_t touch_hard_init(void)
{
    int prs;

    struct ADC_REG *adc=ADC_REG_BASE;

    prs = (ADC_MCLK/ADC_CLK)-1;

    adc->ADCDLY = 4000;                  //Normal conversion mode delay about (1/3.6864M)*50000=13.56ms
    adc->ADCCON = (1<<14) | (prs<<6) | (1<<3)| (0<<2)  ;       //read start
    adc->ADCTSC = 0xd3;  //Wfait,XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En
    return true;
}

static  bool_t ts_is_down(void)
{
    struct ADC_REG *adc=ADC_REG_BASE;
    int i,j;
    ///

    for(i=0;i<4;i++)    //���4��
    {
        if((adc->ADCDAT0&(1<<15)))  return  false;
        if((adc->ADCDAT1&(1<<15)))  return  false;
        for(j=0;j<50;j++);  //Delay
    }
    //DebugPrintf("ts_down\r\n");
    return  true;
}

static  void InsertSort(int *A, int p, int r)
{
    int i,j;
    int key;
    for(i=p+1; i<=r; i++)
    {
        key = A[i];
        j = i-1;
        while (j >= 0 && A[j] > key)
        {
            A[j+1] = A[j];
            j--;
        }
        A[j+1] = key;
    }
}

#define TS_SAMPLE_COUNT 8
static  int ad_buf[TS_SAMPLE_COUNT];

static  int ts_get_x_data_raw(void)
{
    int i,j,prs;
    int dat;
    struct ADC_REG *adc=ADC_REG_BASE;
    /////

    prs  = (ADC_MCLK/ADC_CLK)-1;

    adc->ADCDLY = 1000;
    adc->ADCCON = (1<<14) | (prs<<6)|(1<<3)  ;     //read start
    adc->ADCTSC = (0<<7)|(1<<6)|(0<<5)|(1<<4)|(1<<3)|(0<<2)|(1);// read x
    for(j=200;j>0;j--);

    for(i=0;i<TS_SAMPLE_COUNT;i++)
    {
        adc->ADCCON |=0x1;                  //start ADC
        for(j=200;j>0;j--);
        while(adc->ADCCON & 0x1);           //check if Enable_start is low
        while(!(adc->ADCCON & (1<<15)));    //check if EC(End of Conversion) flag is high, This line is necessary~!!

        ad_buf[i]=adc->ADCDAT0&0xFFF;

    }

    adc->ADCTSC = 0xd3;

    InsertSort(ad_buf,0,TS_SAMPLE_COUNT-1);
    i = TS_SAMPLE_COUNT>>1;
    dat = 0;
    dat += ad_buf[i++];
    dat += ad_buf[i++];
    dat = dat>>1;

//  DebugPrintf("ts_adx =%04XH\r\n",dat);

    return dat;
    ////
}

static  int ts_get_y_data_raw(void)
{
    int i,j,prs;
    int dat;
    struct ADC_REG *adc=ADC_REG_BASE;
    /////

    prs  = (ADC_MCLK/ADC_CLK)-1;

    adc->ADCDLY = 1000;
    adc->ADCCON = (1<<14) | (prs<<6)|(1<<3)  ;     //read start
    adc->ADCTSC = (0<<7)|(1<<6)|(0<<5)|(1<<4)|(1<<3)|(0<<2)|(2);// read y
    for(j=200;j>0;j--);

    for(i=0;i<TS_SAMPLE_COUNT;i++)
    {
        adc->ADCCON |=0x1;                  //start ADC
        for(j=200;j>0;j--);
        while(adc->ADCCON & 0x1);           //check if Enable_start is low
        while(!(adc->ADCCON & (1<<15)));    //check if EC(End of Conversion) flag is high, This line is necessary~!!

        ad_buf[i] =adc->ADCDAT1&0xFFF;


    }

    adc->ADCTSC = 0xd3;

    InsertSort(ad_buf,0,TS_SAMPLE_COUNT-1);
    i = TS_SAMPLE_COUNT>>1;
    dat = 0;
    dat += ad_buf[i++];
    dat += ad_buf[i++];
    dat = dat>>1;

//  DebugPrintf("ts_ady =%04XH\r\n",dat);

    return dat;
    ////

}

static bool_t   ts_get_data_raw(s32 *x,s32 *y)
{
    if(ts_is_down())
    {
        x=ts_get_x_data_raw();
        y=ts_get_y_data_raw();
        return true;
    }
    return false;
}

//----��ȡ����������-----------------------------------------------------------
//����: ��ȡstmpe811�ɼ����Ĵ��������꣬����ж�㣬��ƽ��֮
//����: touch_data���ɼ���������
//����: 1=�����ʰ��£�0=����������
//-----------------------------------------------------------------------------
static ufast_t read_touch_data(struct SingleTouchMsg *touch_data)
{
     static s32 x=0,y=0,z=0;

    touch_data->display = NULL;
    if(ts_is_down())
    {

        x =ts_get_x_data_raw();
        y =ts_get_y_data_raw();
        z =1;
        ts_translate_data(&x,&y);
        touch_data->x =x;
        touch_data->y =y;
        touch_data->z = z;

        return 1;
    }
    else
    {
        touch_data->z = 0;
        return 0;
    }

}


static  void draw_cursor(struct GkWinObj *desktop,int x,int y)
{
    GK_Lineto(desktop,x,y,x,y-20+1,CN_COLOR_RED,CN_R2_COPYPEN,0); //��
    GK_Lineto(desktop,x,y,x,y+20-1,CN_COLOR_RED,CN_R2_COPYPEN,0); //��
    GK_Lineto(desktop,x,y,x+20-1,y,CN_COLOR_RED,CN_R2_COPYPEN,0);//��
    GK_Lineto(desktop,x,y,x-20+1,y,CN_COLOR_RED,CN_R2_COPYPEN,0); //��
    GK_SyncShow(1000*mS);
}

static  void clr_cursor(struct GkWinObj *desktop,int x,int y)
{
    GK_Lineto(desktop,x,y,x,y-20+1,CN_COLOR_WHITE,CN_R2_COPYPEN,0); //��
    GK_Lineto(desktop,x,y,x,y+20-1,CN_COLOR_WHITE,CN_R2_COPYPEN,0); //��
    GK_Lineto(desktop,x,y,x+20-1,y,CN_COLOR_WHITE,CN_R2_COPYPEN,0);//��
    GK_Lineto(desktop,x,y,x-20+1,y,CN_COLOR_WHITE,CN_R2_COPYPEN,0); //��
    GK_SyncShow(1000*mS);

}

//----������У׼---------------------------------------------------------------
//����: �������ĳߴ������Һ������ʾ������ȫһ�£���װҲ������ƫ���Ҫ����
//      У׼ϵ����ƫ������Ϊ��ø��߾��ȣ�ʹ�ö���С����
//����: display_name,����������Ӧ����ʾ����(��Դ��)
//����: ��
//-----------------------------------------------------------------------------
void touch_ratio_adjust(struct GkWinObj *desktop)
{

    struct SingleTouchMsg touch_xyz0,touch_xyz1;
    FILE *touch_init;
    s32 limit_left,limit_top,limit_right,limit_bottom,pen_down_time,step;

    if((touch_init = fopen("sys:\\touch_init.dat","r")) != NULL)
    {

        fread(&ts_cfg_data,sizeof(TS_CFG_DATA),1,touch_init);
    }
    else
    {
        limit_left = desktop->left;
        limit_top = desktop->top;
        limit_right = desktop->right;
        limit_bottom = desktop->bottom;
    //    GK_CreateWin(desktop,desktop,limit_left,limit_top,limit_right,limit_bottom,
    //                      CN_COLOR_WHITE,CN_WINBUF_BUF,"&tg_touch_adjust",CN_R3_SRCCOPY,0);
    //    GK_SetPrio(desktop,-1,CN_GK_SYNC);
        GK_FillWin(desktop,CN_COLOR_WHITE,0);

        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
                            "����������",21,CN_COLOR_BLUE,CN_R2_COPYPEN,0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                            "��׼ȷ���ʮ�ֽ����",21,CN_COLOR_BLUE,CN_R2_COPYPEN,0);

        step=0;
        while(step<4)
        {
            s32 adx,ady;

            if(step>0)
            {
                clr_cursor(desktop,ts_cal_ref_pos[step-1][0],ts_cal_ref_pos[step-1][1]);
            }
            draw_cursor(desktop,ts_cal_ref_pos[step][0],ts_cal_ref_pos[step][1]);

            while(ts_is_down())
            {
                DJY_DelayUs(100*mS);
            }
            pen_down_time=0;
            while(1)
            {
                if(ts_is_down())
                {

                    adx=ts_get_x_data_raw();
                    ady=ts_get_y_data_raw();
                    if(pen_down_time++>5)
                    {
                        break;
                    }
                }
                else
                {
                    pen_down_time=0;
                }
                DJY_DelayUs(100*mS);
            }

            switch(step)
            {
            case 0:
                ts_cfg_data.LUAdx =adx;
                ts_cfg_data.LUAdy =ady;
                break;

            case 1:
                ts_cfg_data.RUAdx =adx;
                ts_cfg_data.RUAdy =ady;
                break;

            case 2:
                ts_cfg_data.RDAdx =adx;
                ts_cfg_data.RDAdy =ady;
                break;

            case 3:
                ts_cfg_data.LDAdx =adx;
                ts_cfg_data.LDAdy =ady;
                break;

            }
            printf("ts_cal[%d]: %04X,%04X\r\n",step,adx,ady);
            step++;
        }

        GK_FillWin(desktop,CN_COLOR_WHITE,0);
        GK_SyncShow(1000*mS);
        while(ts_is_down())
        {
            DJY_DelayUs(100*mS);
        }
        DJY_DelayUs(500*mS);

    //    GK_DestroyWin(desktop);
        touch_init = fopen("sys:\\touch_init.dat","w+");
        fwrite(&ts_cfg_data,sizeof(TS_CFG_DATA),1,touch_init);
    }
    fclose(touch_init);

}
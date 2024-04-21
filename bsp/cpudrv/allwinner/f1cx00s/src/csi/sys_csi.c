#include "stdint.h"
#include "sys_csi.h"
//#include "sys_cache.h"
#include "sys_clock.h"
//#include "sys_uart.h"
//#include "sys_delay.h"
#include "cpu_peri_io.h"
//#include "sys_interrupt.h"
#include "cpu_peri_gpio.h"
#include "sys_defe.h"
#include "int.h"
//#include "sys_timer.h"
//#include "sys_lcd.h"


#define W(offset,value)          (*((volatile __u32 *)(offset))=(value))    //
#define R(offset)                (*((volatile __u32 *)(offset)))            //
#define S(offset,bit)            (*((volatile __u32 *)(offset)) |=(bit))    //
#define C(offset,bit)            (*((volatile __u32 *)(offset)) &=(~(bit)))   //

#define  F1C100S_CSI_BASE_ADDR 0x01cb0000;

__u32 CSIC_REGS_BASE      ;
__u32 CSI_REG_EN          ;
__u32 CSI_REG_CONF        ;
__u32 CSI_REG_CTRL        ;
__u32 CSI_REG_SCALE       ;

__u32 CSI_REG_BUF_0_A     ;//>>----common part for 4CH TDM
__u32 CSI_REG_BUF_0_B     ;
__u32 CSI_REG_BUF_1_A     ;
__u32 CSI_REG_BUF_1_B     ;
__u32 CSI_REG_BUF_2_A     ;
__u32 CSI_REG_BUF_2_B     ;
__u32 CSI_REG_BUF_CTRL    ;
__u32 CSI_REG_STATUS      ;
__u32 CSI_REG_INT_EN      ;
__u32 CSI_REG_INT_STATUS  ;
__u32 CSI_REG_RESIZE_H    ;
__u32 CSI_REG_RESIZE_V    ;
__u32 CSI_REG_BUF_LENGTH  ;//common part for 4CH TDM----<<


__u32 CSI_4CH_TDM_REG_STEP = 0x100;//only for CSI0 interface

#define sysprintf printf
u32 Read_time_ms(void)
{
    return (u32)(DJY_GetSysTime()/1000);
}

/* set base module */
//////////////////////////////////////////////////////////////////////////////
void CSI_set_base(__u32 sel)
{
    __u32 ch_num = sel & 0x3f;
    CSIC_REGS_BASE = F1C100S_CSI_BASE_ADDR;

    ch_num = 0 ;

    CSI_REG_EN           =(CSIC_REGS_BASE + 0x00);
    CSI_REG_CONF         =(CSIC_REGS_BASE + 0x04);
    CSI_REG_CTRL         =(CSIC_REGS_BASE + 0x08);
    CSI_REG_SCALE        =(CSIC_REGS_BASE + 0x0C);
    CSI_REG_BUF_0_A      =(CSIC_REGS_BASE + ch_num * CSI_4CH_TDM_REG_STEP + 0x10);
    CSI_REG_BUF_0_B      =(CSIC_REGS_BASE + ch_num * CSI_4CH_TDM_REG_STEP + 0x14);
    CSI_REG_BUF_1_A      =(CSIC_REGS_BASE + ch_num * CSI_4CH_TDM_REG_STEP + 0x18);
    CSI_REG_BUF_1_B      =(CSIC_REGS_BASE + ch_num * CSI_4CH_TDM_REG_STEP + 0x1C);
    CSI_REG_BUF_2_A      =(CSIC_REGS_BASE + ch_num * CSI_4CH_TDM_REG_STEP + 0x20);
    CSI_REG_BUF_2_B      =(CSIC_REGS_BASE + ch_num * CSI_4CH_TDM_REG_STEP + 0x24);
    CSI_REG_BUF_CTRL     =(CSIC_REGS_BASE + ch_num * CSI_4CH_TDM_REG_STEP + 0x28);
    CSI_REG_STATUS       =(CSIC_REGS_BASE + ch_num * CSI_4CH_TDM_REG_STEP + 0x2C);
    CSI_REG_INT_EN       =(CSIC_REGS_BASE + ch_num * CSI_4CH_TDM_REG_STEP + 0x30);
    CSI_REG_INT_STATUS   =(CSIC_REGS_BASE + ch_num * CSI_4CH_TDM_REG_STEP + 0x34);
    CSI_REG_RESIZE_H     =(CSIC_REGS_BASE + ch_num * CSI_4CH_TDM_REG_STEP + 0x40);
    CSI_REG_RESIZE_V     =(CSIC_REGS_BASE + ch_num * CSI_4CH_TDM_REG_STEP + 0x44);
    CSI_REG_BUF_LENGTH   =(CSIC_REGS_BASE + ch_num * CSI_4CH_TDM_REG_STEP + 0x48);
}

/* open module */
////////////////////////////////////////////////////////////////////////////////
__s32  CSI_open(void)
{
    S(CSI_REG_EN, 0X1<<0);

    return EPDK_OK;
}

__s32  CSI_close(void)
{
    C(CSI_REG_EN, 0X1<<0);

    return EPDK_OK;
}

/* configure */
////////////////////////////////////////////////////////////////////////////////
__s32  CSI_configure(__csi_conf_t *mode)
{
    W(CSI_REG_CONF, mode->input_fmt <<20 | /* [21:20] */
                mode->output_fmt<<16 | /* [18:16] */
                mode->field_sel <<10 | /* [11:10] */
                mode->seq       <<8  | /* [9:8] */
                mode->vref      <<2  | /* [2] */
                mode->href      <<1  | /* [1] */
                mode->clock     <<0    /* [0] */
                );
    return EPDK_OK;
}

/* buffer */
////////////////////////////////////////////////////////////////////////////////
__s32  CSI_set_buffer_address(__csi_buf_t buf, __u32 addr)
{
    W(CSI_REG_BUF_0_A + (buf<<2), addr); /* bufer0a +4 = buffer0b, bufer0a +8 = buffer1a, ...  */
    return EPDK_OK;
}

__u32  CSI_get_buffer_address(__csi_buf_t buf)
{
    __u32 t;
    t = R(CSI_REG_BUF_0_A + (buf<<2));
    return t;
}

__s32  CSI_double_buffer_enable(void)
{
    S(CSI_REG_BUF_CTRL, 0X1<<0);

    return EPDK_OK;
}

__s32  CSI_double_buffer_disable(void)
{
    C(CSI_REG_BUF_CTRL, 0X1<<0);

    return EPDK_OK;
}

__s32  CSI_double_buffer_select_next(__csi_double_buf_t type)
{
    if (CSI_BUF_A == type)
        C(CSI_REG_BUF_CTRL, 0x1<<2);/* [2] */
    else
        S(CSI_REG_BUF_CTRL, 0x1<<2);
    return EPDK_OK;
}

__s32  CSI_double_buffer_get_status(__csi_double_buf_status_t * status)
{
    __u32 t;
    t = R(CSI_REG_BUF_CTRL);
    status->enable  = t&0x1;
    status->current = (__csi_double_buf_t)(t&(0x1<<1));
    status->next    = (__csi_double_buf_t)(t&(0x1<<2));

    return EPDK_OK;
}

/* capture */
////////////////////////////////////////////////////////////////////////////////
__s32  CSI_capture_video_start(void)
{
    S(CSI_REG_CTRL, 0X1<<1); /* [1] */

    return EPDK_OK;
}

__s32  CSI_capture_video_stop(void)
{
    C(CSI_REG_CTRL, 0X1<<1); /* [1] */

    return EPDK_OK;
}

__s32  CSI_capture_picture(void)
{
    S(CSI_REG_CTRL, 0X1<<0); /* [0] */

    return EPDK_OK;
}

__u32  CSI_capture_get_status(__csi_capture_status * status)
{
    __u32 t;
    t = R(CSI_REG_STATUS);
    status->picture_in_progress = t&0x1;
    status->video_in_progress = (t>>1)&0x1;

    return EPDK_OK;
}

/* size */
////////////////////////////////////////////////////////////////////////////////
__s32  CSI_set_size(__u32 start_h, __u32 length_h, __u32 start_v, __u32 length_v, __u32 buf_length_h)
{
       /* make sure yuv422 input 2 byte(clock) output 1 pixel */
    W(CSI_REG_RESIZE_H, (length_h<<16) + start_h);
    W(CSI_REG_RESIZE_V, (length_v<<16) + start_v);
    W(CSI_REG_BUF_LENGTH, buf_length_h);

    return EPDK_OK;
}

/* scale down */
////////////////////////////////////////////////////////////////////////////////
__s32  CSI_scale_down(__csi_scale_down_t scale_h, __csi_scale_down_t scale_v)
{
    __u32 t=0;

    if (scale_h == CSI_ORIGIN)
        t = 0xf<<24;
    if (scale_h == CSI_HALF)
        t = 0x5<<24;
    if (scale_h == CSI_QUARTER)
        t = 0x1<<24;

    if (scale_v == CSI_ORIGIN)
        t += 0xffff;
    if (scale_v == CSI_HALF)
        t += 0x0f0f;
    if (scale_v == CSI_QUARTER)
        t += 0x000f;

    W(CSI_REG_SCALE, t);

    return EPDK_OK;
}

/* interrupt */
////////////////////////////////////////////////////////////////////////////////
__s32  CSI_int_enable(__csi_int_t interrupt)
{
    S(CSI_REG_INT_EN, interrupt);

    return EPDK_OK;
}

__s32  CSI_int_disable(__csi_int_t interrupt)
{
    C(CSI_REG_INT_EN, interrupt);

    return EPDK_OK;
}

__s32  CSI_int_get_status(__csi_int_status_t * status)
{
    __u32 t;
    t = R(CSI_REG_INT_STATUS);
    status->capture_done     = t&CSI_INT_CAPTURE_DONE     ;
    status->frame_done       = t&CSI_INT_FRAME_DONE       ;
    status->buf_0_overflow   = t&CSI_INT_BUF_0_OVERFLOW   ;
    status->buf_1_overflow   = t&CSI_INT_BUF_1_OVERFLOW   ;
    status->buf_2_overflow   = t&CSI_INT_BUF_2_OVERFLOW   ;
    status->protection_error = t&CSI_INT_PROTECTION_ERROR ;
    status->hblank_overflow  = t&CSI_INT_HBLANK_OVERFLOW  ;
    status->vsync_trig           = t&CSI_INT_VSYNC_TRIG             ;

    return EPDK_OK;
}


__s32  CSI_int_clear_status(void)
{
    W(CSI_REG_INT_STATUS, 0xffffffff);

    return EPDK_OK;
}
/*
CSI中断
*/
int buff_n=0;//缓存计数，用于切换缓存，
unsigned char buff_y[1][CSI_W*CSI_H] __attribute__((aligned(32)));
unsigned char buff_c[1][CSI_W*CSI_H] __attribute__((aligned(32)));
extern int SAVE_BMP;//保存图像标志 1=开始保存;

u32 IRQ_SCI_ISR(ptu32_t intline)
{
    static int sn=0;
    static int CSItime=0,Stime=0;
    int inx=0;
    __csi_int_status_t status;
    status.capture_done=FALSEI;
    status.frame_done=TRUEI;
    status.buf_0_overflow=FALSEI;
    status.buf_1_overflow=FALSEI;
    status.buf_2_overflow=FALSEI;
    status.protection_error=FALSEI;
    status.hblank_overflow=FALSEI;
    status.vsync_trig=FALSEI;
    if(CSI_int_get_status(&status)==EPDK_OK)
    {
        u8 n,m;
        n = buff_y[0][0];
        m = buff_c[0][0];
        CSI_int_clear_status();

        //切换摄像头缓存;
//        buff_n++;
//        if(buff_n>=3)buff_n=0;
//        CSI_set_buffer_address(CSI_BUF_0_A,(u32)(buff_y[buff_n]));
//        CSI_set_buffer_address(CSI_BUF_1_A,(u32)(buff_c[buff_n]));
//
//        //切换显示缓存;
//        inx=buff_n+1;
//        if(inx>=3)inx=0;
//        Defe_conversion_buff(inx);
        DE_SCAL_Set_Reg_Rdy(0);//寄存器已设置好
        //大于2帧开启视频通道
        sn++;
        if(sn==3)
        {
            Enable_Layer_Video();
            sn = 3;
        }
        //截图
//    if(SAVE_BMP==1)
//        {
//            CSI_capture_video_stop();//停止采集
//            SAVE_BMP=0;
//        }
        //显示帧率
//      if((Read_time_ms()-Stime)>1000)
//      {
//          sysprintf("CSIin Fram %d fps\r\n",1000/(Read_time_ms()-CSItime));
//          Stime=Read_time_ms();
//      }
//      CSItime=Read_time_ms();
    }
}
u32 t[15];
u32 CSI_CapturePhoto(u8 *filebuf)
{
    u32 i,j=0,k,result = 0;
//    Defe_Stop();
    CSI_capture_video_stop();   //停止视频采集
    ov2640_jpeg_mode();         //切到jpeg模式
    CSI_capture_video_start();  //启动视频采集
    Int_AsynSignalSync(CN_INT_LINE_CSI);    //等待帧中断

    for (k = 0; k < 15; k++)     //寻找jpeg头
    {
        t[k] = (u32)DJY_GetSysTime();
        if ((buff_y[0][0] == 0xff) && (buff_c[0][0] == 0xd8))
        {
            CSI_capture_video_stop();   //停止视频采集
            filebuf[0] = 0xff;
            filebuf[1] = 0xd8;
            j = 1;
            break;
        }
        else
            Int_AsynSignalSync(CN_INT_LINE_CSI);    //等待帧中断

    }
    CSI_capture_video_stop();   //停止视频采集
    for (i = 1; i < CSI_W*CSI_H; i++)     //寻找jpeg头
    {
        if ((buff_y[0][0] == 0xff) && (buff_c[0][0] == 0xd8))
        {
            result = 0;
        }
        filebuf[2*j] = buff_y[0][i];
        filebuf[2*j+1] = buff_c[0][i];
        j++;
        if ((buff_y[0][i] == 0xff) && (buff_c[0][i] == 0xd9))
        {
            result = 2*j;
            break;
        }
    }
//    OV2640_Config();
    ov2640_yuv422_mode();
    CSI_capture_video_start();  //启动视频采集
    Int_AsynSignalSync(CN_INT_LINE_CSI);    //等待帧中断
    CSI_capture_video_stop();   //停止视频采集
//    Defe_Start();
    return result;
}
/*
CSI时钟初始化
*/
void CSI_CLK_Init(void)
{
#define CSI_CLK_REG (0x01C20000+0x134)
#define DRAM_GATING_REG (0x01C20000+0x100)
    Open_Dev_Clock(CSI_CLOCK);
    S_BIT(DRAM_GATING_REG,1);//Gating DRAM SCLK(1X) For CSI
    //配置时钟
    write32(CSI_CLK_REG,(5<<8));//0: PLL_VIDEO(1X) 5: OSC24M
    write32(CSI_CLK_REG,read32(CSI_CLK_REG)|(0<<0));//CSI_MCLK_DIV_M=1-16(0-15)
    S_BIT(CSI_CLK_REG,15);//Clock is ON
}
/*
CSI配置
*/
void CSI_Config(void)
{
    int csi_width  = CSI_W;
    int csi_height = CSI_H;
    __csi_conf_t mode;
    mode.input_fmt=CSI_YUV422;//输入格式
    mode.output_fmt=CSI_UV_CB_YUV422;//输出格式
    mode.field_sel = CSI_ODD;
    mode.seq=CSI_YUYV;
    mode.vref=CSI_LOW;//V极性
    mode.href=CSI_HIGH;//H极性
    mode.clock=CSI_RISING;//CLK极性
  //设置地址
    CSI_set_buffer_address(CSI_BUF_0_A,(u32)(buff_y[0]));
    CSI_set_buffer_address(CSI_BUF_1_A,(u32)(buff_c[0]));
//  CSI_set_buffer_address(CSI_BUF_2_A,(u32)(CSI_BUF_2));
    //设置图像大小
    CSI_set_size(0, csi_width*2, 0, csi_height, csi_width);
    //缩放
    CSI_scale_down(CSI_ORIGIN, CSI_ORIGIN);//原点
  //开启中断
    CSI_int_enable(CSI_INT_FRAME_DONE);//帧完成中断
//  IRQ_Init(IRQ_LEVEL_1,IRQ_CSI,IRQ_SCI_ISR ,3);
//  sysSetLocalInterrupt(ENABLE_IRQ);//开IRQ中断
    Int_Register(CN_INT_LINE_CSI);
    Int_SetClearType(CN_INT_LINE_CSI,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(CN_INT_LINE_CSI,IRQ_SCI_ISR);
    Int_SettoAsynSignal(CN_INT_LINE_CSI);
    Int_ClearLine(CN_INT_LINE_CSI);
    Int_RestoreAsynLine(CN_INT_LINE_CSI);
    CSI_configure(&mode);
}
/*
CSI IO初始化
*/
void CSI_GPIO_Init(void)
{
    int i=0;
    for(i=0;i<11;i++)
        GPIO_Congif(GPIOE,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
}
/*
CSI初始化
 monitorW,monitorH,监控窗口的宽和高
*/
void CSI_Init(s32 monitorW,s32 monitorH)
{
    int i;
    sysprintf("CSI_Init...\r\n");
    Defe_Stop();
    CSI_CLK_Init();
    CSI_set_base(0);
    CSI_GPIO_Init();
    CSI_open();
    CSI_Config();
    CSI_capture_video_start();
//缩放
    Defe_Init();
    Defe_Config(0,monitorW,monitorH);
    Defe_Start();
//  while(1)
//  {
//
//  }
}


#ifndef __SYS_LCD_H__
#define __SYS_LCD_H__

#define TCON_Base_Address           (u32)0x01c0c000
#define CCU_Base_Address            (u32)0x01C20000
#define CCU_TCON_CLK_REG            (u32)CCU_Base_Address+0x0118
#define CCU_BUS_SOFT_RST_REG0       (u32)CCU_Base_Address+0x02C0
#define CCU_BUS_SOFT_RST_REG1       (u32)CCU_Base_Address+0x02C4
#define CCU_BUS_CLK_GATING_REG0     (u32)CCU_Base_Address+0x0060
#define CCU_BUS_CLK_GATING_REG1     (u32)CCU_Base_Address+0x0064
#define DMA_Base_Address            (u32)0x01C02000
#define DEBE_Base_Address           (u32)0x01E60000
#define DEFE_Base_Address           (u32)0x01E00000

struct __lcd_timing
{
    int mode;//LCD模式 0=cpu 1-rgb565 2=rgb666 3=rgb888 4=rgb8位串行 5=ccir656 7=TV_OUT
    int width;
    int h_front_porch;
    int h_back_porch;
    int h_sync_len;
    int height;
    int v_front_porch;
    int v_back_porch;
    int v_sync_len;
    int h_sync_active;
    int v_sync_active;
    int den_active;
    int clk_active;
    /*1=并行,2=2串行,3=3串行*/
    int serial;
      //0: 18bit/256K mode
        //1: 16bit mode0
        //2: 16bit mode1
        //3: 16bit mode2
        //4: 16bit mode3
        //5: 9bit mode
        //6: 8bit 256K mode
        //7: 8bit 65K mode
    int cpu_mode;
    int n;
    int m;
    int f;
};

struct fb_f1c100s_pdata_t
{
    virtual_addr_t virtdefe;
    virtual_addr_t virtdebe;
    virtual_addr_t virttcon;
    int width;
    int height;
    int frm_pixel;
    void * vram[2];
};

extern struct fb_f1c100s_pdata_t * lcd_pdat;

void TCON_ISR(void);
void f1c100s_lcd_set_addr(void);
void Sys_LCD_Init(int width,int height,unsigned int *buff1,unsigned int *buff2);

void f1c100s_debe_set_address_x(int layer,void * vram);
void f1c100s_debe_set_address(void * vram);

unsigned int f1c100s_debe_get_address_x(int layer);
unsigned int f1c100s_debe_get_address(void);

void lcd_wait_vertical_blanking_interrupt(void);
void f1c100s_tcon_enable(void);
void f1c100s_tcon_disable(void);
//------------------------------------------------------------
void Tcon_Clk_Close(void);
void Tcon_Clk_Open(void);
void Debe_Clk_Open(void);
void Debe_Clk_Close(void);
//------------------------------------------------------------
void Set_LayerX_window(int Layer,int x,int y,int w,int h);
void Enable_LayerX_Video(int Layer);
void Disable_LayerX_Video(int Layer);
//------------------------------------------------------------
void set_video_layer_inx(int layer_inx);
void set_video_window(int x,int y,int w,int h);
void Enable_Layer_Video(void);
void Disable_Layer_Video(void);
void debe_set_layer_priority(int layer,int Priority);

//------------------------------------------------------------
void Set_Layer_format(int Layer,int format,int stride);

#endif


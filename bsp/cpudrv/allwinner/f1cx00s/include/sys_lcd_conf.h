#ifndef __SYS_LCD_CONF_H__
#define __SYS_LCD_CONF_H__


/***************显示器配置***************/
//---LCD
//    #define LCD_TYPE_RGB43_480_272
  #define LCD_TYPE_RGB43_800_480
//  #define LCD_TYPE_RGB70_1024_600
//  #define LCD_TYPE_MCU_320_240
//---VGA
//  #define LCD_TYPE_Vga_640_480_60HZ
//  #define LCD_TYPE_Vga_640_480_75HZ
//  #define LCD_TYPE_Vga_1024_768_60HZ
//  #define LCD_TYPE_Vga_1280_720_60HZ
//  #define LCD_TYPE_Vga_1360_768_60HZ
//---TV (TV输出时LCD与VGA不能输出)
//  #define LCD_TYPE_TV_NTSC_720_480
//  #define LCD_TYPE_TV_PAL_720_576

//-------------------------------------------
#ifdef  LCD_TYPE_RGB43_480_272
    #define XSIZE_PHYS 480
    #define YSIZE_PHYS 272
#endif
//-------------------------------------------
#ifdef LCD_TYPE_RGB43_800_480
    #define XSIZE_PHYS 800
    #define YSIZE_PHYS 480
#endif
//-------------------------------------------
#ifdef LCD_TYPE_RGB70_1024_600
    #define XSIZE_PHYS 1024
    #define YSIZE_PHYS 600
#endif
//-------------------------------------------
#ifdef LCD_TYPE_MCU_320_240
    #define XSIZE_PHYS 320
    #define YSIZE_PHYS 240
#endif
//-------------------------------------------
#ifdef LCD_TYPE_Vga_1024_768_60HZ
    #define XSIZE_PHYS 1024
    #define YSIZE_PHYS 768
#endif
//-------------------------------------------
#ifdef LCD_TYPE_Vga_640_480_60HZ
    #define XSIZE_PHYS 640
    #define YSIZE_PHYS 480
#endif
//-------------------------------------------
#ifdef LCD_TYPE_Vga_640_480_75HZ
    #define XSIZE_PHYS 640
    #define YSIZE_PHYS 480
#endif
//-------------------------------------------
#ifdef LCD_TYPE_Vga_1024_768_60HZ
    #define XSIZE_PHYS 1024
    #define YSIZE_PHYS 768
#endif
//-------------------------------------------
#ifdef LCD_TYPE_Vga_1280_720_60HZ
    #define XSIZE_PHYS 1280
    #define YSIZE_PHYS 720
#endif
//-------------------------------------------
#ifdef LCD_TYPE_Vga_1360_768_60HZ
    #define XSIZE_PHYS 1360
    #define YSIZE_PHYS 768
#endif
//-------------------------------------------
#ifdef LCD_TYPE_TV_NTSC_720_480
    #define XSIZE_PHYS 720
    #define YSIZE_PHYS 480
#endif
//-------------------------------------------
#ifdef LCD_TYPE_TV_PAL_720_576
    #define XSIZE_PHYS 720
    #define YSIZE_PHYS 576
#endif
//-------------------------------------------

#endif

//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:键盘
//作者：lst
//版本：V1.0.0
//文件描述: 扫描硬件读按键函数，只需直接给出按键当前值，无须防抖。
//其他说明:
//修订历史:
//2. 日期: 2009-05-29
//   作者: lst
//   新版本号: V1.1.0
//   修改说明: 增加key_hard_init函数
//1. 日期: 2009-03-10
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include <windows.h>
#include <tchar.h>
#include "stdint.h"
#include "Lcd_Touch_Key.h"
#include "cpu_peri.h"
#include "string.h"
#include "keyboard.h"
#include "cpu.h"
#include "cpu_peri_lcd_touch_mouse.h"
//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中

//    ModuleInstall_KeyBoardWindows(0);

//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"key board driver"//Windows模拟键盘驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:none                //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_KEY_BOARD_DRIVER == false )
//#warning  " key_board_driver  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_KEY_BOARD_DRIVER    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,32,512,

//%$#@enum,true,false,
//%$#@string,1,30,
#define CFG_KEY_BOARD_NAME      "KEY_BOARD"//"键盘名称",
#define CFG_KEY_HEAP_NAME        "sys"      //"键盘使用的堆名称",
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@num,32,512,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

TCHAR szKeyDriver[]  =_TEXT("仿真键盘");    // 标题栏文本
LRESULT CALLBACK    WndProc_key(HWND, UINT, WPARAM, LPARAM);

HDC     tg_key_hdc ;
static bool_t key_flag = false;
static u8 u8g_key_value;
HWND tg_keyboard_wnd;
extern RECT tg_WindowRect;
extern s32 s32g_window_height,s32g_window_width;

//----初始化键盘硬件-----------------------------------------------------------
//功能: 如题，初始化后才能扫描键盘
//参数: 无
//返回: 无
//----------------------------------------------------------------------------
bool_t key_hard_init(void)
{
    RECT WindowRect;
    RECT ClientRect;

    RegisterWinClass(hInstance,WndProc_key,szKeyDriver);
    tg_keyboard_wnd = CreateWindow(szKeyDriver, szKeyDriver, WS_CAPTION | WS_MINIMIZEBOX,
                       CW_USEDEFAULT,CW_USEDEFAULT, 0, 0,
                       NULL, NULL, hInstance, NULL);

    if (!tg_keyboard_wnd)
    {
        return FALSE;
    }
    GetWindowRect(tg_keyboard_wnd, &WindowRect);
    GetClientRect(tg_keyboard_wnd, &ClientRect);
    WindowRect.right += (cn_key_board_width - ClientRect.right);
    WindowRect.bottom += (cn_key_board_height - ClientRect.bottom);
    MoveWindow(tg_keyboard_wnd, WindowRect.left, WindowRect.top, cn_key_board_width, cn_key_board_height, false);


    ShowWindow(tg_keyboard_wnd, SW_SHOW);
    UpdateWindow(tg_keyboard_wnd);
    tg_key_hdc = GetDC (tg_keyboard_wnd) ;
    return true;
}

LRESULT CALLBACK WndProc_key(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static u32 move_count = 0;
    switch (message)
    {
        case WM_CREATE:
            CreateWindow(_TEXT("button"),_TEXT("F1"),
                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                            20,40,40,40,
                            hWnd,(HMENU)VK_F1,
                            GetWindowLong(hWnd,GWL_HINSTANCE),NULL);
            CreateWindow(_TEXT("button"),_TEXT("F2"),
                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                            20,160,40,40,
                            hWnd,(HMENU)VK_F2,
                            GetWindowLong(hWnd,GWL_HINSTANCE),NULL);
            CreateWindow(_TEXT("button"),_TEXT("∧"),
                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                            140,40,40,40,
                            hWnd,(HMENU)VK_UP,
                            GetWindowLong(hWnd,GWL_HINSTANCE),NULL);
            CreateWindow(_TEXT("button"),_TEXT("∨"),
                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                            140,160,40,40,
                            hWnd,(HMENU)VK_DOWN,
                            GetWindowLong(hWnd,GWL_HINSTANCE),NULL);
            CreateWindow(_TEXT("button"),_TEXT("＜"),
                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                            80,110,40,40,
                            hWnd,(HMENU)VK_LEFT,
                            GetWindowLong(hWnd,GWL_HINSTANCE),NULL);
            CreateWindow(_TEXT("button"),_TEXT("＞"),
                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                            200,110,40,40,
                            hWnd,(HMENU)VK_RIGHT,
                            GetWindowLong(hWnd,GWL_HINSTANCE),NULL);
            CreateWindow(_TEXT("button"),_TEXT("返回"),
                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                            260,40,40,40,
                            hWnd,(HMENU)'\x1b',
                            GetWindowLong(hWnd,GWL_HINSTANCE),NULL);
            CreateWindow(_TEXT("button"),_TEXT("确认"),
                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                            260,160,40,40,
                            hWnd,(HMENU)'\r',
                           ((LPCREATESTRUCT) lParam)->hInstance,NULL);
            break;
        case WM_KILLFOCUS:
            if(hWnd == GetParent((HWND)wParam))
            {
                u8g_key_value = (u8)GetWindowLong((HWND)wParam,GWL_ID);
                key_flag = true;
            }
            break;
        case WM_COMMAND:
            key_flag = false;
            SetFocus(hWnd);
            break;
        case WM_KEYDOWN:
            switch(wParam)
            {
                case VK_F1:
                case VK_F2:
                case VK_LEFT:
                case VK_UP:
                case VK_RIGHT:
                case VK_DOWN:
                    u8g_key_value = (u8)wParam;
                    key_flag = true;
                    break;
                default:
                    break;
            }
            break ;
        case WM_KEYUP:
            key_flag = false;
            break ;
        case WM_CHAR:
            switch(wParam)
            {
                case '\r':          //确认
                case '\x1b':        //取消
                    key_flag = true;
                    u8g_key_value = (u8)wParam;
                    break;
                default:
                    break;
            }
            break ;
        case WM_MOVE:
            if(move_count < 1)
            {
                move_count++;
                break;
            }
            move_count = 3;
            MoveWindow(tg_keyboard_wnd, tg_WindowRect.left+s32g_window_width +5,
                       tg_WindowRect.top, cn_key_board_width, cn_key_board_height,
                       true);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//----键盘硬件扫描------------------------------------------------------------
//功能: 读入模拟键盘上的按键，但不支持复合键
//参数: 无
//返回: 按下的键的键值，没考虑复合键
//----------------------------------------------------------------------------
u32 keyboard_scan(void)
{
    if(key_flag)
    {
        return (u32)u8g_key_value;
    }
    else
        return 0;
}

//----初始化键盘模块-----------------------------------------------------------
//功能: 初始化一个由windows的键盘和按钮模拟的键盘，该键盘供8个键。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_KeyBoardWindows(ptu32_t para)
{
    static struct KeyBoardPrivate windows_keyboard;
    key_hard_init();
    windows_keyboard.read_keyboard = keyboard_scan;
    Keyboard_InstallDevice("sim keyboard",&windows_keyboard);
    windows_keyboard.vtime_limit = para;
    windows_keyboard.key_bak = 0;
    windows_keyboard.key_now = 0;
    return 1;
}



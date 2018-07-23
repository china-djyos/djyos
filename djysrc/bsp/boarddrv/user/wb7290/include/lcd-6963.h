// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：LCD.c
// 模块描述: LCD驱动程序，使用了SMC外设
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 07/03.2016
// =============================================================================

#ifndef __LCD_H__
#define __LCD_H__


void Lidd_Init(void);
void Lcd_WriteDataPort(uint8_t u8Data);
uint8_t Lcd_ReadStatus(void);
void Lcd_WriteCmdPort(uint8_t u8Data);
uint8_t Lcd_WriteData(uint8_t byLcdData);
uint8_t Lcd_ReadData(void);
uint8_t Lcd_WriteCmd(uint8_t u8Data0,uint8_t u8Data1,uint8_t u8Cmd);
uint8_t Lcd_ReadCmd(void);
void Lcd_ContrastSet(uint16_t u16Contrast);
void Lcd_Rst(void);
void Lcd_SetBackLight(uint8_t byCtrl);
void Lcd_ModInit(void);
void Lcd_wrDISPram(uint32_t dwleft,uint32_t dwtop,uint32_t dwsize,uint8_t * pbysrcadd);
void ModuleInstall_LCD(void);
void LCD_Clr(void);
void LCD_WriteByte(uint8_t dat,uint8_t lcd_write_mode);


#endif

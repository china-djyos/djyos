//#include "c8051F340.h"
//#include <main.h>

#ifndef __LCD_h__
#define __LCD_h__

#define COL 320
#define ROW 480

#ifdef DEBUG
	#define DEBUG_X  50
	#define DEBUG_Y  15
	//extern unsigned int code debug_code[X][Y];
	extern unsigned int debug_code[50][15];
	
	#define END 0xffff
#endif

extern unsigned int pic_eval[];
extern unsigned char ascii[];

//---------------------------------------------------------------------
void WriteComm(unsigned char i);
void WriteData(unsigned char i);
void WriteDispData(unsigned char DataH,unsigned char DataL);
void LCD_Init(void);
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);
void DispBand(void);
void DispColor(unsigned char color1,unsigned char color2,unsigned char color3);
void DispFrame(void);
void DispPic(unsigned int  *picture);
void DispPicFromSD(unsigned char PicNum);

void DispScaleHor1(void);
void DispScaleVer(void);
void DispScaleVer_Red(void);
void DispScaleVer_Green(void);
void DispScaleVer_Blue(void);
void DispScaleVer_Gray(void);
void DispGrayHor16(void);
void DispGrayHor32(void);
void DispScaleHor2(void);
void DispSnow(void);
void DispBlock(void);
void StopDelay(unsigned int sdly);

void WriteOneDot(unsigned long int color);
unsigned char ToOrd(unsigned char ch); 
void  DispOneChar(unsigned char ord,unsigned int Xstart,unsigned int Ystart,unsigned int TextColor,unsigned int BackColor);	 // ord:0~95
void DispStr(unsigned char *str,unsigned int Xstart,unsigned int Ystart,unsigned int TextColor,unsigned int BackColor);
void DispInt(unsigned int i,unsigned int Xstart,unsigned int Ystart,unsigned int TextColor,unsigned int BackColor);

unsigned int ReadData(void);
void DispRegValue(unsigned int RegIndex,unsigned char ParNum);

void Debug(void);

void PutPixel(unsigned int x,unsigned int y,unsigned int color);
void DrawLine(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend,unsigned int color);
void DrawGird(unsigned int color);

#endif

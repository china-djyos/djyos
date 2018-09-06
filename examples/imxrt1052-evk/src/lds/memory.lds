
/*由于MEMORY命令不能使用符号，这些常量的定义，必须与MEMORY命令处一致 */
IbootSize = 800k;   /*此处定义应与相应板件的APP的memory.lds中定义一致 */

FlashConfig = 0x60000000;
IvtTable = 0x60001000;
InnerFlashOffset = 0x60002000;
InnerFlashRange = 64M;		
InnerRam1Start = 0x20000000;
InnerRam1Size = 128K;
InnerRam2Start = 0x20200000;
InnerRam2Size = 256K;
ItamStart = 0x00000000;
ItcmSize = 128K;
SdramStart = 0x80000000;
SdramSize = 30M;
NoCacheStart = 0x81E00000;
NoCacheSize = 2M;

MEMORY
{
	FlashConfig(RX)	 : org = 0x60000000,		 len = 0x1000
	IvtTable(RX)	 : org = 0x60001000,		 len = 0x1000
    InnerFlash(RX)   : org = 0x60002000,         len = 64M-8K   								 					
    RAM1(RXW)        : org = 0x20000000,         len = 128K	  
    RAM2(RXW)        : org = 0x20200000,         len = 256K
    ITCM(RXW)		 : org = 0X00000000,		 len = 128K
    SDRAM(RXW)		 : org = 0x80000000,		 len = 30M
    NOCACHE(RXW)	 : org = 0x81E00000,		 len = 2M
}

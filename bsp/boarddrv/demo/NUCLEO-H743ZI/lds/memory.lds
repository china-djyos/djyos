
/*由于MEMORY命令不能使用符号，这些常量的定义，必须与MEMORY命令处一致 */
IbootSize = 256k;   /*此处定义应与相应板件的APP的memory.lds中定义一致 */
InnerFlashOffset = 0x08000000;
InnerFlashRange = 2048k;

InnerRam1Start = 0x00000000;
InnerRam1Size = 64k;

InnerRam2Start = 0x20000000;
InnerRam2Size = 128k;

InnerRam3Start = 0x24000000;
InnerRam3Size = 512K - 32k;

InnerRam4Start = 0x30000000;
InnerRam4Size = 128k+128k;

InnerRam5Start = 0x38000000;
InnerRam5Size  =  64k;
 
CacheRam1Size    = 64k;        /*第一段cache大小*/
CacheRam2Size    = 128K;
CacheRam3Size    = 512K;
CacheRam4Size    = 256K;

NoCacheRamStart  = 0x30040000;     /*nocache地址*/
NoCacheRamSize   = 32k;            /*nocache区大小*/

RevSectors = 0;                                 /*保留块数，保留位于flash末尾块*/

MEMORY
{
    InnerFlash(RX)   : org = 0x08000000,           len = 1024k   
    RAM1(RXW)        : org = 0x00000000,           len = 64K
    RAM2(RXW)        : org = 0x20000000,           len = 128K
    RAM3(RXW)        : org = 0x24000000,           len = 512K-32K
    RAM4(RXW)        : org = 0x30000000,           len = 128K+128k
    RAM5(RXW)        : org = 0x38000000,           len = 64k
    RAM1NOCACHE(RXW) : org = 0x30040000,           len = 32k 
}
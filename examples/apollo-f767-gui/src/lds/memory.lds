
/*由于MEMORY命令不能使用符号，这些常量的定义，必须与MEMORY命令处一致 */
IbootSize = 256k;   /*此处定义应与相应板件的APP的memory.lds中定义一致 */
InnerFlashOffset = 0x08000000;
InnerFlashRange = 1024k;
InnerRamStart = 0x20000000;
InnerRamSize = 512K - 32k;
ExtRamStart = 0xc0000000;
ExtRamSize  = 0x2000000;/*32M*/
 
CacheRam1Size    = 256k;        /*第一段cache大小*/
CacheRam2Size    = 128K;
CacheRam3Size    = 64K;
CacheRam4Size    = 32K;
NoCacheRamStart  = 0x20078000;    /*0x20078000 = 0x20000000+512K - 32k*/
NoCacheRamSize   = 32k;           /*nocache区大小*/

RevSectors = 0;                                 /*保留块数，保留位于flash末尾块*/

MEMORY
{
    InnerFlash(RX)   : org = 0x08000000,         len = 1024k   
    RAM1(RXW)        : org = 0x20000000,         len = 512K-32K
    RAM1NOCACHE(RXW) : org = 0x20000000+256k+128k+64K+32K,len = 32k 
    extram(RXW)      : org = 0xc0000000,         len = 0x2000000 
}
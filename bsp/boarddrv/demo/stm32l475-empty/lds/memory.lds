/*由于MEMORY命令不能使用符号，这些常量的定义，必须与MEMORY命令处一致 */

IbootSize = 256k;   /*此处定义应与相应板件的APP的memory.lds中定义一致 */
InnerFlashOffset = 0x8000000;
InnerFlashRange = 512k;
InnerRam1Start = 0x20000000;
InnerRam1Size = 256k;

InnerRam2Start = 0x10000000;
InnerRam2Size = 64k;

MEMORY
{
    InnerFlash(RX)    : org = 0x08000000,               len = 512k  
    RAM1(RXW)         : org = 0x20000000,               len = 256k 
    RAM2(RXW)         : org = 0x10000000,               len = 64k
}
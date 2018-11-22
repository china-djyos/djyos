/*由于MEMORY命令不能使用符号，这些常量的定义，必须与MEMORY命令处一致 */

IbootSize = 250k;   /*此处定义应与相应板件的APP的memory.lds中定义一致 */
InnerFlashOffset = 0x8000000;
InnerFlashRange = 256k;
InnerRam1Start = 0x20000000;
InnerRam1Size = 48k;

InnerRam2Start = 0x10000000;
InnerRam2Size = 16k;

MEMORY
{
    InnerFlash(RX)    : org = 0x08000000,               len = 256k  
    RAM1(RXW)         : org = 0x20000000,               len = 48k 
    RAM2(RXW)         : org = 0x10000000,               len = 16k
}
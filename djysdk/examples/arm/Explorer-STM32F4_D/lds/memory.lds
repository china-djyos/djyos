
/*由于MEMORY命令不能使用符号，这些常量的定义，必须与MEMORY命令处一致 */

InnerFlashOffset = 0x8000000;
InnerFlashRange = 1024k;
InnerRam1Start = 0x20000000;
InnerRam1Size = 128k;
InnerRam2Start = 0x10000000;
InnerRam2Size = 64k;
ExtRamStart = 0x68000000;
ExtRamSize  = 1024k;
MEMORY
{
    InnerFlash(RX)    : org = 0x08000000,               len = 1024k
    
    RAM1(RXW)       : org = 0x20000000,                 len = 128k
    RAM2(RXW)       : org = 0x10000000,                 len = 64k
    extram(RXW)     : org = 0x68000000,                 len = 1024k
}

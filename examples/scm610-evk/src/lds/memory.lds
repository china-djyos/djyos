
/*由于MEMORY命令不能使用符号，这些常量的定义，必须与MEMORY命令处一致 */


IbootSize = 256k;   /*此处定义应与相应板件的APP的memory.lds中定义一致 */
InnerFlashOffset = 0x00000000;
InnerFlashRange = 1024k;
InnerRam1Start = 0x20000000;
InnerRam1Size = 128k;

InnerRam2Start = 0x20000000 + 128*1024;
InnerRam2Size = 128k;

MEMORY
{
    InnerFlash(RX)    : org = 0x00000000,               len = 1024k
    RAM1(RXW)         : org = 0x20000000,               len = 128k
    RAM2(RXW)         : org = 0x20020000,               len = 128k

}

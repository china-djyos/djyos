/*由于MEMORY命令不能使用符号，这些常量的定义，必须与MEMORY命令处一致 */

InnerFlashOffset = 0x08000000;
InnerFlashRange = 512k;
InnerRamStart = 0x20000000;
InnerRamSize = 128k;

MEMORY
{
    InnerFlash(RX)   : org = 0x08000000,           len = 512k
    RAM1(RXW)        : org = 0x20000000,           len = 128k
}
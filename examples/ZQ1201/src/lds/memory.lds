/*由于MEMORY命令不能使用符号，这些常量的定义，必须与MEMORY命令处一致 */

IbootSize = 256k;   /*此处定义应与相应板件的APP的memory.lds中定义一致 */
FlashOffset = 0x00000000;
FlashRange = 512k;
Ram1Start = 0x01000000;
Ram1Size = 0x4000;

ExtRamStart = 0x20000000; 
ExtRamSize = 0x8000;

/*0x02000000为SDRAM的起始地址，DSP程序存放在此处*/
SdRamStart = 0x02000000;
SdRamSize  = 0x00400000;  /*8M*/
MEMORY
{
    Flash(RX)    : org = 0x00000000,           len = 512k       
    RAM1(RXW)    : org = 0x01000000,           len = 16k  /*0x01004000*/
    ExtRam(RXW)  : org = 0x20000000,           len = 32k  /*0x20008000*/
    SdRam(RXW)   : org = 0x02000000,           len = 4096k/*0x02400000*/
}
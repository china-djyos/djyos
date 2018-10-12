/*由于MEMORY命令不能使用符号，这些常量的定义，必须与MEMORY命令处一致 */

IbootSize = 256k;   /*此处定义应与相应板件的APP的memory.lds中定义一致 */
FlashOffset = 0x00000410;
FlashRange = 512k;
Ram1Start = 0x01000000;    /*Ram1 start*/
Ram1Size = 0x4000;         /*16k*/

ExtRamStart = 0x20000000; 
ExtRamSize = 0x8000;


/*0x02000000为SDRAM的起始地址，DSP程序存放在此处*/
/*SdRamStart = 0x02000000;*/
/*SdRamSize  = 0x160100;*/ /*1048k*/
SdRamStart = 0x02000000 + 0x80000;
SdRamSize  = 0x86000;   /*1048-512 = 536k*/

MEMORY
{
    VerctorFlash(RX)       : org = 0x00000000,             len = 1024+16 /*就放M0核 的中断向量表以及ck核的复位地址*/
    M0BootRiscFlash(RXW)   : org = 0x00000400,             len = 4       /*用于存储risc 启动地址*/
    Flash(RX)              : org = 0x00000410,             len = 512k       
    RAM1(RXW)              : org = 0x01000000,             len = 16k  /*0x01000000*/
    ExtRam(RXW)            : org = 0x20000000,             len = 32k  /*0x20008000*/
    SdRam(RXW)             : org = 0x02000000 + + 0x80000, len = 4096k /*0x02000000*/
 
}

/*由于MEMORY命令不能使用符号，这些常量的定义，必须与MEMORY命令处一致 */

IbootSize = 250k;   /*此处定义应与相应板件的APP的memory.lds中定义一致 */
InnerFlashOffset = 0x8000000;
InnerFlashRange = 256k;
InnerRamStart = 0x20000000;
InnerRamSize = 40k;
ExtRamStart = 0x10000000;
ExtRamSize  = 0x2000;

RevSectors = 0;                                 /*保留块数，保留位于flash末尾块*/

MEMORY
{
    InnerFlash(RX)    : org = 0x08000000,               len = 256k
    
    RAM1(RXW)       : org = 0x20000000,                 len = 40k
    extram(RXW)     : org = 0x10000000,                 len = 0x2000
}


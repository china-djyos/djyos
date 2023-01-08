IbootSize = 256k;

InnerFlashOffset = 0x8000000;
InnerFlashRange = 2048k;

InnerRam1Start = 0x20000000;
InnerRam1Size = 256k;

InnerRam2Start = 0x20040000;
InnerRam2Size = 64k;

ExtRamStart = 0x20050000;
ExtRamSize  = 64k;

MEMORY
{
    InnerFlash(RX)    : org = 0x08000000,               len = 2048k
    
    RAM1(RXW)       : org = 0x20000000,                 len = 256k
    RAM2(RXW)       : org = 0x20040000,                 len = 64k
    extram(RXW)     : org = 0x20050000,                 len = 64k
}


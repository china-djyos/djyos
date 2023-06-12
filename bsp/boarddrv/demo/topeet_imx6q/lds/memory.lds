
MEMORY
{
    OCRAM(RXW)  :  ORIGIN = 0x00900000, LENGTH = 256k

    InnerFlash(RXW) : ORIGIN = 0x10200000, LENGTH = 0x100000
    RAM1(RXW)       : ORIGIN = 0x10300000, LENGTH = 0x100000
}

IbootSize = 256k;

InnerRam1Start   = ORIGIN(RAM1);
InnerRam1Size    = LENGTH(RAM1);

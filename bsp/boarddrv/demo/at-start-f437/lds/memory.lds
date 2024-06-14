IbootSize = 256k;

InnerFlashOffset = 0x8000000;
InnerFlashRange = 1024k;

InnerRam1Start = 0x20000000;
InnerRam1Size = 384k;

SnSize = 512;

MEMORY
{
    InnerFlash(RX)      : org = 0x08000000,                             len = 1024k
    flash_iboot(rx)     : org = 0x08000000,                             len = IbootSize - SnSize
    flash_sn(rw!x)      : org =  IbootSize - SnSize + 0x08000000,       len = SnSize
    flash_app(rx)       : org = IbootSize + 0x08000000,                 len = 1024k - IbootSize   
    RAM1(RXW)           : org = 0x20000000,                             len = 384k
}


/* TODO: 下面这些常量应该由链接器计算生成 */

IbootSize = 128k;

InnerFlashOffset = 0x8000000;
InnerFlashRange = 128k;

InnerRam1Start = 0x20000000;
InnerRam1Size = 24k;

InnerRam2Start = 0x20006000;
InnerRam2Size = 4k;

ExtRamStart = 0x20007000;
ExtRamSize  = 4k;

MEMORY
{
	InnerFlash(rx): org = 0x8000000 , len = 128k
	RAM1(rxw): org = 0x20000000, len = 24k
	RAM2(rxw): org = 0x20006000, len = 4k
	extram(rxw): org = 0x20007000, len = 4k
}


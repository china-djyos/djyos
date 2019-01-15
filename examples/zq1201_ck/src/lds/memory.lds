
IbootSize = 384K;   /*此处定义应与相应板件的APP的memory.lds中定义一致 */

Flash1Offset = 0x02160000;
Flash2Offset = 0x021e0000;
RamStart = 0x021c0000;
RamSize = 128K;

MEMORY
{
	ROM1_SDRAM(RX) : ORIGIN = 0x02160000 , LENGTH = 0x60000		/*IBOOT ADDR*/
	ROM2_SDRAM(RX) : ORIGIN = 0x021e0100 , LENGTH = 0x80000		/*APP ADDR*/
	RAM_SDRAM(RXW) : ORIGIN = 0x021c0000 , LENGTH = 0x20000   	/* RAM share */
}

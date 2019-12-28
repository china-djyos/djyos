
/*由于MEMORY命令不能使用符号，这些常量的定义，必须与MEMORY命令处一致 */

IbootSize = 512k;   /*此处定义应与相应板件的APP的memory.lds中定义一致 */
/* Split memory into area for vectors and ram */
MEMORY
{
    flash  (rx) : ORIGIN = 0x00000000    ,     LENGTH = 3800k /* 1216KB - 96B */
    
		share (rw!x): ORIGIN = 0x00400000    ,     LENGTH = 256 	/*连续的RAM分出256字节用于APP和Iboot共享*/
    dtcm  (rw!x): ORIGIN = 0x00400000+256,     LENGTH = 32k   /*连续的RAM分出32K做DTCM RAM*/
    ram   (rw!x): ORIGIN = 0x00400000+32k+256, LENGTH = 256k-32k-256
    
    sdram (rw!x): ORIGIN = 0x00900000    ,     LENGTH = 256k
}

 



/*
 * Script for GNU linker.
 * Describes layout of sections, location of stack.
 *
 * In this case vectors are at location 0 (reset @ 0x08)
 *
 * +------------+  0x00400000
 * data         |
 * end
 * |(heap)      |
 * .            .
 * .            .
 * |(heap limit)|
 *
 * |- - - - - - |
 * stack bottom    256k
 * +------------+
 *
 * +------------+  0x0000000
 * |vectors     |
 * |            |
 * |------------+
 * |text        |
 * |data        |
 * |            |  1024k
 * +------------+
 */

/*由于MEMORY命令不能使用符号，这些常量的定义，必须与MEMORY命令处一致 */
IbootSize = 1500k;   /*此处定义应与相应板件的APP的memory.lds中定义一致 */
FlashOffset = 0x00000000;
FlashSize = 2000K;
RamStart = 0x00410000;
RamSize = 192K;
DtcmStart = 0x00400000;
DtcmSize = 64K;
SDRamStart = 0x00900000;
SDRamSize = 256K;

/* Split memory into area for vectors and ram */
MEMORY
{
        flash  (rx) : ORIGIN = 0x00000000, LENGTH = 1500k /* 1216KB - 96B */
        dtcm  (rw!x): ORIGIN = 0x00400000, LENGTH = 64k
        ram   (rw!x): ORIGIN = 0x00410000, LENGTH = 192k
        sdram (rw!x): ORIGIN = 0x00900000, LENGTH = 256k
}

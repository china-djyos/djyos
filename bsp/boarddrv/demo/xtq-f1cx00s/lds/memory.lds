
/*由于MEMORY命令不能使用符号，这些常量的定义，必须与MEMORY命令处一致 */
/*注意，“=”前后必须有空格，否则编译报错*/

IbootSize = 0x80000;

MEMORY
{
   DRAM_iboot  (rwx) : ORIGIN = 0x80000000 ,  LENGTH = IbootSize
   DRAM_app    (rwx) : ORIGIN = 0x80000000+IbootSize,  LENGTH = 0x400000 - IbootSize
   DRAM        (rw)  : ORIGIN = 0x80400000 ,  LENGTH = 0x2000000-0x400000
   SHARE       (rw)  : ORIGIN = 0x00007000 ,  LENGTH = 256   /*256字节用于APP和Iboot共享*/
}

DRAM_iboot_Start   = ORIGIN(DRAM_iboot);
DRAM_iboot_Size    = LENGTH(DRAM_iboot);
DRAM_app_Start     = ORIGIN(DRAM_app);
DRAM_app_Size      = LENGTH(DRAM_app);
DRAM_Start         = ORIGIN(DRAM);
DRAM_Size          = LENGTH(DRAM);
SHARE_Start        = ORIGIN(SHARE);
SHARE_Size       = LENGTH(SHARE);


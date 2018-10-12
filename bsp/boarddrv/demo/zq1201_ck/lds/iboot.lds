ENTRY(Init_Cpu)

/*INCLUDE  memory.ld*/
FlashOffset = 0x02160100;
FlashRange = 512K;
RamStart = 0x021e0100;
RamSize = 127K;

MEMORY
{
	ROM_SDRAM(RX) : ORIGIN = 0x02160000 , LENGTH = 0x80000
	/*ROM_SDRAM(RX) : ORIGIN = 0x02160100 , LENGTH = 0x80000    SDRAM 512KB */
	RAM_SDRAM(RXW) : ORIGIN = 0x021e0100 , LENGTH = 0x1ff00   /* SDRAM 512KB */
}

SECTIONS
{
	stack_rRAM1_lRAM1 (NOLOAD) :
    {
        . = ALIGN(0x02);
        msp_bottom = .;
        KEEP(* (.data.InitStack))
		/*. = ALIGN(0x08);*/
        msp_top = .;
    }>RAM_SDRAM
	
	text_startup_rIbootRom_lIbootRom :
	{
		/*. = ALIGN(10);*/
       /*__VECTOR_TABLE = .;*/
	   /*KEEP(*startup.o(.vectors))*/
	   KEEP(*(.StartupExpTbl .StartupExpTbl.*))
	   . = ALIGN(4);
	   KEEP(*initcpus.o(.text .text.* .rodata .rodata.*))
	   KEEP(*initcpuc.o(.text .text.* .rodata .rodata.*))
	   KEEP(*pre_loader.o(.text .text.* .rodata .rodata.*))
	   . = ALIGN(10);
	}>ROM_SDRAM
	
	text_preload_rIbootRom_lIbootRom :
	{
		. = ALIGN(0x08);
		g_pAppCodeStartRamAddr = .;
        *loader.o (.text .text.* .rodata .rodata.*)
        *int_hard.o (.text .text.* .rodata .rodata.*)
		KEEP(*hard-exps.o (.text .text.* .rodata .rodata.*))
        *hard-exp.o (.text .text.* .rodata .rodata.*)
        *int.o (.text .text.* .rodata .rodata.*)
        *ints.o (.text .text.* .rodata .rodata.*)
        *heap-static.o (.text .text.* .rodata .rodata.*)
        *list.o (.text .text.* .rodata .rodata.*)
		. = ALIGN(0x08);
	}>ROM_SDRAM
	
	text_sysload_rIbootRom_lIbootRom :
    {       
       . = ALIGN(0x08);
        * (.text .text.* .rodata .rodata.* )
        g_pAppCodeEndRamAddr = .;
		. = ALIGN(0x08);
    }>ROM_SDRAM 
	
	rw_preload_rRAM1_lIbootRom :
    {
       . = ALIGN(0x08);
        *loader.o (.data .data.*)
        *int_hard.o (.data .data.*)
        KEEP(*:hard-exps.o (.data .data.*))
        *hard-exp.o (.data .data.*)
        *int.o (.data .data.*)
        *ints.o (.data .data.*)
        *heap-static.o (.data .data.*)
        *list.o (.data .data.*)
		. = ALIGN(0x08);
    }>RAM_SDRAM AT>ROM_SDRAM
	
	zi_preload_rRAM1_lRAM1 (NOLOAD) :
    {
       . = ALIGN(10);
        KEEP(* (.table.exceptions .table.exceptions.*))
        KEEP(* (.table.vectors .table.vectors.*))
        *loader.o (.bss .bss.* COMMON)
        *int_hard.o (.bss .bss.* COMMON)
        KEEP(*hard-exps.o (.bss .bss.* COMMON))
        *hard-exp.o (.bss .bss.* COMMON)
        *int.o (.bss .bss.* COMMON)
        *ints.o (.bss .bss.* COMMON)
        *heap-static.o (.bss .bss.* COMMON)
        *list.o (.bss .bss.* COMMON)
    }>RAM_SDRAM
	
	rw_sysload_rRAM1_lIbootRom :
    {
       . = ALIGN(0x4) ;
        * (.data .data.*)
		. = ALIGN(0x4) ;
    }>RAM_SDRAM AT>ROM_SDRAM

    zi_sysload_rRAM1_lRAM1 (NOLOAD) :
    {
       . = ALIGN(0x04);
        * (.bss .bss.* COMMON)
		. = ALIGN(0x4) ;
    }>RAM_SDRAM
	
	preload_cp_table :
    {
       . = ALIGN(0x08);
        preload_copy_table = .;
        LONG(16)
        LONG(3)

        LONG(LOADADDR(text_preload_rIbootRom_lIbootRom))
        LONG(ADDR(text_preload_rIbootRom_lIbootRom))
        LONG(SIZEOF(text_preload_rIbootRom_lIbootRom))
        LONG(1)

        LONG(LOADADDR(rw_preload_rRAM1_lIbootRom))
        LONG(ADDR(rw_preload_rRAM1_lIbootRom))
        LONG(SIZEOF(rw_preload_rRAM1_lIbootRom))
        LONG(1)

        LONG(LOADADDR(zi_preload_rRAM1_lRAM1))
        LONG(ADDR(zi_preload_rRAM1_lRAM1))
        LONG(SIZEOF(zi_preload_rRAM1_lRAM1))
        LONG(0)
    }>ROM_SDRAM
	
	sysload_cp_table :
    {
       . = ALIGN(0x08);
        sysload_copy_table = .;
        LONG(16)
        LONG(3)

        LONG(LOADADDR(text_sysload_rIbootRom_lIbootRom))
        LONG(ADDR(text_sysload_rIbootRom_lIbootRom))
        LONG(SIZEOF(text_sysload_rIbootRom_lIbootRom))
        LONG(1)

        LONG(LOADADDR(rw_sysload_rRAM1_lIbootRom))
        LONG(ADDR(rw_sysload_rRAM1_lIbootRom))
        LONG(SIZEOF(rw_sysload_rRAM1_lIbootRom))
        LONG(1)
		
        LONG(LOADADDR(zi_sysload_rRAM1_lRAM1))
        LONG(ADDR(zi_sysload_rRAM1_lRAM1))
        LONG(SIZEOF(zi_sysload_rRAM1_lRAM1))
        LONG(0)
    }>ROM_SDRAM
	
	heap_rRAM_lRAM (NOLOAD) :
    {
       . = ALIGN(0x08);
        sys_heap_bottom = .;
		/*__heap_start = .;*/
    }>RAM_SDRAM
    sys_heap_top = RamStart + RamSize;
	/*__heap_end = RamStart + RamSize;*/
	
	heap_cp_table :
    {
        . = ALIGN(0x08);
        pHeapList = .;
        /* 默认堆，必须排首位，名字必须是“sys” */
        LONG(1)                     /*表示该堆由两段内存组成*/
        LONG(0)                     /*该堆上分配的内存的对齐尺寸，0表示使用系统对齐*/
        LONG(0)                     /*0=通用堆,1=专用堆,如果系统只有唯一一个堆,则只能是通用堆*/
        LONG(sys_heap_bottom)       /*第一段基址，须符合对齐要求*/
        LONG(sys_heap_top)          /*第一段地址上限（不含），须符合对齐要求*/
        LONG(32)                    /*第一段页尺寸=32bytes*/
        BYTE(0x73)                  /*'s'*/
        BYTE(0x79)                  /*'y'*/
        BYTE(0x73)                  /*'s'*/
        BYTE(0)                     /*串结束，"sys"是堆的名字*/
        
        . = ALIGN(0x04);
        LONG(0)                     /*结束标志*/
    }>ROM_SDRAM
    
    Shell_Info :
    {
        . = ALIGN(0x08);
        p_shell_info = .;

        LONG(0xffffffff) /* _shell_fun_tab_start 内部函数表的起始地址*/
        LONG(0xffffffff) /*_shell_fun_tab_end  内部函数表的结束地址*/

        LONG(0xffffffff) /*shell_fun_tab_start 普通函数表的起始地址*/
        LONG(0xffffffff) /*shell_fun_tab_end  普通函数表的结束地址*/

        LONG(0xffffffff)  /*shell_var_tab_start 变量表的起始地址*/
        LONG(0xffffffff) /*shell_var_tab_start变量表的结束地址*/
       
        LONG(.)
        pProgRomEnd = .;
    }>ROM_SDRAM 
}

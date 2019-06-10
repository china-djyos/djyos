OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(isr_vector);

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

SECTIONS
{
	Rom_Start :
    {
        restore = .; 
    }>flash

    /*RevRam是Ram保留区域，用于存储preload前使用或保存的内在数据,只有IAP_pload.c*/
    /*的pg_IapVar变量从该段内分配，且该段在iboot.lds和debug/release.lds保持一致*/
    Iboot_RevRam (NOLOAD):
    {
        . = ALIGN(0x08);
        KEEP(*(.IapBootFlgRam .IapBootFlgRam.*))    
    }>ram

	stack_rRAM1_lRAM1 (NOLOAD) :
    {
        . = ALIGN(0x02);
        msp_bottom = .;
        KEEP(* (.data.InitStack))
		/*. = ALIGN(0x08);*/
        msp_top = .;
    }>ram
	
	Rom_Start :
    {
        restore = .; 
    }>flash
    
    /* vectors go to vectors region */
    /*. = ORIGIN(flash);*/
    .vectors :
    {
        KEEP(*(*.vectors))
        KEEP( *(*.rom1))
        . = ALIGN(4);
	   KEEP(*initcpus.o(.text .text.* .rodata .rodata.*))
	   KEEP(*pre_loader.o(.text .text.* .rodata .rodata.*))
	   . = ALIGN(10);
    } > flash

text_preload_rIbootRom_lIbootRom :
	{
		. = ALIGN(0x08);
		g_pAppCodeStartRamAddr = .;
        *loader.o (.text .text.* .rodata .rodata.*)
        *int_hard.o (.text .text.* .rodata .rodata.*)
		KEEP(*hard-exps.o (.text .text.* .rodata .rodata.*))
        *hard-exp.o (.text .text.* .rodata .rodata.*)
        *int.o (.text .text.* .rodata .rodata.*)
        KEEP(*ints.o (.text .text.* .rodata .rodata.*))
        *heap-static.o (.text .text.* .rodata .rodata.*)
        *list.o (.text .text.* .rodata .rodata.*)
		. = ALIGN(0x08);
	}>flash
	
	
	test_debug :
    {       
       . = ALIGN(0x08);
         KEEP(*(*.test_debug))
		. = ALIGN(0x08);
    }>ram AT>flash 
	
	text_sysload_rIbootRom_lIbootRom :
    {       
       . = ALIGN(0x08);
        * (.text .text.* .rodata .rodata.* )
        g_pAppCodeEndRamAddr = .;
		. = ALIGN(0x08);
    }>flash 
	
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
    }>ram AT>flash
	
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
    }>ram
	
	rw_sysload_rRAM1_lIbootRom :
    {
       . = ALIGN(0x4) ;
        * (.data .data.*)
		. = ALIGN(0x4) ;
    }>ram AT>flash

    zi_sysload_rRAM1_lRAM1 (NOLOAD) :
    {
       . = ALIGN(0x04);
        * (.bss .bss.* COMMON)
		. = ALIGN(0x4) ;
    }>ram
	
	preload_cp_table :
    {
       . = ALIGN(0x08);
        preload_copy_table = .;
        LONG(16)
        LONG(4)

        LONG(LOADADDR(text_preload_rIbootRom_lIbootRom))
        LONG(ADDR(text_preload_rIbootRom_lIbootRom))
        LONG(SIZEOF(text_preload_rIbootRom_lIbootRom))
        LONG(1)

		LONG(LOADADDR(test_debug))
        LONG(ADDR(test_debug))
        LONG(SIZEOF(test_debug))
        LONG(1)

        LONG(LOADADDR(rw_preload_rRAM1_lIbootRom))
        LONG(ADDR(rw_preload_rRAM1_lIbootRom))
        LONG(SIZEOF(rw_preload_rRAM1_lIbootRom))
        LONG(1)

        LONG(LOADADDR(zi_preload_rRAM1_lRAM1))
        LONG(ADDR(zi_preload_rRAM1_lRAM1))
        LONG(SIZEOF(zi_preload_rRAM1_lRAM1))
        LONG(0)
    }>flash
	
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
    }>flash
	
	heap_rRAM_lRAM (NOLOAD) :
    {
       . = ALIGN(0x08);
        sys_heap_bottom = .;
		/*__heap_start = .;*/
    }>ram
    sys_heap_top = RamStart + RamSize;
	/*__heap_end = RamStart + RamSize;*/
	
	heap_Dtcm (NOLOAD) :
	{
		. = ALIGN(0x08);
		dtcm_heap_bottom = .;
	}>dtcm
	dtcm_heap_top = DtcmStart + DtcmSize;
	
	SDRAM_lRAM (NOLOAD) :
    {
       . = ALIGN(0x08);
        sdram_bottom = .;
    }>sdram
    sdram_top = SDRamStart + SDRamSize;
	
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
        LONG(1)                      /*表示该堆由1段内存组成*/
        LONG(0)                      /*该堆上分配的内存的对齐尺寸，0表示使用系统对齐*/
        LONG(1)                      /*0=通用堆,1=专用堆,如果系统只有唯一一个堆,则只能是通用堆*/
        LONG(dtcm_heap_bottom)     /*第一段基址，须符合对齐要求*/
        LONG(dtcm_heap_top)        /*第一段地址上限（不含），须符合对齐要求*/
        LONG(32)                   /*第一段页尺寸=32bytes*/
        BYTE(0x64)                   /*'d'*/
        BYTE(0x74)                   /*'t'*/
        BYTE(0x63)                   /*'c'*/
        BYTE(0x6d)                   /*'m'*/
        BYTE(0)                      /*串结束，"dtcm"是堆的名字*/
        
        . = ALIGN(0x04); 
        LONG(1)                      /*表示该堆由1段内存组成*/
        LONG(0)                      /*该堆上分配的内存的对齐尺寸，0表示使用系统对齐*/
        LONG(0)                      /*0=通用堆,1=专用堆,如果系统只有唯一一个堆,则只能是通用堆*/
        LONG(sdram_bottom)     /*第一段基址，须符合对齐要求*/
        LONG(sdram_top)        /*第一段地址上限（不含），须符合对齐要求*/
        LONG(32)                   /*第一段页尺寸=32bytes*/
        BYTE(0x65)                   /*'e'*/
        BYTE(0x78)                   /*'x'*/
        BYTE(0x74)                   /*'t'*/
        BYTE(0x72)                   /*'r'*/
        BYTE(0x61)                   /*'a'*/
        BYTE(0x6d)                   /*'m'*/
        BYTE(0)                      /*串结束，"extram"是堆的名字*/
        
        . = ALIGN(0x04);
        LONG(0)                     /*结束标志*/
    }>flash
    
	Iboot_Info :
    {        
        gc_ptIbootSize = .;
        LONG(IbootSize)
    }>flash
    
    App_Info :
    {
        . = ALIGN(0x08);
        gc_pAppOffset = .;          /* APP空间偏置 */
        LONG(IbootSize)
        gc_pAppRange = .;           /* APP空间范围 */
        LONG(FlashSize - IbootSize)       
    }>flash
    
   Iboot_AppInfo :
    {
       /* . = restore + IbootSize;*/
        gc_ptIbootCtrl = .;         /*结构变量，c中可用，用于检查APP正确性*/
        LONG( restore + IbootSize)
        . += 256;
        gc_ptAppInfo = .;           /*ROM区 存储APP的起始地址及大小 */ 
        LONG( restore + IbootSize + 256)
        . += 256;
        gc_AppAddr = .;             /*函数指针变量，指向APP的起始地址*/
        LONG( restore + IbootSize + 256 + 256)
        __AppStart = .;           /*函数指针变量，指向APP的Preload函数*/
        LONG( restore + IbootSize + 256 + 256)
    }>flash


	
    shell_config_section :
	{
   		KEEP(*(.shellconfig))
 	}>flash
    
    routine_shell_section :
	{
   		 KEEP(* (.ro_shell_cmd))
   		 KEEP(* (.ro_shell_data))
 	}>flash
 	
    expand_shell_section :
	{
 	     KEEP(* (.ex_shell_cmd))
 	     KEEP(* (.ex_shell_data))
 	      . = ALIGN(0x08);
 	}>flash
 	
 	Shell_Info :
    {
        p_shell_info = .;
        LONG(restore) /* _shell_fun_tab_start 内部函数表的起始地址  文件的起始地址*/
        LONG(IbootSize) /*_shell_fun_tab_end  内部函数表的结束地址 文件允许大小*/
    }>flash 

    /* _stack symbol defines initial stack bottom addres. Stack grows to lower addresses.
       Typically you set this to be top of your RAM. Note: code never checks, if stack
       grows into heap area!
    */
    PROVIDE(_stack_unused      = 0x440000 - 0x3F0 - 0x7F0 - 0xFF0 - 0x3F0 - 0x10); /* 0x10*/
    PROVIDE(_stack_svc         = 0x440000 - 0x3F0 - 0x7F0 - 0xFF0 - 0x3F0); /* 0x3F0*/
    PROVIDE(_stack_irq         = 0x440000 - 0x3F0 - 0x7F0 - 0xFF0); /* 0xFF0*/
    PROVIDE(_stack_fiq         = 0x440000 - 0x3F0 - 0x7F0); /* 0x7F0*/
    PROVIDE(_stack_sys         = 0x440000 - 0x3F0); /* 0x3F0*/
}
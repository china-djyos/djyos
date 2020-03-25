ENTRY(Init_Cpu)

INCLUDE  ../src/lds/memory.lds

SECTIONS
{
    /*RevRam是Ram保留区域，用于存储preload前使用或保存的内在数据,只有IAP_pload.c*/
    /*的pg_IapVar变量从该段内分配，且该段在iboot.lds和debug/release.lds保持一致*/
    Iboot_RevRam (NOLOAD):
    {
        . = ALIGN(0x08);
       KEEP(*(.IbootAppInfo .IbootAppInfo.*))
    }>RAM_SDRAM

        stack_rRAM1_lRAM1 (NOLOAD) :
    {
        . = ALIGN(0x02);
        msp_bottom = .;
        KEEP(* (.data.InitStack))
                /*. = ALIGN(0x08);*/
        msp_top = .;
    }>RAM_SDRAM

        Rom_Start :
    {
        restore = .;
    }>ROM1_SDRAM

        text_startup_rIbootRom_lIbootRom :
        {
                /*. = ALIGN(10);*/
       /*__VECTOR_TABLE = .;*/
           /*KEEP(*startup.o(.vectors))*/
           KEEP(*(.StartupExpTbl .StartupExpTbl.*))
           . = ALIGN(4);
           KEEP(*libos_Iboot.a:initcpus.o(.text .text.* .rodata .rodata.*))
           KEEP(*libos_Iboot.a:initcpuc.o(.text .text.* .rodata .rodata.*))
           KEEP(*libos_Iboot.a:pre_loader.o(.text .text.* .rodata .rodata.*))
           . = ALIGN(10);
        }>ROM1_SDRAM

        text_preload_rIbootRom_lIbootRom :
        {
                . = ALIGN(0x08);
                g_pAppCodeStartRamAddr = .;
        *libos_Iboot.a:loader.o (.data .data.*)
        *int_hard.o (.text .text.* .rodata .rodata.*)
                KEEP(*hard-exps.o (.text .text.* .rodata .rodata.*))
        *hard-exp.o (.text .text.* .rodata .rodata.*)

        *libos_Iboot.a:int.o (.data .data.*)
        *libos_Iboot.a:ints.o (.data .data.*)
        *src/iboot/critical/*.o (.data .data.*)
        *libos_Iboot.a:heap-static.o (.data .data.*)
        *libos_Iboot.a:list.o (.data .data.*)


                . = ALIGN(0x08);
        }>ROM1_SDRAM

        text_sysload_rIbootRom_lIbootRom :
    {
       . = ALIGN(0x08);
        * (.text .text.* .rodata .rodata.* )
        g_pAppCodeEndRamAddr = .;
                . = ALIGN(0x08);
    }>ROM1_SDRAM

        rw_preload_rRAM1_lIbootRom :
    {
       . = ALIGN(0x08);
        *libos_Iboot.a:loader.o (.data .data.*)
        *libos_Iboot.a:int_hard.o (.data .data.*)
        KEEP(*libos_Iboot.a:hard-exps.o (.data .data.*))
        *libos_Iboot.a:hard-exp.o (.data .data.*)
        *libos_Iboot.a:int.o (.data .data.*)
        *libos_Iboot.a:ints.o (.data .data.*)
        *libos_Iboot.a:heap-static.o (.data .data.*)
        *libos_Iboot.a:list.o (.data .data.*)
        *libos_Iboot.a:cpu_peri_isr.o (.data .data.*)
                . = ALIGN(0x08);
    }>RAM_SDRAM AT>ROM1_SDRAM

        zi_preload_rRAM1_lRAM1 (NOLOAD) :
    {
       . = ALIGN(10);
        KEEP(* (.table.exceptions .table.exceptions.*))
        KEEP(* (.table.vectors .table.vectors.*))
        *libos_Iboot.a:loader.o (.bss .bss.* COMMON)
        *libos_Iboot.a:int_hard.o (.bss .bss.* COMMON)
        KEEP(*libos_Iboot.a:hard-exps.o (.bss .bss.* COMMON))
        *libos_Iboot.a:hard-exp.o (.bss .bss.* COMMON)
        *libos_Iboot.a:int.o (.bss .bss.* COMMON)
        *libos_Iboot.a:ints.o (.bss .bss.* COMMON)
        *libos_Iboot.a:heap-static.o (.bss .bss.* COMMON)
        *libos_Iboot.a:list.o (.bss .bss.* COMMON)
        *libos_Iboot.a:cpu_peri_isr.o (.bss .bss.* COMMON)
    }>RAM_SDRAM

        rw_sysload_rRAM1_lIbootRom :
    {
       . = ALIGN(0x4) ;
        * (.data .data.*)
                . = ALIGN(0x4) ;
    }>RAM_SDRAM AT>ROM1_SDRAM

    zi_sysload_rRAM1_lRAM1 (NOLOAD) :
    {
       . = ALIGN(0x04);
        * (.bss .bss.* COMMON)
                . = ALIGN(0x4) ;
    }>RAM_SDRAM

    routine_shell_data_section :
        {
                 KEEP(* (.ro_shell_data))
         }>RAM_SDRAM AT>ROM1_SDRAM

    expand_shell_data_section :
        {
             KEEP(* (.ex_shell_data))
         }>RAM_SDRAM AT>ROM1_SDRAM

        preload_cp_table :
    {
       . = ALIGN(0x08);
        preload_copy_table = .;
        LONG(16)
        LONG(5)

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

        LONG(LOADADDR(routine_shell_data_section))
            LONG(ADDR(routine_shell_data_section))
            LONG(SIZEOF(routine_shell_data_section))
            LONG(1)

        LONG(LOADADDR(expand_shell_data_section))
            LONG(ADDR(expand_shell_data_section))
            LONG(SIZEOF(expand_shell_data_section))
            LONG(1)
    }>ROM1_SDRAM

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
    }>ROM1_SDRAM

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
    }>ROM1_SDRAM

    Iboot_Info :
    {
        gc_ptIbootSize = .;
        LONG(IbootSize)
    }>ROM1_SDRAM

    App_Info :
    {
        . = ALIGN(0x08);
        gc_pAppOffset = .;          /* APP空间偏置  为了访问下面四个字节 */
        LONG(Flash2Offset + IbootSize)
    }>ROM1_SDRAM

    Iboot_AppInfo :
    {
       /* . = restore + IbootSize;*/
        gc_ptIbootCtrl = .;         /*结构变量，c中可用，用于检查APP正确性*/
        LONG( Flash2Offset)
        . += 256;
        gc_ptAppInfo = .;           /*ROM区 存储APP的起始地址及大小 */
        LONG( Flash2Offset + 256)
        . += 256;
        gc_AppAddr = .;             /*函数指针变量，指向APP的起始地址*/
        LONG( Flash2Offset + 256 + 256)
        __AppStart = .;           /*函数指针变量，指向APP的Preload函数*/
        LONG( Flash2Offset + 256 + 256)
    }>ROM1_SDRAM

    shell_config_section :
        {
                KEEP(*(.shellconfig))
        }>ROM1_SDRAM

    routine_shell_section :
        {
                 KEEP(* (.ro_shell_cmd))
                 KEEP(* (.ro_shell_data))
        }>ROM1_SDRAM

    expand_shell_section :
        {
             KEEP(* (.ex_shell_cmd))
             KEEP(* (.ex_shell_data))
              . = ALIGN(0x08);
        }>ROM1_SDRAM

        Shell_Info :
    {
        p_shell_info = .;
        LONG(restore) /* 该bin文件的起始地址*/
        LONG(IbootSize) /*该bin文件允许大小*/
    }>ROM1_SDRAM
}

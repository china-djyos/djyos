ENTRY(AppStart);
INCLUDE  ../src/lds/memory.lds

SECTIONS
{
    Iboot_IbootFill :
    {
        gc_IbootStart = .;
    }>flash_app

    Iboot_Apphead :
    {
        gc_AppStart = .;
        KEEP(*(.DjyAppHead .DjyAppHead.*))
        KEEP(*(.DjyProductInfo .DjyProductInfo.*))
    }>flash_app

    /*share_rRAM1_lRAM1段保存的是Iboot和APP共享信息，必须确保在同一地址，*/
    /*故在memory.lds中定义了一段指定地址的内存*/

    share_rRAM1_lRAM1 (NOLOAD) :
    {
        . = ALIGN(0x08);
        KEEP(*(.IbootAppInfo .IbootAppInfo.*))
        . = ALIGN(0x08);
        KEEP(* (.data.ExpTable))
        KEEP(* (.data.interrupt))
    }>share
    stack_rRAM1_lRAM1 (NOLOAD) :
    {
        . = ALIGN(0x08);
        msp_bottom = .;
        KEEP(* (.data.InitStack))
        . = ALIGN(0x08);
        msp_top = .;
    }>ram

    /* vectors go to vectors region */
    /*. = ORIGIN(flash_app);*/
    .vectors :
    {
        KEEP(*(*.text.AppStart))
        . = ALIGN(4);
         KEEP(*initcpu.o(.text .text.* .rodata .rodata.*))
         KEEP(*pre_loader.o(.text .text.* .rodata .rodata.*))
         . = ALIGN(0x10);
    } > flash_app

    wifi_lib :
    {
        . = ALIGN(0x08);
        *librwnx.a* (.text .text.* .rodata .rodata.*)
        . = ALIGN(0x08);
    } > flash_app

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
    }>flash_app

    test_debug :
    {
        . = ALIGN(0x08);
        KEEP(*(*.test_debug))
        . = ALIGN(0x08);
    }>ram AT>flash_app

    text_sysload_rIbootRom_lIbootRom :
    {
        . = ALIGN(0x08);
        * (.text .text.* .rodata .rodata.* )
        g_pAppCodeEndRamAddr = .;
        . = ALIGN(0x08);
    }>flash_app

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
    }>ram AT>flash_app

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
    }>ram AT>flash_app

    zi_sysload_rRAM1_lRAM1 (NOLOAD) :
    {
        . = ALIGN(0x04);
        * (.bss .bss.* COMMON)
        . = ALIGN(0x4) ;
    }>ram

    lcdbuffer (NOLOAD) :
    {
        . = ALIGN(0x04);
        * (.lcdram)
        . = ALIGN(0x4) ;
    }>sdram

     routine_shell_data_section :
    {
        KEEP(* (.ro_shell_data))
     }>ram AT>flash_app

    expand_shell_data_section :
    {
         KEEP(* (.ex_shell_data))
     }>ram AT>flash_app

    preload_cp_table :
    {
       . = ALIGN(0x08);
        preload_copy_table = .;
        LONG(16)
        LONG(6)

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


        LONG(LOADADDR(routine_shell_data_section))
        LONG(ADDR(routine_shell_data_section))
        LONG(SIZEOF(routine_shell_data_section))
        LONG(1)

        LONG(LOADADDR(expand_shell_data_section))
        LONG(ADDR(expand_shell_data_section))
        LONG(SIZEOF(expand_shell_data_section))
        LONG(1)

        LONG(LOADADDR(zi_preload_rRAM1_lRAM1))
        LONG(ADDR(zi_preload_rRAM1_lRAM1))
        LONG(SIZEOF(zi_preload_rRAM1_lRAM1))
        LONG(0)
    }>flash_app

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
    }>flash_app

    heap_rRAM_lRAM (NOLOAD) :
    {
        . = ALIGN(0x08);
        sys_heap_bottom = .;
    }>ram
    sys_heap_top = ORIGIN(ram) + LENGTH(ram);

    heap_Dtcm (NOLOAD) :
    {
        . = ALIGN(0x08);
        dtcm_heap_bottom = .;
    }>dtcm
    dtcm_heap_top =  ORIGIN(dtcm) + LENGTH(dtcm);

    SDRAM_lRAM (NOLOAD) :
    {
        KEEP(*(.LcdRam .LcdRam.*))
       . = ALIGN(0x08);
        sdram_bottom = .;
    }>sdram
    sdram_top =  ORIGIN(sdram) + LENGTH(sdram);

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
    }>flash_app

    Iboot_Info :
    {
        gc_ptIbootSize = .;
        LONG(IbootSize)
    }>flash_app

    App_Info :
    {
        . = ALIGN(0x08);
        gc_pAppOffset = .;          /* APP空间偏置 */
        LONG(IbootSize)
        gc_pAppRange = .;           /* APP空间范围 */
        LONG(LENGTH(flash_app))
    }>flash_app


    shell_config_section :
    {
        KEEP(*(.shellconfig))
    }>flash_app

    routine_shell_section :
    {
        KEEP(* (.ro_shell_cmd))
    }>flash_app

    expand_shell_section :
    {
         KEEP(* (.ex_shell_cmd))
          . = ALIGN(0x08);
    }>flash_app

    Shell_Info :
    {
        p_shell_info = .;
        LONG(gc_AppStart) /* _shell_fun_tab_start 内部函数表的起始地址  文件的起始地址*/
        LONG(LENGTH(flash_app) + gc_AppStart) /*_shell_fun_tab_end  内部函数表的结束地址 文件允许大小*/
    }>flash_app
    product_sn :
    {
        gc_ProductSn = .;
    }>flash_sn
    

}
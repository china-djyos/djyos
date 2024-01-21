ENTRY(isr_vector);
INCLUDE  ../src/lds/memory.lds

SECTIONS
{
    Rom_Start :
    {
        restore = .;
    }>DRAM_iboot

    /*SHARE_rRAM1_lRAM1段保存的是Iboot和APP共享信息，必须确保在同一地址，*/
    /*故在memory.lds中定义了一段指定地址的内存*/

    SHARE_rRAM1_lRAM1 (NOLOAD) :
    {
        . = ALIGN(0x08);
        KEEP(*(.IbootAppInfo .IbootAppInfo.*))
    }>SHARE
    stack_rRAM1_lRAM1 (NOLOAD) :
    {
        . = ALIGN(0x08);
        * (.data.mmutab)
        KEEP(* (.data.ExpTable))
        KEEP(* (.data.interrupt))
        . = ALIGN(0x08);
        msp_bottom = .;
        KEEP(* (.data.InitStack))
        . = ALIGN(0x08);
        msp_top = .;
    }>DRAM

    text_startup_rIbootRom_lIbootRom :
    {
        KEEP(*(*.start_vectors))
        . = ALIGN(4);
         KEEP(*initcpu.o(.text .text.* .rodata .rodata.*))
         KEEP(*pre_loader.o(.text .text.* .rodata .rodata.*))
         KEEP(*sys_clock.o(.text .text.* .rodata .rodata.*))
         . = ALIGN(0x10);
    } > DRAM_iboot

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
    }>DRAM_iboot

    text_sysload_rIbootRom_lIbootRom :
    {
        . = ALIGN(0x08);
        * (.text .text.* .rodata .rodata.* )
        g_pAppCodeEndRamAddr = .;
        . = ALIGN(0x08);
    }>DRAM_iboot

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
    }>DRAM_iboot

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
    }>DRAM

    rw_sysload_rRAM1_lIbootRom :
    {
        . = ALIGN(0x4) ;
        * (.data .data.*)
        . = ALIGN(0x4) ;
    }>DRAM_iboot

    zi_sysload_rRAM1_lRAM1 (NOLOAD) :
    {
        . = ALIGN(0x04);
        * (.bss .bss.* COMMON)
        . = ALIGN(0x4) ;
    }>DRAM

    routine_shell_data_section :
    {
        KEEP(* (.ro_shell_data))
     }>DRAM AT>DRAM_iboot

    expand_shell_data_section :
    {
         KEEP(* (.ex_shell_data))
     }>DRAM AT>DRAM_iboot

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
    }>DRAM_iboot

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
    }>DRAM_iboot

    heap_rRAM_lRAM (NOLOAD) :
    {
        . = ALIGN(0x08);
        sys_heap_bottom = .;
    }>DRAM
    sys_heap_top = DRAM_Start + DRAM_Size;

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
    }>DRAM_iboot

    Iboot_Info :
    {
        gc_ptIbootSize = .;
        LONG(IbootSize)
    }>DRAM_iboot

    App_Info :
    {
        . = ALIGN(0x08);
        gc_pAppOffset = .;          /* APP空间偏置 */
        LONG(DRAM_app_Start)
        gc_pAppRange = .;           /* APP空间范围 */
        LONG(DRAM_app_Size)
    }>DRAM_iboot

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
    }>DRAM_iboot



    shell_config_section :
    {
        KEEP(*(.shellconfig))
    }>DRAM_iboot

    routine_shell_section :
    {
        KEEP(* (.ro_shell_cmd))
    }>DRAM_iboot

    expand_shell_section :
    {
         KEEP(* (.ex_shell_cmd))
          . = ALIGN(0x08);
    }>DRAM_iboot

    Shell_Info :
    {
        p_shell_info = .;
        LONG(restore) /* _shell_fun_tab_start 内部函数表的起始地址  文件的起始地址*/
        LONG(IbootSize) /*_shell_fun_tab_end  内部函数表的结束地址 文件允许大小*/
    }>DRAM_iboot

    product_sn :
    {
        gc_ProductSn = .;
    }>DRAM_iboot
}
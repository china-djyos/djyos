ENTRY(Init_Cpu)

INCLUDE  ../src/lds/memory.lds

/*不同的make程序，文件路径表示方式，是不同的，输入段中文件名写法要相应修改*/
/*正确的写法，可参看console中输出的命令行*/
SECTIONS
{
    Rom_Start :
    {
        restore = .;
    }>Flash

    /*RevRam是Ram保留区域，用于存储preload前使用或保存的内在数据,只有IAP_pload.c*/
    /*的pg_IapVar变量从该段内分配，且该段在iboot.lds和debug/release.lds保持一致*/
    Iboot_RevRam (NOLOAD):
    {
        . = ALIGN(0x08);
         KEEP(*(.IbootAppInfo .IbootAppInfo.*))
    }>RAM1

    /*iboot.lds中InitStack大小及位置必须与应用程序中debug/release.lds*/
    /*的一致，否则程序会可能会出现被覆盖的问题*/
    stack_rRAM1_lRAM1 (NOLOAD) :
    {
        . = ALIGN(0x08);
        msp_bottom = .;
        KEEP(* (.data.InitStack))
        msp_top = ALIGN(0x08);
   }>RAM1

    vectorTab :
    {
        . = ALIGN(0x08);
         isr_vector_tab_start = .;
        KEEP(*(.isr_vector .isr_vector.*))
         isr_vector_tab_end = .;
    }>VerctorFlash

    text_startup_rROM_lROM :
    {
        . = ALIGN(0x08);
        KEEP(*libos_Iboot.a:hard-exps.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_Iboot.a:initcpuc.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_Iboot.a:pre_loader.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_Iboot.a:IAP_pLoad.o (.text .text.* .rodata .rodata.*))
    }>Flash

    text_preload_rROM_lROM :
    {
        . = ALIGN(0x08);
        g_pAppCodeStartRamAddr = .;
        *libos_Iboot.a:loader.o (.text .text.* .rodata .rodata.*)
        *libos_Iboot.a:int_hard.o (.text .text.* .rodata .rodata.*)
        *libos_Iboot.a:hard-exp.o (.text .text.* .rodata .rodata.*)
        *libos_Iboot.a:int.o (.text .text.* .rodata .rodata.*)
        *libos_Iboot.a:ints.o (.text .text.* .rodata .rodata.*)
        *src/iboot/critical/*.o (.text .text.* .rodata .rodata.*)
        *libos_Iboot.a:heap-static.o (.text .text.* .rodata .rodata.*)
        *libos_Iboot.a:list.o (.text .text.* .rodata .rodata.*)
    }>Flash

        QSpi_Flash :
    {
        . = ALIGN(0x04);
        Lock_Cache_Add_Start = .;
        KEEP(*norflash.o (.text .text.* .rodata .rodata.*))
         . = ALIGN(0x04);
        Lock_Cache_Add_End = .;

    }>Flash

    M0BootRisc :
    {
        Risc_Boot_Addr_Start = .;
        LONG(34996480)
        Risc_Boot_Addr_End   = .;
    }>M0BootRiscFlash

    text_sysload_rROM_lROM :
    {
        . = ALIGN(0x08);
        * (.text .text.* .rodata .rodata.*)
        g_pAppCodeEndRamAddr = .;
    }>Flash

    /*Cortex-M 系列的向量表可重定位，对起始地址的要求是：若有N个向量（包含异常），*/
    /*先增大到2的整数次幂，若为M，则Addr = ALIGN(4 * M) */
    /*举例：若cpu有64个中断，则N = 80, M = 128, Addr = ALIGN(0x200)*/
    zi_preload_rExtRam_lExtRam (NOLOAD) :
    {
        . = ALIGN(0x100);
        Remap_Int_Start = . ;
        KEEP(* (.table.exceptions .table.exceptions.*))
        KEEP(* (.table.vectors .table.vectors.*))
        Remap_Int_End = . ;
        *libos_Iboot.a:loader.o (.bss .bss.* COMMON)
        *libos_Iboot.a:int_hard.o (.bss .bss.* COMMON)
        KEEP(*libos_Iboot.a:hard-exps.o (.bss .bss.* COMMON))
        *libos_Iboot.a:hard-exp.o (.bss .bss.* COMMON)
        *libos_Iboot.a:int.o (.bss .bss.* COMMON)
        *libos_Iboot.a:ints.o (.bss .bss.* COMMON)
        *src/iboot/critical/*.o (.bss .bss.* COMMON)
        *libos_Iboot.a:heap-static.o (.bss .bss.* COMMON)
        *libos_Iboot.a:list.o (.bss .bss.* COMMON)
    }>ExtRam

    rw_preload_rExtRam_lROM :
    {
        . = ALIGN(0x08);
        *libos_Iboot.a:loader.o (.data .data.*)
        *libos_Iboot.a:int_hard.o (.data .data.*)
        KEEP(*libos_Iboot.a:hard-exps.o (.data .data.*))
        *libos_Iboot.a:hard-exp.o (.data .data.*)
        *libos_Iboot.a:int.o (.data .data.*)
        *libos_Iboot.a:ints.o (.data .data.*)
        *src/iboot/critical/*.o (.data .data.*)
        *libos_Iboot.a:heap-static.o (.data .data.*)
        *libos_Iboot.a:list.o (.data .data.*)
    }>ExtRam AT>Flash



    rw_sysload_rExtRam_lROM :
    {
        . = ALIGN(0x08);
        * (.data .data.*)
    }>ExtRam AT>Flash

    zi_sysload_rExtRam_lExtRam (NOLOAD) :
    {
        . = ALIGN(0x08);
        * (.bss .bss.* COMMON)
    }>ExtRam

    routine_shell_data_section :
        {
                 KEEP(* (.ro_shell_data))
         }>ExtRam AT>Flash

    expand_shell_data_section :
        {
             KEEP(* (.ex_shell_data))
         }>ExtRam AT>Flash

    preload_cp_table :
    {
        . = ALIGN(0x08);
        preload_copy_table = .;
        LONG(16)
        LONG(3)

        LONG(LOADADDR(text_preload_rROM_lROM))
        LONG(ADDR(text_preload_rROM_lROM))
        LONG(SIZEOF(text_preload_rROM_lROM))
        LONG(1)

        LONG(LOADADDR(rw_preload_rExtRam_lROM))
        LONG(ADDR(rw_preload_rExtRam_lROM))
        LONG(SIZEOF(rw_preload_rExtRam_lROM))
        LONG(1)

        LONG(LOADADDR(zi_preload_rExtRam_lExtRam))
        LONG(ADDR(zi_preload_rExtRam_lExtRam))
        LONG(SIZEOF(zi_preload_rExtRam_lExtRam))
        LONG(0)
    }>Flash

    sysload_cp_table :
    {
        . = ALIGN(0x08);
        sysload_copy_table = .;
        LONG(16)
        LONG(5)

        LONG(LOADADDR(text_sysload_rROM_lROM))
        LONG(ADDR(text_sysload_rROM_lROM))
        LONG(SIZEOF(text_sysload_rROM_lROM))
        LONG(1)

        LONG(LOADADDR(rw_sysload_rExtRam_lROM))
        LONG(ADDR(rw_sysload_rExtRam_lROM))
        LONG(SIZEOF(rw_sysload_rExtRam_lROM))
        LONG(1)

        LONG(LOADADDR(zi_sysload_rExtRam_lExtRam))
        LONG(ADDR(zi_sysload_rExtRam_lExtRam))
        LONG(SIZEOF(zi_sysload_rExtRam_lExtRam))
        LONG(0)

        LONG(LOADADDR(routine_shell_data_section))
                LONG(ADDR(routine_shell_data_section))
                LONG(SIZEOF(routine_shell_data_section))
                LONG(1)

                LONG(LOADADDR(expand_shell_data_section))
                LONG(ADDR(expand_shell_data_section))
                LONG(SIZEOF(expand_shell_data_section))
                LONG(1)
    }>Flash

    heap_rRAM1_lRAM1 (NOLOAD) :
    {
        . = ALIGN(0x08);
        sys_heap_bottom = .;
    }>RAM1
    sys_heap_top = Ram1Start + Ram1Size;

    heap_rExtRam_lExtRam (NOLOAD) :
    {
        . = ALIGN(0x08);
        extram_heap_bottom = .;
    }>ExtRam
    extram_heap_top = ExtRamStart + ExtRamSize;

    /*sdram area*/
    heap_rExtRam_lSdRam (NOLOAD) :
    {
        . = ALIGN(0x08);
        sdram_heap_bottom = .;
    }>SdRam
    sdram_heap_top = SdRamStart + SdRamSize;

    heap_cp_table :
    {
        . = ALIGN(0x08);
        pHeapList = .;

/* 默认堆，必须排首位，名字必须是“sys” */
        LONG(2)                     /*表示该堆由两段内存组成*/
        LONG(0)                     /*该堆上分配的内存的对齐尺寸，0表示使用系统对齐*/
        LONG(0)                     /*0=通用堆,1=专用堆,如果系统只有唯一一个堆,则只能是通用堆*/
        LONG(sys_heap_bottom)       /*第一段基址，须符合对齐要求*/
        LONG(sys_heap_top)          /*第一段地址上限（不含），须符合对齐要求*/
        LONG(32)                    /*第一段页尺寸=32bytes*/
        LONG(msp_bottom)            /*第二段基址，回收初始化栈，须符合对齐要求*/
        LONG(msp_top - (1024+512))  /*第二段地址上限（不含），1024+1024是留给中断使用的，须符合对齐要求*/
        LONG(24)                    /*第二段页尺寸=24bytes*/
        BYTE(0x73)                  /*'s'*/
        BYTE(0x79)                  /*'y'*/
        BYTE(0x73)                  /*'s'*/
        BYTE(0)                     /*串结束，"sys"是堆的名字*/

        . = ALIGN(0x04);

        LONG(1)                     /*表示该堆由1段内存组成*/
        LONG(0)                     /*该堆上分配的内存的对齐尺寸，0表示使用系统对齐*/
        LONG(0)                     /*0=通用堆,1=专用堆,如果系统只有唯一一个堆,则只能是通用堆*/
        LONG(extram_heap_bottom)   /*第一段基址，须符合对齐要求*/
        LONG(extram_heap_top)      /*第一段地址上限（不含），须符合对齐要求*/
        LONG(128)                   /*第一段页尺寸=128bytes*/
        BYTE(0x65)                  /*'e'*/
        BYTE(0x78)                  /*'x'*/
        BYTE(0x74)                  /*'t'*/
        BYTE(0x72)                  /*'r'*/
        BYTE(0x61)                  /*'a'*/
        BYTE(0x6d)                  /*'m'*/
        BYTE(0)                     /*串结束，"extram"是堆的名字*/

        . = ALIGN(0x04);

        LONG(1)                     /*表示该堆由1段内存组成*/
        LONG(0)                     /*该堆上分配的内存的对齐尺寸，0表示使用系统对齐*/
        LONG(0)                     /*0=通用堆,1=专用堆,如果系统只有唯一一个堆,则只能是通用堆*/
        LONG(sdram_heap_bottom)   /*第一段基址，须符合对齐要求*/
        LONG(sdram_heap_top)      /*第一段地址上限（不含），须符合对齐要求*/
        LONG(256)                   /*第一段页尺寸=128bytes*/
        BYTE(0x53)                  /*'s'*/
        BYTE(0x44)                  /*'d'*/
        BYTE(0x72)                  /*'r'*/
        BYTE(0x61)                  /*'a'*/
        BYTE(0x6d)                  /*'m'*/
        BYTE(0)                     /*串结束，"sdram"是堆的名字*/

                . = ALIGN(0x04);
        LONG(0)                     /*结束标志*/
    }>Flash


    Iboot_Info :
    {
        . = ALIGN(0x04);
        gc_ptFlashOffset = .;     /* 为了访问下面四个字节 */
        LONG(FlashOffset)

        gc_ptFlashRange = .;
        LONG(FlashRange)

        gc_ptIbootSize = .;
        LONG(IbootSize)

    }>Flash

    App_Info :
    {
        . = ALIGN(0x08);
        gc_pAppOffset = .;          /* APP空间偏置  为了访问下面四个字节 */
        LONG(FlashOffset + IbootSize)
        gc_pAppRange = .;           /* APP空间范围 */
        LONG(FlashRange - IbootSize)
    }>Flash


    Iboot_AppInfo :
    {
       . = ALIGN(0x04);
       /* . = restore + IbootSize;*/
        gc_ptIbootCtrl = .;         /*结构变量，c中可用，用于检查APP正确性*/
        LONG( restore + IbootSize)
        . += 256;                   /*这里加256 */
        gc_ptAppInfo = .;           /*ROM区 存储APP的起始地址及大小 */
        LONG(restore + IbootSize + 256)/*这里指向 dbg rease 中  App gc_ptAppInfo -> 0x40900 gc_ptAppInfo = 0x40900*/
        . += 256;
        gc_AppAddr = .;             /*函数指针变量，该标号 ->指向APP的起始地址  为的是访问下面long的数值,下面数值地址即为app 入口函数地址*/
        LONG( restore + IbootSize + 256 + 256)
        __AppStart = .;           /*函数指针变量，指向APP的Preload函数  该标号 ->指向APP的起始地址  为的是访问下面long的数值,下面数值地址即为app 入口函数地址*/
        LONG( restore + IbootSize + 256 + 256)
    }>Flash

    shell_config_section :
        {
                KEEP(*(.shellconfig))
        }>Flash

    routine_shell_section :
        {
                 KEEP(* (.ro_shell_cmd))
                 KEEP(* (.ro_shell_data))
        }>Flash

    expand_shell_section :
        {
             KEEP(* (.ex_shell_cmd))
             KEEP(* (.ex_shell_data))
              . = ALIGN(0x08);
        }>Flash

        Shell_Info :
    {
        p_shell_info = .;
        LONG(restore) /*  该bin文件的起始地址*/
        LONG(IbootSize) /* 该bin文件允许大小*/
    }>Flash

 /*
    QSpi_Flash:
    {
        Lock_Cache_Add_Start = .;
        *os_Iboot_Debug.a:norflash.o (.text .text.* .rodata .rodata.*)
        Lock_Cache_Add_End = .;

    }>Flash
*/
}
/**** The End ****/

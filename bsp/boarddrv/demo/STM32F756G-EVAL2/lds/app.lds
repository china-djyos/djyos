ENTRY(AppStart)

INCLUDE  ../src/lds/memory.lds

/*不同的make程序，文件路径表示方式，是不同的，输入段中文件名写法要相应修改*/
/*正确的写法，可参看console中输出的命令行*/
SECTIONS
{
    /*地址填充区域，Release版本下载起始地址实质上是0x0040 0000 + IbootSize + 256*/
    /*因此在此处填充section内，将InnerFlash地址偏移，使生成的bin文件起始地址为0x00440100*/
    /*切勿在填充section内填写数据（例如LONG( . + 256)），否则生成的bin文件中会包含此段*/
        Iboot_Apphead :
    {
        gc_AppStart = .;
        KEEP(*(.DjyAppHead .DjyAppHead.*))
        KEEP(*(.DjyProductInfo .DjyProductInfo.*))
    }>InnerFlash

        Iboot_RevRam (NOLOAD):
    {
        . = ALIGN(0x08);
        KEEP(*(.IbootAppInfo .IbootAppInfo.*))
    }>RAM1

    /*RevRam是Ram保留区域，用于存储preload前使用或保存的内在数据,只有IAP_pload.c*/
    /*的pg_IapVar变量从该段内分配，且该段在iboot.lds和debug/release.lds保持一致*/
    Iboot_RevRam (NOLOAD):
    {
        . = ALIGN(0x08);
        KEEP(*(.IapBootFlgRam .IapBootFlgRam.*))
    }>RAM1

    stack_rRAM1_lRAM1 (NOLOAD) :
    {
        . = ALIGN(0x08);
        msp_bottom = .;
        KEEP(* (.data.InitStack))
        msp_top = ALIGN(0x08);
    }>RAM1

    text_startup_rAppRom_lAppRom :
    {
       . = ALIGN(0x08);
        KEEP(*libos_App.a:initcpuc.o (.text.AppStart))
        KEEP(*libos_App.a:pre_loader.o (.text .text.* .rodata .rodata.*))
    }>InnerFlash

    text_preload_rAppRom_lAppRom :
    {
       . = ALIGN(0x08);
        g_pAppCodeStartRamAddr = .;
        *libos_App.a:loader.o (.text .text.* .rodata .rodata.*)
        *libos_App.a:int_hard.o (.text .text.* .rodata .rodata.*)
        *libos_App.a:set-cache.o (.text .text.* .rodata .rodata.*)
        *libos_App.a:mpu.o (.text .text.* .rodata .rodata.*)
        KEEP(*libos_App.a:hard-exps.o (.text .text.* .rodata .rodata.*))
        *libos_App.a:hard-exp.o (.text .text.* .rodata .rodata.*)
        *libos_App.a:int.o (.text .text.* .rodata .rodata.*)
        *libos_App.a:ints.o (.text .text.* .rodata .rodata.*)
        *src/app/critical/*.o (.text .text.* .rodata .rodata.*)
        *libos_App.a:heap-static.o (.text .text.* .rodata .rodata.*)
        *libos_App.a:list.o (.text .text.* .rodata .rodata.*)
    }>InnerFlash

    text_sysload_rAppRom_lAppRom :
    {
       . = ALIGN(0x08);
        * (.text .text.* .rodata .rodata.*)
        g_pAppCodeEndRamAddr = .;
    }>InnerFlash

    rw_preload_rRAM1_lAppRom :
    {
       . = ALIGN(0x08);
        *libos_App.a:loader.o (.data .data.*)
        *libos_App.a:int_hard.o (.data .data.*)
        KEEP(*libos_App.a:hard-exps.o (.data .data.*))
        *libos_App.a:hard-exp.o (.data .data.*)
        *libos_App.a:int.o (.data .data.*)
        *libos_App.a:ints.o (.data .data.*)
        *src/app/critical/*.o (.data .data.*)
        *libos_App.a:heap-static.o (.data .data.*)
        *libos_App.a:list.o (.data .data.*)
    }>RAM1 AT>InnerFlash

    rw_nocache_rRAM1_lRAM1 (NOLOAD) :
    {
        . = ALIGN(0x20);
        KEEP(* (.nocacheram .nocacheram.*))
    }>RAM1NOCACHE

    /*Cortex-M 系列的向量表可重定位，对起始地址的要求是：若有N个向量（包含异常），*/
    /*先增大到2的整数次幂，若为M，则Addr = ALIGN(4 * M) */
    /*举例：若cpu有64个中断，则N = 80, M = 128, Addr = ALIGN(0x200)*/
    zi_preload_rRAM1_lRAM1 (NOLOAD) :
    {
       . = ALIGN(0x200);
        KEEP(* (.table.exceptions .table.exceptions.*))
        KEEP(* (.table.vectors .table.vectors.*))
        *libos_App.a:loader.o (.bss .bss.* COMMON)
        *libos_App.a:int_hard.o (.bss .bss.* COMMON)
        KEEP(*libos_App.a:hard-exps.o (.bss .bss.* COMMON))
        *libos_App.a:hard-exp.o (.bss .bss.* COMMON)
        *libos_App.a:int.o (.bss .bss.* COMMON)
        *libos_App.a:ints.o (.bss .bss.* COMMON)
        *src/app/critical/*.o (.bss .bss.* COMMON)
        *libos_App.a:heap-static.o (.bss .bss.* COMMON)
        *libos_App.a:list.o (.bss .bss.* COMMON)
    }>RAM1

     rw_sysload_rRAM1_lAppRom :
    {
       . = ALIGN(0x08);
        * (.data .data.*)
    }>RAM1 AT>InnerFlash
    zi_sysload_rextram_lextram (NOLOAD) :
    {
        . = ALIGN(0x08);
        *libos_App.a:gk_draw.o (.bss .bss.* COMMON)
        *libos_App.a:gk_clip.o (.bss .bss.* COMMON)
        *libos_App.a:gk_win.o (.bss .bss.* COMMON)
    }>extram

    zi_sysload_rRAM1_lRAM1 (NOLOAD) :
    {
       . = ALIGN(0x08);
        * (.bss .bss.* COMMON)
    }>RAM1


        routine_shell_data_section :
        {
                 KEEP(* (.ro_shell_data))
         }>RAM1 AT>InnerFlash

    expand_shell_data_section :
        {
             KEEP(* (.ex_shell_data))
         }>RAM1 AT>InnerFlash

    preload_cp_table :
    {
       . = ALIGN(0x08);
        preload_copy_table = .;
        LONG(16)
        LONG(3)

        LONG(LOADADDR(text_preload_rAppRom_lAppRom))
        LONG(ADDR(text_preload_rAppRom_lAppRom))
        LONG(SIZEOF(text_preload_rAppRom_lAppRom))
        LONG(1)

        LONG(LOADADDR(rw_preload_rRAM1_lAppRom))
        LONG(ADDR(rw_preload_rRAM1_lAppRom))
        LONG(SIZEOF(rw_preload_rRAM1_lAppRom))
        LONG(1)

        LONG(LOADADDR(zi_preload_rRAM1_lRAM1))
        LONG(ADDR(zi_preload_rRAM1_lRAM1))
        LONG(SIZEOF(zi_preload_rRAM1_lRAM1))
        LONG(0)
    }>InnerFlash

    sysload_cp_table :
    {
       . = ALIGN(0x08);
        sysload_copy_table = .;
        LONG(16)
        LONG(6)

        LONG(LOADADDR(text_sysload_rAppRom_lAppRom))
        LONG(ADDR(text_sysload_rAppRom_lAppRom))
        LONG(SIZEOF(text_sysload_rAppRom_lAppRom))
        LONG(1)

        LONG(LOADADDR(rw_sysload_rRAM1_lAppRom))
        LONG(ADDR(rw_sysload_rRAM1_lAppRom))
        LONG(SIZEOF(rw_sysload_rRAM1_lAppRom))
        LONG(1)
        LONG(LOADADDR(zi_sysload_rextram_lextram))
        LONG(ADDR(zi_sysload_rextram_lextram))
        LONG(SIZEOF(zi_sysload_rextram_lextram))
        LONG(0)

        LONG(LOADADDR(zi_sysload_rRAM1_lRAM1))
        LONG(ADDR(zi_sysload_rRAM1_lRAM1))
        LONG(SIZEOF(zi_sysload_rRAM1_lRAM1))
        LONG(0)

        LONG(LOADADDR(routine_shell_data_section))
        LONG(ADDR(routine_shell_data_section))
        LONG(SIZEOF(routine_shell_data_section))
        LONG(1)

        LONG(LOADADDR(expand_shell_data_section))
        LONG(ADDR(expand_shell_data_section))
        LONG(SIZEOF(expand_shell_data_section))
        LONG(1)
    }>InnerFlash

    heap_rRAM1_lRAM1 (NOLOAD) :
    {
       . = ALIGN(0x08);
        sys_heap_bottom = .;
    }>RAM1
    sys_heap_top = InnerRamStart + InnerRamSize;
     heap_rextram_lextram (NOLOAD) :
      {
          . = ALIGN(0x08);
          extram_heap_bottom = .;
      }>extram
      extram_heap_top = ExtRamStart + ExtRamSize;

     heap_rnocache_lextram (NOLOAD) :
      {
         . = ALIGN(0x08);
         NoCacheRam_heap_bottom = .;
      }>RAM1NOCACHE
     NoCacheRam_heap_top = NoCacheRamStart + NoCacheRamSize;

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
        LONG(msp_top - 2048)        /*第二段地址上限（不含），1024是留给中断使用的，须符合对齐要求*/
        LONG(24)                    /*第二段页尺寸=24bytes*/
        BYTE(0x73)                  /*'s'*/
        BYTE(0x79)                  /*'y'*/
        BYTE(0x73)                  /*'s'*/
        BYTE(0)                     /*串结束，"sys"是堆的名字*/

/*        . = ALIGN(0x04);*/
/*        LONG(1)        */             /*表示该堆由1段内存组成*/
/*        LONG(0)        */             /*该堆上分配的内存的对齐尺寸，0表示使用系统对齐*/
/*        LONG(0)         */            /*0=通用堆,1=专用堆,如果系统只有唯一一个堆,则只能是通用堆*/
/*        LONG(extram_heap_bottom)*/   /*第一段基址，须符合对齐要求*/
/*        LONG(extram_heap_top) */     /*第一段地址上限（不含），须符合对齐要求*/
/*        LONG(64)          */         /*第一段页尺寸=128bytes*/
/*        BYTE(0x65)         */         /*'e'*/
/*        BYTE(0x78)        */          /*'x'*/
/*        BYTE(0x74)        */          /*'t'*/
/*        BYTE(0x72)        */          /*'r'*/
/*        BYTE(0x61)        */          /*'a'*/
/*        BYTE(0x6d)        */          /*'m'*/
/*        BYTE(0)           */          /*串结束，"extram"是堆的名字*/

                . = ALIGN(0x04);
                LONG(1)                     /*表示该堆由1段内存组成*/
                LONG(0)                     /*该堆上分配的内存的对齐尺寸，0表示使用系统对齐*/
                LONG(0)                     /*0=通用堆,1=专用堆,如果系统只有唯一一个堆,则只能是通用堆*/
                LONG(NoCacheRam_heap_bottom)    /*第一段基址，须符合对齐要求*/
                LONG(NoCacheRam_heap_top)       /*第一段地址上限（不含），须符合对齐要求*/
                LONG(64)                    /*第一段页尺寸=128bytes*/
                BYTE(0x6e)                  /*'n'*/
                BYTE(0x6f)                  /*'o'*/
                BYTE(0x63)                  /*'c'*/
                BYTE(0x61)                  /*'a'*/
                BYTE(0x63)                  /*'c'*/
                BYTE(0x68)                  /*'h'*/
                BYTE(0x65)                  /*'e'*/
                BYTE(0)                     /*串结束，"nocache"是堆的名字*/
        . = ALIGN(0x04);
        LONG(0)                     /*结束标志*/
    }>InnerFlash

    Iboot_Info :
    {
        gc_ptFlashOffset = .;
        LONG(InnerFlashOffset)

        gc_ptFlashRange = .;
        LONG(InnerFlashRange)

        gc_ptIbootSize = .;
        LONG(IbootSize)

        gc_ptRevSectors = .;
        LONG(RevSectors)

        gc_ptCacheRam1Size = .;
        LONG(CacheRam1Size)

        gc_ptCacheRam2Size = .;
        LONG(CacheRam2Size)

        gc_ptNoCacheRamSize = .;
        LONG(NoCacheRamSize)
    }>InnerFlash

    App_Info :
    {
        . = ALIGN(0x08);
        gc_pAppOffset = .;          /* APP空间偏置 */
        LONG(InnerFlashOffset + IbootSize)
        gc_pAppRange = .;           /* APP空间范围 */
        LONG(InnerFlashRange - IbootSize)
    }>InnerFlash

    shell_config_section :
        {
                KEEP(*(.shellconfig))
        }>InnerFlash

    routine_shell_section :
        {
                 KEEP(* (.ro_shell_cmd))
                 KEEP(* (.ro_shell_data))
        }>InnerFlash

    expand_shell_section :
        {
             KEEP(* (.ex_shell_cmd))
             KEEP(* (.ex_shell_data))
              . = ALIGN(0x08);
        }>InnerFlash

        Shell_Info :
    {
        p_shell_info = .;
        LONG(gc_AppStart) /* 该bin文件的起始地址*/
        LONG( LENGTH(InnerFlash)-IbootSize) /* 该bin文件允许大小*/
    }>InnerFlash
}
/**** The End ****/

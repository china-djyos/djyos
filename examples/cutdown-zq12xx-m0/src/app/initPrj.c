/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�
#include "djyos.h"
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
extern ptu32_t djy_main(void);

ptu32_t __djy_main(void)
{
    djy_main();
    return 0;
}

void Sys_ModuleInit(void)
{
    uint16_t evtt_main;

    extern void Board_Init(void);
    Board_Init();

    //-------------------early-------------------------//
    extern bool_t ModuleInstall_MsgQ(void);
    ModuleInstall_MsgQ ( );

    extern bool_t ModuleInstall_Multiplex(void);
//    ModuleInstall_Multiplex ();

    extern ptu32_t ModuleInstall_IAP(void);
//    ModuleInstall_IAP( );

    //-------------------medium-------------------------//
    #if(CFG_OS_TINY == flase)
   extern s32 kernel_command(void);
   kernel_command();
#endif

    //-------------------later-------------------------//
    evtt_main = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
    __djy_main,NULL,CFG_MAINSTACK_LIMIT, "main function");
    //�¼���������������Ϊ0,�����shell����,�������ɼ�shell�����в���
    Djy_EventPop(evtt_main,NULL,0,NULL,0,0);

    #if ((CFG_DYNAMIC_MEM == true))
    extern bool_t Heap_DynamicModuleInit(void);
    Heap_DynamicModuleInit ( );
#endif

    return ;
}


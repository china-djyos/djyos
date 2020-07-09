#include "common_bt.h"
#include "common_utils.h"
#include "RomCallFlash.h"




struct rom_env_tag rom_env;

//vincent
//�������Ϊkernel_malloc����0���¸�λ��Ŀǰ�ǲ���λ�����ǻ�һֱ��ӡ��
//�����ڿ���������ʹ�ã������ʽ��������Ҫ����ԭ����λ�ķ�����
void djyos_reset_notify(uint32_t error)
{
    if (RESET_MEM_ALLOC_FAIL == error) {
        printf(".");
    }
    else {
        platform_reset(error);
    }
}

void rom_env_init(struct rom_env_tag *api)
{
    memset(api,0,sizeof(struct rom_env_tag));
    api->prf_get_id_from_task = prf_get_id_from_task;
    api->prf_get_task_from_id = prf_get_task_from_id;
    api->prf_init = prf_init;
    api->prf_create = prf_create;
    api->prf_cleanup = prf_cleanup;
    api->prf_add_profile = prf_add_profile;
    api->rwble_hl_reset = rwble_hl_reset;

    api->rwip_reset = rwip_reset;

//  api->lld_sleep_init = lld_sleep_init;

//  api->rwip_prevent_sleep_set = rwip_prevent_sleep_set;
//  api->rwip_prevent_sleep_clear = rwip_prevent_sleep_clear;

    api->platform_reset = djyos_reset_notify;//platform_reset;
}
/*
extern int _sysboot_copy_data_to_ram(void);
extern int _sysboot_zi_init(void);
void RomRunC_init(void)
{
#pragma arm
        __asm
            {
                bl _sysboot_copy_data_to_ram
                bl _sysboot_zi_init
          };


}
*/
void lld_bdaddr_init(const struct  bd_addr *addr)
{
        if(addr != NULL)
        {
            memcpy(&common_default_bdaddr, addr, sizeof(struct bd_addr));
        }

}



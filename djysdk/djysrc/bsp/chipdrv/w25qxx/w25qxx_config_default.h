//�û�ע�Ͳ����� ��InitCode����Configure����ͷ
//InitCode ��������ʱ�����´��뽫��IDE copy����ʼ�������У�����Ҫ���õģ����Կ��š��������в���ɾ��
#include "w25qxx_config.h"
extern s32 ModuleInstall_NOR(const char *DevName, u32 Flags, u8 StartBlk);
    if( ModuleInstall_NOR(CFG_W25QXX_BUS_NAME,CFG_W25QXX_FLAG,CFG_W25QXX_START_BLK) == false)
    {
        printf("W25QXX Install Error!\r\n");
        while( 1 );             //��ʼ��ʱ������ѭ������
    }

//Configure �������ó���������IDE��ͼ�λ���ʾ�������û�ͼ�λ����á��������в���ɾ��
#define CFG_W25QXX_BUS_NAME              "NOR_W25QXX"
#define CFG_W25QXX_FLAG                   (2)
#define CFG_W25QXX_START_BLK              0

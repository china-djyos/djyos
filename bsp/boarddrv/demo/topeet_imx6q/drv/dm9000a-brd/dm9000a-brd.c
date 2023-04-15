#include <stdint.h>
#include <stddef.h>

#include "cpu_peri.h"
#include "dm9000a.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_DM9000aBrd(const char *devname,u8 *mac);
//    u8 mac[] = {0xaa,0xbb,0xcc,0xdd,0xee,0x02};
//    ModuleInstall_DM9000aBrd(CFG_DM9000_DEV_NAME,mac);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"ethernet dm900a"//dm9000a������ص���������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                    //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                 //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                   //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                 //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                   //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"                //������������������������none����ʾ�������������
                                   //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                   //����������������������г����á�,���ָ�
//weakdependence:"none"            //��������������������������none����ʾ�������������
                                   //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                   //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                   //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_ETHERNET_DM900A == false )
//#warning  " ethernet_dm900a  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header              //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_ETHERNET_DM900A    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_DM9000_DEV_NAME              "MAC_DM9000"         //"name",������������
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

// =============================================================================
// ���ܣ�dm9000a��ʼ����������صĲ��֡�
//      DM9000�жϳ�ʼ����DM9000�ж����Žӵ�CPU��GPIO���ţ�����轫����������Ϊ�ⲿ
//      �жϣ������ô�����ƽ��ͬʱ�������ж��ߵ��ж�ϵͳ
// ��������
// ����ֵ��
// =============================================================================
static bool_t __ConfigDm9000IO(void)
{
    struct SMC_REG *smc =SMC_REG_BASE;

    smc->SMBCR4 =(7<<22)|(1<<7)|(1<<4);  //Bank4 16bit
    //�����ⲿ����EINT4/GPF4
    pg_gpio_reg->GPFCON &= ~(3<<8);
    pg_gpio_reg->GPFCON |= (2<<8);     //����Ϊ����
    pg_gpio_reg->EXTINT0 &= ~(7<<16);
    pg_gpio_reg->EXTINT0  |= (4<<16);   //���������ش���
    pg_gpio_reg->EINTPEND |= (1<<4);
    pg_gpio_reg->EINTMASK &= (~(01<<4));

    return true;
}

static bool_t __ClearDm9000CpuInt(u32 irqno)
{
    if(pg_gpio_reg->EINTPEND & (1<<4))              //DM9000a�ⲿ�жϱ�־
    {
        pg_gpio_reg->EINTPEND |= (1<<4);            //���ⲿ�жϱ�־
    }

    Int_ClearLine(irqno);

    return true;
}

#define CN_DMA9000A_CMDADDR    ((u16 *)0x20000000)
#define CN_DMA9000A_DATADDR    ((u16 *)(0x20000000|(0xf<<1)))
//----������װ����------------------------------------------------------------
//���ܣ���װ������
//��������
//���أ�true = �ɹ���װ��false = ��װʧ��
//----------------------------------------------------------------------------
bool_t ModuleInstall_DM9000aBrd(const char *devname,u8 *mac)
{
    tagDm9000Para  para;

    __ConfigDm9000IO();
    //����ж��dm9000a������һ�¹����ظ���μ���
    //dm9000a.h��û�и������������ϡ������š�����������Ǵ�Ч�ʿ��ǡ�
    //�������ж������ʱ����4������Ҫʵ��4�ݣ������嶼��С���ռ�ռ�ò�������
    //���ӣ���ִ��ȴ�����ࡣ
    para.devname = devname;
    para.mac = mac;
    para.clearextint = __ClearDm9000CpuInt;
    para.irqno = CN_INT_LINE_EINT4_7;
    para.cmdaddr = CN_DMA9000A_CMDADDR;
    para.dataddr = CN_DMA9000A_DATADDR;

    return Dm9000Install(&para);
}

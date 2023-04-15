#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "keyboard.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_Keyboard_TQ2416(const char *dev_name);
//    ModuleInstall_Keyboard_TQ2416(CFG_DM9000_DEV_NAME);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"keyboard hard driver"//����ļ�������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                   //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                  //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                  //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                  //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                  //����������������������г����á�,���ָ�
//weakdependence:"none"           //��������������������������none����ʾ�������������
                                  //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                  //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                  //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_KEYBOARD_HARD_DRIVER == false )
//#warning  " keyboard_hard_driver  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header             //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_KEYBOARD_HARD_DRIVER    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_KEYBOARD_NAME              "TQ2416_KB"               //"name",���ü�������
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

bool_t key_hard_init(void);
u32 keyboard_scan(void);
//----��ʼ������ģ��-----------------------------------------------------------
//����: ��ʼ��һ����windows�ļ��̺Ͱ�ťģ��ļ��̣��ü��̹�8������
//����: ��
//����: ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_Keyboard_TQ2416(const char *dev_name)
{
    static struct KeyBoardPrivate key_brd;

    key_hard_init();

    key_brd.read_keyboard = keyboard_scan;
    Keyboard_InstallDevice(dev_name,&key_brd);
    key_brd.vtime_limit = 0;
    key_brd.vtime_count = 100;
    key_brd.key_bak = 0;
    key_brd.key_now = 0;
    return true;
}


//----��ʼ������Ӳ��-----------------------------------------------------------
//����: ���⣬��ʼ�������ɨ�����
//����: ��
//����: ��
//----------------------------------------------------------------------------
bool_t key_hard_init(void)
{
/* K1:GPF7
 * K2:GPG1
 * K3:GPG2
 * K4:GPG3
 */
    struct GPIO_REG *gpio =GPIO_REG_BASE;

    //GPF7
    gpio->GPFCON &= ~((0x3<<14));
    gpio->GPFUDP &= ~((0x3<<14));
    gpio->GPFUDP |=  ((0x2<<14));

    //GPG1,2,3
    gpio->GPFCON &= ~((0x3<<6)|(0x3<<4)|(0x3<<2));
    gpio->GPFUDP &= ~((0x3<<6)|(0x3<<4)|(0x3<<2));
    gpio->GPFUDP |=  ((0x2<<6)|(0x2<<4)|(0x2<<2));


    return true;
}


//----����Ӳ��ɨ��------------------------------------------------------------
//����: ��ȡ���̰���״̬��ÿ��������8bit��ʾ��32λ����ֵ���Դ��4��������ɨ�赽
//      �ĵ�һ�������������8λ�����ĸ������������8λ��ɨ�赽4�������󣬲��ټ�
//      ������ͬʱ���³���4��������ֻ��¼4����
//����: ��
//����:  �μ� struct KeyBoardPrivate��ע��
//----------------------------------------------------------------------------
u32 keyboard_scan(void)
{
    u32 readed;
    uint8_t i = 0;

    readed = 0;
    if( ! (pg_gpio_reg->GPFDAT & 0x80))
    {
        readed |= (u32)(1<<(i<<3));
        i++;
    }
    if( ! (pg_gpio_reg->GPGDAT & 0x2))
    {
        readed |= (u32)(2<<(i<<3));
        i++;
    }
    if( ! (pg_gpio_reg->GPGDAT & 0x4))
    {
        readed |= (u32)(3<<(i<<3));
        i++;
    }
    if( ! (pg_gpio_reg->GPGDAT & 0x8))
    {
        readed |= (u32)(4<<(i<<3));
        i++;
    }
    return(readed);
}

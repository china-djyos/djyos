#include <stdint.h>
#include <stdbool.h>

#include <stddef.h>
#include <os.h>
#include <cpu_peri.h>
#include <uartctrl.h>

#include "project_config.h"

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"board config"//�����������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                       //ѡ�third��system��bsp��user����������������IDE�з���
//select:required                     //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                      //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                     //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                      //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"kernel","at32f43x","cpu onchip gpio"//������������������������none����ʾ�������������
                                      //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                      //����������������������г����á�,���ָ�
//weakdependence:"none"               //��������������������������none����ʾ�������������
                                      //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                      //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                      //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_BOARD_CONFIG == false )
//#warning  " board_config  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_BOARD_CONFIG    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,flase,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


bool_t Board_UartGpioInit(u8 SerialNo)
{
    uart_print_init(115200);
    return true;
}


void Board_Init(void)
{
    Board_UartGpioInit(CN_UART1);
}



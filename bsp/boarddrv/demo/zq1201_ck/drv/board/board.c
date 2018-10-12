#include "csi_core.h"
#include "cpu_peri.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_myname.h****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern void Board_GpioInit(void);
//    Board_GpioInit();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"board"       //�����������
//parent:"none"                              //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���                         //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                               //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                            //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                          //��ʼ��ʱ������ѡֵ��early��medium��later��
                                            //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"                //������������������������none����ʾ�������������
                                            //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                            //����������������������г����á�,���ָ�
//weakdependence:"none"                     //��������������������������none����ʾ�������������
                                            //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                            //����������������������г����á�,���ָ�
//mutex:"none"                              //������������������������none����ʾ�������������
                                            //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header                       //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

#define LED1        IO_CONFIG_PB8
#define LED2        IO_CONFIG_PA6

void Board_GpioInit(void)
{
    silan_io_driver_config(LED1, IO_ATTR_DS_8mA);
    silan_io_func_config(LED1, IO_FUNC_GPIO);
    silan_io_output(LED1);
    silan_io_set_low(LED1);
    
    silan_io_driver_config(LED2, IO_ATTR_DS_8mA);
    silan_io_func_config(LED2, IO_FUNC_GPIO);
    silan_io_output(LED2);
    silan_io_set_low(LED2);
}


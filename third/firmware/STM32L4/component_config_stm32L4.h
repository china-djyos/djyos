
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"stm32L4"      //ԭ���̼���
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:third               //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = cmdline   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//#ifndef CFG_STATIC_IP   //****�������Ƿ��Ѿ����ú�
//#warning    ���������á��������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100
//%$#@enum,1,2,3,4,5,6,7,8
#define CFG_STM32_HAL_TIMER     6//����HAL��ʹ�õĶ�ʱ��
//%$#@string,1,16
//%$#@select,2
#define     USE_HAL_DRIVER          //�Ƿ�ʹ��hal��
//#define STM32L431xx             //ֻ��ѡ������һ��
//#define STM32L432xx
//#define STM32L433xx
//#define STM32L442xx
//#define STM32L443xx
//#define STM32L451xx
//#define STM32L452xx
//#define STM32L462xx
//#define STM32L471xx
//#define STM32L475xx
//#define STM32L476xx
//#define STM32L485xx
//#define STM32L486xx
//%$#@free
//#endif
//%$#@end configue  ****�������ý���
//%$#@exclude
//%$#@end exclude
//%$#@include path
//..;..\inc;
//%$#@end include path


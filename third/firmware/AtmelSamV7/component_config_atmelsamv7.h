
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"atsamv7"      //ԭ���̼���
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:���������           //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                   //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:none                    //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
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
//%$#@enum,true,false
//%$#@string,1,16
//%$#@select,1
//#defined __SAMV71J19__          //ֻ��ѡ������һ��
//#defined __SAMV71J20__
//#defined __SAMV71J21__
//#defined __SAMV71N19__
//#defined __SAMV71N20__
//#defined __SAMV71N21__
//#defined __SAMV71Q19__
//#defined __SAMV71Q20__
#defined __SAMV71Q21__
//%$#@free
//#endif
//%$#@end configue  ****�������ý���
//%$#@exclude
//../source/gmac.c
//../source/gmacd.c
//../source/iso7816_4.c
//../source/mcan.c
//../source/mcid_dma.c
//../source/pio_capture.c
//../source/pio_it.c
//../source/pio.c
//../source/qspi_dma.c
//../source/qspi.c
//../source/sdramc.c
//../source/smc.c
//../source/twi.c
//../source/twid.c
//../source/uart_dma.c
//../source/uart.c
//../source/usart_dma.c
//../source/usart.c
//../source/xdma_hardware_interface.c
//../source/xdmac.c
//../source/xdmad.c
//%$#@end exclude
//%$#@include path
//..;..\include;
//..;..\include\samv7;
//%$#@end include path


#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    struct GkWinObj;
//    extern void    ModuleInstall_GDD(struct GkWinObj *desktop);
//    extern struct GkWinObj *GK_CreateDesktop(const char *DisplayName,
//                                      const char *DesktopName,
//                                      s32 width,s32 height,u32 color,
//                                      u32 buf_mode,u16 PixelFormat,
//                                      u32 BaseColor);
//    struct GkWinObj *desktop;
//    desktop = GK_CreateDesktop(CFG_DISPLAY_NAME,CFG_DESKTOP_NAME,
//                                CFG_DESKTOP_WIDTH,CFG_DESKTOP_HEIGHT,
//                                CFG_FILL_COLOR,CN_WINBUF_PARENT,
//                                CFG_DESKTOP_FORMAT,CFG_GRAY_BASE_COLOR);
//    if(desktop == NULL)
//    {
//        printf("�����������");
//        while( 1 );             //��ʼ��ʱ������ѭ������
//    }
//    else
//    {
//        ModuleInstall_GDD(desktop);
//        GDD_AddInputDev(CFG_INPUTDEV_NAME);
//    }
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"gdd"                      //��д�����������
//parent:"none"                             //��д������ĸ�������֣�none��ʾû�и����
//attribute:�������                         //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                               //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                            //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:main                                //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                            //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
//dependence:"gkernel","MsgQueue"           //������������������������none����ʾ�������������
                                            //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                            //����������������������г����á�,���ָ�
//weakdependence:"HmiInput"                 //��������������������������none����ʾ�������������
                                            //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                            //����������������������г����á�,���ָ�
//mutex:"none"                              //������������������������none����ʾ�������������
                                            //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_DESKTOP_WIDTH        //****�������Ƿ��Ѿ����ú�
#warning    gdd�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100,
#define CFG_DESKTOP_WIDTH       0           //"������",����ߴ磨����������ȣ�0=��ʾ�����
#define CFG_DESKTOP_HEIGHT      0           //"����߶�",����ߴ磨���������߶ȣ�0=��ʾ���߶�
//%$#@enum,true,false,
//%$#@string,1,256,
#define CFG_DISPLAY_NAME        " "         //"��ʾ����",����bsp����ʾ������ģ�����õ���ʾ��������ͬ
#define CFG_DESKTOP_NAME        " "         //"������"
#define CFG_INPUTDEV_NAME       " "         //"�����豸����",ʹ��bsp�������豸�����õ����֣��������豸�Ļ���ÿ���豸���ö��Ÿ���
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#define CFG_DESKTOP_FORMAT      CN_SYS_PF_RGB565    //"���ظ�ʽ",���洰�����ظ�ʽ��������gkernel.h�ж��壬һ��ʹ������ʾ����ͬ��ɫ
#define CFG_GRAY_BASE_COLOR     CN_COLOR_WHITE      //"�ҶȻ�ɫ",���ظ�ʽ��Ϊ�Ҷ�ʱ����Ҫ���õġ�������ɫ������gkernel.h���ҵ�������ɫ����
#define CFG_FILL_COLOR          CN_COLOR_BLUE       //"���ɫ",��������ʱ�����ɫ����888��ʽ������gkernel.h���ҵ�������ɫ����
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

#include <mbport.h>
#include <user_mb_app.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_Modbus(void);
//    ModuleInstall_Modbus();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"modbus" //�ں˶���ϵͳ
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:third              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:pre-main                //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","Software Timers"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_MODBUS == false )
//#warning  " kernel_object_system  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_MODBUS    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,1,65536
#define CFG_MB_ASCII_TIMEOUT_SEC                    1       //"�ӻ���ASCIIʱ��ͨ�ŵĳ�ʱʱ��",ʵ�ʵĳ�ʱʱ����CFG_MB_ASCII_TIMEOUT_SEC*20000*50
#define CFG_MB_FUNC_HANDLERS_MAX                    16      //"Э��ջӦ��֧�ֵ�Modbus��������������"
#define CFG_MB_FUNC_OTHER_REP_SLAVEID_BUF           32      //"�ӻ�ID���ݳ���",������������˴ӻ�ID���ݵ�����С��
#define CFG_MB_MASTER_DELAY_MS_CONVERT              200     //"�ȴ��ӳ�ʱ��",��վ����һ���㲥֡����վ�ȴ��ӳ�ʱ�䣬ʱ�䵽�����վ�ſ��Է�������֡
#define CFG_MB_MASTER_TIMEOUT_MS_RESPOND            100     //"�ӻ���Ӧ��ʱʱ��"
#define CFG_MB_MASTER_TOTAL_SLAVE_NUM               16      //"�ӻ�����"
//%$#@enum,true,false
#define CFG_MB_FUNC_OTHER_REP_SLAVEID_ENABLED       true    // "�Ƿ���������ӻ�ID����"
#define CFG_MB_FUNC_READ_INPUT_ENABLED              true    // "�Ƿ����ö�����Ĵ�������"
#define CFG_MB_FUNC_READ_HOLDING_ENABLED            true    // "�Ƿ����ö����ּĴ�������"
#define CFG_MB_FUNC_WRITE_HOLDING_ENABLED           true    // "�Ƿ�����д�����Ĵ�������"
#define CFG_MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED  true    // "�Ƿ�����д����Ĵ�������"
#define CFG_MB_FUNC_READ_COILS_ENABLED              true    // "�Ƿ����ö���Ȧ����"
#define CFG_MB_FUNC_WRITE_COIL_ENABLED              true    // "�Ƿ�����д��Ȧ����"
#define CFG_MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED    true    // "�Ƿ�����д�����Ȧ����"
#define CFG_MB_FUNC_READ_DISCRETE_INPUTS_ENABLED    true    // "�Ƿ����ö���ɢ���빦��"
#define CFG_MB_FUNC_READWRITE_HOLDING_ENABLED       true    // "�Ƿ����ö�д����Ĵ�������"

//%$#@enum,true,false
#define CFG_MB_MASTER_ENABLED                       false    // "����ʹ�ܿ���"
    //%$#@enum,true,false
    #define CFG_MB_MASTER_RTU_ENABLED                   false    // "����RTUʹ�ܿ���"
    //%$#@num,0,65535
    #define CFG_MB_M_DISCRETE_INPUT_START               1        //"������ɢ���������ʼ��ַ"
    #define CFG_MB_M_DISCRETE_INPUT_NDISCRETES          10       //"������ɢ�����������"
    #define CFG_MB_M_COIL_START                         11       //"������Ȧ����ʼ��ַ"
    #define CFG_MB_M_COIL_NCOILS                        10       //"������Ȧ������"
    #define CFG_MB_M_REG_INPUT_START                    21       //"��������Ĵ�������ʼ��ַ"
    #define CFG_MB_M_REG_INPUT_NREGS                    10       //"��������Ĵ���������"
    #define CFG_MB_M_REG_HOLDING_START                  31       //"�������ּĴ�������ʼ��ַ"
    #define CFG_MB_M_REG_HOLDING_NREGS                  10       //"�������ּĴ���������"
    #define CFG_MB_M_PORT_NUM                           2        //"���ڵĶ˿ںţ�����Ҫ��/dev/UART2������2"
    //%$#@enum,110,300,600,1200,2400,4800,9600,14400,19200,38400,57600,115200,230400,380400,460800,921600
    #define CFG_MB_M_PORT_BAUDRATE                      115200    // "���ڲ�����"
    //%$#@enum,MB_PAR_NONE,MB_PAR_ODD,MB_PAR_EVEN
    #define CFG_MB_M_PORT_PARITY                        MB_PAR_NONE    // "У�鷽ʽ"

//%$#@enum,true,false
#define CFG_MB_SLAVE_ENABLED                       false    // "�ӻ�ʹ�ܿ���"
    //%$#@enum,true,false
    #define CFG_MB_SLAVE_ASCII_ENABLED                  false    // "�ӻ�ASCIIʹ�ܿ���"
    #define CFG_MB_SLAVE_RTU_ENABLED                    false    // "�ӻ�RTUʹ�ܿ���"
    #define CFG_MB_SLAVE_TCP_ENABLED                    false    // "�ӻ�TCPʹ�ܿ���"

    //%$#@num,1,65536
    #define CFG_MB_S_DISCRETE_INPUT_START               1        //"�ӻ���ɢ���������ʼ��ַ"
    #define CFG_MB_S_DISCRETE_INPUT_NDISCRETES          10       //"�ӻ���ɢ�����������"
    #define CFG_MB_S_COIL_START                         11       //"�ӻ���Ȧ����ʼ��ַ��"
    #define CFG_MB_S_COIL_NCOILS                        10       //"�ӻ���Ȧ������"
    #define CFG_MB_S_REG_INPUT_START                    21       //"�ӻ�����Ĵ�������ʼ��ַ"
    #define CFG_MB_S_REG_INPUT_NREGS                    10       //"�ӻ�����Ĵ���������"
    #define CFG_MB_S_REG_HOLDING_START                  31       //"�ӻ����ּĴ�������ʼ��ַ"
    #define CFG_MB_S_REG_HOLDING_NREGS                  10       //"�ӻ����ּĴ���������"
    #define CFG_MB_S_ADDR                               2        //"�ӻ���ַ"
    #define CFG_MB_S_PORT_NUM                           2        //"���ڵĶ˿ںţ�����Ҫ��/dev/UART2������2"�����ڵ��������ԣ���ʹ��RTU��ASCIIģʽʱ����Ч
    //%$#@num,0,65536
    #define CFG_MB_S_TCP_PORT_NUM                       0        //"ʹ��TCPʱ��ʹ�õĶ˿ںţ�0��ʾʹ��Ĭ�϶˿ڣ�502����ʹ��TCPģʽʱ����Ч"
    //%$#@enum,110,300,600,1200,2400,4800,9600,14400,19200,38400,57600,115200,230400,380400,460800,921600
    #define CFG_MB_S_PORT_BAUDRATE                      115200    // "���ڲ�����"
    //%$#@enum,MB_PAR_NONE,MB_PAR_ODD,MB_PAR_EVEN
    #define CFG_MB_S_PORT_PARITY                        MB_PAR_NONE    // "У�鷽ʽ"
    //%$#@enum,MB_RTU_MODE,MB_TCP_MODE,MB_ASCII_MODE
    #define CFG_MB_S_TRANSFER_MODE                      MB_RTU_MODE    // "ͨ�ŷ�ʽ"
//%$#@string,1,10
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****�������ý���

//%$#@API include path         ****APIͷ�ļ�·���б��÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ�����Ӧ�ñ���ʱ��DIDE���Զ��ѱ��б��е�ͷ�ļ��ӵ� -I ����·����
//./include;./ascii;./port;./rtu;./tcp;
//%$#@end API include path

//%$#@self include path         ****����������Ҫ��ͷ�ļ�·���б��÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ��������뱾��ʱ��DIDE���Զ��ѱ��б��е�ͷ�ļ��ӵ� -I ����·����
//%$#@end self include path

//@#$%component end configure

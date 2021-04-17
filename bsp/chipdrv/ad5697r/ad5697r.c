// =============================================================================

// �ļ���     ��ad5697r.c
// ģ������: �¶ȼ��оƬmax31865������
// ģ��汾: V1.00
// ������Ա:
// ����ʱ��: 1/20.2017
// =============================================================================

#include "string.h"
#include "os.h"
#include "iicbus.h"
#include "cpu_peri.h"
#include "ad5697r.h"
//#include "ad5697r_config.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t AD5697R_ModuleInit(void)
//    AD5697R_ModuleInit();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"ADC chip ad5697r"//12bit DACת��
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","iicbus","cpu onchip iic"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_ADC_CHIP_AD5697R == false )
//#warning  " ADC_chip_ad5697r  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_ADC_CHIP_AD5697R    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0��0xFFFFFFFF,
#define CFG_AD5697R_TIMEOUT        (-1)   //"��ʱʱ��",-1��ʾ����
//%$#@num,,,
#define CFG_AD5697R_CLK_FRE        (50*1000)            //"�����ٶ�",��λHz
//%$#@enum,0x01,0x08,0x0D
#define CFG_AD5697R_ADDR           0x0D                 //"�豸��ַ",
//%$#@string,1,10,
#define CFG_AD5697R_BUS_NAME              "I2C0"        //"name",�����ص���������,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

// =============================================================================
//#define CFG_AD5697R_ADDR           0x0D            //�豸��ַ
//#define CFG_AD5697R_CLK_FRE        (50*1000)      //�����ٶȣ���λHz
//#define CFG_AD5697R_TIMEOUT        CN_TIMEOUT_FOREVER

static struct SemaphoreLCB AD5697R_Semp;   //оƬ������ʱ���

//����IICBUS�ܹ��µ�IIC�豸�ṹ
static struct IIC_Device *s_ptAD5697R_Dev = NULL;
static u32 s_AD5697R_Timeout = CN_TIMEOUT_FOREVER;
static bool_t sAD5697RInited = false;

//static const Pin AD5697R_LDAC_pin[] = {
//      {PIO_PD25, PIOD, ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT},
//};

// =============================================================================
void __AD5697R_GpioInit(void)
{
    //һ��ὫLDAC����������Ϊ�͵�ƽ����board.c��������
//  PIO_Configure(AD5697R_LDAC_pin,1);
}

// =============================================================================
// ���ܣ�DA2���
// ������DacValue2��DAͨ��2��DAֵ
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t AD5697R_Output1(u16 DacValue1)
{
    u8 cmd,value[2];

    if(!sAD5697RInited)
    {
        return false;
    }

    if(false == Lock_SempPend(&AD5697R_Semp,25*mS))
    {
        return false;
    }

    cmd = 0x31;
    DacValue1 = (DacValue1 << 4) & 0xFFF0;
    value[0] = (DacValue1 & 0xFF00) >> 8;
    value[1] = (DacValue1 & 0x00FF) >> 0;

    IIC_Write(s_ptAD5697R_Dev,cmd,value,2,true,s_AD5697R_Timeout);

    Lock_SempPost(&AD5697R_Semp);
    return true;
}

// =============================================================================
// ���ܣ�DA1���
// ������DacValue1��DAͨ��1��DAֵ
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t AD5697R_Output2(u16 DacValue2)
{
    u8 cmd,value[2];

    if(!sAD5697RInited)
    {
        return false;
    }

    if(false == Lock_SempPend(&AD5697R_Semp,25*mS))
    {
        return false;
    }

    cmd = 0x38;
    DacValue2 = (DacValue2 << 4) & 0xFFF0;
    value[0] = (DacValue2 & 0xFF00) >> 8;
    value[1] = (DacValue2 & 0x00FF) >> 0;

    IIC_Write(s_ptAD5697R_Dev,cmd,value,2,true,s_AD5697R_Timeout);

    Lock_SempPost(&AD5697R_Semp);
    return true;
}

// =============================================================================
// ���ܣ�DA�������ͨ��ͬʱ����
// ������DacValue1��DAͨ��1��DAֵ
//      DacValue2��DAͨ��2��DAֵ
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t AD5697R_OutputAll(u16 DacValue1,u16 DacValue2)
{
    u8 cmd,value[2];

    if(!sAD5697RInited)
    {
        return false;
    }

    if(false == Lock_SempPend(&AD5697R_Semp,25*mS))
    {
        return false;
    }

    if(DacValue1 == DacValue2)
    {
        cmd = 0x39;
        DacValue2 = (DacValue2 << 4) & 0xFFF0;
        value[0] = (DacValue2 & 0xFF00) >> 8;
        value[1] = (DacValue2 & 0x00FF) >> 0;

        IIC_Write(s_ptAD5697R_Dev,cmd,value,2,true,s_AD5697R_Timeout);
    }
    else
    {
        AD5697R_Output1(DacValue1);
        AD5697R_Output2(DacValue2);
    }

    Lock_SempPost(&AD5697R_Semp);
    return true;
}

// =============================================================================
// ���ܣ�оƬ��ʼ��
// ��������
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t AD5697R_ModuleInit(void)
{
    //GPIO��ʼ����SDA��SCL�Ѿ���IIC�г�ʼ���ˣ��˴�ֻ���ʼ��WP����
    __AD5697R_GpioInit();

    if(NULL == Lock_SempCreate_s(&AD5697R_Semp,1,1,CN_BLOCK_FIFO,"AD5697R_semp"))
        return false;

    //���FM24CL64��IIC0����
    s_ptAD5697R_Dev = IIC_DevAdd(CFG_AD5697R_BUS_NAME,"AD5697R",CFG_AD5697R_ADDR,0,8);
    if(s_ptAD5697R_Dev)
    {
        IIC_BusCtrl(s_ptAD5697R_Dev,CN_IIC_SET_CLK,CFG_AD5697R_CLK_FRE,0);
        IIC_BusCtrl(s_ptAD5697R_Dev,CN_IIC_SET_POLL,0,0);

        sAD5697RInited = true;
    }
    else
    {
        Lock_SempDelete_s(&AD5697R_Semp);
        sAD5697RInited = false;
    }
    return sAD5697RInited;
}

// =============================================================================

// �ļ���     ��max11410.c
// ģ������: ADоƬmax11410������
// ģ��汾: V1.00
// ������Ա: czz
// ����ʱ��: 9/8.2017
// =============================================================================

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "os.h"
#include "spibus.h"
#include "cpu_peri.h"
#include "max11410.h"
#include "math.h"
#include "shell.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//   extern bool_t ModuleInstall_Max11410(void);
//   ModuleInstall_Max11410();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"max11410"//spi�ӿ�24λ��-�� ADC
//parent                       //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"spi bus","lock","cpu onchip spi"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_MAX11410 == false )
//#warning  " max11410  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_MAX11410    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_MAX11410_BUS_NAME              "SPI0"       //"SPI��������",��������ʹ�õ�SPI�������ƣ�
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

static struct SPI_Device *s_ptMax_Dev;
static u32 s_Max_Timeout = CN_TIMEOUT_FOREVER;
#define Max_SPI_SPEED      (400*1000)
static bool_t sMaxInited = false;
static bool_t Max11410_config = true;

#define ID_Date  0xf02

const static double A =(0.0039083);
const static double B =(-0.0000005775);
__attribute__((weak))  const u32 Max11410_ref = (4000);
__attribute__((weak))  const bool_t Max11410_RxAadjust = false;
struct Max11410_CfgTab config;
static struct SemaphoreLCB Max_Semp;         //оƬ������ʱ���


__attribute__((weak)) bool_t Max11410_Init(struct Max11410_CfgTab *config)
{
    Max11410_config = false;
    return false;
}
// =============================================================================
// �������ܣ���оƬ���ݽ���
// �����������
// ����ֵ  ��
// =============================================================================
static u32 Max11410_TxRx(u8* sdata,u32 slen,u8* rdata, u32 rlen,u32 RecvOff)
{
    struct SPI_DataFrame data;
    s32 result;
    if(rlen != 0)
        *sdata |=0x80;
    else
        *sdata &=~0x80;
    data.RecvBuf = rdata;
    data.RecvLen = rlen;
    data.RecvOff = RecvOff;
    data.SendBuf = sdata;
    data.SendLen = slen;
    SPI_CsActive(s_ptMax_Dev,s_Max_Timeout);
    result = SPI_Transfer(s_ptMax_Dev,&data,true,s_Max_Timeout);
    SPI_CsInactive(s_ptMax_Dev);
    if(result != CN_SPI_EXIT_NOERR)
        return 0;
    return 1;
}


//=============================================================================
//���ܣ���ȡоƬID
//������������
//���أ�ID
//=============================================================================
static u32 Max11410_GetID(void)
{
    u8 addr = Max11410_PART_ID;
    u8 rdid[3];
    u32 ID;
    Max11410_TxRx(&addr,1,rdid,3,1);
    ID = (rdid[0]<<16)|(rdid[1]<<8)|(rdid[2]);
    return ID;
}

//=============================================================================
//���ܣ���ȡоƬID
//������������
//���أ�ID
//=============================================================================
static u8 Max11410_GetuC(void)
{
    u8 addr = Max11410_uCADDR;
    u8 uCaddr;
    Max11410_TxRx(&addr,1,&uCaddr,1,1);
    return uCaddr;
}
//=============================================================================
//���ܣ�����Max11410���ٰ���ΪĬ�ϳ�ʼֵ
//������������
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
//=============================================================================
static bool_t Max11410_Config(struct Max11410_CfgTab *pt_config)
{
    u8 i;

    for(i = 0;i<10;i++)
    {
        DJY_DelayUs(10*mS);
        if(ID_Date == Max11410_GetID())
            break;
    }
    if(i == 10)
    {
        printf("MAX11410 ID Error!!\n\r");
        return false;
    }
    for(i=0;i<pt_config->Reg16BitNum;i++)
    {
        Max11410_TxRx((u8*)&pt_config->pt_Reg16Bit[i],sizeof(struct Init_Reg16Bit),NULL,0,0);
    }
    for(i=0;i<pt_config->Reg8BitNum;i++)
    {
        Max11410_TxRx((u8*)&pt_config->pt_Reg8Bit[i],sizeof(struct Init_Reg8Bit),NULL,0,0);
    }
    for(i = 0;i<10;i++)
    {
        DJY_DelayUs(10*mS);
        if(0 != Max11410_GetuC())
        {
            sMaxInited = true;
            return true;
        }
    }
    printf("max11410 config error !!\n\r");
    return false;
}

// =============================================================================
// �������ܣ����¶�(ע�⣺��������Ϊfloat���������������򷵻�ֵ����)
// �����������
// ����ֵ  ���¶�ֵ��Ϊfloat���ͣ�TEMP_ERROR����ʾ����
// =============================================================================
float Max11410_TemperatureGet(u8 temp_x)
{
    float temp;
    u8 RegMatrix;
    u8 Data[3] = {0x00,0x00,0x00};
    u32 DATA;
    float RX,Rxadjust;
    double num;

    if((!sMaxInited))
    {
        printf("MA11410 Init Error !!  \n\r");
        return TEMP_ERROR;
    }
    if((temp_x>4))
    {
        printf("MA11410 Channel Error !!  \n\r");
        if(!Max11410_Config(&config))
            return TEMP_ERROR;
    }
    if((false == Lock_SempPend(&Max_Semp,25*mS)))
        return -1;

    RegMatrix = Max11410_DATA0+temp_x-1;
    Max11410_TxRx(&RegMatrix,1,Data,3,1);
    DATA = ((Data[0]<<16)|(Data[1]<<8)|(Data[2]));

    if((DATA != 0xfffffff)&&(DATA != 0x0))
    {
        RX = ((float)DATA/0xffffff)*Max11410_ref;
        if(Max11410_RxAadjust)
        {
            RegMatrix = Max11410_DATA0+temp_x-1+4;
            Max11410_TxRx(&RegMatrix,1,Data,3,1);
            DATA = ((Data[0]<<16)|(Data[1]<<8)|(Data[2]));
            Rxadjust = ((float)DATA/0xffffff)*Max11410_ref;
            RX = (RX- Rxadjust)+1000;
//            printf("RX%d=%.4f aj=%.4f  ",temp_x,RX,Rxadjust);
        }
        num = sqrt(A*A-4*B*((1000-RX)/1000));
        temp = (num-A)/(2*B);
    }
    else
    {
        temp =  TEMP_ERROR;
    }
    temp = ((-50<temp) && (temp<100)) ? temp : (TEMP_ERROR);
    Lock_SempPost(&Max_Semp);
    return temp;

}
//=============================================================================
//���ܣ� shell ���
//������
//���أ�
//=============================================================================
//static bool_t Max11410_shell(char *param)

bool_t pt(char *param)
{
    ptu32_t ch;
    char *word,*next_param;
    float temp;

    word = shell_inputs(param,&next_param);

    if(0==strcmp(word,"info"))
    {
        if(sMaxInited)
            printf("pt ��ʼ����� !!\n\r" );
        else
            printf("pt ��ʼ��ʧ�� !!\n\r" );
        if(Max11410_RxAadjust)
            printf("pt ������У׼״̬ !! \n\r" );
        else
            printf("pt �����ڷ�У׼״̬ !!\n\r" );
        printf("uC Addr = 0x%x \n\r",Max11410_GetuC());
        if(!Max11410_config)
            printf("û���ҵ�������Ϣ!!\n\r" );

    }
    else if(0==strcmp(word,"get"))
    {
        word = shell_inputs(next_param,&next_param);
        ch = strtoul(word, (char **)NULL, 0);
        temp =  Max11410_TemperatureGet(ch);
        if (temp == TEMP_ERROR)
            printf("ͨ��%d �¶ȶ�ȡʧ��   !!  \n\r",ch);
        else
            printf("temp%d = %.4f ��C !!  \n\r",ch,temp);
    }
    else
    {
        printf("����'help pt' ��ȡ���� .\n\r");
    }

    return true;
}

//=============================================================================
//���ܣ���ʼ��Max11410оƬ��������������ģʽ����������ģʽΪ��������
//������������
//     BusName,�ܽ����ƣ��硰SPI1��
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
//=============================================================================
bool_t ModuleInstall_Max11410(void)
{

    if(NULL == Lock_SempCreate_s(&Max_Semp,1,1,CN_BLOCK_FIFO,"MAX11410_semp"))
        return false;

    if(Max11410_Init(&config) ==false)
        return false;

    if(s_ptMax_Dev = SPI_DevAdd(CFG_MAX11410_BUS_NAME,"MAX11410",0,8,SPI_MODE_0,SPI_SHIFT_MSB,Max_SPI_SPEED,false))
    {
        SPI_BusCtrl(s_ptMax_Dev,CN_SPI_SET_POLL,0,0);

        if(Max11410_Config(&config))
        {
            sMaxInited = true;
        }
    }
    else
    {
        printf("MAX11410 init failed.\n\r");
        sMaxInited = false;
    }
    return sMaxInited;
}
ADD_TO_ROUTINE_SHELL(pt,pt,"���¶�оƬ�������   pt info ��ȡmax11410�Ĺ���״̬  .  pt get x  ��ȡ��x��ͨ�����¶�ֵ x = 1,2... ");

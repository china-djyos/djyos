// =============================================================================

// �ļ���     ��max31865.c
// ģ������: �¶ȼ��оƬmax31865������
// ģ��汾: V1.00
// ������Ա:
// ����ʱ��: 1/20.2017
// =============================================================================

#include "string.h"
#include "os.h"
#include "spibus.h"
#include "cpu_peri.h"
#include "max31865.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_Max31865(void);
//    ModuleInstall_Max31865();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"max31865"//spi�ӿڵĲ���оƬ
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
#if ( CFG_MODULE_ENABLE_MAX31865 == false )
//#warning  " max31865  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_MAX31865    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,100000,5000000,
#define CFG_MAX3_SPI_SPEED                 (200*1000)//"ʱ���ٶ�"������max31865��ʹ�õ�spi�����ٶ�
//%$#@num,100000,0xFFFFFFFF,
//%$#@enum,MAX31865_CONTINOUS_MODE,MAX31865_SINGLE_MODE,
#define CFG_MAX31865_SAM_MODE              MAX31865_CONTINOUS_MODE   //"����ģʽ",���������򵥴β���
//%$#@string,1,10,
#define CFG_MAX31865_BUS_NAME              "SPI0"       //"SPI����",MAX31865��ʹ��SPI��������
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#define CFG_MAX3_TIMEOUT                   CN_TIMEOUT_FOREVER//"��ʱʱ��"�����ȴ���ʱ��
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

static struct SPI_Device *s_ptMax3_Dev;
static u32 s_Max3_Timeout = CFG_MAX3_TIMEOUT;
//#define CFG_MAX3_SPI_SPEED      (200*1000)
static bool_t sMax3Inited = false;


static struct SemaphoreLCB s_tMax3_Semp;    //оƬ������ʱ���
static u8 MaxSampleMode = MAX31865_CONTINOUS_MODE;

//extern bool_t Max31865_DataReady(void);         //������أ���board.c��
//max31865  �¶�
//static const Pin Max3_pin[] = {
//      {GPIO_A,PIN4,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},//cs
//      {GPIO_A,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
//      {GPIO_A,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
//      {GPIO_B,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
//      {GPIO_I,PIN11,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},//data ready
//};

__attribute__((weak)) bool_t Max31865_DataReady(void)
{
    return true;//Max31865����Ready �źŶ�ȡ
}
static bool_t Max31865_Ready(u32 WaitTime)
{
    u32 timecnt = 0;
    while(timecnt < WaitTime)
    {
        DJY_DelayUs(10);
//      if(!PIO_Get(&Max3_pin[4]))
        if(!Max31865_DataReady())
            return true;
        timecnt += 10;
    }
    return false;
}

static void Max31865_CsActive(void)
{
    SPI_CsActive(s_ptMax3_Dev,s_Max3_Timeout);
//  PIO_Clear(&Max3_pin[0]);
}
static void Max31865_CsInActive(void)
{
    SPI_CsInactive(s_ptMax3_Dev);
//  PIO_Set(&Max3_pin[0]);
}
static u32 Max31865_TxRx(u8* sdata,u32 slen,u8* rdata, u32 rlen,u32 RecvOff)
{
    struct SPI_DataFrame data;
    s32 result;

    data.RecvBuf = rdata;
    data.RecvLen = rlen;
    data.RecvOff = RecvOff;
    data.SendBuf = sdata;
    data.SendLen = slen;

    result = SPI_Transfer(s_ptMax3_Dev,&data,true,s_Max3_Timeout);
    if(result != CN_SPI_EXIT_NOERR)
        return 0;
    return 1;
}

// =============================================================================
// �������ܣ�����һ�β���
// �����������
// �����������
// ����ֵ  ��true�����óɹ���false�����ò��ɹ�
// =============================================================================
static bool_t Max31865_TrigSingleSample(void)
{
    u8 cfgData[] = {0x80,0xA0};
    u8 cfgRead = 0x00,cfgcnt = 0;
    bool_t ret = false;

    do
    {
        cfgData[0] = 0x80;
        cfgData[1] = 0xA0;
        Max31865_CsActive();
        Max31865_TxRx(cfgData,2,NULL,0,0);
        DJY_EventDelay(15*1000);
        Max31865_CsInActive();

        cfgData[0] = 0x00;
        Max31865_CsActive();
        Max31865_TxRx(cfgData,1,&cfgRead,1,1);
        Max31865_CsInActive();
        if(cfgRead == 0xA0)
        {
            ret = true;
            break;
        }
        cfgcnt ++;
        if(cfgcnt == 20)
        {
            break;
        }
        DJY_EventDelay(1*mS);
    }while(1);

    return ret;
}

// =============================================================================
// �������ܣ�����Max����Ϊ��������ģʽ
// �����������
// �����������
// ����ֵ  ��true�����óɹ���false�����ò��ɹ�
// =============================================================================
static bool_t Max31865_ContinousModeCfg(void)
{
    u8 cfgData[] = {0x80,0xC1};
    u8 cfgRead = 0x00,cfgcnt = 0;
    bool_t ret = false;

    do
    {
        cfgData[0] = 0x80;
        cfgData[1] = 0xC1;
        Max31865_CsActive();
        Max31865_TxRx(cfgData,2,NULL,0,0);
        DJY_EventDelay(15*mS);
        Max31865_CsInActive();

        cfgData[0] = 0x00;
        Max31865_CsActive();
        Max31865_TxRx(cfgData,1,&cfgRead,1,1);
        Max31865_CsInActive();
        if(cfgRead == 0xC1)
        {
            ret = true;
            break;
        }
        cfgcnt ++;
        if(cfgcnt == 20)
        {
            break;
        }
        DJY_EventDelay(10*mS);
    }while(1);

    return ret;
}

// =============================================================================
// �������ܣ��ж��Ƿ��д��󣬲��������
// �����������
// �����������
// ����ֵ  ��true���д���false���޴���
// =============================================================================
static bool_t Max31865_FaultAndClear(u8 *Reg)
{
    bool_t ret = false;
    u8 cfgData[2];

    if(Reg[7] & 0x04)                   //������
    {
        cfgData[0] = 0x80;
        cfgData[1] = Reg[0] & (~((1<<5) | (3<<2))); //����
        cfgData[1] |= (1<<1);
        Max31865_CsActive();
        Max31865_TxRx(cfgData,2,NULL,0,0);
        Max31865_CsInActive();
        ret = true;
    }
    return ret;
}
// =============================================================================
// �������ܣ����¶�(ע�⣺��������Ϊfloat���������������򷵻�ֵ����)
// �����������
// ����ֵ  ���¶�ֵ��Ϊfloat���ͣ�-1����ʾ����
// =============================================================================
float Max31865_TemperatureGet(void)
{
    float temp;
    u8 RegMatrix[8] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};
    u8 Data[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

    if(!sMax3Inited)
    {
        return -1;
    }

    if(false == Lock_SempPend(&s_tMax3_Semp,25*mS))
    {
        return -1;
    }

    if(MaxSampleMode == MAX31865_SINGLE_MODE)
    {
        if(false == Max31865_TrigSingleSample())
        {
            Lock_SempPost(&s_tMax3_Semp);
            return -1;
        }
    }

    if(false == Max31865_Ready(500000))       //��æ������ʱ�򷵻�false
    {
        Lock_SempPost(&s_tMax3_Semp);
        return -1;
    }

    Max31865_CsActive();
    Max31865_TxRx(RegMatrix,8,Data,8,1);
    Max31865_CsInActive();

    if(Max31865_FaultAndClear(Data))
    {
        Lock_SempPost(&s_tMax3_Semp);
        return -1;
    }

    Lock_SempPost(&s_tMax3_Semp);

    //�����¶�
    temp = ((Data[1] <<8) + Data[2]) >> 1;
    temp = (temp / 32 - 256);

    return temp;
}

// =============================================================================
//���ܣ���ʼ��Max31865оƬ��������������ģʽ����������ģʽΪ��������
//������������
//     BusName,�ܽ����ƣ��硰SPI1��
//     Mode������ģʽ�򵥴β���ģʽ,
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
// =============================================================================
bool_t ModuleInstall_Max31865(void)
{
//  PIO_Configure(Max3_pin,PIO_LISTSIZE(Max3_pin));
//  PIO_Set(&Max3_pin[0]);

    if(NULL == Lock_SempCreate_s(&s_tMax3_Semp,1,1,CN_BLOCK_FIFO,"MAX31865_semp"))
        return false;

    if(s_ptMax3_Dev = SPI_DevAdd(CFG_MAX31865_BUS_NAME,"MAX31865",0,8,SPI_MODE_1,SPI_SHIFT_MSB,CFG_MAX3_SPI_SPEED,false))
    {
        SPI_BusCtrl(s_ptMax3_Dev,CN_SPI_SET_POLL,0,0);
        if(CFG_MAX31865_SAM_MODE == MAX31865_CONTINOUS_MODE)
        {
            MaxSampleMode = MAX31865_CONTINOUS_MODE;
            if(Max31865_ContinousModeCfg())
            {
                sMax3Inited = true;
            }
        }
        else
        {
            MaxSampleMode = MAX31865_SINGLE_MODE;
            sMax3Inited = true;
        }
        return true;
    }
    else
    {
        printf("MAX31865 init failed.\n\r");
        return false;
    }

    return sMax3Inited;
}

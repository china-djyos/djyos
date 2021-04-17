// =============================================================================

// �ļ���     ��max11410.c
// ģ������: ADоƬmax14001������
// ģ��汾: V1.00
// ������Ա: yt
// ����ʱ��:
// =============================================================================
#include "string.h"
#include "stdio.h"
#include "os.h"
#include "spibus.h"
#include "cpu_peri.h"
#include "max14001.h"
#include "math.h"
#include "cpu_peri_gpio.h"
//#include "Threshold.h"
//#include "MAX14001Cfg.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_Max14001(void);
//    ModuleInstall_Max14001();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"max14001"//spi�ӿ�10bit����ADC
//parent                       //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
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
#if ( CFG_MODULE_ENABLE_MAX14001 == false )
//#warning  " max14001  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_MAX14001    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

//static struct SemaphoreLCB Max_Semp;       //оƬ������ʱ���
//static struct SPI_Device Max14001_Dev;   //SPI���������ṹ��
//static bool_t sMaxInited = false;
#define Max14001_TIMEOUT        (5*1000)     //Max14001 ��ʱ����

//extern  KI_Threshold    *WB7363_KI_CFG;
//extern  SPI_Device_CFG  *WB7363_SPI_DEV;

// =============================================================================
// ���ܣ�Max14001��CPU�������ݽ����Ľӿں���������SPIͨ�ţ�Ӳ��ע����������Ƭѡ
// ������   wBuf,д������
//      wLen,д���ݳ��ȣ��ֽڵ�λ
//      rBuf,��������
//      rLen,���ֽ���
//      port����������port��ţ�����Ҫ����GPIOA,��port=GPIO_A
//      mask��port����Ҫ����ߵ�ƽ��λ����,�������PIOA,PIN5��msk = (1<<5)
// ���أ���
// =============================================================================
u16  Max14001_TxRx(u16 data,u8 addr,u8 rdflag,u32 timeout,struct SPI_Device Max14001_Dev)
{
    struct SPI_DataFrame Data;
    u8 buf[2];
    u8 Rcvbuf[2];

    buf[0] = (u8)(data&0xff);                            //��һ���ֽ�
    buf[1] = (u8)(((data>>8)&(0x3))|((addr&0x1f)<<3));   //�ڶ����ֽ�

    if(rdflag)
    {
        buf[1] &=~(1<<2);  //R��0
        Data.RecvLen = 2;  //���ճ���2�ֽ�
    }
    else
    {
       buf[1] |= (1<<2);   //W��1
       Data.RecvLen = 0;   //������
    }

    Data.SendBuf = buf;
    Data.SendLen = 2;
    Data.RecvBuf = Rcvbuf;
    Data.RecvOff = 0;

    SPI_CsActive(&Max14001_Dev,CN_TIMEOUT_FOREVER);
    DJY_DelayUs(10);
    SPI_Transfer(&Max14001_Dev,&Data,true,Max14001_TIMEOUT);  //���ݴ��ͺ�����������ݵķ��ͺͽ��ա�
    DJY_DelayUs(3);
    SPI_CsInactive(&Max14001_Dev);

    if(rdflag)
    {
        SPI_CsActive(&Max14001_Dev,CN_TIMEOUT_FOREVER);
        DJY_DelayUs(10);
        SPI_Transfer(&Max14001_Dev,&Data,true,Max14001_TIMEOUT);  //���ݴ��ͺ�����������ݵķ��ͺͽ��ա�
        DJY_DelayUs(3);
        SPI_CsInactive(&Max14001_Dev);

//        printf("addr=0x%x , rcvdata =0x%x \n\r",addr,(u16)(Rcvbuf[0]|((Rcvbuf[1]&(0x3))<<8)));
//        DJY_EventDelay(100*mS);
        return (u16)(Rcvbuf[0]|((Rcvbuf[1]&(0x3))<<8));
    }
    else
        return 0;
}

#if 0
u32 Max14001_TxRx1(u8* sBuf,u16 sLen,u8* rBuf,u16 rLen,u32 recvoff)
{
    struct SPI_DataFrame data;
    s32 result;

    data.SendBuf = sBuf;      //��������ָ��
    data.SendLen = 2;         //�������ݳ��ȣ��ֽ�
    data.RecvBuf = rBuf;      //��������ָ��
    data.RecvLen = rLen;      //�������ݳ��ȣ��ֽ�
    data.RecvOff = recvoff;   //��������ƫ��

    SPI_CsActive(&Max14001_Dev,1000);
    result =SPI_Transfer(&Max14001_Dev,&data,true,Max14001_TIMEOUT);  //���ݴ��ͺ�����������ݵķ��ͺͽ��ա�
    SPI_CsInactive(&Max14001_Dev);
    if(result != CN_SPI_EXIT_NOERR)
            return 0;
    return 1;
}
#endif

//=============================================================================
//���ܣ���ȡĳһ���ŵ�ƽ״̬
//������port
//    pin
//���أ�1-�ߵ�ƽ
//    0-�͵�ƽ
//=============================================================================
u16 Get_Data(u32 port,u16 pin)
{
    u16 data;
    data=GPIO_GetData(port);
    data=(data>>(pin-1))&1;   //��������������λ����λ����
    return data;
}
//=============================================================================
//���ܣ�����Max14001Ĭ�ϳ�ʼֵ
//������������
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
//=============================================================================
u16 max_data[0x20];
bool_t Max14001_Config(u16 thl,u16 thu,struct SPI_Device Max14001_Dev)
{
        u16 readdate;
        bool_t result;
        DJY_EventDelay(100*mS);

        //1��READ FLAGS
        readdate=Max14001_TxRx(0,0x02,1,10000,Max14001_Dev);   //��FLAGS
        DJY_EventDelay(10);
        readdate=Max14001_TxRx(0,0x02,1,10000,Max14001_Dev);   //��FLAGS
        DJY_EventDelay(10);

        //2��WRITEN WEN
        Max14001_TxRx(0x294,0x0c,0,10000,Max14001_Dev);     //дWEN=0x294
        DJY_EventDelay(10);

        //3��DISABLE FAULT MV
        Max14001_TxRx(0x0ff,0x03,0,10000,Max14001_Dev);     //дFLTEN=0x0ff  EMV=0 ��Ӧfault�����
        DJY_EventDelay(10);

        //4��WRITE CONFIGURATION REGISTERS
        Max14001_TxRx(0x0ff,0x03,0,10000,Max14001_Dev);     //дFLTEN 0x0ff
        DJY_EventDelay(10);
        Max14001_TxRx(thl,0x04,0,10000,Max14001_Dev);       //дTHL
        DJY_EventDelay(10);
        Max14001_TxRx(thu,0x05,0,10000,Max14001_Dev);       //дTHU
        DJY_EventDelay(10);
        Max14001_TxRx(0x20d,0x09,0,10000,Max14001_Dev);     //дCFG
        DJY_EventDelay(10);
        Max14001_TxRx(0x0c0,0x06,0,10000,Max14001_Dev);     //дINRR
        DJY_EventDelay(10);
        Max14001_TxRx(0x180,0x07,0,10000,Max14001_Dev);     //дINRT
        DJY_EventDelay(10);
        Max14001_TxRx(0x1d8,0x08,0,10000,Max14001_Dev);     //дINRP
        DJY_EventDelay(10);

        //5��WRITE VERIFICATION REGISTERS
        Max14001_TxRx(0x0ff,0x13,0,10000,Max14001_Dev);     //дFLTV=FLTEN
        DJY_EventDelay(10);
        Max14001_TxRx(thl,0x14,0,10000,Max14001_Dev);       //дTHLV=THL
        DJY_EventDelay(10);
        Max14001_TxRx(thu,0x15,0,10000,Max14001_Dev);       //дTHUV=THU
        DJY_EventDelay(10);
        Max14001_TxRx(0x20d,0x19,0,10000,Max14001_Dev);     //дCFGV=CFG
        DJY_EventDelay(10);
        Max14001_TxRx(0x0c0,0x16,0,10000,Max14001_Dev);     //дINRRV=INRR
        DJY_EventDelay(10);
        Max14001_TxRx(0x180,0x17,0,10000,Max14001_Dev);     //дINRTV=INRT
        DJY_EventDelay(10);
        Max14001_TxRx(0x1d8,0x18,0,10000,Max14001_Dev);     //дINRPV=INRP
        DJY_EventDelay(10);

        Max14001_TxRx(0x010,0x0a,0,10000,Max14001_Dev);     //дENBL
        Max14001_TxRx(0x010,0x1a,0,10000,Max14001_Dev);     //дENBLV

        //6��ENABLE FALUT MV(WRITE FLTEN AND FLTV)
        Max14001_TxRx(0x1ff,0x03,0,10000,Max14001_Dev);   //дFLTEN=0x1ff
        DJY_EventDelay(10);
        Max14001_TxRx(0x1ff,0x13,0,10000,Max14001_Dev);   //дFLTV=0x1ff
        DJY_EventDelay(10);

        //7��WRITE WEN
        Max14001_TxRx(0x000,0x0c,0,10000,Max14001_Dev);   //дWEN=0x000  ֮����д
        DJY_EventDelay(10);

        //8��READ BACK CONFIGURATION REGISTERS
        readdate=Max14001_TxRx(0x000,0x03,1,10000,Max14001_Dev);     //��FLTEN
        DJY_EventDelay(10);
        readdate=Max14001_TxRx(0x000,0x04,1,10000,Max14001_Dev);     //��THL
        DJY_EventDelay(10);
        readdate=Max14001_TxRx(0x000,0x05,1,10000,Max14001_Dev);     //��THU
        DJY_EventDelay(10);
        readdate=Max14001_TxRx(0x000,0x09,1,10000,Max14001_Dev);     //��CFG=0x203
        DJY_EventDelay(10);
        readdate=Max14001_TxRx(0x000,0x06,1,10000,Max14001_Dev);     //��INRR
        DJY_EventDelay(10);
        readdate=Max14001_TxRx(0x000,0x07,1,10000,Max14001_Dev);     //��INRT
        DJY_EventDelay(10);
        readdate=Max14001_TxRx(0x000,0x08,1,10000,Max14001_Dev);     //��INRP
        DJY_EventDelay(10);

//        readdate1=Max14001_TxRx(0x000,0x13,1,10000);     //��FLTEN
//        DJY_EventDelay(10);
//        readdate1=Max14001_TxRx(0x000,0x14,1,10000);     //��THL
//        DJY_EventDelay(10);
//        readdate1=Max14001_TxRx(0x000,0x15,1,10000);     //��THU
//        DJY_EventDelay(10);
//        readdate1=Max14001_TxRx(0x000,0x19,1,10000);     //��CFG=0x203
//        DJY_EventDelay(10);
//        readdate1=Max14001_TxRx(0x000,0x16,1,10000);     //��INRRV
//        DJY_EventDelay(10);
//        readdate1=Max14001_TxRx(0x000,0x17,1,10000);     //��INRTV
//        DJY_EventDelay(10);
//        readdate1=Max14001_TxRx(0x000,0x18,1,10000);     //��INRPV
//        DJY_EventDelay(10);

        //9��READ FLAGS
        readdate=Max14001_TxRx(0,0x02,1,10000,Max14001_Dev);   //��FLAGS
        DJY_EventDelay(10);
        //10��READ FLAGS
        readdate=Max14001_TxRx(0,0x02,1,10000,Max14001_Dev);   //��FLAGS
        DJY_EventDelay(10);

        if(readdate==0x000)
        {
            //11,WRITE WEN
            Max14001_TxRx(0x294,0x0c,0,10000,Max14001_Dev);   //дWEN=0x294
            //12,WRITE ENBL:ENA=1
            Max14001_TxRx(0x010,0x0a,0,10000,Max14001_Dev);   //дENBL
            Max14001_TxRx(0x010,0x1a,0,10000,Max14001_Dev);   //дENBLV
            //13��READ FLAGS
            readdate=Max14001_TxRx(0,0x02,1,10000,Max14001_Dev);   //��FLAGS
            //14��READ FLAGS
            readdate=Max14001_TxRx(0,0x02,1,10000,Max14001_Dev);   //��FLAGS
            if(readdate==0x000)
            {
                //15��WRITE WEN
                Max14001_TxRx(0x000,0x0c,0,10000,Max14001_Dev);   //дWEN=0x000  ֮����д
                result=true;
            }
        }
        else
        {
            printf("Max14001��ʼ��ʧ��\r\n");
            result=false;
        }

        //�ѼĴ���ȫ���������һ��
        for(u8 i=0;i<0x1b;i++)
        {
            max_data[i] = Max14001_TxRx(0x000,i,1,10000,Max14001_Dev);
            printf("addr=0x%x , rcvdata =0x%x \n\r",i,(u16)max_data[i]);
        }
        return result;
}

//=============================================================================
//���ܣ����²���ֵ��ȫ������
//������
//���أ�
//=============================================================================
extern u16 COUT_GPIO_CFG[18];
extern u16 COUT_PIN_CFG[18];

u16 MAX14001_ADC_BUFF[18];   //���һ��18·COUTֵ

void ADC_SAMPLE_UPDATA(void)
{
    for(int i=0;i<18;i++)
    {
        MAX14001_ADC_BUFF[i]=Get_Data(COUT_GPIO_CFG[i],COUT_PIN_CFG[i]);
    }
}

//=============================================================================
//���ܣ���ʼ��Max14001оƬ
//������������
//    BusName,�ܽ����ƣ��硰SPI1��
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
//=============================================================================
extern struct SPI_Device Max14001_Dev[18];

extern u16 WB7363_KI_THL_CFG[18];
extern u16 WB7363_KI_THU_CFG[18];

extern u16 WB7363_SPI_DEV_AUTOCS[18];
extern u16 WB7363_SPI_DEV_LEN[18];
extern u16 WB7363_SPI_DEV_SPI_MODE[18];
extern u16 WB7363_SPI_DEV_SPI_SHIFT[18];
extern u32 WB7363_SPI_DEV_SPI_FREQ[18];

extern char *WB7363_SPI_DEV_SPI_NAME[18];

bool_t ModuleInstall_Max14001(void)
{
    u32 gNUM=0;
    u16 THL_Value;
    u16 THU_Value;

    if(18==18)
    {
        for(u32 t=0;t<18;t++)
        {
           THL_Value=WB7363_KI_THL_CFG[t];
           THU_Value=WB7363_KI_THU_CFG[t];

           Max14001_Dev[t].AutoCs = WB7363_SPI_DEV_AUTOCS[t];       //���Զ�Ƭѡ��������ʱͨ���Լ��ֶ�Ƭѡ
           Max14001_Dev[t].CharLen = WB7363_SPI_DEV_LEN[t];         //���ݳ���8λ
           Max14001_Dev[t].Mode = WB7363_SPI_DEV_SPI_MODE[t];       //clk����Ϊ�͵�ƽ��һ�������زɼ�/�������
           Max14001_Dev[t].ShiftDir = WB7363_SPI_DEV_SPI_SHIFT[t];  //��λ���ȣ���ͬ������оƬ
           Max14001_Dev[t].Freq = WB7363_SPI_DEV_SPI_FREQ[t];       //����
           Max14001_Dev[t].Cs = t%6 ;

           if((gNUM>=0)&&(gNUM<=5))
           {
                 SPI_DevAdd_s(&Max14001_Dev[t],"SPI1",WB7363_SPI_DEV_SPI_NAME[t]);     //1-6 ����SPI1�£���������������
                 SPI_BusCtrl(&Max14001_Dev[t],CN_SPI_SET_POLL,0,0);
                 Max14001_Config(THL_Value,THU_Value,Max14001_Dev[t]);
           }

           if((gNUM>=6)&&(gNUM<=11))
           {
                 SPI_DevAdd_s(&Max14001_Dev[t],"SPI2",WB7363_SPI_DEV_SPI_NAME[t]);     //6-12����SPI2��
                 SPI_BusCtrl(&Max14001_Dev[t],CN_SPI_SET_POLL,0,0);
                 Max14001_Config(THL_Value,THU_Value,Max14001_Dev[t]);
           }

           if((gNUM>=12)&&(gNUM<=17))
           {
                 SPI_DevAdd_s(&Max14001_Dev[t],"SPI3",WB7363_SPI_DEV_SPI_NAME[t]);     //12-18����SPI3��
                 SPI_BusCtrl(&Max14001_Dev[t],CN_SPI_SET_POLL,0,0);
                 Max14001_Config(THL_Value,THU_Value,Max14001_Dev[t]);
           }
           gNUM++;
         }
         return true;
    }
    else
    {
        printf("KIͨ������ADоƬ����һ��\r\n");
        return false;
    }
}

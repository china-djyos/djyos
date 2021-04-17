// =============================================================================

// �ļ���     ��AD7606.c
// ģ������: AD7606�ɼ�ģ��ĵײ�������������SPI�ӿ�ͨ�ź�����ȡAD�ɼ�ֵ
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 09/04.2015
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "djyos.h"
#include "spibus.h"
#include "cpu_peri.h"
#include "iodefine.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t AD7606_Init(void);
//    AD7606_Init();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"ADC chip ad7606"//spi�ӿڵ�16bitADC
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"spi bus","cpu onchip spi"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_ADC_CHIP_AD7606 == false )
//#warning  " ADC_chip_ad7606  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_ADC_CHIP_AD7606    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,500000,30000000,
#define CFG_AD7606_SPEED        10000000//"speed",���������ٶ�
//%$#@num,1000,200000,
#define CFG_AD7606_TIMEOUT      (5*1000)//"timeout",ad7606 ��ʱ����
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

// =============================================================================
//#define CFG_AD7606_SPEED  10000000       //AD7606��SSP�����ٶ�
//#define CFG_AD7606_TIMEOUT (5*1000)     //ad7606 ��ʱ����Ϊ5mS

struct SPI_Device *s_ptAD7606_Dev;

// =============================================================================
// ���ܣ�AD7606��CPU�ӿں���������SPIͨ�ţ�Ӳ��ע����������Ƭѡ
// ������wBuf,д������
//      wLen,д���ݳ��ȣ��ֽڵ�λ
//      rBuf,��������
//      rLen,���ֽ���
// ���أ���
// =============================================================================
void __AD7606_SpiInterfalce(u8* wBuf,u16 wLen,u8* rBuf,u16 rLen)
{
    struct SPI_DataFrame frame;

    frame.RecvBuf = rBuf;
    frame.RecvLen = rLen;
    frame.RecvOff = wLen;
    frame.SendBuf = wBuf;
    frame.SendLen = wLen;

    GPIO_SetToLow( ADCS_PORT, (1 << ADCS_PIN));     // ����Ƭѡ�ź�
    SPI_Transfer(s_ptAD7606_Dev,&frame,1,CFG_AD7606_TIMEOUT);
    GPIO_SetToHigh( ADCS_PORT, (1 << ADCS_PIN));        // ����Ƭѡ�ź�
}
// =============================================================================
// ���ܣ���ʼ��AD7606�������������úͳ�ʼ�����ã��ӿڵĳ�ʼ����
// ��������
// ���أ���
// =============================================================================
bool_t AD7606_Init(void)
{
    //����AD7606�Ƿ���ù�����
    GPIO_CfgPinFunc(ADOS0_PORT,ADOS0_PIN,CN_GPIO_MODE_OD_PULLUP);
    GPIO_CfgPinFunc(ADOS1_PORT,ADOS1_PIN,CN_GPIO_MODE_OD_PULLUP);
    GPIO_CfgPinFunc(ADOS2_PORT,ADOS2_PIN,CN_GPIO_MODE_OD_PULLUP);

    //�رչ���������000
    GPIO_SetToOut(ADOS0_PORT,(1 << ADOS0_PIN));
    GPIO_SetToOut(ADOS1_PORT,(1 << ADOS1_PIN));
    GPIO_SetToOut(ADOS2_PORT,(1 << ADOS2_PIN));

    GPIO_SetToLow(ADOS2_PORT, (1 << ADOS2_PIN));
    GPIO_SetToLow(ADOS1_PORT, (1 << ADOS1_PIN));
    GPIO_SetToLow(ADOS0_PORT, (1 << ADOS0_PIN));

    //GPIO��ʼ��������RST��SH��BUSY���������������
    GPIO_CfgPinFunc(ADRST_PORT,ADRST_PIN,CN_GPIO_MODE_OD_PULLUP);
    GPIO_CfgPinFunc(ADCONVS_PORT,ADCONVS_PIN,CN_GPIO_MODE_OD_PULLUP);
    GPIO_CfgPinFunc(ADBUSY_PORT,ADBUSY_PIN,CN_GPIO_MODE_OD_PULLUP);

    GPIO_SetToOut(ADRST_PORT,(1 << ADRST_PIN));         //��ʼ������RST״̬
    GPIO_SetToHigh(ADRST_PORT, (1 << ADRST_PIN));

    GPIO_SetToOut( ADCONVS_PORT, (1 << ADCONVS_PIN));       //����=0������ADת��
    GPIO_SetToLow( ADCONVS_PORT, (1 << ADCONVS_PIN));

    GPIO_SetToIn( ADBUSY_PORT, (1 << ADBUSY_PIN));      //BUSY�ܽ�Ϊ����

    GPIO_SetToLow(ADRST_PORT,(1 << ADRST_PIN));

    // ���³�ʼ����SSP���
    //CS��ʼ��
    GPIO_CfgPinFunc(ADCS_PORT,ADCS_PIN,CN_GPIO_MODE_OD_PULLUP);
    GPIO_SetToOut(ADCS_PORT,(1 << ADCS_PIN));

    //SSP��������ʼ��
    if(s_ptAD7606_Dev = SPI_DevAdd("SSP0","AD7606",1,8,SPI_MODE_3,SPI_SHIFT_MSB,CFG_AD7606_SPEED,false))
    {
        SPI_BusCtrl(s_ptAD7606_Dev,CN_SPI_SET_POLL,0,0);
        return true;
    }
    else
    {
        printf("\r\n: error : device :AD7606 init failed.\n\r");
        return false;
    }
}

// =============================================================================
// ���ܣ�����AD��ʼ������ͨ������IO���ŵĸߵ͵�ƽ�����ж�BUSYλ
// ��������
// ���أ���
// =============================================================================
void __AD7606_Start(void)
{
    GPIO_SetToHigh( ADCONVS_PORT, (1 << ADCONVS_PIN));
    while( GPIO_DataGet( ADBUSY_PORT) & (1 << ADBUSY_PIN));
}

// =============================================================================
// ���ܣ�ֹͣADת����ͨ������IO��ƽ�ķ�ʽ����
// ��������
// ���أ���
// =============================================================================
void __AD7606_Stop(void)
{
    GPIO_SetToLow( ADCONVS_PORT, (1 << ADCONVS_PIN));
}

// =============================================================================
// ���ܣ��ж�AD7606�Ƿ��Ѿ�ת��������AD7606�����ù�������ʱ��Ϊ4us��64�����Ĺ��������ʱ��
//      Ϊ315us
// ��������
// ���أ���
// =============================================================================
static bool_t _AD7606_ConvFinished(void)
{
    u32 timeout = 500;
    u32 data;

    DJY_DelayUs(4);         //��������������ʱ����315uS

    while(1)
    {
        data = GPIO_DataGet(ADBUSY_PORT);
        if( (!(data & (1 << ADBUSY_PIN))) || (timeout == 0) )
            break;
        DJY_DelayUs(1);
        timeout--;
    }
    if(timeout)
        return true;
    else
        return false;
}

// =============================================================================
// ���ܣ���ȡAD7606�Ĳɼ�ֵ����������ת����ת���������ٶ�ȡADת����ֵ
// ��������
// ���أ���
// =============================================================================
bool_t AD7606_GetValue(uint16_t *AinBuf)
{
    bool_t result = false;
    u8 *pAinBuf = (u8*)AinBuf;
    u8 temp,i;

    __AD7606_Start();
    if(_AD7606_ConvFinished())
    {
        __AD7606_SpiInterfalce(NULL,0,pAinBuf,16);
        result = true;
    }
    __AD7606_Stop();
    if(result == true)
    {
        if(CN_CFG_BYTE_ORDER == CN_CFG_LITTLE_ENDIAN)   //��С��ת��
        {
            for(i = 0; i < 8; i ++)
            {
                temp = pAinBuf[2*i];
                pAinBuf[2*i] = pAinBuf[2*i + 1];
                pAinBuf[2*i + 1] = temp;
            }
        }
    }

    return result;
}

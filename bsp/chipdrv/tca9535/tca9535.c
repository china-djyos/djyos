#include "stdint.h"
#include <string.h>
#include <stdlib.h>
#include "os.h"
#include "cpu_peri.h"
#include "iicbus.h"
#include "tca9535.h"
#include "board-config.h"
#include <time.h>

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t TCA9535_Init(void);
//    TCA9535_Init();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"tca9535"//IIC�ӿڵ�16��IO��չ
//parent:"iicbus"    //��д������ĸ�������֣�none��ʾû�и����
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
#if ( CFG_MODULE_ENABLE_TCA9535 == false )
//#warning  " tca9535  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_TCA9535    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,100000,400000,
#define CFG_TCA9535_CLK_FRE             (100*1000)      //"�����ٶ�"��TCA9535�������ٶ�
//%$#@enum,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
#define CFG_TCA9535_ADDR                (0x20)          //"�豸��ַ",�豸�����ߵ�ַ
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#define CFG_TCA9535_BUS_NAME              "IIC0"            //"��������",�豸�������ߵ�����
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


static struct IIC_Device *pg_TCA9535_Dev=NULL;
struct MutexLCB *  ptSemID_TCA9535;       // IIC���������־
//#define CFG_TCA9535_CLK_FRE             (100*1000)      //�����ٶȣ���λHz

//#define CFG_TCA9535_ADDR (0x20) //��Ͱ��Ӳ�������йأ���ʱ��ŵ�������涨��

//оƬ8���Ĵ������ڲ���ַ
#define TCA9535_REG_InputPort0  0x00
#define TCA9535_REG_InputPort1  0x01
#define TCA9535_REG_OutputPort0 0x02
#define TCA9535_REG_OutputPort1 0x03
#define TCA9535_REG_PolarityInversion0  0x04
#define TCA9535_REG_PolarityInversion1  0x05
#define TCA9535_REG_Configuration0  0x06
#define TCA9535_REG_Configuration1  0x07

static s32 timeout = 1000*mS;                 //1000*mS;

//ȫ�ֱ������ԼĴ��������ã���Ϊÿ��ֻ��һ��IO�ڣ���д��Ĵ�����Ҫд8λ��
static u8 p0conf=255;//1111 1111�˸�p0�ڳ�ʼȫ�趨Ϊ����
static u8 p1conf=255;
static u8 p0out=255;//1111 1111�˸�p0�ڳ�ʼֵȫ����ߵ�ƽ
static u8 p1out=255;




// =============================================================================
// ���ܣ���ָ����ַ��ʼDstlen���ȵ�����
// ������
//       TCA9535_DstAddr��Ŀ��洢��Ԥ��ȡ���ݵĵ�ַ
//       pDstBuf����Ž��յ������ݻ����ַ
//       DstLen�� Ԥ��ȡ���ݵĳ���,Ϊһ���ֽ�
// ���أ�ret, ʵ�ʶ������ֽ���
// =============================================================================
static s32 TCA9535_Read_Data(u32 TCA9535_DstAddr,u8 *pDstBuf,u32 DstLen)
{
    s32 ret = 0;

    ret=IIC_Read(pg_TCA9535_Dev, TCA9535_DstAddr, pDstBuf, DstLen, timeout);
    return ret;
}

// =============================================================================
// ���ܣ�дָ����ַ��ʼDstlen���ȵ�����
// ������
//      TCA9535_DstAddr��Ŀ��洢��Ԥд���ݵ�ַ
//      pSrcBuf���������ݻ����ַ
//      SrcLen�� �������ݳ��ȣ�һ��Ϊһ���ֽ�
// ���أ�ret, ʵ��д����ֽ���
// =============================================================================
static s32 TCA9535_Write_Data(u32 TCA9535_DstAddr,u8 *pSrcBuf,u32 SrcLen)
{
    s32 ret = 0;

    ret=IIC_Write(pg_TCA9535_Dev, TCA9535_DstAddr, pSrcBuf, SrcLen,0, timeout);
    return ret;
}


// =============================================================================
// ���ܣ�����ÿ��IO�ڵ��������
// ������
//      portid��IO�ڵ����֣�.h�ļ��е�ö��������port03��port04
//      mode��0Ϊ�����output����1Ϊ���루input��
// ���أ�true or false
// =============================================================================
bool_t TCA9535_PortConfigure(u8 portid,bool_t mode)
{

    s32 ret=0;
    u8 *p0;
    u8 *p1;

    p0=&p0conf;//ָ��8λ��ֵ��ָ��
    p1=&p1conf;

    if(portid<8)//˵����P0�˿�
    {
        if(mode==0)//ģʽΪ���
        {
            p0conf&=(~(1<<portid));//8λȫ�ֱ�����Ӧ8��IO�ڣ���Ӧ�Ķ˿�ֵ��0
        }
        if(mode==1)//����
        {
            p0conf|=(1<<portid);//��Ӧ�Ķ˿�ֵ��1
        }
        ret = TCA9535_Write_Data(TCA9535_REG_Configuration0,p0,1);//���üĴ���������IO���ŵķ���
    }
    if(portid>9)//˵����P1�˿�
    {
        portid-=10;
        if(mode==0)//ģʽΪ���
        {
            p1conf&=(~(1<<portid));//��Ӧ�Ķ˿�ֵ��0
        }
        if(mode==1)//����
        {
            p1conf|=(1<<portid);//��Ӧ�Ķ˿�ֵ��1
        }
        portid+=10;
        ret = TCA9535_Write_Data(TCA9535_REG_Configuration1,p1,1);
    }

    if(ret==-1)
    return false;
    return true;
}


// =============================================================================
// ���ܣ���ȡ������IO��ֵ
// ������portid��IO�ڵ����֣���.h�ļ��е�ö��������port03��port04
// ���أ�true or -1
// =============================================================================
s32 TCA9535_PortRead(u8 portid)
{
    u8 data;
    s32 ret;
    u8 *buf=&data;

    if(portid<8)//˵����P0�˿�
    {
        if(0==(p0conf&(1<<portid)))//��IO������Ϊ���ģʽ���޷���ȡ
            return -1;
        ret=TCA9535_Read_Data(TCA9535_REG_InputPort0,buf,1);//��ȡ��ؼĴ�����ֵ����bufָ��ָ���data��
        if(ret==-1)
            return -1;
        data&=(1<<portid);//ֻ���뿴����һ��IO��
        if(data>0)
        return 1;
        if(data==0)
        return 0;
    }
    if(portid>9)//˵����P1�˿�
    {
        if(0==(p1conf&(1<<(portid-10))))//��IO������Ϊ���ģʽ���޷���ȡ
            return -1;
        ret=TCA9535_Read_Data(TCA9535_REG_InputPort1,buf,1);//��ȡ��ؼĴ�����ֵ����bufָ��ָ���data��
        if(ret==-1)
            return -1;
        data&=(1<<(portid-10));//ֻ���뿴����һ��IO��
        if(data>0)
        return 1;
        if(data==0)
        return 0;
    }
    return -1;
}

// =============================================================================
// ���ܣ�����ָ��IO����������ĵ�ƽ
// ������portid��IO�ڵ����֣���.h�ļ��е�ö��������port03��port04
//      value��0Ϊ�͵�ƽ��1Ϊ�ߵ�ƽ
// ���أ�true or -1
// =============================================================================
s32 TCA9535_PortWrite(u8 portid,bool_t value)
{
    s32 ret;


    if(portid<8)//˵����P0�˿�
    {
        if(1==(p0conf&(1<<portid)))//��λ��Ϊ1˵��������˿ڣ��޷��������
            return -1;
        if(value==1)//����ߵ�ƽ
            p0out|=(1<<portid);//��Ӧ��λ����1
        if(value==0)//����͵�ƽ
            p0out&=(~(1<<portid));//��Ӧ��λ����0
        ret=TCA9535_Write_Data(TCA9535_REG_OutputPort0,&p0out,1);//�����úõ�8λ��ֵд���Ӧ�Ĵ���
        if(ret==-1)
            return -1;
    }
    if(portid>9)//˵����P0�˿�
    {
        portid-=10;
        if(1==(p1conf&(1<<portid)))//��λ��Ϊ1˵��������˿ڣ��޷��������
            return -1;
        if(value==1)//����ߵ�ƽ
            p1out|=(1<<portid);//��Ӧ��λ����1
        if(value==0)//����͵�ƽ
            p1out&=(~(1<<portid));//��Ӧ��λ����0
        ret=TCA9535_Write_Data(TCA9535_REG_OutputPort1,&p1out,1);//�����úõ�8λ��ֵд���Ӧ�Ĵ���
        if(ret==-1)
            return -1;

    }
    return true;
}


bool_t text(void)
{
//  s8 value;
    u8 p0=0;
    u8 p1=255;

//  TCA9535_Write_Data(TCA9535_REG_Configuration0,&p1,8);
//  TCA9535_Write_Data(TCA9535_REG_OutputPort0,&p0,8);

    TCA9535_Write_Data(TCA9535_REG_Configuration1,&p0,1);

//  IIC_Write(pg_TCA9535_Dev, 0x07, &p0, 1,0, timeout);
//    TCA9535_PortWrite(port14,1);

    TCA9535_Write_Data(TCA9535_REG_OutputPort1,&p1,1);



    return 1;
}
// =============================================================================
// ���ܣ���ʼ��tca9535оƬ������IIC������ʼ������
// ����:
// ����: true= �ɹ���false=ʧ��
// =============================================================================
/* =============================================================================
 ���ܣ�tca9535оƬ��ʼ������ʼ���ͼ����豸����Ӧ��IIC����.
 ��������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/

bool_t TCA9535_Init(void)
{
    ptSemID_TCA9535 = Lock_MutexCreate("TCA9535 Lock");
    if(!ptSemID_TCA9535)
    {
        printf("\r\n: error : device : TAC9535 initialization failed<lock>.");
        return (FALSE);
    }

    pg_TCA9535_Dev = IIC_DevAdd(CFG_TCA9535_BUS_NAME, "TCA9535", CFG_TCA9535_ADDR, 0, 8);
    if(!pg_TCA9535_Dev)
    {
        Lock_MutexDelete(ptSemID_TCA9535);
        printf("\r\n: error : device : TAC9535 initialization failed<device>.");
        return (FALSE);
    }

    IIC_BusCtrl(pg_TCA9535_Dev,CN_IIC_SET_CLK,CFG_TCA9535_CLK_FRE,0);//����ʱ�Ӵ�С
    IIC_BusCtrl(pg_TCA9535_Dev,CN_IIC_SET_POLL,0,0);       //ʹ�÷�ʽ����
    printf("\r\n: error : device : TAC9535 added.");
    return (TRUE);
}

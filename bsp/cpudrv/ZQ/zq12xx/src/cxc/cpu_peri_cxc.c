#ifdef CFG_CPU_ZQ12XX_M0
#include "cpu_peri.h"
#include "cpu_peri_cxc.h"
#include "lock.h"

//��ǰģ��ΪM0 ʹ��
#define CN_CURRENT_M0_USE   (true)
//��ǰģ��ΪRISCʹ��
#define CN_CURRENT_RISC_USE (false)
//��ǰģ��ΪDSPʹ��
#define CN_CURRENT_DSP_USE  (false)

#define CN_SOFT_INT_NUM_MAX        (32)
typedef u32 (*fnSoftInt)(u32 softIrqID);

static u32 sg_SoftIntMapBit = 0;

typedef struct
{
    struct SemaphoreLCB *m_ptM0RevFromRisc;
    struct SemaphoreLCB *m_ptM0RevFromDsp;
    struct SemaphoreLCB *m_ptRiscRevFromM0;
    struct SemaphoreLCB *m_ptRiscRevFromDsp;
}tagSempInformCtl;

static tagSempInformCtl spg_SempCtl;

//soft int para

typedef struct
{
    fnSoftInt m_Handle;
    u32 m_Para;
}tagSoftIntPara;

static tagSoftIntPara  g_SoftIntPara[CN_SOFT_INT_NUM_MAX] = {0};

//Mutex Register

#define RegMutexBase ((volatile u32*)(0x42020000))

//MailBox

#define RegMailBoxBase ((volatile u32*)(0x42020000 + 0x400))

//MailBox Lenth

#define RegMailBoxNumMax (32)


#define CN_MAIL_BOX_M0_RISC_START  (0 )  //M0��Risc����ͨ��������ʼ��ַ
#define CN_MAIL_BOX_M0_DSP_START   (10)  //M0 Dsp ����ͨ��������ʼ��ַ
#define	CN_MAIL_BOX_RISC_DSP_START (20)  //RiscDsp����ͨ��������ʼ��ַ
#define CN_EACH_MAIL_BOX_LENTH     (10)  //�����޳���

//M0   ������ж��ж��� 0 ~ 12
//RISC ������ж��ж���13 ~25
//Dsp�� ������,ʣ����ж������ں˼�ͨ����
//�˼�ͨ�Ų��õ�����ʽ-˫������ʹ��һ����ͬ������,ͬһʱ��ֻ����һ������
//ֻ�еȵ����շ�������Ϻ���ܽ��з���
//Mutex0 M0 - RISC
//Mutex1 M0 - DSP
//Mutex2 RISC - DSP

typedef struct
{
   volatile u32 RAW_INT_STATUS;    //0x800
   volatile u32 RAW_INT_SET;       //0x804
   volatile u32 RAW_INT_CLR;       //0x808
   volatile u32 RISC_INT_MASK;     //0x80c
   volatile u32 DSP_INT_MASK;      //0x810
   volatile u32 MCU_INT_MASK;      //0x814
   volatile u32 RISC_INT_STATUS;   //0x818
   volatile u32 DSP_INT_STATUS;    //0x81C
   volatile u32 MCU_INT_STATUS;    //0x820
}tagRegCXCInt;

#define RegNuclearIntBase ((volatile tagRegCXCInt*) (0x42020000 + 0x800))


//-----------------------------------------------------------------------------
//ģ��˵��:
//����Ϊ����ж�ģ��:
//��Ҫ�����¼����������:
//                  softIntIrqRequst:��������ж�
//                  softIntIrqFree  :�ͷ�����ж�
//                  SoftIntStart    :��������ж�
//                  RegisterSoftInt :ע���û����жϷ�����
//Return Value:
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//��������:
//
//�β�: ����ж϶�Ӧ���жϺ�- 0~31
//����ֵ :
//-----------------------------------------------------------------------------

//static volatile u32 softIntStatus;
static u32 softIntIsr(u32 irqID)
{
    u32 softIrq;
    u32 softIntStatus;
    u32 loop = 0;
    u32 UsrPara;
    u8 saveIntLine;

    softIntStatus = RegNuclearIntBase->RAW_INT_STATUS;

    while(loop < 32)
    {
        if(softIntStatus & (1<<loop))
        {
            saveIntLine = loop;
//            RegNuclearIntBase->RAW_INT_CLR |= (1<<loop);

            //�����û��жϷ�����
            if(g_SoftIntPara[loop].m_Handle != NULL)
            {
                UsrPara = g_SoftIntPara[loop].m_Para;
                g_SoftIntPara[loop].m_Handle(UsrPara);
//                break;
            }
        }
        loop++;
    }

    RegNuclearIntBase->RAW_INT_CLR |= (1<<saveIntLine);
}


//-----------------------------------------------------------------------------
//��������: ��������ж��ж���
//
//����:void
//����ֵ:����M0�����뷵��ֵ 0~12 ����Risc������ ����ֵ 13~25
//-----------------------------------------------------------------------------

u8 SoftIntIrqRequst(void)
{
    u8 loop  = 0;
    u8 byRet = 0xff;

    if(CN_CURRENT_M0_USE)
    {
    	//�鿴��ǰ��M0 �˻���RISC��ʹ��,M0��ʹ�� ���� 0~12���ж�������������ж�
    	loop = 0;
    	//��ѯ�Ƿ������жϺſ���
		while(loop < 13)//0~13
		{
			if((sg_SoftIntMapBit & (1<<loop)) == 0)
			{
				byRet = loop;
				sg_SoftIntMapBit |= (1<<loop);
				//�ر�����
			    RegNuclearIntBase->MCU_INT_MASK &= ~(1<<loop);
			    //��ֹRisc �Լ�Dsp
			    RegNuclearIntBase->RISC_INT_MASK |= (1<<loop);
			    RegNuclearIntBase->DSP_INT_MASK  |= (1<<loop);
				break;
			}else
			{
				loop++;
			}
		}

    }
    else if(CN_CURRENT_RISC_USE == true)
	{
    	loop = 13;

    	//��ѯ�Ƿ������жϺſ���
		while(loop < 26)//13~26
		{
			if((sg_SoftIntMapBit & (1<<loop)) == 0)
			{
				byRet = loop;
				sg_SoftIntMapBit |= (1<<loop);
				//�ر�����
				RegNuclearIntBase->RISC_INT_MASK &= ~(1<<loop);
				//��ֹM0��Dsp
				RegNuclearIntBase->MCU_INT_MASK |= (1<<loop);
				RegNuclearIntBase->DSP_INT_MASK  |= (1<<loop);
				break;
			}else
			{
				loop++;
			}
		}

	}

    return byRet;
}


//-----------------------------------------------------------------------------
//�������� :�ͷ����ж��ж���-M0 RISC ͨ��
//����˵��:
//irqID: ����жϺ�  0~25
//����ֵ : �ɹ� true ʧ�� false
//-----------------------------------------------------------------------------

bool_t SoftIntIrqFree(u8 irqID)
{
    bool_t bRet = false;

    if(irqID >= 32)
    {
        printk("softIntLine Err\r\n");
        return bRet;
    }

    if((sg_SoftIntMapBit & (1<<irqID)) != 0)
    {
        sg_SoftIntMapBit &= ~(1<<irqID);

        if(CN_CURRENT_M0_USE == true)                     //��ǰ��M0��ʹ��
        {
            RegNuclearIntBase->MCU_INT_MASK |= (1<<irqID);

        }else if(CN_CURRENT_RISC_USE == true)             //CK��ʹ��
        {
            RegNuclearIntBase->RISC_INT_MASK |= (1<<irqID);

        }else if(CN_CURRENT_DSP_USE == true)              //DSP��ʹ��
        {
            RegNuclearIntBase->DSP_INT_MASK |= (1<<irqID);
        }

        bRet = true;
    }else
    {
        bRet = false;
    }
}


//-----------------------------------------------------------------------------
//�������ܣ�
//
//����:
//����ֵ:
//-----------------------------------------------------------------------------

void SoftIntStart(u8 irqID)
{
    if(irqID > 31)
    {
        printk("SoftIrq Out Of Range\r\n");
        return;
    }

    RegNuclearIntBase->RAW_INT_SET |= (1<<irqID);

}


//-----------------------------------------------------------------------------
//����ԭ��: ע������жϷ�����
//      
//�β�:irqID ���ж��жϺ�
//    djybsp_isr_hdl_t:�û��жϷ�����
//    para            :�û�����
//����ֵ:true - �ɹ�ע��  false - ʧ��ע��
//-----------------------------------------------------------------------------

#include "cpu_peri_isr.h"

#define NUCLEAR_CXC_INT_ILNE (15)

bool_t RegisterSoftInt(u8 irqID,djybsp_isr_hdl_t cxc_hdl,u32 para)
{
	bool_t bRet = false;

    if(irqID > 31)
    {
    	bRet = false;
    }else
    {
    	bRet = true;
    }

    g_SoftIntPara[irqID].m_Handle  = cxc_hdl;
    g_SoftIntPara[irqID].m_Para    = para;

    djybsp_isr_hdl_register(NUCLEAR_CXC_INT_ILNE, irqID, softIntIsr,0);

    return bRet;

}


//-----------------------------------------------------------------------------
//ģ��˵��:
//���º˼�ͨ��ģ��:
//��Ҫ�����¼����������:
//
//Return Value:
//-----------------------------------------------------------------------------

#define CN_RISC_SEND_TO_M0  (31)   //����RISC ��M0 ������Ϣ
#define CN_M0_SEND_TO_RISC  (30)   //����M0��   RISC ������Ϣ
#define CN_DSP_SEND_TO_M0   (29)   //����DSP��M0 ������Ϣ
#define CN_M0_SEND_TO_DSP   (28)   //����M0��DSP������Ϣ
#define CN_DSP_SEND_TO_RISC (27)   //��DSP��RISC������Ϣ
#define CN_RISC_SEND_TO_DSP (26)   //����RISC��DSP������Ϣ

//-----------------------------------------------------------------------------
//����ԭ��: M0������Ϣ������
//
//�β�:
//-----------------------------------------------------------------------------

//M0 ��RISC��������

u32 M0RevDataFromRiscHld(u32 irqID)
{
    u8 loop = 0;
    //�ͷ��ź���
    if(spg_SempCtl.m_ptM0RevFromRisc != NULL)
    {
        Lock_SempPost(spg_SempCtl.m_ptM0RevFromRisc);
    }
}

//M0 ��DSP��������

u32 M0RevDataFromDspHld(u32 irqID)
{
    u8 loop = 0;

    if(spg_SempCtl.m_ptM0RevFromDsp != NULL)
    {
        Lock_SempPost(spg_SempCtl.m_ptM0RevFromDsp);
    }
}


//-----------------------------------------------------------------------------
//����ԭ��: M0������Ϣ��ʼ��
//
//�β�:fromObj ���ͷ�
//    :0 - ����Risc
//    :1 - ����Dsp
//����ֵ:None
//-----------------------------------------------------------------------------

void M0ToRevMessageInit(u8 fromObj)
{
    switch(fromObj)
    {
        case EN_M0_REV_FROM_RISC:
        //��������֪ͨ�ź���
        spg_SempCtl.m_ptM0RevFromRisc = Lock_SempCreate(1,0,0,"M0RevRisc");
        if(spg_SempCtl.m_ptM0RevFromRisc == NULL)
        {
            printk("Create m_ptM0RevFromRisc Failed\r\n");
            return;
        }
        //�����ж���-M0��Ӧ�ж� CK ����DSP�����ж�,M0���ý���ģʽ
        RegNuclearIntBase->MCU_INT_MASK &= ~(1<<CN_RISC_SEND_TO_M0);
        //��ֹRISC �Լ�DSP����Ӧ���ж�
        RegNuclearIntBase->RISC_INT_MASK |= (1<<CN_RISC_SEND_TO_M0);
        RegNuclearIntBase->DSP_INT_MASK  |= (1<<CN_RISC_SEND_TO_M0);
        //ע�����ݽ����жϷ�����
        RegisterSoftInt(CN_RISC_SEND_TO_M0,M0RevDataFromRiscHld,CN_RISC_SEND_TO_M0);
        break;
        case EN_RISC_REV_FROM_DSP:
        spg_SempCtl.m_ptM0RevFromDsp = Lock_SempCreate(1,0,0,"M0RevDsp");
        if(spg_SempCtl.m_ptM0RevFromDsp == NULL)
        {
            printk("Create m_ptM0RevFromDsp Failed\r\n");
            return;
        }
        //�����ж���-M0��Ӧ�ж� CK ����DSP�����ж�,M0���ý���ģʽ
        RegNuclearIntBase->MCU_INT_MASK &= ~(1<<CN_DSP_SEND_TO_M0);
        //��ֹRISC �Լ�DSP����Ӧ���ж�
        RegNuclearIntBase->RISC_INT_MASK |= (1<<CN_DSP_SEND_TO_M0);
        RegNuclearIntBase->DSP_INT_MASK  |= (1<<CN_DSP_SEND_TO_M0);

        //ע�����ݽ����жϷ�����
        RegisterSoftInt(CN_DSP_SEND_TO_M0,M0RevDataFromDspHld,CN_DSP_SEND_TO_M0);
        break;
        default:;break;
    }

}


//-----------------------------------------------------------------------------
//����: RISC ����M0 �˷�������
//
//�β�:buf �������ݻ���
//    len:�������ݳ���-���Ȳ�����32*4 bytes
//
//����ֵ:None
//-----------------------------------------------------------------------------

void RiscSendToM0(u32 *buf,u8 len)
{
    u8 loop;
    //M0 ��û���������Ȳ�����
    if(len > CN_EACH_MAIL_BOX_LENTH)
    {
    	len = CN_EACH_MAIL_BOX_LENTH;
    }

    while(1)
    {   //Use Mutex 0
        if(RegMutexBase[0] & 0x1)//����M0��û���գ��ȵ�M0������ɺ��ٽ���
        {
            continue;
        }

        for(loop = 0;loop < len;loop++)
        {
            RegMailBoxBase[loop + CN_MAIL_BOX_M0_RISC_START] = buf[loop];
        }
        //֪ͨM0����
        SoftIntStart(CN_RISC_SEND_TO_M0);
        //�ȴ�M0��Ӧ���ݽ���
        while(RegNuclearIntBase->MCU_INT_STATUS & (1<<CN_RISC_SEND_TO_M0) == 0)
        {

        }
        break;
    }
}


//-----------------------------------------------------------------------------
//����ԭ��: Dsp ��������
//
//�β�:buf �������ݻ���
//    len:�������ݳ���-���Ȳ�����32*4 bytes
//
//����ֵ:None
//-----------------------------------------------------------------------------

void DspSendToM0(u32 *buf,u8 len)
{
    u8 loop;

    if(len > CN_EACH_MAIL_BOX_LENTH)
    {
       len = CN_EACH_MAIL_BOX_LENTH;
    }

    //M0 ��û���������Ȳ�����
    while(1)
    {   //Use Mutex 1
        if(RegMutexBase[1] & 0x1)//����M0��û���գ��ȵ�M0������ɺ��ٽ���
        {
            continue;
        }

        for(loop = 0;loop < len;loop++)
        {
            RegMailBoxBase[loop + CN_MAIL_BOX_M0_DSP_START] = buf[loop];
        }

        //֪ͨM0����
        SoftIntStart(CN_RISC_SEND_TO_M0);
        //�ȴ�M0��Ӧ���ݽ���
        while(RegNuclearIntBase->MCU_INT_STATUS & (1<<CN_RISC_SEND_TO_M0) == 0)
        {

        }
        break;

    }

}


//-----------------------------------------------------------------------------
//����ԭ��: M0�˽���RISC������
//
//�β�:buf �������ݻ���
//    len:�������ݳ���-���Ȳ�����10*4 bytes
//
//����ֵ:None
//-----------------------------------------------------------------------------

u8 M0ToRevRiscData(u32 *buf,u8 len)
{
   if(buf == NULL)
   {
       printk("buf is NUll\r\n");
       return 0;
   }

   if(len > CN_EACH_MAIL_BOX_LENTH)//���ֻ�ܽ���CN_EACH_MAIL_BOX_LENTH
   {
	   len = CN_EACH_MAIL_BOX_LENTH;
   }
   //�ȴ����� ����
   if(spg_SempCtl.m_ptM0RevFromRisc != NULL)
   {
       Lock_SempPend(spg_SempCtl.m_ptM0RevFromRisc,0xffffffff);
   }

   RegNuclearIntBase->RAW_INT_CLR |= (1<<CN_RISC_SEND_TO_M0);
   u8 loop = 0;
   while(loop < len)
   {   //���ջ����ַ
       buf[loop] = RegMailBoxBase[loop + CN_MAIL_BOX_M0_RISC_START];
       loop++;
   }

   RegMutexBase[0] = 0;//�ͷź˼以����
   memset(&RegMailBoxBase[CN_MAIL_BOX_M0_RISC_START],0,len);

   return loop;
}


//-----------------------------------------------------------------------------
//����ԭ��: M0����Dsp����
//
//�β�:buf �������ݻ���
//    len:�������ݳ���-���Ȳ�����32*4 bytes
//
//����ֵ:None
//-----------------------------------------------------------------------------

u8 M0ToRevDspData(u32 *buf,u8 len)
{
    if(buf == NULL)
    {
        printk("buf is NUll\r\n");
        return 0;
    }

    if(len > CN_EACH_MAIL_BOX_LENTH)//���ֻ�ܽ���CN_EACH_MAIL_BOX_LENTH
    {
        len = CN_EACH_MAIL_BOX_LENTH;
    }

    //�ȴ����� ����
    if(spg_SempCtl.m_ptM0RevFromRisc != NULL)
    {
        Lock_SempPend(spg_SempCtl.m_ptM0RevFromDsp,0xffffffff);
    }

    RegNuclearIntBase->RAW_INT_CLR |= (1<<CN_DSP_SEND_TO_M0);

    u8 loop = 0;//���ջ����ַ
    //���ݿ������û��ռ�
    while(loop < len)
    {
         buf[loop] = RegMailBoxBase[loop + CN_MAIL_BOX_M0_DSP_START];
         loop++;
    }
    //�ͷź˼以����-��֪M0 �������
    RegMutexBase[1] = 0;//�ͷź˼以����
    memset(&RegMailBoxBase[CN_MAIL_BOX_M0_DSP_START],0,len);

    return (loop - CN_MAIL_BOX_M0_DSP_START);
}

//-----------------------------------------------------------------------------
//����ԭ��: M0������Ϣ��ʼ��
//
//�β�:fromObj ���ͷ�
//    :2 - ����M0
//    :3 - ����Dsp
//����ֵ:None
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//����ԭ��: M0������Ϣ������
//
//�β�:
//-----------------------------------------------------------------------------

//RISC�� M0 ��������

u32 RiscRevDataFromM0Hld(u32 irqID)
{
    u8 loop = 0;
    //�ͷ��ź���
    if(spg_SempCtl.m_ptRiscRevFromM0 != NULL)
    {
        Lock_SempPost(spg_SempCtl.m_ptRiscRevFromM0);
    }
}

//Risc ��Dsp ��������

u32 RiscRevDataFromDspHld(u32 irqID)
{

    if(spg_SempCtl.m_ptRiscRevFromDsp != NULL)
    {
        Lock_SempPost(spg_SempCtl.m_ptRiscRevFromDsp);
    }
}


void RiscToRevMessageInit(u8 fromObj)
{
    switch(fromObj)
    {
        case EN_RISC_REV_FROM_M0:
        //��������֪ͨ�ź���
        spg_SempCtl.m_ptRiscRevFromM0 = Lock_SempCreate(1,0,0,"RiscRevM0");
        if(spg_SempCtl.m_ptRiscRevFromM0 == NULL)
        {
            printk("Create m_ptRiscRevFromM0 Failed\r\n");
            return;
        }
        //�����ж���-CK��Ӧ�ж� M0 ����DSP�����ж�,Risc���ý���ģʽ
        RegNuclearIntBase->RISC_INT_MASK &= ~(1<<CN_M0_SEND_TO_RISC);
        //��ֹM0 �Լ�DSP����Ӧ���ж�
        RegNuclearIntBase->MCU_INT_MASK |= (1<<CN_M0_SEND_TO_RISC);
        RegNuclearIntBase->DSP_INT_MASK  |= (1<<CN_M0_SEND_TO_RISC);
        //ע�����ݽ����жϷ�����
        RegisterSoftInt(CN_M0_SEND_TO_RISC,RiscRevDataFromM0Hld,CN_M0_SEND_TO_RISC);
        break;
        case EN_RISC_REV_FROM_DSP:
        spg_SempCtl.m_ptRiscRevFromDsp = Lock_SempCreate(1,0,0,"RiscRevDsp");
        if(spg_SempCtl.m_ptRiscRevFromDsp == NULL)
        {
            printk("Create m_ptRiscRevFromDsp Failed\r\n");
            return;
        }
        //�����ж���-CK��Ӧ�ж� M0 ����DSP�����ж�,Risc���ý���ģʽ
        RegNuclearIntBase->RISC_INT_MASK &= ~(1<<CN_DSP_SEND_TO_RISC);
        //��ֹRISC �Լ�DSP����Ӧ���ж�
        RegNuclearIntBase->MCU_INT_MASK  |= (1<<CN_DSP_SEND_TO_RISC);
        RegNuclearIntBase->DSP_INT_MASK  |= (1<<CN_DSP_SEND_TO_RISC);

        //ע�����ݽ����жϷ�����
        RegisterSoftInt(CN_DSP_SEND_TO_RISC,RiscRevDataFromDspHld,CN_DSP_SEND_TO_RISC);
        break;
        default:;break;
    }

}


//-----------------------------------------------------------------------------
//����ԭ��: M0���� Risc����
//
//�β�:buf �������ݻ���
//    len:�������ݳ���-���Ȳ�����32*4 bytes
//
//����ֵ:���͵�����
//-----------------------------------------------------------------------------

u8 M0SendToRisc(u32 *buf,u8 len)
{
    u8 loop;

    if(buf == NULL)
    {
        printk("buf is Null\r\n");
        return 0;
    }

    if(len > CN_EACH_MAIL_BOX_LENTH)
    {
        len = CN_EACH_MAIL_BOX_LENTH;
    }

    //Risc ��û���������Ȳ�����
    while(1)
    {   //Use Mutex 0
        if(RegMutexBase[0] & 0x1)//����Risc��û���գ��ȵ�Risc������ɺ��ٽ���
        {
            continue;
        }

        for(loop = 0;loop < len;loop++)
        {
            RegMailBoxBase[CN_MAIL_BOX_M0_RISC_START + loop] = buf[loop];
        }
        //֪ͨM0����
        SoftIntStart(CN_M0_SEND_TO_RISC);

        //�ȴ�M0��Ӧ���ݽ���
        while(RegNuclearIntBase->RISC_INT_STATUS & (1<<CN_M0_SEND_TO_RISC) == 0)
        {

        }
        break;
   }

}


//-----------------------------------------------------------------------------
//����ԭ��: Risc����M0����
//
//�β�:buf �������ݻ���
//    len:�������ݳ���-���Ȳ�����32*4 bytes
//
//����ֵ:None
//-----------------------------------------------------------------------------

u8 RiscToRevM0Data(u32 *buf,u8 len)
{
    if(buf == NULL)
    {
        printk("buf is NUll\r\n");
        return 0;
    }

    if(len > CN_EACH_MAIL_BOX_LENTH)//���ֻ�ܽ���CN_EACH_MAIL_BOX_LENTH
    {
        len = CN_EACH_MAIL_BOX_LENTH;
    }

    //�ȴ����� ����
    if(spg_SempCtl.m_ptRiscRevFromM0 != NULL)
    {
        Lock_SempPend(spg_SempCtl.m_ptRiscRevFromM0,0xffffffff);
    }

    RegNuclearIntBase->RAW_INT_CLR |= (1<<CN_M0_SEND_TO_RISC);

    u8 loop = 0;//���ջ����ַ
    //���ݿ������û��ռ�
    while(loop < len)
    {
         buf[loop] = RegMailBoxBase[loop + CN_MAIL_BOX_M0_RISC_START];
         loop++;
    }
    //�ͷź˼以����-��֪M0 �������
    RegMutexBase[0] = 0;//�ͷź˼以����

    memset(&RegMailBoxBase[CN_MAIL_BOX_M0_RISC_START],0,len);

    return (loop - CN_MAIL_BOX_M0_RISC_START);
}


//-----------------------------------------------------------------------------
//����: Dsp���� Risc����
//
//�β�:buf �������ݻ���
//    len:�������ݳ���-���Ȳ�����32*4 bytes
//
//����ֵ:���͵�����
//-----------------------------------------------------------------------------

u8 DspToSendRiscData(u32 *buf,u8 len)
{
    u8 loop;

    if(buf == NULL)
    {
        printk("buf is Null\r\n");
        return 0;
    }

    if(len > CN_EACH_MAIL_BOX_LENTH)
    {
        len = CN_EACH_MAIL_BOX_LENTH;
    }

    //Risc ��û���������Ȳ�����
    while(1)
    {   //Use Mutex 2
        if(RegMutexBase[2] & 0x1)//����Risc��û���գ��ȵ�Risc������ɺ��ٽ���
        {
            continue;
        }

        for(loop = 0;loop < len;loop++)
        {
            RegMailBoxBase[loop + CN_MAIL_BOX_RISC_DSP_START] = buf[loop];
        }
        //֪ͨRisc����
        SoftIntStart(CN_DSP_SEND_TO_RISC);
        //�ȴ�Risc��Ӧ���ݽ���
        while(RegNuclearIntBase->RISC_INT_STATUS & (1<<CN_DSP_SEND_TO_RISC) == 0)
        {

        }
        break;
   }

}


//-----------------------------------------------------------------------------
//����ԭ��: Risc����M0����
//
//�β�:buf �������ݻ���
//    len:�������ݳ���-���Ȳ�����32*4 bytes
//
//����ֵ:None
//-----------------------------------------------------------------------------

u8 RiscToRevDspData(u32 *buf,u8 len)
{
    if(buf == NULL)
    {
        printk("buf is NUll\r\n");
        return 0;
    }

    if(len > CN_EACH_MAIL_BOX_LENTH)//���ֻ�ܽ���CN_EACH_MAIL_BOX_LENTH
    {
        len = CN_EACH_MAIL_BOX_LENTH;
    }

    //�ȴ����� ����
    if(spg_SempCtl.m_ptRiscRevFromDsp != NULL)
    {
        Lock_SempPend(spg_SempCtl.m_ptRiscRevFromDsp,0xffffffff);
    }

    RegNuclearIntBase->RAW_INT_CLR |= (1<<CN_DSP_SEND_TO_RISC);

    u8 loop = 0;//���ջ����ַ
    //���ݿ������û��ռ�
    while(loop < len)
    {
         buf[loop] = RegMailBoxBase[loop + CN_MAIL_BOX_RISC_DSP_START];
         loop++;
    }
    //�ͷź˼以����-��֪M0 �������
    RegMutexBase[2] = 0;//�ͷź˼以����
    //���ݿ������û��ռ��,�建��
    memset(&RegMailBoxBase[CN_MAIL_BOX_RISC_DSP_START],0,len);

    return (loop - CN_MAIL_BOX_RISC_DSP_START);
}

#endif













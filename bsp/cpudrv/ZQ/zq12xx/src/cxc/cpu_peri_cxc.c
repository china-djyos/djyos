#ifdef CFG_CPU_ZQ12XX_M0
#include "cpu_peri.h"
#include "cpu_peri_cxc.h"
#include "lock.h"

//当前模块为M0 使用
#define CN_CURRENT_M0_USE   (true)
//当前模块为RISC使用
#define CN_CURRENT_RISC_USE (false)
//当前模块为DSP使用
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


#define CN_MAIL_BOX_M0_RISC_START  (0 )  //M0与Risc单工通信邮箱起始地址
#define CN_MAIL_BOX_M0_DSP_START   (10)  //M0 Dsp 单工通信邮箱起始地址
#define	CN_MAIL_BOX_RISC_DSP_START (20)  //RiscDsp单工通信邮箱起始地址
#define CN_EACH_MAIL_BOX_LENTH     (10)  //各有限长度

//M0   核软件中断中断线 0 ~ 12
//RISC 核软件中断中断线13 ~25
//Dsp暂 不分配,剩余的中断线用于核间通信用
//核间通信采用单工形式-双方发送使用一对相同互斥量,同一时间只能有一方发送
//只有等到接收方接收完毕后才能进行发送
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
//模块说明:
//以下为软件中断模块:
//主要由以下几个函数组成:
//                  softIntIrqRequst:申请软件中断
//                  softIntIrqFree  :释放软件中断
//                  SoftIntStart    :启动软件中断
//                  RegisterSoftInt :注册用户软中断服务函数
//Return Value:
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//函数功能:
//
//形参: 软件中断对应的中断号- 0~31
//返回值 :
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

            //调用用户中断服务函数
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
//函数功能: 申请软件中断中断线
//
//参数:void
//返回值:若是M0核申请返回值 0~12 若是Risc核申请 返回值 13~25
//-----------------------------------------------------------------------------

u8 SoftIntIrqRequst(void)
{
    u8 loop  = 0;
    u8 byRet = 0xff;

    if(CN_CURRENT_M0_USE)
    {
    	//查看当前是M0 核还是RISC核使用,M0核使用 分配 0~12号中断用于软件触发中断
    	loop = 0;
    	//查询是否有软中断号可用
		while(loop < 13)//0~13
		{
			if((sg_SoftIntMapBit & (1<<loop)) == 0)
			{
				byRet = loop;
				sg_SoftIntMapBit |= (1<<loop);
				//关闭屏蔽
			    RegNuclearIntBase->MCU_INT_MASK &= ~(1<<loop);
			    //禁止Risc 以及Dsp
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

    	//查询是否有软中断号可用
		while(loop < 26)//13~26
		{
			if((sg_SoftIntMapBit & (1<<loop)) == 0)
			{
				byRet = loop;
				sg_SoftIntMapBit |= (1<<loop);
				//关闭屏蔽
				RegNuclearIntBase->RISC_INT_MASK &= ~(1<<loop);
				//禁止M0和Dsp
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
//函数功能 :释放软中断中断线-M0 RISC 通用
//参数说明:
//irqID: 软件中断号  0~25
//返回值 : 成功 true 失败 false
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

        if(CN_CURRENT_M0_USE == true)                     //当前是M0核使用
        {
            RegNuclearIntBase->MCU_INT_MASK |= (1<<irqID);

        }else if(CN_CURRENT_RISC_USE == true)             //CK核使用
        {
            RegNuclearIntBase->RISC_INT_MASK |= (1<<irqID);

        }else if(CN_CURRENT_DSP_USE == true)              //DSP核使用
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
//函数功能：
//
//参数:
//返回值:
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
//函数原型: 注册软件中断服务函数
//      
//形参:irqID 软中断中断号
//    djybsp_isr_hdl_t:用户中断服务函数
//    para            :用户参数
//返回值:true - 成功注册  false - 失败注册
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
//模块说明:
//以下核间通信模块:
//主要由以下几个函数组成:
//
//Return Value:
//-----------------------------------------------------------------------------

#define CN_RISC_SEND_TO_M0  (31)   //用于RISC 向M0 发送消息
#define CN_M0_SEND_TO_RISC  (30)   //用于M0向   RISC 发送消息
#define CN_DSP_SEND_TO_M0   (29)   //用于DSP向M0 发送消息
#define CN_M0_SEND_TO_DSP   (28)   //用于M0向DSP发送消息
#define CN_DSP_SEND_TO_RISC (27)   //用DSP向RISC发送消息
#define CN_RISC_SEND_TO_DSP (26)   //用于RISC向DSP发送消息

//-----------------------------------------------------------------------------
//函数原型: M0接收消息服务函数
//
//形参:
//-----------------------------------------------------------------------------

//M0 从RISC接收数据

u32 M0RevDataFromRiscHld(u32 irqID)
{
    u8 loop = 0;
    //释放信号量
    if(spg_SempCtl.m_ptM0RevFromRisc != NULL)
    {
        Lock_SempPost(spg_SempCtl.m_ptM0RevFromRisc);
    }
}

//M0 从DSP接收数据

u32 M0RevDataFromDspHld(u32 irqID)
{
    u8 loop = 0;

    if(spg_SempCtl.m_ptM0RevFromDsp != NULL)
    {
        Lock_SempPost(spg_SempCtl.m_ptM0RevFromDsp);
    }
}


//-----------------------------------------------------------------------------
//函数原型: M0接收消息初始化
//
//形参:fromObj 发送方
//    :0 - 来自Risc
//    :1 - 来自Dsp
//返回值:None
//-----------------------------------------------------------------------------

void M0ToRevMessageInit(u8 fromObj)
{
    switch(fromObj)
    {
        case EN_M0_REV_FROM_RISC:
        //创建接收通知信号量
        spg_SempCtl.m_ptM0RevFromRisc = Lock_SempCreate(1,0,0,"M0RevRisc");
        if(spg_SempCtl.m_ptM0RevFromRisc == NULL)
        {
            printk("Create m_ptM0RevFromRisc Failed\r\n");
            return;
        }
        //申请中断线-M0响应中断 CK 或者DSP触发中断,M0配置接收模式
        RegNuclearIntBase->MCU_INT_MASK &= ~(1<<CN_RISC_SEND_TO_M0);
        //禁止RISC 以及DSP核响应该中断
        RegNuclearIntBase->RISC_INT_MASK |= (1<<CN_RISC_SEND_TO_M0);
        RegNuclearIntBase->DSP_INT_MASK  |= (1<<CN_RISC_SEND_TO_M0);
        //注册数据接收中断服务函数
        RegisterSoftInt(CN_RISC_SEND_TO_M0,M0RevDataFromRiscHld,CN_RISC_SEND_TO_M0);
        break;
        case EN_RISC_REV_FROM_DSP:
        spg_SempCtl.m_ptM0RevFromDsp = Lock_SempCreate(1,0,0,"M0RevDsp");
        if(spg_SempCtl.m_ptM0RevFromDsp == NULL)
        {
            printk("Create m_ptM0RevFromDsp Failed\r\n");
            return;
        }
        //申请中断线-M0响应中断 CK 或者DSP触发中断,M0配置接收模式
        RegNuclearIntBase->MCU_INT_MASK &= ~(1<<CN_DSP_SEND_TO_M0);
        //禁止RISC 以及DSP核响应该中断
        RegNuclearIntBase->RISC_INT_MASK |= (1<<CN_DSP_SEND_TO_M0);
        RegNuclearIntBase->DSP_INT_MASK  |= (1<<CN_DSP_SEND_TO_M0);

        //注册数据接收中断服务函数
        RegisterSoftInt(CN_DSP_SEND_TO_M0,M0RevDataFromDspHld,CN_DSP_SEND_TO_M0);
        break;
        default:;break;
    }

}


//-----------------------------------------------------------------------------
//功能: RISC 核向M0 核发送数据
//
//形参:buf 发送数据缓存
//    len:发送数据长度-长度不超过32*4 bytes
//
//返回值:None
//-----------------------------------------------------------------------------

void RiscSendToM0(u32 *buf,u8 len)
{
    u8 loop;
    //M0 还没接受数据先不发送
    if(len > CN_EACH_MAIL_BOX_LENTH)
    {
    	len = CN_EACH_MAIL_BOX_LENTH;
    }

    while(1)
    {   //Use Mutex 0
        if(RegMutexBase[0] & 0x1)//假设M0还没接收，等到M0接收完成后再接收
        {
            continue;
        }

        for(loop = 0;loop < len;loop++)
        {
            RegMailBoxBase[loop + CN_MAIL_BOX_M0_RISC_START] = buf[loop];
        }
        //通知M0接收
        SoftIntStart(CN_RISC_SEND_TO_M0);
        //等待M0响应数据接收
        while(RegNuclearIntBase->MCU_INT_STATUS & (1<<CN_RISC_SEND_TO_M0) == 0)
        {

        }
        break;
    }
}


//-----------------------------------------------------------------------------
//函数原型: Dsp 发送数据
//
//形参:buf 发送数据缓存
//    len:发送数据长度-长度不超过32*4 bytes
//
//返回值:None
//-----------------------------------------------------------------------------

void DspSendToM0(u32 *buf,u8 len)
{
    u8 loop;

    if(len > CN_EACH_MAIL_BOX_LENTH)
    {
       len = CN_EACH_MAIL_BOX_LENTH;
    }

    //M0 还没接受数据先不发送
    while(1)
    {   //Use Mutex 1
        if(RegMutexBase[1] & 0x1)//假设M0还没接收，等到M0接收完成后再接收
        {
            continue;
        }

        for(loop = 0;loop < len;loop++)
        {
            RegMailBoxBase[loop + CN_MAIL_BOX_M0_DSP_START] = buf[loop];
        }

        //通知M0接收
        SoftIntStart(CN_RISC_SEND_TO_M0);
        //等待M0响应数据接收
        while(RegNuclearIntBase->MCU_INT_STATUS & (1<<CN_RISC_SEND_TO_M0) == 0)
        {

        }
        break;

    }

}


//-----------------------------------------------------------------------------
//函数原型: M0核接收RISC核数据
//
//形参:buf 发送数据缓存
//    len:发送数据长度-长度不超过10*4 bytes
//
//返回值:None
//-----------------------------------------------------------------------------

u8 M0ToRevRiscData(u32 *buf,u8 len)
{
   if(buf == NULL)
   {
       printk("buf is NUll\r\n");
       return 0;
   }

   if(len > CN_EACH_MAIL_BOX_LENTH)//最大只能接收CN_EACH_MAIL_BOX_LENTH
   {
	   len = CN_EACH_MAIL_BOX_LENTH;
   }
   //等待接收 数据
   if(spg_SempCtl.m_ptM0RevFromRisc != NULL)
   {
       Lock_SempPend(spg_SempCtl.m_ptM0RevFromRisc,0xffffffff);
   }

   RegNuclearIntBase->RAW_INT_CLR |= (1<<CN_RISC_SEND_TO_M0);
   u8 loop = 0;
   while(loop < len)
   {   //接收缓存地址
       buf[loop] = RegMailBoxBase[loop + CN_MAIL_BOX_M0_RISC_START];
       loop++;
   }

   RegMutexBase[0] = 0;//释放核间互斥量
   memset(&RegMailBoxBase[CN_MAIL_BOX_M0_RISC_START],0,len);

   return loop;
}


//-----------------------------------------------------------------------------
//函数原型: M0接收Dsp数据
//
//形参:buf 发送数据缓存
//    len:发送数据长度-长度不超过32*4 bytes
//
//返回值:None
//-----------------------------------------------------------------------------

u8 M0ToRevDspData(u32 *buf,u8 len)
{
    if(buf == NULL)
    {
        printk("buf is NUll\r\n");
        return 0;
    }

    if(len > CN_EACH_MAIL_BOX_LENTH)//最大只能接收CN_EACH_MAIL_BOX_LENTH
    {
        len = CN_EACH_MAIL_BOX_LENTH;
    }

    //等待接收 数据
    if(spg_SempCtl.m_ptM0RevFromRisc != NULL)
    {
        Lock_SempPend(spg_SempCtl.m_ptM0RevFromDsp,0xffffffff);
    }

    RegNuclearIntBase->RAW_INT_CLR |= (1<<CN_DSP_SEND_TO_M0);

    u8 loop = 0;//接收缓存地址
    //数据拷贝到用户空间
    while(loop < len)
    {
         buf[loop] = RegMailBoxBase[loop + CN_MAIL_BOX_M0_DSP_START];
         loop++;
    }
    //释放核间互斥量-告知M0 接收完毕
    RegMutexBase[1] = 0;//释放核间互斥量
    memset(&RegMailBoxBase[CN_MAIL_BOX_M0_DSP_START],0,len);

    return (loop - CN_MAIL_BOX_M0_DSP_START);
}

//-----------------------------------------------------------------------------
//函数原型: M0接收消息初始化
//
//形参:fromObj 发送方
//    :2 - 来自M0
//    :3 - 来自Dsp
//返回值:None
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//函数原型: M0接收消息服务函数
//
//形参:
//-----------------------------------------------------------------------------

//RISC从 M0 接收数据

u32 RiscRevDataFromM0Hld(u32 irqID)
{
    u8 loop = 0;
    //释放信号量
    if(spg_SempCtl.m_ptRiscRevFromM0 != NULL)
    {
        Lock_SempPost(spg_SempCtl.m_ptRiscRevFromM0);
    }
}

//Risc 从Dsp 接收数据

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
        //创建接收通知信号量
        spg_SempCtl.m_ptRiscRevFromM0 = Lock_SempCreate(1,0,0,"RiscRevM0");
        if(spg_SempCtl.m_ptRiscRevFromM0 == NULL)
        {
            printk("Create m_ptRiscRevFromM0 Failed\r\n");
            return;
        }
        //申请中断线-CK响应中断 M0 或者DSP触发中断,Risc配置接收模式
        RegNuclearIntBase->RISC_INT_MASK &= ~(1<<CN_M0_SEND_TO_RISC);
        //禁止M0 以及DSP核响应该中断
        RegNuclearIntBase->MCU_INT_MASK |= (1<<CN_M0_SEND_TO_RISC);
        RegNuclearIntBase->DSP_INT_MASK  |= (1<<CN_M0_SEND_TO_RISC);
        //注册数据接收中断服务函数
        RegisterSoftInt(CN_M0_SEND_TO_RISC,RiscRevDataFromM0Hld,CN_M0_SEND_TO_RISC);
        break;
        case EN_RISC_REV_FROM_DSP:
        spg_SempCtl.m_ptRiscRevFromDsp = Lock_SempCreate(1,0,0,"RiscRevDsp");
        if(spg_SempCtl.m_ptRiscRevFromDsp == NULL)
        {
            printk("Create m_ptRiscRevFromDsp Failed\r\n");
            return;
        }
        //申请中断线-CK响应中断 M0 或者DSP触发中断,Risc配置接收模式
        RegNuclearIntBase->RISC_INT_MASK &= ~(1<<CN_DSP_SEND_TO_RISC);
        //禁止RISC 以及DSP核响应该中断
        RegNuclearIntBase->MCU_INT_MASK  |= (1<<CN_DSP_SEND_TO_RISC);
        RegNuclearIntBase->DSP_INT_MASK  |= (1<<CN_DSP_SEND_TO_RISC);

        //注册数据接收中断服务函数
        RegisterSoftInt(CN_DSP_SEND_TO_RISC,RiscRevDataFromDspHld,CN_DSP_SEND_TO_RISC);
        break;
        default:;break;
    }

}


//-----------------------------------------------------------------------------
//函数原型: M0发送 Risc数据
//
//形参:buf 发送数据缓存
//    len:发送数据长度-长度不超过32*4 bytes
//
//返回值:发送的数据
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

    //Risc 还没接受数据先不发送
    while(1)
    {   //Use Mutex 0
        if(RegMutexBase[0] & 0x1)//假设Risc还没接收，等到Risc接收完成后再接收
        {
            continue;
        }

        for(loop = 0;loop < len;loop++)
        {
            RegMailBoxBase[CN_MAIL_BOX_M0_RISC_START + loop] = buf[loop];
        }
        //通知M0接收
        SoftIntStart(CN_M0_SEND_TO_RISC);

        //等待M0响应数据接收
        while(RegNuclearIntBase->RISC_INT_STATUS & (1<<CN_M0_SEND_TO_RISC) == 0)
        {

        }
        break;
   }

}


//-----------------------------------------------------------------------------
//函数原型: Risc接收M0数据
//
//形参:buf 发送数据缓存
//    len:发送数据长度-长度不超过32*4 bytes
//
//返回值:None
//-----------------------------------------------------------------------------

u8 RiscToRevM0Data(u32 *buf,u8 len)
{
    if(buf == NULL)
    {
        printk("buf is NUll\r\n");
        return 0;
    }

    if(len > CN_EACH_MAIL_BOX_LENTH)//最大只能接收CN_EACH_MAIL_BOX_LENTH
    {
        len = CN_EACH_MAIL_BOX_LENTH;
    }

    //等待接收 数据
    if(spg_SempCtl.m_ptRiscRevFromM0 != NULL)
    {
        Lock_SempPend(spg_SempCtl.m_ptRiscRevFromM0,0xffffffff);
    }

    RegNuclearIntBase->RAW_INT_CLR |= (1<<CN_M0_SEND_TO_RISC);

    u8 loop = 0;//接收缓存地址
    //数据拷贝到用户空间
    while(loop < len)
    {
         buf[loop] = RegMailBoxBase[loop + CN_MAIL_BOX_M0_RISC_START];
         loop++;
    }
    //释放核间互斥量-告知M0 接收完毕
    RegMutexBase[0] = 0;//释放核间互斥量

    memset(&RegMailBoxBase[CN_MAIL_BOX_M0_RISC_START],0,len);

    return (loop - CN_MAIL_BOX_M0_RISC_START);
}


//-----------------------------------------------------------------------------
//功能: Dsp发送 Risc数据
//
//形参:buf 发送数据缓存
//    len:发送数据长度-长度不超过32*4 bytes
//
//返回值:发送的数据
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

    //Risc 还没接受数据先不发送
    while(1)
    {   //Use Mutex 2
        if(RegMutexBase[2] & 0x1)//假设Risc还没接收，等到Risc接收完成后再接收
        {
            continue;
        }

        for(loop = 0;loop < len;loop++)
        {
            RegMailBoxBase[loop + CN_MAIL_BOX_RISC_DSP_START] = buf[loop];
        }
        //通知Risc接收
        SoftIntStart(CN_DSP_SEND_TO_RISC);
        //等待Risc响应数据接收
        while(RegNuclearIntBase->RISC_INT_STATUS & (1<<CN_DSP_SEND_TO_RISC) == 0)
        {

        }
        break;
   }

}


//-----------------------------------------------------------------------------
//函数原型: Risc接收M0数据
//
//形参:buf 发送数据缓存
//    len:发送数据长度-长度不超过32*4 bytes
//
//返回值:None
//-----------------------------------------------------------------------------

u8 RiscToRevDspData(u32 *buf,u8 len)
{
    if(buf == NULL)
    {
        printk("buf is NUll\r\n");
        return 0;
    }

    if(len > CN_EACH_MAIL_BOX_LENTH)//最大只能接收CN_EACH_MAIL_BOX_LENTH
    {
        len = CN_EACH_MAIL_BOX_LENTH;
    }

    //等待接收 数据
    if(spg_SempCtl.m_ptRiscRevFromDsp != NULL)
    {
        Lock_SempPend(spg_SempCtl.m_ptRiscRevFromDsp,0xffffffff);
    }

    RegNuclearIntBase->RAW_INT_CLR |= (1<<CN_DSP_SEND_TO_RISC);

    u8 loop = 0;//接收缓存地址
    //数据拷贝到用户空间
    while(loop < len)
    {
         buf[loop] = RegMailBoxBase[loop + CN_MAIL_BOX_RISC_DSP_START];
         loop++;
    }
    //释放核间互斥量-告知M0 接收完毕
    RegMutexBase[2] = 0;//释放核间互斥量
    //数据拷贝到用户空间后,清缓存
    memset(&RegMailBoxBase[CN_MAIL_BOX_RISC_DSP_START],0,len);

    return (loop - CN_MAIL_BOX_RISC_DSP_START);
}

#endif













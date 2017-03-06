#include "IOT.h"
#include "cpuflash.h"
#include    "stdlib.h"
#include "verify.h"
#include "cpu_peri.h"



static struct tagDjyDevice *hdevUART_Host=NULL;

extern void reset(void);
extern s32 Ecc_Corect256(u8 *data, const u8 *old_ecc);
extern void Ecc_Make256(const u8 *data,u8 *ecc);

static	u8 cur_net_id,cur_user_id_h,cur_user_id_l;

extern volatile u32 UpdateFlashWriteAddr;
extern volatile u32 UpdateFlashReadAddr;
extern u16 g_UpdatePkgNum;
extern u16 g_UpdatePkgIndex;
extern u8  g_UpdateLastPkgSize;
extern u8  g_MonitorNum;
extern u8 g_UpdateSuccessNetId[MONITOR_NUM_MAX];
extern u8 g_UpdateFailedInfor[2*MONITOR_NUM_MAX];
extern u8 g_UpdateFailedNum;
extern u8 g_UpdateSuccessNum;

volatile u16 reSendPkgIndex[UPDATE_PKG_NUM_MAX];
volatile u16 reSendPkgCount=0;
extern u8 reUpdateTimes;


/*=====================================================================*/

static void TransferSetNetId(u8 net_id)
{
	SX1278_SetNetId(net_id);
}

/*=====================================================================*/

void	IOT_TransferInit(void)
{
	u32 dev;

	cur_net_id =0xFF;
	cur_user_id_h =0xFF;
	cur_user_id_l =0xFF;

	////ͨѶ��(��Android�м�ͨѶ): USART3(PB10,PB11)
	dev = Driver_FindDevice("UART3");
	hdevUART_Host = Driver_OpenDeviceAlias(dev,O_RDWR,0);
	if(hdevUART_Host == NULL)
	{
		return;
	}

	Driver_CtrlDevice(hdevUART_Host,CN_UART_START,0,0);
	Driver_CtrlDevice(hdevUART_Host,CN_UART_DMA_UNUSED,0,0);
	//Driver_CtrlDevice(hdevUART_Host,CN_UART_DMA_USED,0,0);
	//���ô��ڲ�����Ϊ115200��
	Driver_CtrlDevice(hdevUART_Host,CN_UART_SET_BAUD,115200,0);

}

static int	Transfer_ReceiveFromHost(u8 *buf,int size,u32 wait_ms)
{
	int res;
	res=Driver_ReadDevice(hdevUART_Host,buf,size,0,wait_ms*mS);

#if 0 //todo: ������
	{
		printf("**** RX From Host(cnt:%d):\r\n",res);
		DebugPutHex(0,res,buf);
		printf("*********************\r\n\r\n");
	}
#endif

	return res;
}

int Transfer_SendToHost(const u8 *buf,int size)
{
	int res;

#if 0 //todo:������
	{
		printf("**** TX To Host(cnt:%d):\r\n",size);
		DebugPutHex(0,size,buf);
		printf("*********************\r\n\r\n");
	}
#endif

	res=Driver_WriteDevice(hdevUART_Host,(u8*)buf,size,0,CN_BLOCK_COMPLETE,2000*mS);
	return res;
}

/*=====================================================================*/
//���������豸

void Transfer_WakeupAll(void)
{
	SX1278_SendWakeupCode();
}

/*=====================================================================*/

bool_t	IsHostPacketValidate(struct tagHOST_PKT_HDR *hdr)
{

	if(hdr->tag != HOST_PKT_TAG)
	{
		return false;
	}

	if(hdr->hdr_len != sizeof(struct tagHOST_PKT_HDR))
	{
		return false;
	}

	return true;
}

bool_t	IsMonitorPacketValidate(struct tagMON_PKT_HDR *hdr,u8 cmd)
{
	if(hdr->tag != MON_PKT_TAG)
	{
		return false;
	}

	if(hdr->cmd != cmd)
	{
		return false;
	}

	return true;
}

/*=====================================================================*/
#pragma	pack(4)

	u8 transfer_buf[256];
static  u8 voice_buf[VOICE_DATA_PKT_NUM*SIZEOF_VOICE_DATA_PKT];

#pragma	pack()


#define CN_RESEND_UPDATE_CMD_COUNT_MAX    5


void __TransferUpdateBin()
{
    u8 k,pkg_len,m;
    u8 buf[256];
    u32 j,ecc,temp;
    u8 ecc_code[4];
    struct tagUpdatePara *pUpdatePara=NULL;
    UpdateFlashReadAddr=CN_APP_START_ADDRESS;
    pUpdatePara=(struct tagUpdatePara *)M_Malloc(sizeof(struct tagUpdatePara),0);
   	if(pUpdatePara==NULL)
   	{
   	   return;
   	}

    //��ʼ��������
    for(j=0;j<g_UpdatePkgNum;j++)
    {
	   UpdateFlashReadAddr=CN_APP_START_ADDRESS+SINGLE_UPDATE_PKG_SIZE_MAX*j;
	   if(j!=g_UpdatePkgNum-1)
	   {
		   pkg_len=SINGLE_UPDATE_PKG_SIZE_MAX;
	   }
	   else
	   {
		   pkg_len=g_UpdateLastPkgSize;
	   }
	   Flash_Read(UpdateFlashReadAddr,buf,pkg_len);
	   pUpdatePara->pkgnum=g_UpdatePkgNum;
	   pUpdatePara->index=j;

	   for(k=0;k<4;k++)
	   {
		   ecc_code[k]=0;
	   }
	   ecc=0;
	   //����ECC��
	   for(m=0;m<256-pkg_len;m++)
	   {
		   buf[pkg_len+m]=0x00;
	   }
	   Ecc_Make256(buf,ecc_code);

	   for(k=0;k<3;k++)
	   {
		  temp=(u32)ecc_code[k];
		  temp=temp<<(8*k);
		  ecc|=temp;
	   }
	   pUpdatePara->ecc_code=ecc;

	   IOT_SendPacket(UPDATE_DATA,buf,pkg_len,pUpdatePara);

	   printf("Send %d pkg.\r\n",j);

	   Djy_EventDelay(500*mS);  //��ʱ500ms.
   }

}


void __DealResendPkg(u8 reSendPkgCount)
{
	u32 j,m,temp;
	u16 index=0;
	u8 k;
	u8 ecc_code[4];
	u32 ecc;
	u8 buf[256];
	struct tagUpdatePara *pUpdatePara=NULL;
	pUpdatePara=(struct tagUpdatePara *)M_Malloc(sizeof(struct tagUpdatePara),0);
    if(pUpdatePara==NULL)
		   return;
    if(reSendPkgCount==0)
    	return;
    for(j=0;j<reSendPkgCount;j++)
	{
	    index=reSendPkgIndex[j];
	    UpdateFlashReadAddr=CN_APP_START_ADDRESS+SINGLE_UPDATE_PKG_SIZE_MAX*index;
	    Flash_Read(UpdateFlashReadAddr,buf,SINGLE_UPDATE_PKG_SIZE_MAX);
	    pUpdatePara->pkgnum=reSendPkgCount;
	    pUpdatePara->index=index;

		for(k=0;k<4;k++)
		{
			ecc_code[k]=0;
		}
		ecc=0;
	   //����ECC��
	   for(m=0;m<256-SINGLE_UPDATE_PKG_SIZE_MAX;m++)
	   {
		  buf[SINGLE_UPDATE_PKG_SIZE_MAX+m]=0x00;
	   }
	   Ecc_Make256(buf,ecc_code);
       temp=0;
	   for(k=0;k<3;k++)
	   {
		  temp=(u32)ecc_code[k];
		  temp=temp<<(8*k);
		  ecc|=temp;
	   }
	   pUpdatePara->ecc_code=ecc;

	   IOT_SendPacket(UPDATE_DATA_REPEAT,buf,SINGLE_UPDATE_PKG_SIZE_MAX,pUpdatePara);

	   printf("Repeat Send %d pkg.\r\n",index);

	   Djy_EventDelay(500*mS);  //��ʱ500ms.
	}

}


void __RecordUpdateInfor(u8 type,u8 net_id)
{
	if(type==CN_UPDATE_FAILED)
	{
	   g_UpdateFailedInfor[g_UpdateFailedNum]=net_id;
	   g_UpdateFailedNum++;
	}
	else
	{
	   g_UpdateSuccessNetId[g_UpdateSuccessNum]=net_id;
	   g_UpdateSuccessNum++;
	}
}


void __MakeSureMonitorWakeUp(u8 net_id)
{
	u8 count,rx_cnt;
	struct tagMON_PKT_HDR  *iot_hdr;
	bool_t bMonitorIsSleep=false;
	for(count=0;count<3;count++)
	{
		//����������ɼ���.
		IOT_SendPacket(GET_STATUS,NULL,0,NULL);
		//�ȴ��ɼ��˻ظ�
		rx_cnt =IOT_GetPacket(transfer_buf,SX1278_RX_TIMEOUT);
		if(rx_cnt > 2)
		{
			iot_hdr =(struct tagMON_PKT_HDR*)transfer_buf;
			//���MON���ݰ���Ч��
			if(iot_hdr->tag == MON_PKT_TAG)
			{
			   //ȷ���յ�����GET_STATUS����Ļظ�
			   if(iot_hdr->cmd==GET_STATUS)
			   {
				   bMonitorIsSleep=false;
				   break;
			   }
			}
		}
		else
		{
			bMonitorIsSleep=true;
		}
	}

	//������˽ڵ㴦������״̬�����ѽڵ�
	if(bMonitorIsSleep)
	{
		printf("Monitor %d is Sleep.\r\n",net_id);
		//���ѽڵ�.
		Transfer_WakeupAll();
		printf("Wakeup Device...\r\n");
	}
	else
	{
	   printf("Monitor %d is Wakeup.\r\n",net_id);
	}
}


void __UpdateResultInfro(void)
{
	u8 i;
	//�����������Ϣ��֪����
	printf("Update Success Monitor Num %d.\r\n",g_UpdateSuccessNum);
	if(g_UpdateSuccessNum!=0)
	{
	    printf("Update Success Monitor Net id:\r\n");
		for(i=0;i<g_UpdateSuccessNum;i++)
		{
			 printf("%d ",g_UpdateSuccessNetId[i]);
		}
		printf("\r\nEnd...\r\n");
	}


	printf("Update Failed Monitor Num %d.\r\n",g_UpdateFailedNum);
	if(g_UpdateFailedNum!=0)
	{
	    printf("Update Failed Monitor Net id:\r\n");
		for(i=0;i<g_UpdateFailedNum;i++)
		{
			printf("%d,Reason:%x ",g_UpdateFailedInfor[2*i],g_UpdateFailedInfor[2*i+1]);
		}
		printf("\r\nEnd...\r\n");
	}

}
// =============================================================================
// ��������:�м̶˶Լ��˽���������ֻ��һ��һ����.
// �������:��.
// �������:��
// ����ֵ  :��.
// =============================================================================
void TransferToMonitorUpdate(void)
{
    u8 i,rx_cnt,k,net_id,pkg_len,reSendUpdateCmdCount;
    u8 RecvUpdateFailedCount,RecvUpdateOKCount,RecvACKCount,RecvUpdateACKCount;
    u8 dat[180];
    u32 j;
    bool_t bRecvAckFlag,bHardFailure,bUpdateEndFlag;
    struct tagUpdatePara *pUpdatePara=NULL;
    struct tagMON_PKT_HDR  *iot_hdr;
    UpdateFlashReadAddr=CN_APP_START_ADDRESS;
    pUpdatePara=(struct tagUpdatePara *)M_Malloc(sizeof(struct tagUpdatePara),0);
	if(pUpdatePara==NULL)
	{
	   return;
	}
    for(i=0;i<g_MonitorNum;i++)
    {
    	for(j=0;j<UPDATE_PKG_NUM_MAX;j++)
    	{
    		reSendPkgIndex[j]=0;
    	}
    	reSendPkgCount=0;
    	reSendUpdateCmdCount=0;
    	RecvUpdateFailedCount=0;
    	RecvUpdateOKCount=0;
    	reUpdateTimes=0;
    	RecvACKCount=0;
    	net_id=i+1;
    	bUpdateEndFlag=false;
    	bHardFailure=false;
    	bRecvAckFlag=false;
    	RecvUpdateACKCount=0;
    	//�����м̶�SX1278 ID,�Խ�����ɼ��˵�ͨѶ.
    	TransferSetNetId(net_id);
        //����ڵ�ͨ��֮ǰ��ȷ���ڵ㴦����������״̬�����Ǵ���״̬.
    	__MakeSureMonitorWakeUp(net_id);

		while(1)
		{
			//�������while(1)���յ����˷�������Ч���ݰ�ʱ������������һֱ��ѭ���Ķ�����
			while(1)
			{
				if(!bRecvAckFlag)
				{
					if(reSendUpdateCmdCount<CN_RESEND_UPDATE_CMD_COUNT_MAX)
					{
					   //����������ɼ���.
					   IOT_SendPacket(UPDATE_CMD,NULL,0,NULL);
					   printf("Send UPDATE CMD to Monitor %d.\r\n",net_id);
					   reSendUpdateCmdCount++;
					   Djy_EventDelay(500*mS);//����UPDATE_CMD����֮����1s
					}
					else
					{
						printf("Tx/Rx UPDATE CMD to Monitor %d failed,Update Failed.\r\n",net_id);
						bHardFailure=true;
						__RecordUpdateInfor(CN_UPDATE_FAILED,net_id);
						break;
					}
				 }

				rx_cnt =IOT_GetPacket(transfer_buf,SX1278_RX_TIMEOUT);
				if(rx_cnt > sizeof(struct tagMON_PKT_HDR))
				{
					iot_hdr=(struct tagMON_PKT_HDR*)transfer_buf;
					//���Monitor���ݰ���Ч��
					if(iot_hdr->tag==MON_PKT_TAG)
					{
						break;
					}
				}
			}

			if(bHardFailure)
			{
               break;
			}
			iot_hdr =(struct tagMON_PKT_HDR*)transfer_buf;

			switch(iot_hdr->cmd)
			{
			   case ACK:
			   {
				   if(RecvACKCount==0)
				   {
					   RecvACKCount++;
					   printf("Monitor %d ACK Msg Recved...\r\n",net_id);
					   bRecvAckFlag=true;
					   printf("Wait Monitor Device reset and Erase Flash.\r\n");
					   Djy_EventDelay(10000*mS);
					   //��ʼ��������
					   __TransferUpdateBin();
					   bUpdateEndFlag=false;
				   }
			   break;
			   ////

               //�յ����˻ظ���UPDATE_ACK����
			   case UPDATE_ACK:
			   {
				   printf("Recv UPDATE_ACK.\r\n");
                   u8 result=0;
                   pkg_len=iot_hdr->dat_len;
				   //���һ�¼�������������
				   if(pkg_len==1)  //˵���������ɹ���������ʧ�ܣ���Ҫ���´���
				   {
                       result=iot_hdr->dat[0];
                       if(result==ACK_CODE_UPDATE_SUCCESS)
                       {
                          //�����ɹ�
                          if(RecvUpdateOKCount==0)
                          {
                        	printf("Monitor %d Update Success.\r\n",net_id);
                        	__RecordUpdateInfor(CN_UPDATE_SUCCESS,net_id);
                    	    RecvUpdateOKCount++;
                    	    bUpdateEndFlag=true;
                          }
                       }
                       else if(result==ACK_CODE_UPDATE_FAILED)
                       {
                    	  //ĳ�����˽ڵ�������������ֻ����һ�Σ����ٴ�����ʧ��(����дFlash����)������Ϊ�ýڵ�
                    	  //Flash����Ӳ�����󣬸ýڵ��޷���������.
                    	  if(reUpdateTimes<=CN_REPEAT_MAX_NUM)
                    	  {
                    	     if(RecvUpdateFailedCount==0)
                    	     {
                    		     //����ʧ��,��Ҫ�ش�
                    		     __TransferUpdateBin();
                    		     RecvUpdateFailedCount++;
                    		     reUpdateTimes++;
                    	     }
                    	  }
                    	  else
                    	  {
                    		 __RecordUpdateInfor(CN_UPDATE_FAILED,net_id);
                    		 bUpdateEndFlag=true;
                    	  }

                       }
                       //ֱ��ĳ���ڵ�����ʧ��.
                       else
                       {
                    	   __RecordUpdateInfor(CN_UPDATE_FAILED,net_id);
                    	   bUpdateEndFlag=true;
                       }

				   }

				   //�յ����Ƕ�ʧ��ĳЩ��
				   else
				   {
					  if(reUpdateTimes<=CN_REPEAT_MAX_NUM)
					  {
						  if(RecvUpdateACKCount==0)
						  {
							  u16 index,temp0;
							  printf("Recv reSend Request.\r\n");
							  pkg_len=iot_hdr->dat_len;
							  reSendPkgCount=pkg_len/2;
							  printf("reSend Num %d.\r\n",reSendPkgCount);
							  memcpy(dat,iot_hdr->dat,pkg_len);
							  printf("reSend Pkg Index:\r\n");
							  for(k=0;k<reSendPkgCount;k++)
							  {
								  index=0;
								  temp0=0;
								  temp0=dat[2*k];
								  index|=temp0;
								  temp0=dat[2*k+1];
								  temp0=temp0<<8;
								  index|=temp0;
								  reSendPkgIndex[k]=index;
								  printf("%d ",reSendPkgIndex[k]);
							  }
							  printf("\r\n");
							  __DealResendPkg(reSendPkgCount);
							  RecvUpdateACKCount++;
							  reUpdateTimes++;
						  }

					  }
					  else
					  {
						  __RecordUpdateInfor(CN_UPDATE_FAILED,net_id);
						  bUpdateEndFlag=true;

					  }
				   }

			   }

			   break;
			   ////

			   default:
				   break;
				   ////
			   }
			}

			if(bUpdateEndFlag)
			{
				break;
			}

		}

    }

    __UpdateResultInfro();

    HandleAfterUpdate();

}

// =============================================================================
// ��������:�м̶˸��³���
// �������:��.
// �������:��
// ����ֵ  :��.
// =============================================================================
void IOT_TransferUpdate(void)
{
	struct tagHOST_PKT_HDR *host_hdr;
	struct tagMON_PKT_HDR  *iot_hdr;
	int rx_cnt,i;
	u16 dat[180];
	u8 *pdata;
	struct tagUpdatePara *pUpdatePara=NULL;
	printf(TRANSFER_MODULE_UPDATE_LOG);
	UpdateFlashWriteAddr=CN_APP_START_ADDRESS;
	IOT_TransferInit();
	pUpdatePara=(struct tagUpdatePara *)M_Malloc(sizeof(struct tagUpdatePara),0);
    if(pUpdatePara==NULL)
	      return;

	while(1)
	{
		while(1)
		{
			//�ȴ�Host������
			rx_cnt=Transfer_ReceiveFromHost(transfer_buf,200,500);
			if(rx_cnt >= sizeof(struct tagHOST_PKT_HDR))
			{
				host_hdr =(struct tagHOST_PKT_HDR*)transfer_buf;
				//���Host���ݰ���Ч��
				if(IsHostPacketValidate(host_hdr))
				{
					break;
				}
			}
			else
			{

			}
		}

	    host_hdr =(struct tagHOST_PKT_HDR*)transfer_buf;
	    iot_hdr  =(struct tagMON_PKT_HDR*)host_hdr->dat;
	    //����Host���������ִ��.
		switch(iot_hdr->cmd)
		{
		   case  UPDATE_DATA:
		   {
			   UpdateRecvData(CN_TRANSFER_ROLE,iot_hdr);
		   }
		  break;
		  ////

		   case UPDATE_MONITOR_NUM:
		   {
			  pdata=iot_hdr->dat;
			  g_MonitorNum=*pdata;
			  dat[0]=ACK_CODE_UPDATE_MINITOR;
			  printf("The monitor num is %d.\r\n",g_MonitorNum);
			  i=IOT_MakePacket(transfer_buf,ACK,dat,1,NULL);
			  Transfer_SendToHost(transfer_buf,i);
		   }
		   break;
		   ////

		  case UPDATE_DATA_REPEAT:
		  {

		  }
		  break;

		  ////

		  default:
			  break;

		 }

   }
}



// =============================================================================
// ��������:�м̶˸��³���
// �������:��.
// �������:��
// ����ֵ  :��.
// =============================================================================

void	IOT_TransferMain(void)	//�м̶�������
{
	struct tagHOST_PKT_HDR *host_hdr;
	struct tagMON_PKT_HDR  *iot_hdr;
	int rx_cnt,i;
	u8 net_id,usr_id_h,usr_id_l;

	printf(TRANSFER_MODULE_LOG);

	IOT_TransferInit();

	while(1)
	{
		while(1)
		{
			//�ȴ�Host������
			rx_cnt=Transfer_ReceiveFromHost(transfer_buf,200,500);
			if(rx_cnt >= sizeof(struct tagHOST_PKT_HDR))
			{
				host_hdr =(struct tagHOST_PKT_HDR*)transfer_buf;

				//���Host���ݰ���Ч��
				if(IsHostPacketValidate(host_hdr))
				{
					break;
				}
			}
			else
			{
				//IDLE

			}
		}

		host_hdr =(struct tagHOST_PKT_HDR*)transfer_buf;

		net_id   =host_hdr->net_id;
		usr_id_h =host_hdr->usr_id_h;
		usr_id_l =host_hdr->usr_id_l;
		iot_hdr  =(struct tagMON_PKT_HDR*)host_hdr->dat;

		//����Host���������ִ��.
		switch(iot_hdr->cmd)
		{

			case	GET_STATUS:
			{
				//�����м̶�SX1278 ID,�Խ�����ɼ��˵�ͨѶ.
				TransferSetNetId(net_id);

				//����������ɼ���.
				IOT_SendPacket(GET_STATUS,NULL,0,NULL);

				//�ȴ��ɼ��˻ظ�
				rx_cnt =IOT_GetPacket(transfer_buf,SX1278_RX_TIMEOUT);
				if(rx_cnt > 2)
				{
					iot_hdr =(struct tagMON_PKT_HDR*)transfer_buf;
					//���MON���ݰ���Ч��
					if(iot_hdr->tag == MON_PKT_TAG)
					{
						struct tagMON_STATUS_DATA *sta;
						sta =(struct tagMON_STATUS_DATA *)iot_hdr->dat;

						//���ɼ������ݷ��͸�����
						Transfer_SendToHost(transfer_buf,rx_cnt);

						printf("STATUS(%02X,%02X,%02X): ext_vol:%dmV,temp:%d,smog:%08X.\r\n",
								net_id,usr_id_h,usr_id_l,
								sta->batt_voltage*BATT_VOL_STEP,sta->temperature,sta->smog_status);
					}
				}
			}
			break;
			////

			case	GET_VOICE_VOLUME:
			{
				//�����м̶�SX1278 ID,�Խ�����ɼ��˵�ͨѶ.
				TransferSetNetId(net_id);

				//����������ɼ���.
				IOT_SendPacket(GET_VOICE_VOLUME,NULL,0,NULL);

				//�ȴ��ɼ��˻ظ�
				rx_cnt =IOT_GetPacket(transfer_buf,SX1278_RX_TIMEOUT);
				if(rx_cnt > 2)
				{
					iot_hdr =(struct tagMON_PKT_HDR*)transfer_buf;
					//���MON���ݰ���Ч��
					if(iot_hdr->tag == MON_PKT_TAG)
					{
						struct tagMON_VOICE_VOLUME_DATA *vol;
						vol =(struct tagMON_VOICE_VOLUME_DATA *)iot_hdr->dat;

						Transfer_SendToHost(transfer_buf,rx_cnt);	//���ɼ������ݷ��͸�����

						printf("VOICE_VOL(%02X,%02X,%02X): %03d,%03d,%03d,%03d,%03d,%03d,%03d,%03d...\r\n",
								net_id,usr_id_h,usr_id_l,
								vol->vol[0],vol->vol[1],vol->vol[2],vol->vol[3],vol->vol[4],vol->vol[5],vol->vol[6],vol->vol[7]);
					}
				}

			}
			break;
			////

			case	SET_VOICE_ONOFF:
			{
				//�����м̶�SX1278 ID,�Խ�����ɼ��˵�ͨѶ.
				TransferSetNetId(net_id);

				//����������ɼ���.
				IOT_SendPacket(SET_VOICE_ONOFF,iot_hdr->dat,iot_hdr->dat_len,NULL);

				//�ȴ��ɼ��˻ظ�
				rx_cnt=IOT_GetPacket(transfer_buf,SX1278_RX_TIMEOUT);
				if(rx_cnt > 2)
				{
					iot_hdr =(struct tagMON_PKT_HDR*)transfer_buf;
					if(iot_hdr->tag == MON_PKT_TAG)
					{
						Transfer_SendToHost(transfer_buf,i);
					}

					printf("VOICE_ONOFF(%02X,%02X,%02X):\r\n",
													net_id,usr_id_h,usr_id_l);
				}

			}

			break;
			////

			case	RESET_VOICE_VOLUME:
			{
				//�����м̶�SX1278 ID,�Խ�����ɼ��˵�ͨѶ.
				TransferSetNetId(net_id);

				//����������ɼ���.
				IOT_SendPacket(RESET_VOICE_VOLUME,NULL,0,NULL);		//RESET_VOICE_VOLUME�����ݲ���

				//�ȴ��ɼ��˻ظ�
				rx_cnt=IOT_GetPacket(transfer_buf,SX1278_RX_TIMEOUT);
				if(rx_cnt > 2)
				{
					iot_hdr =(struct tagMON_PKT_HDR*)transfer_buf;
					//���MON���ݰ���Ч��
					if(iot_hdr->tag == MON_PKT_TAG)
					{
						Transfer_SendToHost(transfer_buf,rx_cnt);

						printf("RESET_VOICE_VOLUME(%02X,%02X,%02X).\r\n",
						net_id,usr_id_h,usr_id_l);
					}
				}
			}
			break;
			////

			case	SET_ENTER_SLEEP_TIME:
			{
				//�����м̶�SX1278 ID,�Խ�����ɼ��˵�ͨѶ.
				TransferSetNetId(net_id);

				//����������ɼ���.
				IOT_SendPacket(SET_ENTER_SLEEP_TIME,iot_hdr->dat,iot_hdr->dat_len,NULL);

				if(1)
				{
					struct tagMON_SET_ENTER_SLEEP_TIME_DATA *sta;
					sta =(struct tagMON_SET_ENTER_SLEEP_TIME_DATA *)iot_hdr->dat;

					printf("SET_ENTER_SLEEP_TIME(%02X,%02X,%02X): %dS.\r\n",
					net_id,usr_id_h,usr_id_l,
					sta->time);
				}

			}
			break;
			////
			case	VOICE_RECODE_RESTART:
			{
				//�����м̶�SX1278 ID,�Խ�����ɼ��˵�ͨѶ.
				TransferSetNetId(net_id);

				//����������ɼ���.
				IOT_SendPacket(VOICE_RECODE_RESTART,NULL,0,NULL);

				//�ȴ��ɼ��˻ظ�
				rx_cnt=IOT_GetPacket(transfer_buf,SX1278_RX_TIMEOUT);
				if(rx_cnt > 2)
				{
					iot_hdr =(struct tagMON_PKT_HDR*)transfer_buf;
					//���MON���ݰ���Ч��
					if(iot_hdr->tag == MON_PKT_TAG)
					{
						Transfer_SendToHost(transfer_buf,rx_cnt);
						printf("VOICE_RECODE_RESTART(%02X,%02X,%02X).\r\n",
									net_id,usr_id_h,usr_id_l);
					}
				}
				else
				{
					printf("VOICE_RECODE NoACK.\r\n");
				}
			}
			break;
			////

			case	GET_VOICE_RECODE:
			{
				//�����м̶�SX1278 ID,�Խ�����ɼ��˵�ͨѶ.
				TransferSetNetId(net_id);

				//����������ɼ���.
				IOT_SendPacket(GET_VOICE_RECODE,NULL,0,NULL);

				i=0;
				while(1)
				{
					rx_cnt =IOT_GetPacket(transfer_buf,SX1278_RX_TIMEOUT);
					if(rx_cnt < sizeof(struct tagMON_PKT_HDR))
					{	//��ʱ����
						break;
					}

					iot_hdr =(struct tagMON_PKT_HDR*)transfer_buf;
					//���MON���ݰ���Ч��
					if(IsMonitorPacketValidate(iot_hdr,GET_VOICE_RECODE))
					{
						struct tagVOICE_DATA *vo;
						vo =(struct tagVOICE_DATA *)iot_hdr->dat;

						//���ɼ������ݷ��͸�����
						Transfer_SendToHost(transfer_buf,rx_cnt);

						//����ACK������ɼ���.
						IOT_SendPacket(ACK,NULL,0,NULL);

						if(vo->pkt_cur == vo->pkt_max)
						{//���ս���
							break;
						}
					}
					else
					{
						printf("GET_VOICE_RECODE pkt error!\r\n");
					}
				}

				//VoicePlay(voice_buf,i);
				printf("GET_VOICE(%02X,%02X,%02X).\r\n",
										net_id,usr_id_h,usr_id_l
										);

			}
			break;
			////

			case	VOICE_PLAY_HOST:
			{
				int i,pkt_num;

				//�����м̶�SX1278 ID,�Խ�����ɼ��˵�ͨѶ.
				TransferSetNetId(net_id);

				printf("VOICE_PLAY_HOST: Receive from host...");
				rx_cnt =0;
				pkt_num =0;
				while(1)
				{
					//����ACK��Host,��ʼ��������
					i=IOT_MakePacket(transfer_buf,ACK,NULL,0,NULL);
					Transfer_SendToHost(transfer_buf,i);

					//��Host�˽�����������
					i=Transfer_ReceiveFromHost(transfer_buf,200,2000);
					if(i <= sizeof(struct tagMON_PKT_HDR))
					{
						//��ʱ����
						printf("host timeout.\r\n");
						break;
					}

					iot_hdr =(struct tagMON_PKT_HDR*)transfer_buf;
					//���MON���ݰ���Ч��
					if(IsMonitorPacketValidate(iot_hdr,VOICE_PLAY_HOST))
					{
						u8 max_pkt,cur_pkt;

						max_pkt =iot_hdr->dat[0];
						cur_pkt =iot_hdr->dat[1];
						memcpy(&voice_buf[rx_cnt],&iot_hdr->dat[2],SIZEOF_VOICE_DATA_SLOT);
						rx_cnt +=SIZEOF_VOICE_DATA_SLOT;
						pkt_num++;

						if(cur_pkt == max_pkt)
						{
							//�������.
							break;
						}

						if(rx_cnt >= sizeof(voice_buf))
						{
							//�ﵽ�������ֵ(�ݴ���).
							break;
						}
					}
					else
					{
						printf("pkt_error.\r\n");
					}
				}

				if(pkt_num > 0)
				{
					printf("VOICE_PLAY_HOST: Send to monitor...");
					IOT_SendPacket(VOICE_PLAY_HOST,NULL,0,NULL);	//ͬ��

					for(i=0;i<pkt_num;i++)
					{
						struct	tagVOICE_DATA *voice_data;

						//�ȼ��Ӧ��
						if(IOT_GetPacket(transfer_buf,SX1278_RX_TIMEOUT)<=0)
						{
							//���û��Ӧ��.
							printf("VOICE_PLAY_HOST: Monitor NoACK!\r\n");
							break;
						}

						//��װVOICE���ݰ����͸�����.
						voice_data =(struct	tagVOICE_DATA*)transfer_buf;

						voice_data->pkt_max =pkt_num-1;
						voice_data->pkt_cur =i;
						memcpy(voice_data->dat,&voice_buf[i*SIZEOF_VOICE_DATA_SLOT],SIZEOF_VOICE_DATA_SLOT);

						IOT_SendPacket(VOICE_PLAY_HOST,voice_data,sizeof(struct	tagVOICE_DATA),NULL);

						printf("VOICE_PLAY_HOST: send:%d\r\n",i);

					}

				}

				printf("VOICE_PLAY_HOST(%02X,%02X,%02X).\r\n",
										net_id,usr_id_h,usr_id_l
										);

			}
			break;
			////
			case	VOICE_PLAY_SOUND_1:
			{
				//net_id =0xFF; //ʹ�ù㲥��Ϣ��ʽ

				//�����м̶�SX1278 ID,�Խ�����ɼ��˵�ͨѶ.
				TransferSetNetId(net_id);

				//����������ɼ���.
				IOT_SendPacket(VOICE_PLAY_SOUND_1,NULL,0,NULL);

			}
			break;
			////

			case	WAKEUP_DEVICE:
			{
				printf("WAKEUP_DEVICE Start...\r\n");

				TransferSetNetId(net_id);
				Transfer_WakeupAll();

				//����ACK��Host
				i=IOT_MakePacket(transfer_buf,ACK,NULL,0,NULL);
				Transfer_SendToHost(transfer_buf,i);
				printf("WAKEUP_DEVICE End.\r\n");
			}
			break;
			////

			//�յ�UPDATE_CMD�����Ҫ����������ʱ��Ҫ����CPU(������)
			case UPDATE_CMD:
			{
				volatile u8 *pAddr;
				u8 i;
				u8 dat=ACK_CODE_UPDATE_CMD;

				pAddr=(u8 *)(0x2001fff0);
				for(i=0;i<16;i++)
				{
					*(pAddr+i)=(0xA0+i);
				}

				//�������ظ�һ��ACK
				i=IOT_MakePacket(transfer_buf,ACK,&dat,1,NULL);
				Transfer_SendToHost(transfer_buf,i);
				printf("Transfer Need Update,Transfer will reset.\r\n");
				Djy_EventDelay(50*mS);
				reset();
			}
			break;
            ////
			default:
			{
				printf("UNKNOW IOT_CMD: %02XH.\r\n",iot_hdr->cmd);
			}
			break;
			////
		}


	}
}

/*=====================================================================*/

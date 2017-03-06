/*
 * IOT_Monitor.c
 *
 *  Created on: 2015��11��12��
 *      Author: liuwei
 */

#include	"IOT.h"
#include    "cpuflash.h"
#include    "stdlib.h"
#include    "verify.h"
#include "cpu_peri.h"

/*=====================================================================*/

#pragma	pack(4)
static	u8 monitor_buf[256];	//����ͨѶ������
#pragma	pack()

static	u32 monitor_enter_sleep_time=5;
static  u32 enter_sleep_time;
static  u32 system_time_count=0;

extern void reset(void);
extern volatile u32 UpdateFlashWriteAddr;

extern u8 g_NetId;

extern u16 reUpdatePkgIndex;
extern u8 reUpdatePkgNum;
extern volatile u16 re_ok_index_list[90];
extern volatile u8 re_ok_count;
//extern u8 reUpdateTimes;
extern volatile u16 err_index_list[90];
extern volatile u8 err_cur_count;


static const u32 auto_play_time[]=
{
	2,		//0
	10,		//1
	20,		//2
	30,		//3
	50,		//4
	70,		//5
	90,		//6
	120,	//7
	150,	//8
	180,	//9
	210,	//10
	240,	//11
	270,	//12
	300,	//13
	330,	//14
	360,	//15
	390,	//16
	420,	//17
	480,	//18
	540,	//19
	600,	//20
	660,	//21
	720,	//22
	780,	//23
	840,	//24
	900,	//25

};

#define	AUTO_PLAY_TIME_NUM	(sizeof(auto_play_time)/sizeof(auto_play_time[0]))


void CheckReUpdateResult(void)
{
   u8 i,j;
   u8 count=0;
   for(i=0;i<re_ok_count;i++)
   {
      for(j=0;j<err_cur_count;j++)
      {
    	  if(re_ok_index_list[i]==err_index_list[j])
    	  {
    		  err_index_list[j]=0xFFFF;
    		  break;
    	  }
      }
   }

   //���������¶�err_cur_count��err_index_list��re_ok_count��re_ok_index_list��ֵ
   for(j=0;j<err_cur_count;j++)
   {
	   if(err_index_list[j]!=0xFFFF)
	   {
		   err_index_list[count]=err_index_list[j];
		   count++;
	   }
   }
   err_cur_count=count;
   re_ok_count=0;
   for(i=0;i<90;i++)
   {
	   re_ok_index_list[i]=0xFFFF;
   }
}




/*=====================================================================*/

static void check_auto_play(u32 time)
{
	int i;
	u32 count=0;

	if(time>900)
	{
		count=time-900;
        if(count%30==0)
        {
        	VoiceSetStatus(VOICE_STATUS_PLAY_SOUND_1);
        }
	}
	else
	{

		for(i=0;i<AUTO_PLAY_TIME_NUM;i++)
		{
			if(time == auto_play_time[i])
			{
				VoiceSetStatus(VOICE_STATUS_PLAY_SOUND_1);
				break;
			}
		}
	}
}

/*=====================================================================*/
extern int IsIWDT_Reset;

static void  monitor_idle_100ms(void)
{
	static int time=0,a;


	if(time++ >= 10)
	{	//1000ms;
		time=0;
		system_time_count++;
		check_auto_play(system_time_count);

#if 0 //for debug
		enter_sleep_time =1000; //todo:�����ã��������ߡ�
#endif

		if(IsIWDT_Reset)
		{	//���Ź���λ,����������Sleep.
			IsIWDT_Reset =0;
			enter_sleep_time=0;
			printf("Is WDT Reset...\r\n");
		}
		if(enter_sleep_time>0)
		{
			enter_sleep_time--;
		}
		else
		{
			//����Sleep״̬.
			printf("Monitor Enter Sleep...\r\n");

			SX1278_EnterSleep();
			Djy_EventDelay(500*mS);
			LP_SetSleepLevel(CN_SLEEP_L4);
			while(1)
			{
				Djy_EventDelay(500*mS);
			}

		}
	}

	a++;
	if(a&(1<<0))
	{	//200ms
		IOT_VoiceVoltageCheck_200ms();
		IOT_SmogCheck_200ms();
	}

}

// =============================================================================
// ��������:���˸��³���
// �������:��.
// �������:��
// ����ֵ  :��.
// =============================================================================
void IOT_MonitorUpdate(void)
{
	u8 dat[180];
	u8 *pdata;
	u8 count,i,k,len;
	s32 result;
	u32 ecc;
	u8 reRecvCount=0;
	struct tagUpdatePara *pUpdatePara=NULL;
	printf(MONITOR_MODULE_UPDATE_LOG);
	UpdateFlashWriteAddr=CN_APP_START_ADDRESS;
	pUpdatePara=(struct tagUpdatePara *)M_Malloc(sizeof(struct tagUpdatePara),0);
	if(pUpdatePara==NULL)
	{
	   return;
	}

	while(1)
	{
		struct tagMON_PKT_HDR *hdr;
		u8 cmd;
		int rx_cnt;
		//�ȴ����ݽ���
		//ע�⣬������״̬��,��OS_BOOT״̬�£����м��˽ڵ��������.
		while(1)
		{
			rx_cnt =IOT_GetPacket(monitor_buf,100);
			if(rx_cnt > 2)
			{
				break;
			}
		}
		//��ʼ�������ݰ�
		hdr =(struct tagMON_PKT_HDR*)monitor_buf;

		if(hdr->tag != MON_PKT_TAG)
		{
			//���ǺϷ������ݰ�...
			continue;
		}

		cmd =hdr->cmd;

		switch(cmd)
		{
		  case UPDATE_DATA:
		  {
			  UpdateRecvData(CN_MONITOR_ROLE,hdr);
		  }
		  break;
		  ////

		  case UPDATE_DATA_REPEAT:
		  {
			  static u8 flag=0;
			  if(!flag)
			  {
			     reUpdatePkgNum=(u8)hdr->pkgnum;
			     flag++;
			     printf("reRecv pkg num %d.\r\n",reUpdatePkgNum);

			  }
			  //�����������
			  reUpdatePkgIndex=hdr->index;
			  pdata=hdr->dat;
			  ecc=hdr->ecc_code;
              len=hdr->dat_len;
			  result=DealECCandWriteFlash(pdata,reUpdatePkgIndex,len,ecc,CN_MONITOR_ROLE);
			  if(result==1)
			  {
				  reRecvCount++;
				  re_ok_count++;
				  re_ok_index_list[re_ok_count]=reUpdatePkgIndex;
			  }
              //���յ��������һ��
              if(reUpdatePkgIndex==err_index_list[err_cur_count-1])
              {
            	  printf("reRecv the Last pkg.\r\n");
            	  printf("reRecvCount:%d, reUpdatePkgNum:%d\r\n",reRecvCount,reUpdatePkgNum);
            	  //�����ɹ�
				  if(reRecvCount==reUpdatePkgNum)
				  {
					 dat[0]=ACK_CODE_UPDATE_SUCCESS;
					 for(k=0;k<2;k++)
					 {
						IOT_SendPacket(UPDATE_ACK,dat,1,NULL);
						printf("Send Success ACK.\r\n");
						Djy_EventDelay(200*mS);
					 }
					 printf("Monitor %d Update Success...\r\n",g_NetId);
					 HandleAfterUpdate();
				  }
				  else
				  {
					  CheckReUpdateResult();
					  //�ٴη���UPDATE_ACK��Ϣ�����ٴη���.
					  printf("The need reSend pkg count %d.\r\n",err_cur_count);
					  for(k=0;k<err_cur_count;k++)
					  {
						 printf("%d ",err_index_list[k]);
					  }
					  printf("\r\n");

					  len=2*err_cur_count;
					  for(k=0;k<err_cur_count;k++)
					  {
						dat[2*k]=err_index_list[k];
						dat[2*k+1]=err_index_list[k]>>8;
					  }

					  for(count=0;count<2;count++)
					  {
						 IOT_SendPacket(UPDATE_ACK,dat,len,NULL);
						 printf("Lost Pkg,Send UPDATE_ACK to Transfer.\r\n");
						 Djy_EventDelay(100*mS);
					  }
				  }
              }

		  }
		  break;
		  ////

		  default:
			  break;
		}
	}
}




void IOT_MonitorMain(bool_t enter_sleep)	//����������
{

	printf(MONITOR_MODULE_LOG);
	system_time_count =0;

	if(enter_sleep)
	{
		monitor_enter_sleep_time=0;
	}
	else
	{
		monitor_enter_sleep_time=30;	//�ϵ�Ĭ���ڿ���״̬��,30���Ӻ��Զ�����Sleep״̬.
	}

	IOT_TemperatureModuleInit();	//�¶�ģ���ʼ��
	IOT_SmogModuleInit();			//����ģ���ʼ��
	IOT_VoiceModuleInit();			//����ģ���ʼ��

	while(1)
	{
		struct tagMON_PKT_HDR *hdr;
		u8 cmd;
		int rx_cnt;

		enter_sleep_time =monitor_enter_sleep_time;

		//�ȴ����ݽ���
		while(1)
		{
			rx_cnt =IOT_GetPacket(monitor_buf,100);
			if(rx_cnt > 2)
			{
				printf("Recv pkg.\r\n");
				break;
			}
			monitor_idle_100ms();
		}

#if 0 //todo: ������
		{
			printf("**** RX From Transfer(cnt:%d):\r\n",rx_cnt);
			DebugPutHex(0,rx_cnt,monitor_buf);
			printf("*********************\r\n\r\n");
		}
#endif

		//��ʼ�������ݰ�
		hdr =(struct tagMON_PKT_HDR*)monitor_buf;

		if(hdr->tag != MON_PKT_TAG)
		{
			//���ǺϷ������ݰ�...
            printf("Recv Invalid pkg.\r\n");
			continue;
		}

		printf("MonitorCMD:%d\r\n",hdr->cmd);
		cmd =hdr->cmd;
		switch(cmd)
		{
			case GET_STATUS:
			{
				struct tagMON_STATUS_DATA *sta;

				sta =(struct tagMON_STATUS_DATA *)hdr->dat;
				sta->batt_voltage =IOT_GetExternVoltage();
				sta->temperature =IOT_GetTemperature();
				sta->smog_status =IOT_GetSmogStatus();
				printf("Monitor: STATUS.\r\n");
				IOT_SendPacket(GET_STATUS,sta,sizeof(struct tagMON_STATUS_DATA),NULL);
			}
			break;
			////

			case SET_VOICE_ONOFF:
			{
				struct tagMON_SET_VOICE_ONOFF_DATA *sta;
				sta =(struct tagMON_SET_VOICE_ONOFF_DATA *)hdr->dat;

				if(sta->onoff==0)
				{
					printf("Monitor: SET_VOICE_OFF.\r\n");
					VoiceSetStatus(VOICE_STATUS_POWER_OFF);

				}
				else
				{
					printf("Monitor: SET_VOICE_ON\r\n");
					VoiceSetStatus(VOICE_STATUS_POWER_ON);
				}
				IOT_SendPacket(ACK,NULL,0,NULL);
			}
			break;
			////

			case GET_VOICE_VOLUME:
			{
				printf("Monitor: VOICE_VOLUME.\r\n");
				IOT_SendPacket(GET_VOICE_VOLUME,VoiceVolBuf,sizeof(VoiceVolBuf),NULL);
			}
			break;
			////

			case	RESET_VOICE_VOLUME:
			{
				printf("Monitor: RESET_VOICE_VOLUME.\r\n");
				IOT_VoiceResetVolume();
				IOT_SendPacket(ACK,NULL,0,NULL);
			}
			break;
			////

			case	SET_ENTER_SLEEP_TIME:	//����������ǹ㲥��Ϣ,���ûظ�Ack
			{
				struct tagMON_SET_ENTER_SLEEP_TIME_DATA *slp;
				slp =(struct tagMON_SET_ENTER_SLEEP_TIME_DATA *)hdr->dat;

				monitor_enter_sleep_time =slp->time;

				printf("Monitor: SET_ENTER_SLEEP_TIME,%d.\r\n",monitor_enter_sleep_time);

			}
			break;
			////

			case VOICE_RECODE_RESTART:
			{
				printf("Monitor: VOICE_RECODE\r\n");

				IOT_SendPacket(ACK,NULL,0,NULL);
				VoiceSetStatus(VOICE_STATUS_RECODE_RESTART);
			}
			break;
			////

			case GET_VOICE_RECODE:
			{
				int i,rx_cnt,pkt_num;
				struct tagVOICE_DATA *vo;

				VoiceSetStatus(VOICE_STATUS_STOP);

				pkt_num =VOICE_DATA_PKT_NUM;

				for(i=0;i<pkt_num;i++)
				{
					vo =(struct tagVOICE_DATA*)monitor_buf;

					vo->pkt_max =pkt_num-1;
					vo->pkt_cur =i;

					memcpy(vo->dat,&voice_rec_buf[i*SIZEOF_VOICE_DATA_SLOT],SIZEOF_VOICE_DATA_SLOT);
					IOT_SendPacket(GET_VOICE_RECODE,vo,sizeof(struct tagVOICE_DATA),NULL);

					////�ȴ�Ӧ�����һ��
					rx_cnt =IOT_GetPacket(monitor_buf,SX1278_RX_TIMEOUT);
					if(rx_cnt <= 0)
					{
						//��ʱ�˳�,�м�û��Ӧ��
						printf("Monitor: Transfer NoACK.\r\n");
						break;
					}
					printf("Monitor: GET_VOICE_RECODE: pkt:%d\r\n",i);

				}

				VoiceSetStatus(VOICE_STATUS_RECODE_RESTART);

				//printf("Monitor: GET_VOICE_RECODE: pkt:%d\r\n",i);
			}
			break;
			////

			case VOICE_PLAY_HOST:
			{
				int i,rx_cnt;

				printf("Monitor: VOICE_PLAY_HOST.\r\n");

				if(VoiceGetStatus()!=VOICE_STATUS_NONE)
				{
					printf("Monitor: Voice Module Busy...\r\n");
					break;
				}

				rx_cnt =0;
				while(1)
				{
					struct tagMON_PKT_HDR *hdr;

					//����ACK���м�,��ʾ���Խ���������...
					IOT_SendPacket(ACK,NULL,0,NULL);

					i =IOT_GetPacket(monitor_buf,SX1278_RX_TIMEOUT);
					if(i <= sizeof(struct tagMON_PKT_HDR))
					{	//��ʱ����.
						printf("Monitor wait host timeout.\r\n");
						break;
					}

					hdr =(struct tagMON_PKT_HDR*)monitor_buf;
					if(hdr->cmd==VOICE_PLAY_HOST)
					{
						u8 pkt_max;
						u8 pkt_cur;

						pkt_max =hdr->dat[0];
						pkt_cur =hdr->dat[1];

						memcpy(&voice_play_buf[rx_cnt],&hdr->dat[2],SIZEOF_VOICE_DATA_SLOT);
						rx_cnt += SIZEOF_VOICE_DATA_SLOT;

						printf("Monitor VOICE_PLAY_HOST:%d.\r\n",pkt_cur);

						if(pkt_cur >= pkt_max)
						{
							//�������(���յ������һ������)
							break;
						}

						if(rx_cnt >= sizeof(voice_play_buf))
						{
							//�������(���ջ�������,�ݴ���...)
							break;
						}

					}
					else
					{
						printf("Monitor pkt err.\r\n");
					}
				}

				voice_play_count =rx_cnt;

				if(voice_play_buf[0]!=0x4C)
				{
					printf("Monitor: VOICE_PLAY_HOST: voice data error!\r\n");
				}

				VoiceSetStatus(VOICE_STATUS_PLAY);

			}
			break;
			////

			case VOICE_PLAY_SOUND_1:
			{
				printf("Monitor: VOICE_PLAY_SOUND_1\r\n");

				VoiceSetStatus(VOICE_STATUS_PLAY_SOUND_1);
			}
			break;
			////

			case UPDATE_CMD:
			{
			    volatile u8 *pAddr;
				u8 i;
				pAddr=(u8 *)(0x2001fff0);
				u8 dat=ACK_CODE_UPDATE_CMD;

				printf("Recv UPDATE CMD from Transfer.\r\n");

				for(i=0;i<16;i++)
				{
					*(pAddr+i)=(0xA0+i);
				}

				//�Է����ݰ���ʧ�����˸��м����ظ�ACK��ʱÿ�λظ�����.
				//�������ظ�һ��ACK
				for(i=0;i<3;i++)
				{
					IOT_SendPacket(ACK,&dat,1,NULL);
					printf("Send ACK to Transfer.\r\n");
					printf("Monitor %d Need Update,will reset.\r\n",g_NetId);
				    Djy_EventDelay(100*mS);
				}
				reset();
			}
			break;
			////


			default:
			{
				printf("Monitor: UNKNOW CMD: %02XH.\r\n",hdr->cmd);
			}
			break;
			////
		}
	}

}

/*=====================================================================*/


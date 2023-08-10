


#ifndef _UART_COMMAND_HEADER_
#define	_UART_COMMAND_HEADER_
#include "motoTypedef.h"

#define		MONITOR_CHANNEL_NUMBER 2


typedef enum
{
	USER_DATA_TYPE_NONE = 0,
	USER_DATA_TYPE_PPDELAY,
	USER_DATA_TYPE_STARTUP_STSP,
	USER_DATA_TYPE_STARTUP_ENSP,
	USER_DATA_TYPE_RAMPUP_TIME,
	USER_DATA_TYPE_INIT_ANGLE,
	USER_DATA_TYPE_TMRLIMIT,
	USER_DATA_TYPE_CURLIMIT,
	USER_DATA_TYPE_RUNCFG,
	USER_DATA_TYPE_STDEA,
	USER_DATA_TYPE_ENDEA,
	USER_DATA_TYPE_DSTEP,
	USER_DATA_TYPE_RUNPARAM,
	USER_DATA_TYPE_VOLLIMIT,
	USER_DATA_TYPE_REFIQ,
	USER_DATA_TYPEINITVQ,
	USER_DATA_FM_BEMF_UL,
	USER_DATA_FM_BEMF_DL,
	USER_DATA_FM_BEMF_FK,
	USER_DATA_FM_BEMF_TLimit,
	USER_DATA_FM_BEMF_RLimit,
	USER_DATA_FM_BEMF_RTLimit,
	USER_DATA_FM_VBUS_Limit,
	USER_DATA_FM_IPHRASE_Limit,
	USER_DATA_FM_VBUS_Adjust,
	USER_DATA_TYPE_MAXITEMS
}UserDataTypeList;

typedef enum
{
	MOTO_PARAM_TYPE_NONE = 0,
	MOTO_PARAM_TYPE_PEK_VOL,
	MOTO_PARAM_TYPE_PEK_CUR,
	MOTO_PARAM_TYPE_PWM_TS,
	MOTO_PARAM_TYPE_PWM_DS,
	MOTO_PARAM_TYPE_NOPOLE,
	MOTO_PARAM_TYPE_RSVAL,
	MOTO_PARAM_TYPE_LSVAL,
	MOTO_PARAM_TYPE_VC_KFI,
	MOTO_PARAM_TYPE_NOR_SP,
	MOTO_PARAM_TYPE_MAX_SP,
	MOTO_PARAM_TYPE_NOR_RS,
	MOTO_PARAM_TYPE_NOR_LSDB,
	MOTO_PARAM_TYPE_NOR_KFI,
	MOTO_PARAM_TYPE_NOR_DELTA,
	MOTO_PARAM_TYPE_NOR_HS,
	MOTO_PARAM_TYPE_NOR_LS,
	MOTO_PARAM_TYPE_MAXITEMS
}MotoParam_TypeList;
	
typedef enum
{
	USER_PI_TYPE_NONE = 0,
	USER_PI_TYPE_QP,
	USER_PI_TYPE_QI,
	USER_PI_TYPE_DP,
	USER_PI_TYPE_DI,
	USER_PI_TYPE_Speed_P,
	USER_PI_TYPE_Speed_I,
	USER_PI_TYPE_MAXITEMS
}UserPiTypeList;

typedef enum
{
	USERFDWEAK_TYPE_NONE = 0,
	USERFDWEAK_TYPE_TABLEN,
	USERFDWEAK_TYPE_STRPM,
	USERFDWEAK_TYPE_ENRPM,
	USERFDWEAK_TYPE_KCSTART,
	USERFDWEAK_TYPE_INVKFI,
	USERFDWEAK_TYPE_REFID,
	USERFDWEAK_TYPE_LSDT,
	USERFDWEAK_TYPE_MAXNUM
}UserFdWeakTypeList;

typedef enum
{
	HFI_DATA_TYPE_NONE = 0,
	HFI_DATA_TYPE_FREQ,
	HFI_DATA_TYPE_VOLAMP,
	HFI_DATA_TYPE_STVQ,
	HFI_DATA_TYPE_TMRLIMIT,
	HFI_DATA_TYPE_UPDCFG,
	HFI_DATA_TYPE_SHOWANGLE,
	HFI_DATA_TYPE_MODESEL,
	FLT_BEMF_TYPE_ULIMIT,
	FLT_BEMF_TYPE_DLIMIT,
	FLT_BEMF_TYPE_FILTK,
	FLT_BEMF_TYPE_TLIMIT,
	FLT_BEMF_TYPE_RLIMIT,
	FLT_BEMF_TYPE_RTLIMIT,
	USERMOTO_VCONST_ADJUST,
	FLT_BEMF_TYPE_LPLIMIT,
	HFI_DATA_TYPE_MAXITEMS
}HfiDataTypeList;

typedef enum
{
	CH_SOURCE_IU = 0,
	CH_SOURCE_IV,
	CH_SOURCE_EANGLE,
	CH_SOURCE_IALPHA,
	CH_SOURCE_IBETA,
	CH_SOURCE_IQD_IQ,
	CH_SOURCE_IQD_ID,
	CH_SOURCE_VQD_VQ,
	CH_SOURCE_VQD_VD,
	CH_SOURCE_VALPHA,
	CH_SOURCE_VBETA,
	CH_SOURCE_IQREF_IQ,
	CH_SOURCE_IQREF_ID,
	CH_SOURCE_BEMF_IQ,
	CH_SOURCE_BEMF_ID,
	CH_SOURCE_ESTI_QES,
	CH_SOURCE_ESTI_DES,
	CH_SOURCE_ESTI_OMGA,
	CH_SOURCE_ESTI_QRHO,
	CH_SOURCE_QVEL,
	CH_SOURCE_SPEED_SREF,
	CH_SOURCE_CUR_PEAK,
	CH_SOURCE_CUR_MAX,
	CH_SOURCE_HFI_AMPMAX,
	CH_SOURCE_HFI_EPOS,
	CH_IQ_AVG,
	CH_ID_AVG,
	CH_IQDAVG_QRHO,
	CH_SOURCE__MAXITEMS
}Channel_SourceList;


typedef enum _uart_datalist_
{
	MOTO_CURRENT_PA = 0,
	MOTO_CURRENT_PB,
	MOTO_CURRENT_PC,
	MOTO_CAL_OMEGA,
	MOTO_CAL_SPEED,
	MOTO_PID_QPi,
	MOTO_PID_QPo,
	MOTO_PID_QDi,
	MOTO_PID_QDo,
	MOTO_PID_SPi,
	MOTO_PID_SPo,
	MOTO_BEMF_Iq,
	MOTO_BEMF_Id
}MONITOR_DATALIST;

typedef enum _uart_command_list_
{
	UART_CMD_INVALIDE = 0,
	UART_CMD_READALLPARAM,
	UART_CMD_STARTMOTO,
	UART_CMD_STOPMOTO,
	UART_CMD_UPDSPREF,
	UART_CMD_START_CCW
}UART_CMD_LIST;

typedef enum _uart_tx_dtype_
{
	DTYPE_KFI_REFPARAM = 0,
	DTYPE_KFI_PARAM,
	DTYPE_PI_PARAM,
	DTYPE_STARTUP_PARAM,
	DTYPE_HFI_PARAM,
	DTYPE_MOTO_PARAM,
	DTYPE_GRP_NUMBER,
}UART_TX_DTYPE;

typedef enum _uart_command_
{
	UART_CMD_NONE = 0,
	UART_CMD_READ_MOTOPARAM,
	UART_CMD_READ_PID
}UART_COMMAND_LIST;

typedef	struct _uart_cmd_body_
{
	uint16_t 	recTimeOut;
	uint8_t		recIndex;
	uint8_t		recFlag;
	union 	
	{
		struct
		{
			uint8_t		header;
			uint8_t		dataLength;
			uint8_t		dataType;
			uint8_t		msgIndex;
			int16_t		dataValue;
			uint16_t	crc;
		};
		struct
		{
			uint8_t		pkgHeader;
			uint8_t		pkgType;
			uint8_t		datIndex;
			uint8_t		dataRh;
			uint16_t		dataR16;
			uint16_t	crc1;
		};
		uint8_t	uartData[8];
	};

}UART_CMD_PACKET;

typedef struct _oscope_data_
{
	uint8_t			bLen1;
	uint8_t			bLen2;
	uint8_t			bLen3;
	uint8_t *		pSrc1;
	uint8_t *		pSrc2;
	uint8_t *		pSrc3[16];
}OSCOPE_Group;

typedef struct _ChannelParam_
{
	uint16_t		TimerCounter;
	uint8_t			grpCtrl;
	uint8_t			grpFlag;
	OSCOPE_Group	oscopeData[MONITOR_CHANNEL_NUMBER];
	uint8_t			libState;
	uint8_t			txDType;
}Group_Channel;

void	PtUartPro_Init(void * motoHandle);
void	PtUartPro_SetOscrope(uint8_t channel,uint8_t * pSrc,uint8_t unit);
void	PtUartPro_Tx_OscopeData(void);
void	PtUartPro_TxEnable(uint8_t nStat);
void	PtUartPro_Oscrope_SetReg(uint8_t * pSrc,uint8_t unit);
void	PtUartPro_Process(void);
void	PtUartPro_SetChannelReg(uint8_t * pSrc,uint8_t chIndex,uint8_t unit);
void	PtUartPro_SetOscUartHandle(void * pOscUart);
void	Uart_Idel_Process(void);
extern UART_CMD_PACKET		mUart_Command;

#endif


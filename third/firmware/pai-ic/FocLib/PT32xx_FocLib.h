/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "motoTypedef.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void PtLib_Moto_PidReset(MOTO_INSTANCE * pUserMoto);
void InitEstimParm(MOTO_INSTANCE * pUserMoto);
void	bemf_eAngle_Estimate(void * param);
void	PtLib_SetFocFunc_Null(MOTO_INSTANCE * pMoto);
void mainMoto_DirectLpInit(void * pParam);
void	mainMoto_IdelCurrent_Zero(void * pParam);
/*************************************************************************************************/
bool PtLib_StartMotor(MOTO_INSTANCE * pUserMoto);
bool PtLib_StopMotor(MOTO_INSTANCE * pUserMoto);
bool PtLib_IsMotorRunning(MOTO_INSTANCE * pUserMoto);
bool PtLib_isMotorWaitStart(MOTO_INSTANCE * pUserMoto);
bool PtLib_IsMotorAllignComplete(MOTO_INSTANCE * pUserMoto);
bool PtLib_isMotorWaitRunStart(MOTO_INSTANCE * pUserMoto);

void	PtLib_InitMoto(MOTO_INSTANCE * pUserMoto,PTMC_CONFIG_PKG * pParam,PTMC_RUN_PARAM *	mMotoRunCfg);
void	PtLib_Before_StartRun(void * pParam);
void	PtLib_MotoContrllLoop(void * pMoto);
void	PtLib_EmptyFunction(void * pMoto);
bool	PtLib_SetSTate_AllignInit(MOTO_INSTANCE * pUserMoto);
void	PtLib_SetStartRun(MOTO_INSTANCE * pMoto);
bool	PtLib_SetStopRun(MOTO_INSTANCE * pMoto);
void	PtLib_Moto_BeforeStart_Init(MOTO_INSTANCE * pMoto);
bool	PtLib_SetSTate_OpenLoop(MOTO_INSTANCE * pUserMoto);
void	PtLib_BeforeCloseLoop_Init(void * param);
bool	PtLib_SetSTate_CloseLoop(MOTO_INSTANCE * pUserMoto);

void	PtLib_Moto_SetSeedRef_CB(MOTO_INSTANCE * pUserMoto,Moto_SetSpeedRef_CB  pCb,Moto_SlowToStop_CB pStopCB);
void	PtLib_Moto_SetReadCurrentCb(MOTO_INSTANCE * pUserMoto,Moto_ReadFunction_CB  pCB_callBack);
void	PtLib_Moto_SetPwmOutPutCb(MOTO_INSTANCE * pUserMoto,Moto_PwmOutPut_CB  pCB_callBack);
void	PrLib_Moro_SetFWeek_Table(MOTO_INSTANCE * pUserMoto,PTFWEEK_TABLE_t * pTable);
void PtMoto_HFI_Init( void * handle, void * param );
/*************************************************************************************************/
/*************************************************************************************************/
void	PtMoto_State_Init(MOTO_INSTANCE * pUserMoto);
void	PtMoto_State_InitMoto_State_Init(MOTO_INSTANCE * pUserMoto);
void	PtMoto_RunTime_Manager(MOTO_INSTANCE * pUserMoto);

void PtMoto_StartRun_GoLoop(MOTO_INSTANCE * pUserMoto,MOTO_RUN_DIR dir);
void	PtMoto_StartRun(MOTO_INSTANCE * pUserMoto,MOTO_RUN_DIR dir);
void	PtMoto_StopRun(MOTO_INSTANCE * pUserMoto);
bool	PtMoto_IsMotorRunning(MOTO_INSTANCE * pUserMoto);

void	PtMoto_Hfi_SetPwmDutyRef(MOTO_INSTANCE * pUserMoto,uint16_t maxDuty);
void	PtMoto_CloseLp_SetPwmDutyRef(MOTO_INSTANCE * pUserMoto,uint16_t maxDuty);
void	PtMoto_OpenLp_SetPwmDutyRef(MOTO_INSTANCE * pUserMoto,uint16_t maxDuty);
void	SetDefault_eAnbleEstim_Cb(void * pParam);
/*************************************************************************************************/
void	SetMoto_RtMonitor_Default(MOTO_INSTANCE * pUserMoto);
bool PtLib_Motor_SetFaultStat(MOTO_INSTANCE * pUserMoto);
void	App_BeforeCloseLoop_Init(MOTO_INSTANCE * pUserMoto);
/*************************************************************************************************/
Clark_Item MotoFunction_Clarke( Phrase_Item Current_Input );
Park_Item MotoFunction_Park( Clark_Item Curr_Input, int16_t Theta );
int16_t MotoFunction_DeltaThetaCal( Clark_Item Curr_Input, int16_t Theta );
int16_t PI_Controller( PID_Handle_t * pHandle, int32_t wProcessVarError );
void Circle_Limitation( void * param );
Clark_Item MCM_Rev_Park( Park_Item Volt_Input, int16_t Theta );
Vector3Item MCM_Rev_Clark( Clark_Item Volt_Input );
PWM_DUTY PWMC_SetPhaseVoltage( Vector3Item Vabc ,uint16_t deadtmr,uint16_t pwmLimit);
void	PtLib_SetClp_SlowRun_CB(MOTO_INSTANCE * pMoto,Moto_FullRun_CB_Slow cb);
void PtLib_ErrProcess_Callback(MOTO_INSTANCE * pMoto,Moto_ErrPost_CB_Slow cb);
void PtLib_SetStopEvent_CB(MOTO_INSTANCE * pMoto,Moto_StopEvent_Notify cb);
void PtLib_SetStartEvent_CB(MOTO_INSTANCE * pMoto,Moto_StartEvent_Notify cb);
void	PtLib_MotoManager_Init(MOTO_INSTANCE * pMoto);
void DeadTime_Compensition( void * param );
/*************************************************************************************************/
/*************************************************************************************************/
int16_t PtLib_Get_Valid_eSpeed(MOTO_INSTANCE * pMoto);
/*************************************************************************************************/
/*************************************************************************************************/




#ifndef _MOTOBRD_TYPEDEF_HEADER_
#define _MOTOBRD_TYPEDEF_HEADER_
#include "pt32z192xx_it.h"

#include "stdint.h"
#include "string.h"
#include <types.h>         //add by lst for porting djyos

#ifdef PTLIB_VER2_1
//typedef enum {FALSE = 0, TRUE = !FALSE} bool;     //C库中有定义 lst dbg
#endif
#define Q15(Float_Value)    \
        ((Float_Value < 0.0) ? (int)(32768 * (Float_Value) - 0.5) \
        : (int)(32767 * (Float_Value) + 0.5))

#define divSQRT_3   Q15((double)(0.5773315))
//#define   SQRT_3      Q15((double)(0.8660254))
#define SQRT_3      28378
#define LOG2(x) \
  ((x) == 65535 ? 16 : \
   ((x) == 2*2*2*2*2*2*2*2*2*2*2*2*2*2*2 ? 15 : \
    ((x) == 2*2*2*2*2*2*2*2*2*2*2*2*2*2 ? 14 : \
     ((x) == 2*2*2*2*2*2*2*2*2*2*2*2*2 ? 13 : \
      ((x) == 2*2*2*2*2*2*2*2*2*2*2*2 ? 12 : \
       ((x) == 2*2*2*2*2*2*2*2*2*2*2 ? 11 : \
        ((x) == 2*2*2*2*2*2*2*2*2*2 ? 10 : \
         ((x) == 2*2*2*2*2*2*2*2*2 ? 9 : \
          ((x) == 2*2*2*2*2*2*2*2 ? 8 : \
           ((x) == 2*2*2*2*2*2*2 ? 7 : \
            ((x) == 2*2*2*2*2*2 ? 6 : \
             ((x) == 2*2*2*2*2 ? 5 : \
              ((x) == 2*2*2*2 ? 4 : \
               ((x) == 2*2*2 ? 3 : \
                ((x) == 2*2 ? 2 : \
                 ((x) == 2 ? 1 : \
                  ((x) == 1 ? 0 : -1)))))))))))))))))

typedef enum
{
    DIR_NONE = 0,
    DIR_CCW = 1,
    DIR_CW = -1,
}MOTO_RUN_DIR;

typedef struct
{
    int32_t rSum;
    int16_t rFiltK;
    int16_t rValue;
}Filter_Item;

typedef struct _inParam_
{
    uint16_t    current_Ia;
    uint16_t    current_Ib;
    uint16_t    speed_Vref;
    uint16_t    iBus_Val;
    uint16_t    vBus_Val;
}IN_PARAM;

typedef union _VECTOR3_
{
    struct{
        int16_t Va;
        int16_t Vb;
        int16_t Vc;
    };
    struct
    {
        int16_t Ia;
        int16_t Ib;
        int16_t Ic;
    };
} Vector3Item,Phrase_Item;

typedef union _VECTOR2_
{
    struct{
        int16_t Ialpha;
        int16_t Ibeta;
    };
    struct
    {
        int16_t Iq;
        int16_t Id;
    };
    struct
    {
        int16_t Vq;
        int16_t Vd;
    };
} Vector2Item,Clark_Item,Park_Item;

typedef struct
{
    uint8_t result;
    int16_t value;
}CB_Result;

typedef struct
{
    int16_t Da;
    int16_t Db;
    int16_t Dc;
    uint8_t sector;
}PWM_DUTY,PWM_OUTPUT;

typedef struct
{
    int16_t hCos;
    int16_t hSin;
}Math_Components;


/**
  * @brief Handle of a PID component
  *
  * @detail This structure stores all the parameters needed to perform a proportional,
  * integral and derivative regulation computation. It also stores configurable limits
  * in order to saturate the integral terms and the output value. This structure is
  * passed to each PID component function.
  */
typedef struct PID_Handle
{
  int16_t   hKpGain;              /**< @f$K_{pg}@f$ gain used by PID component */
  int16_t   hKiGain;              /**< @f$K_{ig}@f$ gain used by PID component */
  int32_t   wIntegralTerm;        /**< integral term */
  int32_t   wUpperIntegralLimit;  /**< Upper limit used to saturate the integral
                                       term given by @f$\frac{K_{ig}}{K_{id}} @f$ * integral of
                                       process variable error */
  int32_t   wLowerIntegralLimit;  /**< Lower limit used to saturate the integral
                                       term given by Ki / Ki divisor * integral of
                                       process variable error */
  int16_t   hUpperOutputLimit;    /**< Upper limit used to saturate the PI output */
  int16_t   hLowerOutputLimit;    /**< Lower limit used to saturate the PI output */

  uint16_t  hKpDivisorPOW2;       /**< Kp gain divisor expressed as power of 2.
                                       E.g. if gain divisor is 512 the value
                                       must be 9 as 2^9 = 512 */
  uint16_t  hKiDivisorPOW2;       /**< Ki gain divisor expressed as power of 2.
                                       E.g. if gain divisor is 512 the value
                                       must be 9 as 2^9 = 512 */
} PID_Handle_t;


typedef struct _moto_PWMINST_Handle_ MOTO_PWMINST_Handle_t;

typedef void ( *Moto_AdcChannelParase_Cb_t )( uint16_t adcValue );


/* Exported types ------------------------------------------------------------*/
typedef enum
{
  MCI_BUFFER_EMPTY,                  /*!< If no buffered command has been
                                            called.*/
  MCI_COMMAND_NOT_ALREADY_EXECUTED,  /*!< If the buffered command condition
                                            hasn't already occurred.*/
  MCI_COMMAND_EXECUTED_SUCCESFULLY,  /*!< If the buffered command has been
                                            executed successfully.*/
  MCI_COMMAND_EXECUTED_UNSUCCESFULLY /*!< If the buffered command has been
                                            executed unsuccessfully.*/
} MCI_CommandState_t ;

typedef enum
{
  MCI_NOCOMMANDSYET,        /*!< No command has been set by the user.*/
  MCI_EXECSPEEDRAMP,        /*!< ExecSpeedRamp command coming from the user.*/
  MCI_EXECTORQUERAMP,       /*!< ExecTorqueRamp command coming from the user.*/
  MCI_SETCURRENTREFERENCES, /*!< SetCurrentReferences command coming from the
                                 user.*/
} MCI_UserCommands_t;

typedef struct
{
    MCI_UserCommands_t lastCommand; /*!< Last command coming from the user.*/
    int16_t hFinalSpeed;            /*!< Final speed of last ExecSpeedRamp command.*/
    int16_t hFinalTorque;           /*!< Final torque of last ExecTorqueRamp command.*/
    uint16_t hDurationms;           /*!< Duration in ms of last ExecSpeedRamp or ExecTorqueRamp command.*/
    MCI_CommandState_t CommandState;    /*!< The status of the buffered command.*/
} MCI_Handle_t;

/**
  * @brief  State_t enum type definition, it lists all the possible state machine states
  */
typedef enum
{
    STATE_PWR_RST = 0,
    STATE_STOP_IDEL,        //停机且空闲状态
    STATE_START_DELAY,      //启动前等待时间,用于连续启停间隔控制
    STATE_START_INIT,       //启动前准备工作，部分参数初始设置等
    STATE_CHARGE_CAP,       //PWM输出,驱动电路'预热'等待时间
    STATE_CAL_OFFSET,       //相电路偏移校准采样时间
    STATE_START_PREPARE,    //运行参数初始计算

    STATE_ALLIGN_STEP_1,    //转子初始角度估算状态 1~8(保留)
    STATE_ALLIGN_STEP_2,    //转子初始角度估算状态 1~8(保留)
    STATE_ALLIGN_STEP_3,    //转子初始角度估算状态 1~8(保留)
    STATE_ALLIGN_STEP_4,    //转子初始角度估算状态 1~8(保留)
    STATE_ALLIGN_STEP_5,    //转子初始角度估算状态 1~8(保留)
    STATE_ALLIGN_STEP_6,    //转子初始角度估算状态 1~8(保留)
    STATE_ALLIGN_STEP_7,    //转子初始角度估算状态 1~8(保留)
    STATE_ALLIGN_STEP_8,    //转子初始角度估算状态 1~8(保留)

    STATE_START_OPEN_LP,    //开环启动
    STATE_OPEN_LP_DELAY,    //启动延时
    STATE_OPEN_LP_GO,       //加速过程
    STATE_OPEN_LP_END,
    STATE_OLP_CLP_SWITCH,   //加速完成

    STATE_CLOSE_LOOP_RUN,   //闭环运行

    STATE_FAULT_STATE,      //故障异常入口
    STATE_FAULT_OVER,       //故障异常出口

    STATE_ANY_STOP,         //停机入口
    STATE_STOP_OVER,        //停机出口

    STATE_MAX_LIMIT
} MOTO_RUN_STATE_t;

typedef struct
{
  uint16_t MaxModule;
  uint8_t  Start_index;
} CCLimitation_Handle_t;

typedef struct
{
        uint16_t    qVelScale;
    /* Integration constant */
    int16_t qDeltaT;
    /* angle of estimation */
        int16_t qRhoInter;
    int16_t qRho;
        int16_t qLsDt;
    /* internal variable for angle */
    int32_t qRhoStateVar;
    /* primary speed estimation */
    int32_t qOmegaMrInter;
        int16_t qOmegaMr;
    /* last value for Ialpha */
    int16_t qLastIalpha;
    /* last value for Ibeta */
    int16_t qLastIbeta;
    /* difference Ialpha */
    int16_t qDIalpha;
    /* difference Ibeta */
    int16_t qDIbeta;
    /* BEMF alpha */
    int16_t qEsa;
    /* BEMF beta */
    int16_t qEsb;
    /* BEMF d */
    int16_t qEsd;
    /* BEMF q */
    int16_t qEsq;
    /* counter in Last DI tables */
    int16_t qDiCounter;
    /* dI*Ls/dt alpha */
    int16_t qVIndalpha;
    /* dI*Ls/dt beta */
    int16_t qVIndbeta;
    /* BEMF d filtered */
    int16_t qEsdf;
    /* state variable for BEMF d Filtered */
    int32_t qEsdStateVar;
    /* BEMF q filtered */
    int16_t qEsqf;
    /* state variable for BEMF q Filtered */
    int32_t qEsqStateVar;
    /* filter constant for d-q BEMF */
    int16_t qKfilterEsdq;
    /* Estimated speed */
    int16_t qVelEstim;
        int32_t qVelEstmInter;
    /* Filter constant for Estimated speed */
    int16_t qVelEstimFilterK;
    /* State Variable for Estimated speed */
    int32_t qVelEstimStateVar;
    /* Value from last control step Ialpha */
    int16_t qLastValpha;
    /* Value from last control step Ibeta */
    int16_t qLastVbeta;
    /* dIalphabeta/dt */
    int16_t qDIlimitLS;
    /* dIalphabeta/dt */
    int16_t qDIlimitHS;
    /*  last  value for Ialpha */
    int16_t qLastIalphaHS[8];
    /* last  value for Ibeta */
    int16_t qLastIbetaHS[8];
    /* estimator angle initial offset */
    int16_t qRhoOffset;

} PTBEMF_EST_PARM_t;

typedef struct {
        int16_t qMaxSpeedStepNumber;
    int16_t qFwCurve[32];
    uint32_t qInvKFiCurve[32];
    int16_t qLsCurve[32];
}PTFWEEK_TABLE_t;


typedef struct
{
        int16_t eAngleOffset;
        uint32_t                    qRpmStart;
        uint32_t                    qRpmEnd;
//    int16_t                       qFwOnSpeed;
    int16_t                     qIdRef;
    int16_t                     qIndex;
        int16_t                     qLsDt;
        uint32_t                    qInvKFi;
        PTFWEEK_TABLE_t * pfw_Table;
} PTFWEEK_PARM_t;

typedef struct _RunTime_calDat_
{
//逐周期(PWM周期)-电流限幅
    uint32_t        motoPwrOn_Tick;
    int32_t         bemf_AccReg;
    int32_t         vbus_AccReg;
    int32_t         mIq_Acc;
    int32_t         mId_Acc;
    Park_Item       mIqdAvg;
    uint16_t        mIqd_pRho;
    int16_t     mIqd_Theta;
    int16_t         mDead_Uerr;
//  int32_t         iBus_AccReg;
    int16_t         iBus_Average;
    uint16_t        bemf_AmpErrTmrCnt;
    uint16_t        bemf_AmpNorTmrCnt;
    uint16_t        bemf_AvrErrTmrCnt;
    uint16_t        bemf_AvrNorTmrCnt;
    uint16_t        phraseErr_Count;
    uint16_t        phraseErr_TLimit;
    uint16_t        curAmp_OvCnt;
    uint16_t        curAmp_OvClrCnt;
//  uint16_t        current_amp_counter;
    uint16_t        iPhrase_OverLimit_cnt;
//  uint16_t        iv_OverLimit_Period;


    uint16_t        RtFualt_DectState;              //bit0  == 1: BEMF幅值异常
                                                                                //bit1  == 1: BEMF均值异常
                                                                                //bit4  == 1: 相电流幅值超过设定值
                                                                                //bit5  == 1: 欠压/过压
                                                                                //bit7  == 1: 缺相异常
                                                                                //bit8  == 1: 刹车中断已经发生，已经检测到过流并已经停机(PWM关闭输出)
                                                                                //bit14 == 1: 电机停止状态下，检测到电流幅值超过设定值
                                                                                //bit15 == 1: 内部计算标志位--反电动势数据准备好，启动反电动势幅值异常检测
    uint16_t        RtFault_ErrCode;
    int16_t         vBus_FiltOut;
    uint16_t            bemf_cFromCurrent;
    uint16_t        fifo_WrIndex;
    uint16_t        bemf_FifoBuff[32];
    uint16_t            bemf_max;
    uint16_t            bemf_min;
}RT_Monitor;

typedef struct _hsitem_
{
    uint16_t    runCounter;
    uint16_t    eAngle_TestOut;         //测试电角度位置
    uint16_t        Hfi_SlRun_EnSpeed;
    uint16_t        Hfi_InsVol;
    Clark_Item  deltahsInter;
    Clark_Item  oldhsInter;
    Park_Item       oldIqd;
    uint16_t    eAngleOffset;
    int16_t     qVelEstim;
    uint16_t        qInvKfi_SetVal;
    int16_t         qDeltaT_SetVal;
    uint16_t        qInvKfiRun_SetVal;
    int16_t         qDeltaTRun_SetVal;
    int16_t     qDeltaT;
    uint16_t        qInvKFi;
    uint16_t    qRho;
union
{
    uint16_t    qRho_Inter;
    int16_t     qOmegaMr;
};
union
{
    int16_t     qPi_in;
    uint16_t    qRho_min;
};
union {
    int16_t     qPi_out;
    uint16_t    qRho_max;
};
union
{
    int32_t     maxIqd_Value;
    int32_t     angledStateVar;
};
union
{
    int32_t     minIqd_Value;
    int32_t     angleqStateVar;
};
union
{
    uint32_t    maxId_acc;
    int32_t     qRhoStateVar;
};
    int32_t     qVelEstimStateVar;
}HFI_estim;

typedef struct _moto_ctrl_
{
    uint8_t     runPerPwmPeriod;
    uint8_t     runPwmCounter;
    uint8_t     remCmd;
    uint8_t     regResv;
    int16_t     DcBus_uMax;
    int16_t     speedRef_Slow;
    int16_t     speedRef_Calculate;
    int16_t     speedRef_FromRem;
    IN_PARAM    pInParam;
}MOTO_INTER_DATA;

typedef void ( *Moto_StateFucntion_t )( void * pParam );
typedef void ( *Moto_ReadFunction_CB ) (void * pMoto );
typedef void ( *Moto_CurrentProcess_CB ) (void * pMoto );
typedef void ( *Moto_RtMonitorProcess ) (void * pMoto );
typedef void ( *Moto_PwmOutPut_CB ) (PWM_OUTPUT * pDutyOut );
typedef int16_t     ( *Moto_SetSpeedRef_CB ) (void * pParam );
typedef void        ( *Moto_Fun_ControllLoop ) (void * pParam );
typedef void        ( *Moto_VqdPost_ProCB ) ( void * param );
typedef CB_Result   ( *Moto_SlowToStop_CB ) ( void * param );
typedef void    ( *Moto_eAngleCal_CB ) ( void * param );
typedef void    ( *Moto_FullRun_CB_Slow ) ( void * param );
typedef void    ( *Moto_ErrPost_CB_Slow ) ( void * param );
typedef void    ( *Moto_StopEvent_Notify ) ( void * param );
typedef void    ( *Moto_StartEvent_Notify ) ( void * param );


typedef enum {
    MS_IDEL         = 0,
    MS_WAITSTART    = 1,
    MS_ALIGN_INIT   = 2,
    MS_ALIGN_NOR    = 3,
    MS_ALIGN_POL    = 4,
    MS_ALIGN_RAMUP  = 5,
    MS_ALIGN_END    = 6,
    MS_OPENLOOPRUN  = 7,
    MS_RUN          = 8,
    MS_SLOWRUN      = 9,
    MS_STOP         = 10,
    MS_WAIT_CMD     = 11,
    MS_STATE_NUM
} MOTO_STATE_T;


typedef struct _moto_param_
{
    uint32_t    qRs_K1;
    uint32_t    qLsDt_K2;
    uint32_t    infKfi_K3;
//-----------------------------------------------------------------------------------------
    uint32_t    qRpmStart;
    uint32_t    qRpmEnd;
//  int16_t     qFwOnSpeed;
//---------------------------电机参数------------------------------------------------------
    int16_t     VoltageBus;
    int16_t     CurPeakBus;
    int16_t     PwmPeriod;                  //微秒
    int16_t     PwmDeath;                       //微秒
    int16_t     noPolesPairs;               //极对数
    int16_t     PhraseRes;                  //相电阻(mR)
    int16_t     PhraseInd;                  //相电感(uH)
    int16_t     conKFI;                         //Vp/KRPM
    int16_t     NorSpeed;
    int16_t     MaxSpeed;
    int16_t     conKFI_Offset;
//-----------------------------------------------------------------------------------------
    int16_t     Vadc_Vbus_Ratio;        //VDC转VBUS系数
//  uint16_t    Vqd_MinLimit;               //启动运行中Vqd.Vq的最小限制
    uint16_t    DeadTmr_ComValue;
    uint16_t    MaxPwmDutyValue;        //占空比输出最大值
    uint16_t    OpenLp_PwmDuty;         //开环加速占空比输出最大值
    uint16_t    CloseLp_PwmDutyRef; //闭环运行占空比输出最大值
    uint16_t    Hfi_PwmDutyRef;         //高频注入占空比输出最大值

    int16_t     qRs;
    int16_t     qLsDt;
    uint16_t    qInvKFi;
    int16_t     qEstmDeltaT;
    int16_t     qDIlimitLS;
  int16_t       qDIlimitHS;
    int16_t     pidQ_hDefKpGain;            //q轴PI控制器比例系数
    int16_t     pidQ_hDefKiGain;            //q轴PI控制器积分系数
    int16_t     pidD_hDefKpGain;            //d轴PI控制器比例系数
    int16_t     pidD_hDefKiGain;            //d轴PI控制器积分系数
    int16_t     pidSpeed_hDefKpGain;    //速度PI控制器比例系数
    int16_t     pidSpeed_hDefKiGain;    //速度PI控制器积分系数
    int16_t     piHfiPll_hDefKpGain;    //HFI角度估算器PI比例系数
    int16_t     piHfiPll_hDefKiGain;    //HFI角度估算器PI积分系数
    int16_t     qRhoOffsetBase;             //电机运行初始超前角度

    uint16_t    hfi_controll;       //bit0:  电压注入方向标志位
                                                        //bit8 ~bit10: 运行控制位
                                                        //  b8: 0~180估算测试模式
                                                        //bit11:  强制使电机转到指定角度，用于查看位置
                                                        //bit15 = 1: 第一阶段角度估算完成
}PTMC_CONFIG_PKG;

typedef struct _moto_runparam_
{
    uint8_t                 tstFlag;
    MOTO_RUN_DIR        bRun_Dir;                               //电机运行方向(2.0版本前，暂时没有使用，不支持电机运行方向选择)
    MOTO_RUN_STATE_t    bState;                             /*慢速线程: 电机运行状态控制 */
    uint32_t            bStateDelay;
    uint32_t            bStateTmrCnt;
//---2相电流采样，0点校准后，电流幅度软件校准------------
    int16_t             uPhrase_Scale;                      /*u相电流幅度校正 */
    int16_t             vPhrase_Scale;                      /*v相电流幅度校正 */
/********************************************************************************************/
    uint16_t            mFocRun_Period;                     //FOC计算周期数：设置多少个PWM周期进行一次FOC计算
    int16_t         conKFI_Inter;
    int16_t         conKFIreal;                                 //Vp/KRPM
    uint32_t        qInvKFi;                                        //正常运行状态下反电动势常数
/********************************************************************************************/
/**VBUS/相电压电流:**************************************************************************/
/********************************************************************************************/
    uint16_t        Vbus_Voltage;                               //VBUS电源电压--单位(伏)
//  uint16_t        recal_Vbus;
//---总线电压，上下限保护设置参数---对应的ADC采样值------
    int16_t             FaultDect_VUpLimit;             //Vbus上限保护
    int16_t             FaultDect_VDnimit;              //Vbus下限保护
//---相电流，  上限保护设置参数---对应的ADC采样值------
    int16_t             iCurrent_MaxIa;
    uint16_t            bRunTimeStat_Enable;
    uint16_t            bForceLoop_Delay;
/********************************************************************************************/
/**高频注入参数:*****************************************************************************/
/********************************************************************************************/
    uint16_t        Hfi_ModeSel;
    uint16_t        Hfi_Frequency;                          //HFI注入频率
    uint16_t        Hfi_InsVol;                                 //HFI注入电压幅度
    uint16_t        HfiPol_InsVol;                          //HFI注入电压幅度
    uint16_t        HfiRun_InsVol;                          //HFI注入电压幅度
    uint16_t        Hfi_TmrLimit;                               //HFI时间限制(无效，未使用)
    uint16_t        Hfi_Pol_Thold;                          //HFI极性判断阈值
    int16_t         bMotoHFI_Pol_Doffset;               //高频注入极性判断D轴偏置
    int16_t         bMotoHFI_slRun_Qref;                //高频注入强拖Q参考值
/**运行控制参数******************************************************************************/
    uint16_t        bMoto_ReStartUp_Interval;       //电机启停后，再次启动运行的时间间隔(5毫秒时间单位)
    uint16_t        bMoto_StartUp_Cfg;                  //启动运行参数选项
    uint16_t        bOutpuPowerLimit;                       //运行时最大输出电压控制
/********************************************************************************************/
/**开环加速控制参数******************************************************************************/
    uint16_t        bMoto_accelerate_tmrlimit;      //开环加速时间 - ms
    uint16_t        bMoto_EndOpenLoop_Delay;            //从开环进入闭环FOC前的延时
    uint16_t        bMoto_Init_eAngle;                      //起始默认角度 - 如果启用HFI,这个参数由HFI决定
    int16_t         bMotoStartUp_Speed;                     /*开环启动速度 */                       //准备删除
    int16_t         bMotoOpenLp_enSpeed;                    /*开环结束速度 */
    int16_t         bMotoFullRun_Qref;                      //开环启动或闭环启动前，Iq初值
    int16_t         bMotoCloseLoop_QrefInit;            //闭环启动前，Iq初值
    int16_t         bMotoClpInit_Qref;                      //顺风闭环启动，Iq初值
    int16_t         bMotoFullRun_Dref;                      //开环启动或闭环启动前，Id初值
    int16_t         bMotoVqd_InitRef;                           //开环启动或闭环启动前，Vq初值
    int16_t         bMotoOpenRun_DeltaEa;                   //内部寄存器
    uint16_t        stSpeed_DeltaEa;
    uint16_t        enSpeed_DeltaEa;
    uint16_t        delta_Interval;
/********************************************************************************************/
/**堵转检测设置参数******************************************************************************/
    uint16_t        phraseErr_TmrLimit;                 //缺相故障电流检测阈值
    int16_t         bemf_UpLimit;                               //堵转检测反电动势上限
    int16_t         bemf_DnLimit;                               //堵转检测反电动势下限
    int16_t         bemf_FiltK;                                 //堵转检测反电动势专用滤波系数
    int16_t         bemf_OvTLimit;                          //堵转检测反电动势超限时间阈值
    int16_t         bemf_rLimit;                                //堵转检测反电动势波动圆直径阈值
    int16_t         bemf_rTmrLimit;                         //堵转检测反电动势波动圆超阈值时间设置
    int16_t         qVel_CalRatio;                          //内部计算参数
    int16_t         qVel_IntRatio;                          //内部计算参数
/********************************************************************************************/
}PTMC_RUN_PARAM;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
//#pragma pack (1)
typedef struct _moto_Inst_
{
    uint32_t                            userRegA;
    uint32_t                            userRegB;
    uint32_t                            userRegC;
    uint32_t                            userRegD;
/********************************************************************************************/
/**堵转检测设置参数******************************************************************************/
    IN_PARAM                            mCurrentOffset;                 //相电流0点校准值
    MOTO_STATE_T                    motoState_set;
    MOTO_STATE_T                    motoState_now;
/********************************************************************************************/
/**FOC 函数指针定义**************************************************************************/
    Moto_Fun_ControllLoop   MotoControllLoop;               //FOC主函数指针
    Moto_StateFucntion_t *  MotoFunction;                   //FOC-LIB 状态控制函数指针数组
    Moto_ReadFunction_CB        Moto_ReadCurrent;           //电流采样回调函数指针
    Moto_CurrentProcess_CB  CurrentIn_PreProcess;   //FOC电流预处理函数指针
    Moto_RtMonitorProcess       Moto_MonitorProcess;    //电机运行监控函数指针
    Moto_SetSpeedRef_CB     Moto_SetSpeedRef;               //FOC闭环运行过程回调函数，用于更新速度等参数
    Moto_PwmOutPut_CB           Moto_PwmOutDriver;          //更新输出PWM占空比
    Moto_VqdPost_ProCB      Moto_Compensation_CB;
    Moto_VqdPost_ProCB      Moto_VqdPostProcess;        //FOC Vqd数据后处理函数指针
    Moto_SlowToStop_CB      Moto_SetSlowToStopRef;  //停机控制函数
    Moto_eAngleCal_CB           Moto_eAngle_EstemCB;        //FOC 角度估算器函数指针
    Moto_FullRun_CB_Slow    Moto_FullRun_ClpCB;         //低速进程闭环运行回调函数
    Moto_ErrPost_CB_Slow    Moto_ErrPost_CB;                //异常时间处理程序，(堵转/过流/过压/过流/缺相)
    Moto_StopEvent_Notify   Moto_StopEvent_CB;
    Moto_StartEvent_Notify  Moto_StartEvent_CB;
/********************************************************************************************/
/*FOC计算寄配置参数**************************************************************************/
    MOTO_INTER_DATA         InterBuff;                              //保存AD采样值等运行参数
    PTMC_CONFIG_PKG *   mMotoCfg;                                   //电机配置参数
    PTMC_RUN_PARAM      *   mMotoRunCfg;                            //电机运行参数
    uint16_t                            eAngleIn;                               //上次输出的电角度
    uint16_t                            eAngleOut;                          //本次将输出的电角度
    Phrase_Item                     mCurrent;                               //AD采样0点校准后，经过预处理后的2相电流
    Phrase_Item                     mCurMax;
    int16_t                             mCurPeak;                               //三相电流峰值的最大值 -- 每个FOC周期更新一次
    Clark_Item                      Ialphabeta;                         //保存2相Clark变换输出结果
    Park_Item                           Iqd;                                        //保存2相Park变换输出结果
    Park_Item                           IqdRef;                                 //Iqd 参考
    Park_Item                           Vqd;                                        //电流环输出
    Clark_Item                      valphabeta;                         //保存2相逆Park变换输出结果
//  Vector3Item                     mVoltage;                               //输入到SVPWM的三相电压值
    Park_Item                           bemfdq;                                 //估算的dq轴反电动势
    Clark_Item                      bemfAlphaBeta;                  //估算的静止轴反电动势
    PID_Handle_t                    pidIq;                                  //q轴 PI 控制器参数
    PID_Handle_t                    pidId;                                  //d轴 PI 控制器参数
    PID_Handle_t                    pidSpeed;                               //速度 PI 控制器参数
    PID_Handle_t                    piHFIPll;                               //高频注入锁相环 PI 控制器参数
    PID_Handle_t                    pi_CurPhrase;                       //高频注入锁相环 PI 控制器参数
    PTBEMF_EST_PARM_t           estimator;                          //FOC 角度观测器运行参数
    PTFWEEK_PARM_t              ptFdWeek;                               //弱磁运行参数
    CCLimitation_Handle_t   circleLimit;                        //旋转轴下，输出功率限制
    HFI_estim                           hsaEstim;                               //高频注入运行参数
    RT_Monitor                      runTime_Monitor;                //电机运行状态监控参数
    PWM_OUTPUT                      mPwmOutput;                         //对应逆变器三路输出PWM的占空比寄存器的值
}MOTO_INSTANCE;
//#pragma pack ()
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif




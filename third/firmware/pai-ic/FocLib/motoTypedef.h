


#ifndef _MOTOBRD_TYPEDEF_HEADER_
#define _MOTOBRD_TYPEDEF_HEADER_
#include "pt32z192xx_it.h"

#include "stdint.h"
#include "string.h"
#include <types.h>         //add by lst for porting djyos

#ifdef PTLIB_VER2_1
//typedef enum {FALSE = 0, TRUE = !FALSE} bool;     //C�����ж��� lst dbg
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
    STATE_STOP_IDEL,        //ͣ���ҿ���״̬
    STATE_START_DELAY,      //����ǰ�ȴ�ʱ��,����������ͣ�������
    STATE_START_INIT,       //����ǰ׼�����������ֲ�����ʼ���õ�
    STATE_CHARGE_CAP,       //PWM���,������·'Ԥ��'�ȴ�ʱ��
    STATE_CAL_OFFSET,       //���·ƫ��У׼����ʱ��
    STATE_START_PREPARE,    //���в�����ʼ����

    STATE_ALLIGN_STEP_1,    //ת�ӳ�ʼ�Ƕȹ���״̬ 1~8(����)
    STATE_ALLIGN_STEP_2,    //ת�ӳ�ʼ�Ƕȹ���״̬ 1~8(����)
    STATE_ALLIGN_STEP_3,    //ת�ӳ�ʼ�Ƕȹ���״̬ 1~8(����)
    STATE_ALLIGN_STEP_4,    //ת�ӳ�ʼ�Ƕȹ���״̬ 1~8(����)
    STATE_ALLIGN_STEP_5,    //ת�ӳ�ʼ�Ƕȹ���״̬ 1~8(����)
    STATE_ALLIGN_STEP_6,    //ת�ӳ�ʼ�Ƕȹ���״̬ 1~8(����)
    STATE_ALLIGN_STEP_7,    //ת�ӳ�ʼ�Ƕȹ���״̬ 1~8(����)
    STATE_ALLIGN_STEP_8,    //ת�ӳ�ʼ�Ƕȹ���״̬ 1~8(����)

    STATE_START_OPEN_LP,    //��������
    STATE_OPEN_LP_DELAY,    //������ʱ
    STATE_OPEN_LP_GO,       //���ٹ���
    STATE_OPEN_LP_END,
    STATE_OLP_CLP_SWITCH,   //�������

    STATE_CLOSE_LOOP_RUN,   //�ջ�����

    STATE_FAULT_STATE,      //�����쳣���
    STATE_FAULT_OVER,       //�����쳣����

    STATE_ANY_STOP,         //ͣ�����
    STATE_STOP_OVER,        //ͣ������

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
//������(PWM����)-�����޷�
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


    uint16_t        RtFualt_DectState;              //bit0  == 1: BEMF��ֵ�쳣
                                                                                //bit1  == 1: BEMF��ֵ�쳣
                                                                                //bit4  == 1: �������ֵ�����趨ֵ
                                                                                //bit5  == 1: Ƿѹ/��ѹ
                                                                                //bit7  == 1: ȱ���쳣
                                                                                //bit8  == 1: ɲ���ж��Ѿ��������Ѿ���⵽�������Ѿ�ͣ��(PWM�ر����)
                                                                                //bit14 == 1: ���ֹͣ״̬�£���⵽������ֵ�����趨ֵ
                                                                                //bit15 == 1: �ڲ������־λ--���綯������׼���ã��������綯�Ʒ�ֵ�쳣���
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
    uint16_t    eAngle_TestOut;         //���Ե�Ƕ�λ��
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
//---------------------------�������------------------------------------------------------
    int16_t     VoltageBus;
    int16_t     CurPeakBus;
    int16_t     PwmPeriod;                  //΢��
    int16_t     PwmDeath;                       //΢��
    int16_t     noPolesPairs;               //������
    int16_t     PhraseRes;                  //�����(mR)
    int16_t     PhraseInd;                  //����(uH)
    int16_t     conKFI;                         //Vp/KRPM
    int16_t     NorSpeed;
    int16_t     MaxSpeed;
    int16_t     conKFI_Offset;
//-----------------------------------------------------------------------------------------
    int16_t     Vadc_Vbus_Ratio;        //VDCתVBUSϵ��
//  uint16_t    Vqd_MinLimit;               //����������Vqd.Vq����С����
    uint16_t    DeadTmr_ComValue;
    uint16_t    MaxPwmDutyValue;        //ռ�ձ�������ֵ
    uint16_t    OpenLp_PwmDuty;         //��������ռ�ձ�������ֵ
    uint16_t    CloseLp_PwmDutyRef; //�ջ�����ռ�ձ�������ֵ
    uint16_t    Hfi_PwmDutyRef;         //��Ƶע��ռ�ձ�������ֵ

    int16_t     qRs;
    int16_t     qLsDt;
    uint16_t    qInvKFi;
    int16_t     qEstmDeltaT;
    int16_t     qDIlimitLS;
  int16_t       qDIlimitHS;
    int16_t     pidQ_hDefKpGain;            //q��PI����������ϵ��
    int16_t     pidQ_hDefKiGain;            //q��PI����������ϵ��
    int16_t     pidD_hDefKpGain;            //d��PI����������ϵ��
    int16_t     pidD_hDefKiGain;            //d��PI����������ϵ��
    int16_t     pidSpeed_hDefKpGain;    //�ٶ�PI����������ϵ��
    int16_t     pidSpeed_hDefKiGain;    //�ٶ�PI����������ϵ��
    int16_t     piHfiPll_hDefKpGain;    //HFI�Ƕȹ�����PI����ϵ��
    int16_t     piHfiPll_hDefKiGain;    //HFI�Ƕȹ�����PI����ϵ��
    int16_t     qRhoOffsetBase;             //������г�ʼ��ǰ�Ƕ�

    uint16_t    hfi_controll;       //bit0:  ��ѹע�뷽���־λ
                                                        //bit8 ~bit10: ���п���λ
                                                        //  b8: 0~180�������ģʽ
                                                        //bit11:  ǿ��ʹ���ת��ָ���Ƕȣ����ڲ鿴λ��
                                                        //bit15 = 1: ��һ�׶νǶȹ������
}PTMC_CONFIG_PKG;

typedef struct _moto_runparam_
{
    uint8_t                 tstFlag;
    MOTO_RUN_DIR        bRun_Dir;                               //������з���(2.0�汾ǰ����ʱû��ʹ�ã���֧�ֵ�����з���ѡ��)
    MOTO_RUN_STATE_t    bState;                             /*�����߳�: �������״̬���� */
    uint32_t            bStateDelay;
    uint32_t            bStateTmrCnt;
//---2�����������0��У׼�󣬵����������У׼------------
    int16_t             uPhrase_Scale;                      /*u���������У�� */
    int16_t             vPhrase_Scale;                      /*v���������У�� */
/********************************************************************************************/
    uint16_t            mFocRun_Period;                     //FOC���������������ö��ٸ�PWM���ڽ���һ��FOC����
    int16_t         conKFI_Inter;
    int16_t         conKFIreal;                                 //Vp/KRPM
    uint32_t        qInvKFi;                                        //��������״̬�·��綯�Ƴ���
/********************************************************************************************/
/**VBUS/���ѹ����:**************************************************************************/
/********************************************************************************************/
    uint16_t        Vbus_Voltage;                               //VBUS��Դ��ѹ--��λ(��)
//  uint16_t        recal_Vbus;
//---���ߵ�ѹ�������ޱ������ò���---��Ӧ��ADC����ֵ------
    int16_t             FaultDect_VUpLimit;             //Vbus���ޱ���
    int16_t             FaultDect_VDnimit;              //Vbus���ޱ���
//---�������  ���ޱ������ò���---��Ӧ��ADC����ֵ------
    int16_t             iCurrent_MaxIa;
    uint16_t            bRunTimeStat_Enable;
    uint16_t            bForceLoop_Delay;
/********************************************************************************************/
/**��Ƶע�����:*****************************************************************************/
/********************************************************************************************/
    uint16_t        Hfi_ModeSel;
    uint16_t        Hfi_Frequency;                          //HFIע��Ƶ��
    uint16_t        Hfi_InsVol;                                 //HFIע���ѹ����
    uint16_t        HfiPol_InsVol;                          //HFIע���ѹ����
    uint16_t        HfiRun_InsVol;                          //HFIע���ѹ����
    uint16_t        Hfi_TmrLimit;                               //HFIʱ������(��Ч��δʹ��)
    uint16_t        Hfi_Pol_Thold;                          //HFI�����ж���ֵ
    int16_t         bMotoHFI_Pol_Doffset;               //��Ƶע�뼫���ж�D��ƫ��
    int16_t         bMotoHFI_slRun_Qref;                //��Ƶע��ǿ��Q�ο�ֵ
/**���п��Ʋ���******************************************************************************/
    uint16_t        bMoto_ReStartUp_Interval;       //�����ͣ���ٴ��������е�ʱ����(5����ʱ�䵥λ)
    uint16_t        bMoto_StartUp_Cfg;                  //�������в���ѡ��
    uint16_t        bOutpuPowerLimit;                       //����ʱ��������ѹ����
/********************************************************************************************/
/**�������ٿ��Ʋ���******************************************************************************/
    uint16_t        bMoto_accelerate_tmrlimit;      //��������ʱ�� - ms
    uint16_t        bMoto_EndOpenLoop_Delay;            //�ӿ�������ջ�FOCǰ����ʱ
    uint16_t        bMoto_Init_eAngle;                      //��ʼĬ�ϽǶ� - �������HFI,���������HFI����
    int16_t         bMotoStartUp_Speed;                     /*���������ٶ� */                       //׼��ɾ��
    int16_t         bMotoOpenLp_enSpeed;                    /*���������ٶ� */
    int16_t         bMotoFullRun_Qref;                      //����������ջ�����ǰ��Iq��ֵ
    int16_t         bMotoCloseLoop_QrefInit;            //�ջ�����ǰ��Iq��ֵ
    int16_t         bMotoClpInit_Qref;                      //˳��ջ�������Iq��ֵ
    int16_t         bMotoFullRun_Dref;                      //����������ջ�����ǰ��Id��ֵ
    int16_t         bMotoVqd_InitRef;                           //����������ջ�����ǰ��Vq��ֵ
    int16_t         bMotoOpenRun_DeltaEa;                   //�ڲ��Ĵ���
    uint16_t        stSpeed_DeltaEa;
    uint16_t        enSpeed_DeltaEa;
    uint16_t        delta_Interval;
/********************************************************************************************/
/**��ת������ò���******************************************************************************/
    uint16_t        phraseErr_TmrLimit;                 //ȱ����ϵ��������ֵ
    int16_t         bemf_UpLimit;                               //��ת��ⷴ�綯������
    int16_t         bemf_DnLimit;                               //��ת��ⷴ�綯������
    int16_t         bemf_FiltK;                                 //��ת��ⷴ�綯��ר���˲�ϵ��
    int16_t         bemf_OvTLimit;                          //��ת��ⷴ�綯�Ƴ���ʱ����ֵ
    int16_t         bemf_rLimit;                                //��ת��ⷴ�綯�Ʋ���Բֱ����ֵ
    int16_t         bemf_rTmrLimit;                         //��ת��ⷴ�綯�Ʋ���Բ����ֵʱ������
    int16_t         qVel_CalRatio;                          //�ڲ��������
    int16_t         qVel_IntRatio;                          //�ڲ��������
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
/**��ת������ò���******************************************************************************/
    IN_PARAM                            mCurrentOffset;                 //�����0��У׼ֵ
    MOTO_STATE_T                    motoState_set;
    MOTO_STATE_T                    motoState_now;
/********************************************************************************************/
/**FOC ����ָ�붨��**************************************************************************/
    Moto_Fun_ControllLoop   MotoControllLoop;               //FOC������ָ��
    Moto_StateFucntion_t *  MotoFunction;                   //FOC-LIB ״̬���ƺ���ָ������
    Moto_ReadFunction_CB        Moto_ReadCurrent;           //���������ص�����ָ��
    Moto_CurrentProcess_CB  CurrentIn_PreProcess;   //FOC����Ԥ������ָ��
    Moto_RtMonitorProcess       Moto_MonitorProcess;    //������м�غ���ָ��
    Moto_SetSpeedRef_CB     Moto_SetSpeedRef;               //FOC�ջ����й��̻ص����������ڸ����ٶȵȲ���
    Moto_PwmOutPut_CB           Moto_PwmOutDriver;          //�������PWMռ�ձ�
    Moto_VqdPost_ProCB      Moto_Compensation_CB;
    Moto_VqdPost_ProCB      Moto_VqdPostProcess;        //FOC Vqd���ݺ�����ָ��
    Moto_SlowToStop_CB      Moto_SetSlowToStopRef;  //ͣ�����ƺ���
    Moto_eAngleCal_CB           Moto_eAngle_EstemCB;        //FOC �Ƕȹ���������ָ��
    Moto_FullRun_CB_Slow    Moto_FullRun_ClpCB;         //���ٽ��̱ջ����лص�����
    Moto_ErrPost_CB_Slow    Moto_ErrPost_CB;                //�쳣ʱ�䴦�����(��ת/����/��ѹ/����/ȱ��)
    Moto_StopEvent_Notify   Moto_StopEvent_CB;
    Moto_StartEvent_Notify  Moto_StartEvent_CB;
/********************************************************************************************/
/*FOC��������ò���**************************************************************************/
    MOTO_INTER_DATA         InterBuff;                              //����AD����ֵ�����в���
    PTMC_CONFIG_PKG *   mMotoCfg;                                   //������ò���
    PTMC_RUN_PARAM      *   mMotoRunCfg;                            //������в���
    uint16_t                            eAngleIn;                               //�ϴ�����ĵ�Ƕ�
    uint16_t                            eAngleOut;                          //���ν�����ĵ�Ƕ�
    Phrase_Item                     mCurrent;                               //AD����0��У׼�󣬾���Ԥ������2�����
    Phrase_Item                     mCurMax;
    int16_t                             mCurPeak;                               //���������ֵ�����ֵ -- ÿ��FOC���ڸ���һ��
    Clark_Item                      Ialphabeta;                         //����2��Clark�任������
    Park_Item                           Iqd;                                        //����2��Park�任������
    Park_Item                           IqdRef;                                 //Iqd �ο�
    Park_Item                           Vqd;                                        //���������
    Clark_Item                      valphabeta;                         //����2����Park�任������
//  Vector3Item                     mVoltage;                               //���뵽SVPWM�������ѹֵ
    Park_Item                           bemfdq;                                 //�����dq�ᷴ�綯��
    Clark_Item                      bemfAlphaBeta;                  //����ľ�ֹ�ᷴ�綯��
    PID_Handle_t                    pidIq;                                  //q�� PI ����������
    PID_Handle_t                    pidId;                                  //d�� PI ����������
    PID_Handle_t                    pidSpeed;                               //�ٶ� PI ����������
    PID_Handle_t                    piHFIPll;                               //��Ƶע�����໷ PI ����������
    PID_Handle_t                    pi_CurPhrase;                       //��Ƶע�����໷ PI ����������
    PTBEMF_EST_PARM_t           estimator;                          //FOC �Ƕȹ۲������в���
    PTFWEEK_PARM_t              ptFdWeek;                               //�������в���
    CCLimitation_Handle_t   circleLimit;                        //��ת���£������������
    HFI_estim                           hsaEstim;                               //��Ƶע�����в���
    RT_Monitor                      runTime_Monitor;                //�������״̬��ز���
    PWM_OUTPUT                      mPwmOutput;                         //��Ӧ�������·���PWM��ռ�ձȼĴ�����ֵ
}MOTO_INSTANCE;
//#pragma pack ()
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif




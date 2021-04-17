
#include "../core/inc/usbh_core.h"
#include "../../usbh_lowlevel.h"
#include <systime.h>

struct USBH_LowLevel USBH_HS, USBH_FS; // TODO: �ı�Ϊ��̬

static struct USBH_LowLevel *__USBH_LL_GetLL(USBH_HandleTypeDef *pHost);


// ============================================================================
// ���ܣ�Initialize the Low Level portion of the Host driver.
// ������ pHost -- Host handle.
// ���أ�
// ��ע��
// ============================================================================
USBH_StatusTypeDef USBH_LL_Init (USBH_HandleTypeDef *pHost)
{
    struct USBH_LowLevel *USBH_LL;
    USBH_StatusTypeDef res = USBH_FAIL;

    USBH_LL = __USBH_LL_GetLL(pHost);
    if((!USBH_LL) || (!USBH_LL->Init))
        return USBH_NOT_SUPPORTED;

    if(Lock_MutexPend((struct MutexLCB*)USBH_LL->pMutex, CN_TIMEOUT_FOREVER))
        res = USBH_LL->Init(pHost);
    Lock_MutexPost((struct MutexLCB*)USBH_LL->pMutex);
    return (res);

}

// ============================================================================
// ���ܣ� De-Initialize the Low Level portion of the Host driver.
// ������ pHost -- Host handle.
// ���أ�
// ��ע��
// ============================================================================
USBH_StatusTypeDef USBH_LL_DeInit (USBH_HandleTypeDef *pHost)
{
    struct USBH_LowLevel *USBH_LL;
    USBH_StatusTypeDef res = USBH_FAIL;

    USBH_LL = __USBH_LL_GetLL(pHost);
    if((!USBH_LL) || (!USBH_LL->DeInit))
        return USBH_NOT_SUPPORTED;

    if(Lock_MutexPend((struct MutexLCB*)USBH_LL->pMutex, CN_TIMEOUT_FOREVER))
        res = USBH_LL->DeInit(pHost);
    Lock_MutexPost((struct MutexLCB*)USBH_LL->pMutex);
    return (res);
}

// ============================================================================
// ���ܣ� Start the Low Level portion of the Host driver.
// ������ pHost -- Host handle.
// ���أ�
// ��ע��
// ============================================================================
USBH_StatusTypeDef USBH_LL_Start(USBH_HandleTypeDef *pHost)
{
    struct USBH_LowLevel *USBH_LL;
    USBH_StatusTypeDef res = USBH_FAIL;

    USBH_LL = __USBH_LL_GetLL(pHost);
    if((!USBH_LL) || (!USBH_LL->Start))
        return USBH_NOT_SUPPORTED;

    if(Lock_MutexPend((struct MutexLCB*)USBH_LL->pMutex, CN_TIMEOUT_FOREVER))
        res = USBH_LL->Start(pHost);
    Lock_MutexPost((struct MutexLCB*)USBH_LL->pMutex);
    return (res);
}

// ============================================================================
// ���ܣ� Stop the Low Level portion of the Host driver.
// ������ pHost -- Host handle.
// ���أ�
// ��ע��
// ============================================================================
USBH_StatusTypeDef USBH_LL_Stop (USBH_HandleTypeDef *pHost)
{
    struct USBH_LowLevel *USBH_LL;
    USBH_StatusTypeDef res = USBH_FAIL;

    USBH_LL = __USBH_LL_GetLL(pHost);
    if((!USBH_LL) || (!USBH_LL->Stop))
        return USBH_NOT_SUPPORTED;

    if(Lock_MutexPend((struct MutexLCB*)USBH_LL->pMutex, CN_TIMEOUT_FOREVER))
        res = USBH_LL->Stop(pHost);
    Lock_MutexPost((struct MutexLCB*)USBH_LL->pMutex);
    return (res);
}

// ============================================================================
// ���ܣ� Return the USB Host Speed from the Low Level Driver.
// ������ pHost -- Host handle.
// ���أ�
// ��ע��
// ============================================================================
USBH_SpeedTypeDef USBH_LL_GetSpeed (USBH_HandleTypeDef *pHost)
{
    struct USBH_LowLevel *USBH_LL;
    USBH_StatusTypeDef res = USBH_FAIL;

    USBH_LL = __USBH_LL_GetLL(pHost);
    if((!USBH_LL) || (!USBH_LL->GetSpeed))
        return (-1);

    if(Lock_MutexPend((struct MutexLCB*)USBH_LL->pMutex, CN_TIMEOUT_FOREVER))
        res = USBH_LL->GetSpeed(pHost);
    Lock_MutexPost((struct MutexLCB*)USBH_LL->pMutex);
    return (res);
}

// ============================================================================
// ���ܣ� Reset the Host Port of the Low Level Driver.
// ������ pHost -- Host handle.
// ���أ�
// ��ע��
// ============================================================================
USBH_StatusTypeDef USBH_LL_ResetPort (USBH_HandleTypeDef *pHost)
{
    struct USBH_LowLevel *USBH_LL;
    USBH_StatusTypeDef res = USBH_FAIL;

    USBH_LL = __USBH_LL_GetLL(pHost);
    if((!USBH_LL) || (!USBH_LL->ResetPort))
        return USBH_NOT_SUPPORTED;

    if(Lock_MutexPend((struct MutexLCB*)USBH_LL->pMutex, CN_TIMEOUT_FOREVER))
        res = USBH_LL->ResetPort(pHost);
    Lock_MutexPost((struct MutexLCB*)USBH_LL->pMutex);
    return (res);

}

// ============================================================================
// ���ܣ� Return the last transferred packet size.
// ������ pHost -- Host handle.
// ���أ�
// ��ע��
// ============================================================================
uint32_t USBH_LL_GetLastXferSize (USBH_HandleTypeDef *pHost, uint8_t pipe)
{
    struct USBH_LowLevel *USBH_LL;
    USBH_StatusTypeDef res = USBH_FAIL;

    USBH_LL = __USBH_LL_GetLL(pHost);
    if((!USBH_LL) || (!USBH_LL->GetLastXferSize))
        return (-1);

    if(Lock_MutexPend((struct MutexLCB*)USBH_LL->pMutex, CN_TIMEOUT_FOREVER))
        res = USBH_LL->GetLastXferSize(pHost, pipe);
    Lock_MutexPost((struct MutexLCB*)USBH_LL->pMutex);
    return (res);
}

// ============================================================================
// ���ܣ�Open a pipe of the Low Level Driver.
// ������pHost -- Host handle
//      pipe_num -- Pipe index
//         epnum -- Endpoint Number
//         dev_address -- Device USB address
//         speed -- Device Speed
//         ep_type -- Endpoint Type
//         mps -- Endpoint Max Packet Size
// ���أ�
// ��ע��
// ============================================================================
USBH_StatusTypeDef USBH_LL_OpenPipe (USBH_HandleTypeDef *pHost,
                                     uint8_t pipe_num,
                                     uint8_t epnum,                                      
                                     uint8_t dev_address,
                                     uint8_t speed,
                                     uint8_t ep_type,
                                     uint16_t mps)
{
    struct USBH_LowLevel *USBH_LL;
    USBH_StatusTypeDef res = USBH_FAIL;

    USBH_LL = __USBH_LL_GetLL(pHost);
    if((!USBH_LL) || (!USBH_LL->OpenPipe))
        return (USBH_NOT_SUPPORTED);

    if(Lock_MutexPend((struct MutexLCB*)USBH_LL->pMutex, CN_TIMEOUT_FOREVER))
        res = USBH_LL->OpenPipe(pHost, pipe_num, epnum, dev_address,
                        speed, ep_type, mps);
    Lock_MutexPost((struct MutexLCB*)USBH_LL->pMutex);
    return (res);
}

// ============================================================================
// ���ܣ�Close a pipe of the Low Level Driver.
// ������pHost -- Host handle��pipe_num -- Pipe index��
// ���أ�
// ��ע��
// ============================================================================
USBH_StatusTypeDef USBH_LL_ClosePipe (USBH_HandleTypeDef *pHost, uint8_t pipe)
{
    struct USBH_LowLevel *USBH_LL;
    USBH_StatusTypeDef res = USBH_FAIL;

    USBH_LL = __USBH_LL_GetLL(pHost);
    if((!USBH_LL) || (!USBH_LL->ClosePipe))
        return (USBH_NOT_SUPPORTED);

    if(Lock_MutexPend((struct MutexLCB*)USBH_LL->pMutex, CN_TIMEOUT_FOREVER))
        res = USBH_LL->ClosePipe(pHost, pipe);
    Lock_MutexPost((struct MutexLCB*)USBH_LL->pMutex);
    return (res);
}

// ============================================================================
// ���ܣ�Submit a new URB to the low level driver.
// ������pHost -- Host handle
//        pipe -- Pipe index
//               This parameter can be a value from 1 to 15
//      direction -- Channel number
//                     This parameter can be one of the these values:
//                       0 : Output 
//                       1 : Input
//      ep_type -- Endpoint Type
//                    This parameter can be one of the these values:
//                  EP_TYPE_CTRL: Control type
//                  EP_TYPE_ISOC: Isochronous type
//                  EP_TYPE_BULK: Bulk type
//                  EP_TYPE_INTR: Interrupt type
//      token -- Endpoint Type
//               This parameter can be one of the these values:
//                0: PID_SETUP
//                1: PID_DATA
//      pbuff -- pointer to URB data
//      length -- Length of URB data
//      do_ping -- activate do PING protocol (for high speed only)
//                 This parameter can be one of the these values:
//                  0 : do PING inactive 
//                  1 : do PING active 
// ���أ�
// ��ע��
// ============================================================================
USBH_StatusTypeDef USBH_LL_SubmitURB (USBH_HandleTypeDef *pHost,
                                      uint8_t pipe, 
                                      uint8_t direction,
                                      uint8_t ep_type,  
                                      uint8_t token, 
                                      uint8_t* pbuff, 
                                      uint16_t length,
                                      uint8_t do_ping) 
{
    struct USBH_LowLevel *USBH_LL;
    USBH_StatusTypeDef res = USBH_FAIL;

    USBH_LL = __USBH_LL_GetLL(pHost);
    if((!USBH_LL) || (!USBH_LL->SubmitURB))
        return (USBH_NOT_SUPPORTED);


    if(Lock_MutexPend((struct MutexLCB*)USBH_LL->pMutex, CN_TIMEOUT_FOREVER))
        res = USBH_LL->SubmitURB(pHost, pipe, direction, ep_type,
                            token, pbuff, length, do_ping);
    Lock_MutexPost((struct MutexLCB*)USBH_LL->pMutex);

    return (res);
}

// ============================================================================
// ���ܣ�Get a URB state from the low level driver.
// ������pHost -- Host handle��pipe -- Pipe index��
// ���أ�URB state,This parameter can be one of the these values:
//          @arg URB_IDLE
//          @arg URB_DONE
//          @arg URB_NOTREADY
//          @arg URB_NYET
//          @arg URB_ERROR
//          @arg URB_STALL
// ��ע��
// ============================================================================
USBH_URBStateTypeDef  USBH_LL_GetURBState (USBH_HandleTypeDef *pHost, uint8_t pipe)
{
    struct USBH_LowLevel *USBH_LL;
    USBH_StatusTypeDef res = USBH_FAIL;

    USBH_LL = __USBH_LL_GetLL(pHost);
    if((!USBH_LL) || (!USBH_LL->GetURBState))
        return (USBH_NOT_SUPPORTED); // URB_NOTREADY;

    if(Lock_MutexPend((struct MutexLCB*)USBH_LL->pMutex, CN_TIMEOUT_FOREVER))
        res = USBH_LL->GetURBState(pHost, pipe);
    Lock_MutexPost((struct MutexLCB*)USBH_LL->pMutex);
    return (res);
}

// ============================================================================
// ���ܣ�Drive VBUS.
// ������pHost -- Host handle
//      state -- VBUS state
//               This parameter can be one of the these values:
//               0 : VBUS Active
//               1 : VBUS Inactive
// ���أ�
// ��ע��
// ============================================================================
USBH_StatusTypeDef  USBH_LL_DriverVBUS (USBH_HandleTypeDef *pHost, uint8_t state)
{
    struct USBH_LowLevel *USBH_LL;
    USBH_StatusTypeDef res = USBH_FAIL;

    USBH_LL = __USBH_LL_GetLL(pHost);
    if((!USBH_LL) || (!USBH_LL->DriverVBUS))
        return (USBH_NOT_SUPPORTED);

    if(Lock_MutexPend((struct MutexLCB*)USBH_LL->pMutex, CN_TIMEOUT_FOREVER))
        res = USBH_LL->DriverVBUS(pHost, state);
    Lock_MutexPost((struct MutexLCB*)USBH_LL->pMutex);
    return (res);
}

// ============================================================================
// ���ܣ�Set toggle for a pipe.
// ������pHost -- Host handle��pipe -- Pipe index��toggle -- toggle (0/1)
// ���أ�
// ��ע��
// ============================================================================
USBH_StatusTypeDef USBH_LL_SetToggle (USBH_HandleTypeDef *pHost, uint8_t pipe, uint8_t toggle)
{
    struct USBH_LowLevel *USBH_LL;
    USBH_StatusTypeDef res = USBH_FAIL;

    USBH_LL = __USBH_LL_GetLL(pHost);
    if((!USBH_LL) || (!USBH_LL->SetToggle))
        return (USBH_NOT_SUPPORTED);

    if(Lock_MutexPend((struct MutexLCB*)USBH_LL->pMutex, CN_TIMEOUT_FOREVER))
        res = USBH_LL->SetToggle(pHost, pipe, toggle);
    Lock_MutexPost((struct MutexLCB*)USBH_LL->pMutex);
    return (res);
}

// ============================================================================
// ���ܣ�Return the current toggle of a pipe.
// ������
// ���أ�toggle (0/1)
// ��ע��
// ============================================================================
uint8_t USBH_LL_GetToggle (USBH_HandleTypeDef *pHost, uint8_t pipe)
{
    struct USBH_LowLevel *USBH_LL;
    USBH_StatusTypeDef res = USBH_FAIL;

    USBH_LL = __USBH_LL_GetLL(pHost);
    if((!USBH_LL) || (!USBH_LL->GetToggle))
        return (USBH_NOT_SUPPORTED);

    if(Lock_MutexPend((struct MutexLCB*)USBH_LL->pMutex, CN_TIMEOUT_FOREVER))
        res = USBH_LL->GetToggle(pHost, pipe);
    Lock_MutexPost((struct MutexLCB*)USBH_LL->pMutex);
    return (res);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
void  USBH_Delay (uint32_t Delay)
{

}

// ============================================================================
// ���ܣ� �����ܵ����첽���幦��
// ������
// ���أ�
// ��ע��
// ============================================================================
USBH_StatusTypeDef USBH_LL_EnAsync (USBH_HandleTypeDef *pHost, uint8_t pipe)
{
    struct USBH_LowLevel *USBH_LL;
    USBH_StatusTypeDef res = USBH_FAIL;

    USBH_LL = __USBH_LL_GetLL(pHost);
    if((!USBH_LL) || (!USBH_LL->EnAsync))
        return (USBH_NOT_SUPPORTED);

    if(Lock_MutexPend((struct MutexLCB*)USBH_LL->pMutex, CN_TIMEOUT_FOREVER))
        res = USBH_LL->EnAsync(pHost, pipe);
    Lock_MutexPost((struct MutexLCB*)USBH_LL->pMutex);
    return (res);
}

// ============================================================================
// ���ܣ� �ر�pipe���첽���幦��
// ������
// ���أ�
// ��ע��
// ============================================================================
USBH_StatusTypeDef USBH_LL_DisAsync (USBH_HandleTypeDef *pHost, uint8_t pipe)
{
    struct USBH_LowLevel *USBH_LL;
    USBH_StatusTypeDef res = USBH_FAIL;

    USBH_LL = __USBH_LL_GetLL(pHost);
    if((!USBH_LL) || (!USBH_LL->DisAsync))
        return (USBH_NOT_SUPPORTED);

    if(Lock_MutexPend((struct MutexLCB*)USBH_LL->pMutex, CN_TIMEOUT_FOREVER))
        res = USBH_LL->DisAsync(pHost, pipe);
    Lock_MutexPost((struct MutexLCB*)USBH_LL->pMutex);
    return (res);
}

// ============================================================================
// ���ܣ�����USB
// ������
// ���أ�
// ��ע��
// ============================================================================
USBH_StatusTypeDef USBH_LL_Suspend (USBH_HandleTypeDef *pHost)
{
    struct USBH_LowLevel *USBH_LL;
    USBH_StatusTypeDef res = USBH_FAIL;

    USBH_LL = __USBH_LL_GetLL(pHost);
    if((!USBH_LL) || (!USBH_LL->Suspend))
        return (USBH_NOT_SUPPORTED);

    if(Lock_MutexPend((struct MutexLCB*)USBH_LL->pMutex, CN_TIMEOUT_FOREVER))
        res = USBH_LL->Suspend(pHost);
    Lock_MutexPost((struct MutexLCB*)USBH_LL->pMutex);
    return (res);
}

// ============================================================================
// ���ܣ��ָ�USB
// ������
// ���أ�
// ��ע��
// ============================================================================
USBH_StatusTypeDef USBH_LL_Resume (USBH_HandleTypeDef *pHost)
{
    struct USBH_LowLevel *USBH_LL;
    USBH_StatusTypeDef res = USBH_FAIL;

    USBH_LL = __USBH_LL_GetLL(pHost);
    if((!USBH_LL) || (!USBH_LL->Resume))
        return (USBH_NOT_SUPPORTED);

    if(Lock_MutexPend((struct MutexLCB*)USBH_LL->pMutex, CN_TIMEOUT_FOREVER))
        res = USBH_LL->Resume(pHost);
    Lock_MutexPost((struct MutexLCB*)USBH_LL->pMutex);
    return (res);
}


// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��TODO:���Ϊ��̬
// ============================================================================
struct USBH_LowLlevel *USBH_LL_GetLL(u8 USBH_Type)
{
    if(HIGH_SPEED_HOST == USBH_Type)
        return ((struct USBH_LowLlevel *)&USBH_HS);

    if(FULL_SPEED_HOST == USBH_Type)
        return ((struct USBH_LowLlevel *)&USBH_FS);

    return (NULL);
}

// ============================================================================
// ���ܣ�
// ������
// ����:
// ��ע��
// ============================================================================
static struct USBH_LowLevel *__USBH_LL_GetLL(USBH_HandleTypeDef *pHost)
{
    return ((struct USBH_LowLevel *)USBH_LL_GetLL((u8)pHost->id));
}


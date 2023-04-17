/**
  ******************************************************************************
  * @file    USB_Host/MSC_Standalone/Src/usbh_conf.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    22-April-2016
  * @brief   USB Host configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
//#include "../core/inc/usbh_core.h"
#include "cpu_peri_int_line.h"
#include <int.h>
#include <usbh_lowlevel.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32 ModuleInstall_USB(const char *TargetFs,u8 controller);
//    ModuleInstall_USB(CFG_USB_UDISK_FS,CFG_USB_CONTROLLER);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip usb"//CPU��USB��������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"int","time","lock","heap","stm32usb"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_USB == false )
//#warning  " cpu_onchip_usb  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header          //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_USB    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,2,
#define CFG_USB_CONTROLLER           0      //"usb�Ĺ���
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

HCD_HandleTypeDef HHCD_FS, HHCD_HS;


u32 OTG_HS_IRQHandler(u32 dwData);
u32 OTG_FS_IRQHandler(u32 dwData);
/*******************************************************************************
                       HCD BSP Routines
*******************************************************************************/
/**
  * @brief  Initializes the HCD MSP.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_MspInit(HCD_HandleTypeDef *hhcd)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    if(hhcd->Instance == USB_OTG_FS)
    {
        /* Configure USB FS GPIOs */
        __HAL_RCC_GPIOA_CLK_ENABLE();
        #if 0
        __HAL_RCC_GPIOG_CLK_ENABLE();
        #endif

        /*USB DM and DP */
        GPIO_InitStruct.Pin = (GPIO_PIN_11 | GPIO_PIN_12); // PA11=DM,PA12=DP
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        #if 0
        /*USB ID */
        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* Configure POWER_SWITCH IO pin */
        GPIO_InitStruct.Pin = GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
        #endif
        /* Enable USB FS Clocks */
        __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

        // �ж�
        Int_Register(CN_INT_LINE_OTG_FS);
        Int_SetClearType(CN_INT_LINE_OTG_FS, CN_INT_CLEAR_AUTO);
        Int_IsrConnect(CN_INT_LINE_OTG_FS, OTG_FS_IRQHandler);
        Int_SettoAsynSignal(CN_INT_LINE_OTG_FS);
        Int_ClearLine(CN_INT_LINE_OTG_FS);
        Int_RestoreAsynLine(CN_INT_LINE_OTG_FS);
    }

    if(hhcd->Instance == USB_OTG_HS) // HS����ַ
    {
        __HAL_RCC_GPIOH_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        /* Configure Over Current IO pin */
        GPIO_InitStruct.Pin = GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

        /* Configure POWER_SWITCH IO pin */
        GPIO_InitStruct.Pin = GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

        /* Configure GPIO for HS in FS mode */
        GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14 |GPIO_PIN_15; // PB12=ID δ��,PB13=VBUS_FS2(TODO),PB14=DM,PB15=DP
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF12_OTG_HS_FS;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#if 0
        USBH_LL_DriverVBUS(hhcd, 0);
        // ȷ��USB�豸��ȫ����
        for(;;)
        {
            if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13)) // TODO
                DJY_EventDelay(100);
            else
                break;
        }
#endif
        /* Enable USB HS Clocks */
        __HAL_RCC_USB_OTG_HS_CLK_ENABLE();


        // �ж�
        Int_Register(CN_INT_LINE_OTG_HS);
        Int_SetClearType(CN_INT_LINE_OTG_HS, CN_INT_CLEAR_AUTO);
        Int_IsrConnect(CN_INT_LINE_OTG_HS, OTG_HS_IRQHandler);
        Int_SettoAsynSignal(CN_INT_LINE_OTG_HS);
        Int_ClearLine(CN_INT_LINE_OTG_HS);
        Int_RestoreAsynLine(CN_INT_LINE_OTG_HS);
    }

#if 0
  /* Set USBFS Interrupt priority */
  HAL_NVIC_SetPriority(OTG_FS_IRQn, 6, 0);

  /* Enable USBFS Interrupt */
  HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
#endif
}

/**
  * @brief  DeInitializes the HCD MSP.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_MspDeInit(HCD_HandleTypeDef *hhcd)
{

    if(hhcd->Instance == USB_OTG_FS)
    {
        /* Disable USB FS Clocks */
        __HAL_RCC_USB_OTG_FS_CLK_DISABLE();
    }

    if(hhcd->Instance == USB_OTG_HS)
    {
        /* Disable USB HS Clocks */
        __HAL_RCC_USB_OTG_HS_CLK_DISABLE();
        __HAL_RCC_USB_OTG_HS_ULPI_CLK_DISABLE();
    }
}

/*******************************************************************************
                       LL Driver Callbacks (HCD -> USB Host Library)
*******************************************************************************/

/**
  * @brief  SOF callback.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_SOF_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_IncTimer (hhcd->pData); // usb_core.c
}

/**
  * @brief  Connect callback.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_Connect_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_Connect (hhcd->pData); // �жϼ�⵽����������
}

/**
  * @brief  Disconnect callback.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef *hhcd)
{
  USBH_LL_Disconnect (hhcd->pData); // �жϼ�⵽�Ͽ�����
}


/**
  * @brief  Notify URB state change callback.
  * @param  hhcd: HCD handle
  * @param  chnum: Channel number
  * @param  urb_state: URB State
  * @retval None
  */
void __HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef *hhcd, uint8_t chnum, HCD_URBStateTypeDef urb_state)
{
  /* To be used with OS to sync URB state with the global state machine */
  // URB״̬�����仯��Ļص�����, Not working
}

/*******************************************************************************
                       LL Driver Interface (USB Host Library --> HCD)
*******************************************************************************/
/**
  * @brief  USBH_LL_FS_Init
  *         Initialize the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef __USBH_LL_FS_Init(USBH_HandleTypeDef *phost)
{
    /* Set the LL Driver parameters */
    HHCD_FS.Instance = USB_OTG_FS; // FS����ַ
    HHCD_FS.Init.Host_channels = 8;
    HHCD_FS.Init.dma_enable = 0; // DMAδʹ��
    HHCD_FS.Init.low_power_enable = 0;
    HHCD_FS.Init.phy_itface = HCD_PHY_EMBEDDED; // �ӿ�����
    HHCD_FS.Init.Sof_enable = 0;
    HHCD_FS.Init.speed = HCD_SPEED_FULL;
    HHCD_FS.Init.vbus_sensing_enable = 0;
    HHCD_FS.Init.lpm_enable = 0;

    /* Link the driver to the stack */
    HHCD_FS.pData = phost;
    phost->pData = &HHCD_FS;

    /* Initialize the LL Driver */
    HAL_HCD_Init(&HHCD_FS); //

    USBH_LL_SetTimer (phost, HAL_HCD_GetCurrentFrame(&HHCD_FS));

    return USBH_OK;
}

/**
  * @brief  USBH_LL_HS_Init
  *         Initialize the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef __USBH_LL_HS_Init(USBH_HandleTypeDef *phost)
{
    /* Set the LL driver parameters */
    HHCD_HS.Instance = USB_OTG_HS; // HS����ַ
    HHCD_HS.Init.Host_channels = 11;
    HHCD_HS.Init.dma_enable = 0;
    HHCD_HS.Init.low_power_enable = 0;
    HHCD_HS.Init.phy_itface = HCD_PHY_EMBEDDED;
    HHCD_HS.Init.Sof_enable = 0;
    HHCD_HS.Init.speed = HCD_SPEED_HIGH;
    HHCD_HS.Init.vbus_sensing_enable = 0;
    HHCD_HS.Init.use_external_vbus = 1;
    HHCD_HS.Init.lpm_enable = 0;
    /* Link the driver to the stack */
    HHCD_HS.pData = phost;
    phost->pData = &HHCD_HS;

    /* Initialize the LL driver */
    HAL_HCD_Init(&HHCD_HS);

    USBH_LL_SetTimer(phost, HAL_HCD_GetCurrentFrame(&HHCD_HS));

    return USBH_OK;
}

/**
  * @brief  De-Initializes the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef __USBH_LL_DeInit(USBH_HandleTypeDef *phost)
{
  HAL_HCD_DeInit(phost->pData);
  return USBH_OK;
}

/**
  * @brief  Starts the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef __USBH_LL_Start(USBH_HandleTypeDef *phost)
{
  HAL_HCD_Start(phost->pData); // ���жϣ�VBUS�ϵ�
  return USBH_OK;
}

/**
  * @brief  Stops the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef __USBH_LL_Stop(USBH_HandleTypeDef *phost)
{
  HAL_HCD_Stop(phost->pData); // ˢ��FIFO����ͨ�������ж�
  return USBH_OK;
}

/**
  * @brief  Returns the USB Host Speed from the Low Level Driver.
  * @param  phost: Host handle
  * @retval USBH Speeds
  */
static USBH_SpeedTypeDef __USBH_LL_GetSpeed(USBH_HandleTypeDef *phost)
{
  USBH_SpeedTypeDef speed = USBH_SPEED_FULL;

  switch (HAL_HCD_GetCurrentSpeed(phost->pData))
  {
  case 0:
    speed = USBH_SPEED_HIGH;
    break;

  case 1:
    speed = USBH_SPEED_FULL;
    break;

  case 2:
    speed = USBH_SPEED_LOW;
    break;

  default:
    speed = USBH_SPEED_FULL;
    break;
  }
  return speed;
}

/**
  * @brief  Resets the Host Port of the Low Level Driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef __USBH_LL_ResetPort (USBH_HandleTypeDef *phost)
{
  HAL_HCD_ResetPort(phost->pData); // �˿ڸ�λ
  return USBH_OK;
}

/**
  * @brief  Returns the last transferred packet size.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @retval Packet Size
  */
static uint32_t __USBH_LL_GetLastXferSize(USBH_HandleTypeDef *phost, uint8_t pipe)
{
  return HAL_HCD_HC_GetXferCount(phost->pData, pipe);
}

/**
  * @brief  Opens a pipe of the Low Level Driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @param  epnum: Endpoint Number
  * @param  dev_address: Device USB address
  * @param  speed: Device Speed
  * @param  ep_type: Endpoint Type
  * @param  mps: Endpoint Max Packet Size
  * @retval USBH Status
  */
static USBH_StatusTypeDef __USBH_LL_OpenPipe(USBH_HandleTypeDef *phost,
                                    uint8_t pipe,
                                    uint8_t epnum,
                                    uint8_t dev_address,
                                    uint8_t speed,
                                    uint8_t ep_type,
                                    uint16_t mps)
{
  HAL_HCD_HC_Init(phost->pData,
                  pipe,
                  epnum,
                  dev_address,
                  speed,
                  ep_type,
                  mps);
  return USBH_OK;
}

/**
  * @brief  Closes a pipe of the Low Level Driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @retval USBH Status
  */
static USBH_StatusTypeDef __USBH_LL_ClosePipe(USBH_HandleTypeDef *phost, uint8_t pipe)
{
  HAL_HCD_HC_Halt(phost->pData, pipe);
  return USBH_OK;
}

/**
  * @brief  Submits a new URB to the low level driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  *          This parameter can be a value from 1 to 15
  * @param  direction: Channel number
  *          This parameter can be one of these values:
  *           0: Output
  *           1: Input
  * @param  ep_type: Endpoint Type
  *          This parameter can be one of these values:
  *            @arg EP_TYPE_CTRL: Control type
  *            @arg EP_TYPE_ISOC: Isochronous type
  *            @arg EP_TYPE_BULK: Bulk type
  *            @arg EP_TYPE_INTR: Interrupt type
  * @param  token: Endpoint Type
  *          This parameter can be one of these values:
  *            @arg 0: PID_SETUP
  *            @arg 1: PID_DATA
  * @param  pbuff: pointer to URB data
  * @param  length: length of URB data
  * @param  do_ping: activate do PING protocol (for high speed only)
  *          This parameter can be one of these values:
  *           0: do PING inactive
  *           1: do PING active
  * @retval USBH Status
  */
static USBH_StatusTypeDef __USBH_LL_SubmitURB(USBH_HandleTypeDef *phost,
                                     uint8_t pipe,
                                     uint8_t direction,
                                     uint8_t ep_type,
                                     uint8_t token,
                                     uint8_t* pbuff,
                                     uint16_t length,
                                     uint8_t do_ping)
{
  HAL_HCD_HC_SubmitRequest(phost->pData,
                           pipe,
                           direction,
                           ep_type,
                           token,
                           pbuff,
                           length,
                           do_ping);
  return USBH_OK;
}

/**
  * @brief  Gets a URB state from the low level driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  *          This parameter can be a value from 1 to 15
  * @retval URB state
  *          This parameter can be one of these values:
  *            @arg URB_IDLE
  *            @arg URB_DONE
  *            @arg URB_NOTREADY
  *            @arg URB_NYET
  *            @arg URB_ERROR
  *            @arg URB_STALL
  */
static USBH_URBStateTypeDef __USBH_LL_GetURBState(USBH_HandleTypeDef *phost, uint8_t pipe)
{
  return (USBH_URBStateTypeDef)HAL_HCD_HC_GetURBState (phost->pData, pipe);
}

/**
  * @brief  Drives VBUS.
  * @param  phost: Host handle
  * @param  state: VBUS state
  *          This parameter can be one of these values:
  *           0: VBUS Active
  *           1: VBUS Inactive
  * @retval USBH Status
  */
static USBH_StatusTypeDef __USBH_LL_FS_DriverVBUS(USBH_HandleTypeDef *phost, uint8_t state)
{
  return USBH_OK;
}

static USBH_StatusTypeDef __USBH_LL_HS_DriverVBUS(USBH_HandleTypeDef *phost, uint8_t state)
{
  if(state == 0)
  {
    /* Configure Low Charge pump */
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_3, GPIO_PIN_SET);
  }
  else
  {
    /* Drive High Charge pump */
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_3, GPIO_PIN_RESET);
  }
  HAL_Delay(200);
  return USBH_OK;
}
/**
  * @brief  Sets toggle for a pipe.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @param  toggle: toggle (0/1)
  * @retval USBH Status
  */
static USBH_StatusTypeDef __USBH_LL_SetToggle(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t toggle)
{
    HCD_HandleTypeDef *hhcd = phost->pData;

    if(hhcd->hc[pipe].ep_is_in)
    {
        hhcd->hc[pipe].toggle_in = toggle;
    }
    else
    {
        hhcd->hc[pipe].toggle_out = toggle;
    }
    return USBH_OK;
}

/**
  * @brief  Returns the current toggle of a pipe.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @retval toggle (0/1)
  */
static uint8_t __USBH_LL_GetToggle(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    uint8_t toggle = 0;
    HCD_HandleTypeDef *hhcd = phost->pData;

    if(hhcd->hc[pipe].ep_is_in)
    {
        toggle = hhcd->hc[pipe].toggle_in;
    }
    else
    {
        toggle = hhcd->hc[pipe].toggle_out;
    }
    return toggle;
}

#if 0
/**
  * @brief  Delay routine for the USB Host Library
  * @param  Delay: Delay in ms
  * @retval None
  */
void __USBH_Delay(uint32_t Delay)
{
#if (USBH_USE_OS == 1)
  osDelay(Delay);
#else
  // HAL_Delay(Delay);
  DJY_EventDelay(Delay);
#endif
}
#endif

/**
  * @brief
  * @param
  * @retval
  */
USBH_StatusTypeDef __USBH_LL_EnAsync(USBH_HandleTypeDef *phost, uint8_t pipe)
{

    HCD_HandleTypeDef *hhcd = phost->pData;

    hhcd->hc[pipe].async = 1;

    return (USBH_OK);
}



/**
  * @brief
  * @param
  * @retval
  */
USBH_StatusTypeDef __USBH_LL_DisAsync(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    HCD_HandleTypeDef *hhcd = phost->pData;

    hhcd->hc[pipe].async = 0;

    return (USBH_OK);
}
/**
  * @brief  This function handles USB-On-The-Go FS global interrupt request.
  * @param  None
  * @retval None
  */
u32 OTG_FS_IRQHandler(u32 dwData)
{
    HAL_HCD_IRQHandler(&HHCD_FS);
    return (0);
}

/**
  * @brief  This function handles USB-On-The-Go HS global interrupt request.
  * @param  None
  * @retval None
  */
u32 OTG_HS_IRQHandler(u32 dwData)
{
    HAL_HCD_IRQHandler(&HHCD_HS);
    return (0);
}

//-----------------------------------------------------------------------------
//����: USB�ײ�����ע��
//����: bController - HIGH_SPEED_HOST��HS�������� FULL_SPEED_HOST��FS��������
//����: �ޣ�
//��ע:
//-----------------------------------------------------------------------------
void USBH_LL_Register(u8 bController)
{
    struct USBH_LowLevel *ll;
    extern struct USBH_LowLlevel *USBH_LL_GetLL(u8 USBH_Type);

    ll = USBH_LL_GetLL(bController);
    if(bController == HIGH_SPEED_HOST)
    {
        ll->Init = __USBH_LL_HS_Init;
        ll->DriverVBUS = __USBH_LL_HS_DriverVBUS;
    }
    else if(bController == FULL_SPEED_HOST)
    {
        ll->Init = __USBH_LL_FS_Init;
        ll->DriverVBUS = __USBH_LL_FS_DriverVBUS;
    }
    else
        return;

    ll->DeInit = __USBH_LL_DeInit;
    ll->Start = __USBH_LL_Start;
    ll->Stop = __USBH_LL_Stop;
    ll->GetSpeed = __USBH_LL_GetSpeed;
    ll->ResetPort = __USBH_LL_ResetPort;
    ll->GetLastXferSize = __USBH_LL_GetLastXferSize;
    ll->OpenPipe = __USBH_LL_OpenPipe;
    ll->ClosePipe = __USBH_LL_ClosePipe;
    ll->SubmitURB = __USBH_LL_SubmitURB;
    ll->GetURBState = __USBH_LL_GetURBState;
    ll->SetToggle = __USBH_LL_SetToggle;
    ll->GetToggle =  __USBH_LL_GetToggle;
    ll->EnAsync = __USBH_LL_EnAsync;
    ll->DisAsync = __USBH_LL_DisAsync;

    ll->bID = bController;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

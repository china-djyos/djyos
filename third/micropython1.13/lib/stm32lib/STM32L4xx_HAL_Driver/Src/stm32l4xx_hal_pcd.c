/**
  ******************************************************************************
  * @file    stm32l4xx_hal_pcd.c
  * @author  MCD Application Team
  * @version V1.7.2
  * @date    16-June-2017
  * @brief   PCD HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the USB Peripheral Controller:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral Control functions
  *           + Peripheral State functions
  *
  @verbatim
  ==============================================================================
                    ##### How to use this driver #####
  ==============================================================================
    [..]
      The PCD HAL driver can be used as follows:

     (#) Declare a PCD_HandleTypeDef handle structure, for example:
         PCD_HandleTypeDef  hpcd;

     (#) Fill parameters of Init structure in HCD handle

     (#) Call HAL_PCD_Init() API to initialize the PCD peripheral (Core, Device core, ...)

     (#) Initialize the PCD low level resources through the HAL_PCD_MspInit() API:
         (##) Enable the PCD/USB Low Level interface clock using
              (+++) __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
         (##) Initialize the related GPIO clocks
         (##) Configure PCD pin-out
         (##) Configure PCD NVIC interrupt

     (#)Associate the Upper USB device stack to the HAL PCD Driver:
         (##) hpcd.pData = pdev;

     (#)Enable PCD transmission and reception:
         (##) HAL_PCD_Start();

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/** @addtogroup STM32L4xx_HAL_Driver
  * @{
  */

/** @defgroup PCD PCD
  * @brief PCD HAL module driver
  * @{
  */

#ifdef HAL_PCD_MODULE_ENABLED

#if defined(STM32L432xx) || defined(STM32L433xx) || defined(STM32L442xx) || defined(STM32L443xx) || \
    defined(STM32L452xx) || defined(STM32L462xx) || \
    defined(STM32L475xx) || defined(STM32L476xx) || defined(STM32L485xx) || defined(STM32L486xx) || \
    defined(STM32L496xx) || defined(STM32L4A6xx)

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/**
  * USB_OTG_CORE VERSION ID
  */
#define USB_OTG_CORE_ID_310A          0x4F54310A
#define USB_OTG_CORE_ID_320A          0x4F54320A

/* Private macros ------------------------------------------------------------*/
/** @defgroup PCD_Private_Macros PCD Private Macros
  * @{
  */
#define PCD_MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define PCD_MAX(a, b)  (((a) > (b)) ? (a) : (b))
/**
  * @}
  */

/* Private functions prototypes ----------------------------------------------*/
/** @defgroup PCD_Private_Functions PCD Private Functions
  * @{
  */
#if defined (USB_OTG_FS)
static HAL_StatusTypeDef PCD_WriteEmptyTxFifo(PCD_HandleTypeDef *hpcd, uint32_t epnum);
#endif /* USB_OTG_FS */
#if defined (USB)
static HAL_StatusTypeDef PCD_EP_ISR_Handler(PCD_HandleTypeDef *hpcd);
#endif /* USB */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup PCD_Exported_Functions PCD Exported Functions
  * @{
  */

/** @defgroup PCD_Exported_Functions_Group1 Initialization and de-initialization functions
 *  @brief    Initialization and Configuration functions
 *
@verbatim
 ===============================================================================
            ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the PCD according to the specified
  *         parameters in the PCD_InitTypeDef and initialize the associated handle.
  * @param  hpcd: PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_Init(PCD_HandleTypeDef *hpcd)
{
  uint32_t index = 0U;

  /* Check the PCD handle allocation */
  if(hpcd == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_PCD_ALL_INSTANCE(hpcd->Instance));

  if(hpcd->State == HAL_PCD_STATE_RESET)
  {
    /* Allocate lock resource and initialize it */
    hpcd->Lock = HAL_UNLOCKED;

    /* Init the low level hardware : GPIO, CLOCK, NVIC... */
    HAL_PCD_MspInit(hpcd);
  }

  hpcd->State = HAL_PCD_STATE_BUSY;

  /* Disable the Interrupts */
  __HAL_PCD_DISABLE(hpcd);

  /*Init the Core (common init.) */
  USB_CoreInit(hpcd->Instance, hpcd->Init);

  /* Force Device Mode*/
  USB_SetCurrentMode(hpcd->Instance , USB_DEVICE_MODE);

  /* Init endpoints structures */
  for (index = 0; index < hpcd->Init.dev_endpoints ; index++)
  {
    /* Init ep structure */
    hpcd->IN_ep[index].is_in = 1;
    hpcd->IN_ep[index].num = index;
    hpcd->IN_ep[index].tx_fifo_num = index;
    /* Control until ep is activated */
    hpcd->IN_ep[index].type = EP_TYPE_CTRL;
    hpcd->IN_ep[index].maxpacket =  0;
    hpcd->IN_ep[index].xfer_buff = 0;
    hpcd->IN_ep[index].xfer_len = 0;
  }

  for (index = 0; index < 15 ; index++)
  {
    hpcd->OUT_ep[index].is_in = 0;
    hpcd->OUT_ep[index].num = index;
    hpcd->IN_ep[index].tx_fifo_num = index;
    /* Control until ep is activated */
    hpcd->OUT_ep[index].type = EP_TYPE_CTRL;
    hpcd->OUT_ep[index].maxpacket = 0;
    hpcd->OUT_ep[index].xfer_buff = 0;
    hpcd->OUT_ep[index].xfer_len = 0;
  }

  /* Init Device */
  USB_DevInit(hpcd->Instance, hpcd->Init);

  hpcd->USB_Address = 0;

  hpcd->State= HAL_PCD_STATE_READY;

   /* Activate LPM */
  if (hpcd->Init.lpm_enable ==1)
  {
    HAL_PCDEx_ActivateLPM(hpcd);
  }
  /* Activate Battery charging */
  if (hpcd->Init.battery_charging_enable ==1)
  {
    HAL_PCDEx_ActivateBCD(hpcd);
  }
  USB_DevDisconnect (hpcd->Instance);
  return HAL_OK;
}

/**
  * @brief  DeInitializes the PCD peripheral.
  * @param  hpcd: PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_DeInit(PCD_HandleTypeDef *hpcd)
{
  /* Check the PCD handle allocation */
  if(hpcd == NULL)
  {
    return HAL_ERROR;
  }

  hpcd->State = HAL_PCD_STATE_BUSY;

  /* Stop Device */
  HAL_PCD_Stop(hpcd);

  /* DeInit the low level hardware */
  HAL_PCD_MspDeInit(hpcd);

  hpcd->State = HAL_PCD_STATE_RESET;

  return HAL_OK;
}

/**
  * @brief  Initializes the PCD MSP.
  * @param  hpcd: PCD handle
  * @retval None
  */
__weak void HAL_PCD_MspInit(PCD_HandleTypeDef *hpcd)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_MspInit could be implemented in the user file
   */
}

/**
  * @brief  DeInitializes PCD MSP.
  * @param  hpcd: PCD handle
  * @retval None
  */
__weak void HAL_PCD_MspDeInit(PCD_HandleTypeDef *hpcd)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_MspDeInit could be implemented in the user file
   */
}

/**
  * @}
  */

/** @defgroup PCD_Exported_Functions_Group2 Input and Output operation functions
 *  @brief   Data transfers functions
 *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to manage the PCD data
    transfers.

@endverbatim
  * @{
  */

/**
  * @brief  Start The USB OTG Device.
  * @param  hpcd: PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_Start(PCD_HandleTypeDef *hpcd)
{
  __HAL_LOCK(hpcd);
  USB_DevConnect (hpcd->Instance);
  __HAL_PCD_ENABLE(hpcd);
  __HAL_UNLOCK(hpcd);
  return HAL_OK;
}

/**
  * @brief  Stop The USB OTG Device.
  * @param  hpcd: PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_Stop(PCD_HandleTypeDef *hpcd)
{
  __HAL_LOCK(hpcd);
  __HAL_PCD_DISABLE(hpcd);
  USB_StopDevice(hpcd->Instance);
  USB_DevDisconnect (hpcd->Instance);
  __HAL_UNLOCK(hpcd);
  return HAL_OK;
}
#if defined (USB_OTG_FS)
/**
  * @brief  Handles PCD interrupt request.
  * @param  hpcd: PCD handle
  * @retval HAL status
  */
void HAL_PCD_IRQHandler(PCD_HandleTypeDef *hpcd)
{
  USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;
  uint32_t index = 0U, ep_intr = 0U, epint = 0U, epnum = 0U;
  uint32_t fifoemptymsk = 0U, temp = 0U;
  USB_OTG_EPTypeDef *ep = NULL;
  uint32_t hclk = 80000000;

  /* ensure that we are in device mode */
  if (USB_GetMode(hpcd->Instance) == USB_OTG_MODE_DEVICE)
  {
    /* avoid spurious interrupt */
    if(__HAL_PCD_IS_INVALID_INTERRUPT(hpcd))
    {
      return;
    }

    if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_MMIS))
    {
      /* incorrect mode, acknowledge the interrupt */
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_MMIS);
    }

    if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_OEPINT))
    {
      epnum = 0;

      /* Read in the device interrupt bits */
      ep_intr = USB_ReadDevAllOutEpInterrupt(hpcd->Instance);

      while (ep_intr)
      {
        if (ep_intr & 0x1)
        {
          epint = USB_ReadDevOutEPInterrupt(hpcd->Instance, epnum);

          if (( epint & USB_OTG_DOEPINT_XFRC) == USB_OTG_DOEPINT_XFRC)
          {
            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_XFRC);

            /* setup/out transaction management for Core ID 310A */
            if (USBx->GSNPSID == USB_OTG_CORE_ID_310A)
            {
              if (!(USBx_OUTEP(0)->DOEPINT & (0x1 << 15)))
                {
                  if (hpcd->Init.dma_enable == 1)
                  {
                    hpcd->OUT_ep[epnum].xfer_count =
                                                 hpcd->OUT_ep[epnum].maxpacket -
                        (USBx_OUTEP(epnum)->DOEPTSIZ & USB_OTG_DOEPTSIZ_XFRSIZ);

                    hpcd->OUT_ep[epnum].xfer_buff +=
                                                  hpcd->OUT_ep[epnum].maxpacket;
                  }

                HAL_PCD_DataOutStageCallback(hpcd, epnum);

                if (hpcd->Init.dma_enable == 1)
                {
                  if (!epnum && !hpcd->OUT_ep[epnum].xfer_len)
                  {
                    /* this is ZLP, so prepare EP0 for next setup */
                    USB_EP0_OutStart(hpcd->Instance, 1, (uint8_t *)hpcd->Setup);
                  }
                }
              }

              /* Clear the SetPktRcvd flag*/
              USBx_OUTEP(0)->DOEPINT |= (0x1 << 15) | (0x1 << 5);
            }
            else
            {
              if (hpcd->Init.dma_enable == 1)
              {
                hpcd->OUT_ep[epnum].xfer_count =
                                               hpcd->OUT_ep[epnum].maxpacket -
                      (USBx_OUTEP(epnum)->DOEPTSIZ & USB_OTG_DOEPTSIZ_XFRSIZ);
                hpcd->OUT_ep[epnum].xfer_buff += hpcd->OUT_ep[epnum].maxpacket;
              }

              HAL_PCD_DataOutStageCallback(hpcd, epnum);

              if (hpcd->Init.dma_enable == 1)
              {
                if (!epnum && !hpcd->OUT_ep[epnum].xfer_len)
                {
                  /* this is ZLP, so prepare EP0 for next setup */
                  USB_EP0_OutStart(hpcd->Instance, 1, (uint8_t *)hpcd->Setup);
                }
              }
            }
          }

          if(( epint & USB_OTG_DOEPINT_STUP) == USB_OTG_DOEPINT_STUP)
          {
            /* Inform the upper layer that a setup packet is available */
            HAL_PCD_SetupStageCallback(hpcd);
            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_STUP);
          }

          if(( epint & USB_OTG_DOEPINT_OTEPDIS) == USB_OTG_DOEPINT_OTEPDIS)
          {
            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_OTEPDIS);
          }

#ifdef USB_OTG_DOEPINT_OTEPSPR
          /* Clear Status Phase Received interrupt */
          if(( epint & USB_OTG_DOEPINT_OTEPSPR) == USB_OTG_DOEPINT_OTEPSPR)
          {
            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_OTEPSPR);
          }
#endif /* USB_OTG_DOEPINT_OTEPSPR */
        }
        epnum++;
        ep_intr >>= 1;
      }
    }

    if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_IEPINT))
    {
      /* Read in the device interrupt bits */
      ep_intr = USB_ReadDevAllInEpInterrupt(hpcd->Instance);

      epnum = 0;

      while ( ep_intr )
      {
        if (ep_intr & 0x1) /* In ITR */
        {
          epint = USB_ReadDevInEPInterrupt(hpcd->Instance, epnum);

          if(( epint & USB_OTG_DIEPINT_XFRC) == USB_OTG_DIEPINT_XFRC)
          {
            fifoemptymsk = 0x1 << epnum;
            USBx_DEVICE->DIEPEMPMSK &= ~fifoemptymsk;

            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_XFRC);

            if (hpcd->Init.dma_enable == 1)
            {
              hpcd->IN_ep[epnum].xfer_buff += hpcd->IN_ep[epnum].maxpacket;
            }

            HAL_PCD_DataInStageCallback(hpcd, epnum);

            if (hpcd->Init.dma_enable == 1)
            {
              /* this is ZLP, so prepare EP0 for next setup */
              if((epnum == 0) && (hpcd->IN_ep[epnum].xfer_len == 0))
              {
                /* prepare to rx more setup packets */
                USB_EP0_OutStart(hpcd->Instance, 1, (uint8_t *)hpcd->Setup);
              }
            }
          }
          if(( epint & USB_OTG_DIEPINT_TOC) == USB_OTG_DIEPINT_TOC)
          {
            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_TOC);
          }
          if(( epint & USB_OTG_DIEPINT_ITTXFE) == USB_OTG_DIEPINT_ITTXFE)
          {
            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_ITTXFE);
          }
          if(( epint & USB_OTG_DIEPINT_INEPNE) == USB_OTG_DIEPINT_INEPNE)
          {
            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_INEPNE);
          }
          if(( epint & USB_OTG_DIEPINT_EPDISD) == USB_OTG_DIEPINT_EPDISD)
          {
            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_EPDISD);
          }
          if(( epint & USB_OTG_DIEPINT_TXFE) == USB_OTG_DIEPINT_TXFE)
          {
            PCD_WriteEmptyTxFifo(hpcd , epnum);
          }
        }
        epnum++;
        ep_intr >>= 1;
      }
    }

    /* Handle Resume Interrupt */
    if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_WKUINT))
    {
      /* Clear the Remote Wake-up Signaling */
      USBx_DEVICE->DCTL &= ~USB_OTG_DCTL_RWUSIG;

      if(hpcd->LPM_State == LPM_L1)
      {
        hpcd->LPM_State = LPM_L0;
        HAL_PCDEx_LPM_Callback(hpcd, PCD_LPM_L0_ACTIVE);
      }
      else
      {
        HAL_PCD_ResumeCallback(hpcd);
      }

      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_WKUINT);
    }

    /* Handle Suspend Interrupt */
    if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_USBSUSP))
    {
      if((USBx_DEVICE->DSTS & USB_OTG_DSTS_SUSPSTS) == USB_OTG_DSTS_SUSPSTS)
      {

        HAL_PCD_SuspendCallback(hpcd);
      }
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_USBSUSP);
    }

    /* Handle LPM Interrupt */
    if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_LPMINT))
    {
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_LPMINT);
      if( hpcd->LPM_State == LPM_L0)
      {
        hpcd->LPM_State = LPM_L1;
        hpcd->BESL = (hpcd->Instance->GLPMCFG & USB_OTG_GLPMCFG_BESL) >>2 ;
        HAL_PCDEx_LPM_Callback(hpcd, PCD_LPM_L1_ACTIVE);
      }
      else
      {
        HAL_PCD_SuspendCallback(hpcd);
      }
    }

    /* Handle Reset Interrupt */
    if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_USBRST))
    {
      USBx_DEVICE->DCTL &= ~USB_OTG_DCTL_RWUSIG;
      USB_FlushTxFifo(hpcd->Instance ,  0x10);

      for (index = 0; index < hpcd->Init.dev_endpoints ; index++)
      {
        USBx_INEP(index)->DIEPINT = 0xFF;
        USBx_OUTEP(index)->DOEPINT = 0xFF;
      }
      USBx_DEVICE->DAINT = 0xFFFFFFFF;
      USBx_DEVICE->DAINTMSK |= 0x10001;

      if(hpcd->Init.use_dedicated_ep1)
      {
        USBx_DEVICE->DOUTEP1MSK |= (USB_OTG_DOEPMSK_STUPM | USB_OTG_DOEPMSK_XFRCM | USB_OTG_DOEPMSK_EPDM);
        USBx_DEVICE->DINEP1MSK |= (USB_OTG_DIEPMSK_TOM | USB_OTG_DIEPMSK_XFRCM | USB_OTG_DIEPMSK_EPDM);
      }
      else
      {
#ifdef USB_OTG_DOEPINT_OTEPSPR
        USBx_DEVICE->DOEPMSK |= (USB_OTG_DOEPMSK_STUPM | USB_OTG_DOEPMSK_XFRCM | USB_OTG_DOEPMSK_EPDM | USB_OTG_DOEPMSK_OTEPSPRM);
#else
        USBx_DEVICE->DOEPMSK |= (USB_OTG_DOEPMSK_STUPM | USB_OTG_DOEPMSK_XFRCM | USB_OTG_DOEPMSK_EPDM);
#endif /* USB_OTG_DOEPINT_OTEPSPR */
        USBx_DEVICE->DIEPMSK |= (USB_OTG_DIEPMSK_TOM | USB_OTG_DIEPMSK_XFRCM | USB_OTG_DIEPMSK_EPDM);
      }

      /* Set Default Address to 0 */
      USBx_DEVICE->DCFG &= ~USB_OTG_DCFG_DAD;

      /* setup EP0 to receive SETUP packets */
      USB_EP0_OutStart(hpcd->Instance, hpcd->Init.dma_enable, (uint8_t *)hpcd->Setup);

      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_USBRST);
    }

    /* Handle Enumeration done Interrupt */
    if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_ENUMDNE))
    {
      USB_ActivateSetup(hpcd->Instance);
      hpcd->Instance->GUSBCFG &= ~USB_OTG_GUSBCFG_TRDT;

      hpcd->Init.speed            = USB_OTG_SPEED_FULL;
      hpcd->Init.ep0_mps          = USB_OTG_FS_MAX_PACKET_SIZE ;

        /* The USBTRD is configured according to the tables below, depending on AHB frequency
        used by application. In the low AHB frequency range it is used to stretch enough the USB response
        time to IN tokens, the USB turnaround time, so to compensate for the longer AHB read access
        latency to the Data FIFO */

      /* Get hclk frequency value */
      hclk = HAL_RCC_GetHCLKFreq();

      if((hclk >= 14200000)&&(hclk < 15000000))
      {
        /* hclk Clock Range between 14.2-15 MHz */
        hpcd->Instance->GUSBCFG |= (uint32_t)((0xF << 10) & USB_OTG_GUSBCFG_TRDT);
      }

      else if((hclk >= 15000000)&&(hclk < 16000000))
      {
        /* hclk Clock Range between 15-16 MHz */
        hpcd->Instance->GUSBCFG |= (uint32_t)((0xE << 10) & USB_OTG_GUSBCFG_TRDT);
      }

      else if((hclk >= 16000000)&&(hclk < 17200000))
      {
        /* hclk Clock Range between 16-17.2 MHz */
        hpcd->Instance->GUSBCFG |= (uint32_t)((0xD << 10) & USB_OTG_GUSBCFG_TRDT);
      }

      else if((hclk >= 17200000)&&(hclk < 18500000))
      {
        /* hclk Clock Range between 17.2-18.5 MHz */
        hpcd->Instance->GUSBCFG |= (uint32_t)((0xC << 10) & USB_OTG_GUSBCFG_TRDT);
      }

      else if((hclk >= 18500000)&&(hclk < 20000000))
      {
        /* hclk Clock Range between 18.5-20 MHz */
        hpcd->Instance->GUSBCFG |= (uint32_t)((0xB << 10) & USB_OTG_GUSBCFG_TRDT);
      }

      else if((hclk >= 20000000)&&(hclk < 21800000))
      {
        /* hclk Clock Range between 20-21.8 MHz */
        hpcd->Instance->GUSBCFG |= (uint32_t)((0xA << 10) & USB_OTG_GUSBCFG_TRDT);
      }

      else if((hclk >= 21800000)&&(hclk < 24000000))
      {
        /* hclk Clock Range between 21.8-24 MHz */
        hpcd->Instance->GUSBCFG |= (uint32_t)((0x9 << 10) & USB_OTG_GUSBCFG_TRDT);
      }

      else if((hclk >= 24000000)&&(hclk < 27700000))
      {
        /* hclk Clock Range between 24-27.7 MHz */
        hpcd->Instance->GUSBCFG |= (uint32_t)((0x8 << 10) & USB_OTG_GUSBCFG_TRDT);
      }

      else if((hclk >= 27700000)&&(hclk < 32000000))
      {
        /* hclk Clock Range between 27.7-32 MHz */
        hpcd->Instance->GUSBCFG |= (uint32_t)((0x7 << 10) & USB_OTG_GUSBCFG_TRDT);
      }

      else /* if(hclk >= 32000000) */
      {
        /* hclk Clock Range between 32-80 MHz */
        hpcd->Instance->GUSBCFG |= (uint32_t)((0x6 << 10) & USB_OTG_GUSBCFG_TRDT);
      }

      HAL_PCD_ResetCallback(hpcd);

      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_ENUMDNE);
    }

    /* Handle RxQLevel Interrupt */
    if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_RXFLVL))
    {
      USB_MASK_INTERRUPT(hpcd->Instance, USB_OTG_GINTSTS_RXFLVL);

      temp = USBx->GRXSTSP;

      ep = &hpcd->OUT_ep[temp & USB_OTG_GRXSTSP_EPNUM];

      if(((temp & USB_OTG_GRXSTSP_PKTSTS) >> 17) ==  STS_DATA_UPDT)
      {
        if((temp & USB_OTG_GRXSTSP_BCNT) != 0)
        {
          USB_ReadPacket(USBx, ep->xfer_buff, (temp & USB_OTG_GRXSTSP_BCNT) >> 4);
          ep->xfer_buff += (temp & USB_OTG_GRXSTSP_BCNT) >> 4;
          ep->xfer_count += (temp & USB_OTG_GRXSTSP_BCNT) >> 4;
        }
      }
      else if (((temp & USB_OTG_GRXSTSP_PKTSTS) >> 17) ==  STS_SETUP_UPDT)
      {
        USB_ReadPacket(USBx, (uint8_t *)hpcd->Setup, 8);
        ep->xfer_count += (temp & USB_OTG_GRXSTSP_BCNT) >> 4;
      }
      USB_UNMASK_INTERRUPT(hpcd->Instance, USB_OTG_GINTSTS_RXFLVL);
    }

    /* Handle SOF Interrupt */
    if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_SOF))
    {
      HAL_PCD_SOFCallback(hpcd);
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_SOF);
    }

    /* Handle Incomplete ISO IN Interrupt */
    if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_IISOIXFR))
    {
      HAL_PCD_ISOINIncompleteCallback(hpcd, epnum);
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_IISOIXFR);
    }

    /* Handle Incomplete ISO OUT Interrupt */
    if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT))
    {
      HAL_PCD_ISOOUTIncompleteCallback(hpcd, epnum);
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT);
    }

    /* Handle Connection event Interrupt */
    if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_SRQINT))
    {
      HAL_PCD_ConnectCallback(hpcd);
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_SRQINT);
    }

    /* Handle Disconnection event Interrupt */
    if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_OTGINT))
    {
      temp = hpcd->Instance->GOTGINT;

      if((temp & USB_OTG_GOTGINT_SEDET) == USB_OTG_GOTGINT_SEDET)
      {
        HAL_PCD_DisconnectCallback(hpcd);
      }
      hpcd->Instance->GOTGINT |= temp;
    }
  }
}

#endif /* USB_OTG_FS */

#if defined (USB)
/**
  * @brief  This function handles PCD interrupt request.
  * @param  hpcd: PCD handle
  * @retval HAL status
  */
void HAL_PCD_IRQHandler(PCD_HandleTypeDef *hpcd)
{
  uint32_t wInterrupt_Mask = 0;

  if (__HAL_PCD_GET_FLAG (hpcd, USB_ISTR_CTR))
  {
    /* servicing of the endpoint correct transfer interrupt */
    /* clear of the CTR flag into the sub */
    PCD_EP_ISR_Handler(hpcd);
  }

  if (__HAL_PCD_GET_FLAG (hpcd, USB_ISTR_RESET))
  {
    __HAL_PCD_CLEAR_FLAG(hpcd, USB_ISTR_RESET);
    HAL_PCD_ResetCallback(hpcd);
    HAL_PCD_SetAddress(hpcd, 0);
  }

  if (__HAL_PCD_GET_FLAG (hpcd, USB_ISTR_PMAOVR))
  {
    __HAL_PCD_CLEAR_FLAG(hpcd, USB_ISTR_PMAOVR);
  }

  if (__HAL_PCD_GET_FLAG (hpcd, USB_ISTR_ERR))
  {
    __HAL_PCD_CLEAR_FLAG(hpcd, USB_ISTR_ERR);
  }

  if (__HAL_PCD_GET_FLAG (hpcd, USB_ISTR_WKUP))
  {

    hpcd->Instance->CNTR &= ~(USB_CNTR_LPMODE);

    /*set wInterrupt_Mask global variable*/
    wInterrupt_Mask = USB_CNTR_CTRM  | USB_CNTR_WKUPM | USB_CNTR_SUSPM | USB_CNTR_ERRM \
      | USB_CNTR_SOFM | USB_CNTR_ESOFM | USB_CNTR_RESETM;

    /*Set interrupt mask*/
    hpcd->Instance->CNTR = wInterrupt_Mask;

    /* enable L1REQ interrupt */
    if (hpcd->Init.lpm_enable ==1)
    {
      wInterrupt_Mask |= USB_CNTR_L1REQM;

      /* Enable LPM support and enable ACK answer to LPM request*/
      USB_TypeDef *USBx = hpcd->Instance;
      hpcd->lpm_active = ENABLE;
      hpcd->LPM_State = LPM_L0;

      USBx->LPMCSR |= (USB_LPMCSR_LMPEN);
      USBx->LPMCSR |= (USB_LPMCSR_LPMACK);
    }

    if(hpcd->LPM_State == LPM_L1)
    {
      hpcd->LPM_State = LPM_L0;
      HAL_PCDEx_LPM_Callback(hpcd, PCD_LPM_L0_ACTIVE);
    }

    HAL_PCD_ResumeCallback(hpcd);

    __HAL_PCD_CLEAR_FLAG(hpcd, USB_ISTR_WKUP);
  }

  if (__HAL_PCD_GET_FLAG (hpcd, USB_ISTR_SUSP))
  {
    /* clear of the ISTR bit must be done after setting of CNTR_FSUSP */
    __HAL_PCD_CLEAR_FLAG(hpcd, USB_ISTR_SUSP);

    /* Force low-power mode in the macrocell */
    hpcd->Instance->CNTR |= USB_CNTR_FSUSP;
    hpcd->Instance->CNTR |= USB_CNTR_LPMODE;

    if (__HAL_PCD_GET_FLAG (hpcd, USB_ISTR_WKUP) == 0)
    {
      HAL_PCD_SuspendCallback(hpcd);
    }
  }

  /* Handle LPM Interrupt */
  if(__HAL_PCD_GET_FLAG(hpcd, USB_ISTR_L1REQ))
  {
    __HAL_PCD_CLEAR_FLAG(hpcd, USB_ISTR_L1REQ);
    if( hpcd->LPM_State == LPM_L0)
    {
      /* Force suspend and low-power mode before going to L1 state*/
      hpcd->Instance->CNTR |= USB_CNTR_LPMODE;
      hpcd->Instance->CNTR |= USB_CNTR_FSUSP;

      hpcd->LPM_State = LPM_L1;
      hpcd->BESL = (hpcd->Instance->LPMCSR & USB_LPMCSR_BESL) >>2 ;
      HAL_PCDEx_LPM_Callback(hpcd, PCD_LPM_L1_ACTIVE);
    }
    else
    {
      HAL_PCD_SuspendCallback(hpcd);
    }
  }

  if (__HAL_PCD_GET_FLAG (hpcd, USB_ISTR_SOF))
  {
    __HAL_PCD_CLEAR_FLAG(hpcd, USB_ISTR_SOF);
    HAL_PCD_SOFCallback(hpcd);
  }

  if (__HAL_PCD_GET_FLAG (hpcd, USB_ISTR_ESOF))
  {
    /* clear ESOF flag in ISTR */
    __HAL_PCD_CLEAR_FLAG(hpcd, USB_ISTR_ESOF);
  }
}
#endif /* USB */

/**
  * @brief  Data OUT stage callback.
  * @param  hpcd: PCD handle
  * @param  epnum: endpoint number
  * @retval None
  */
__weak void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);
  UNUSED(epnum);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_DataOutStageCallback could be implemented in the user file
   */
}

/**
  * @brief  Data IN stage callback.
  * @param  hpcd: PCD handle
  * @param  epnum: endpoint number
  * @retval None
  */
__weak void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);
  UNUSED(epnum);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_DataInStageCallback could be implemented in the user file
   */
}
/**
  * @brief  Setup stage callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
__weak void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_SetupStageCallback could be implemented in the user file
   */
}

/**
  * @brief  USB Start Of Frame callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
__weak void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_SOFCallback could be implemented in the user file
   */
}

/**
  * @brief  USB Reset callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
__weak void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_ResetCallback could be implemented in the user file
   */
}

/**
  * @brief  Suspend event callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
__weak void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_SuspendCallback could be implemented in the user file
   */
}

/**
  * @brief  Resume event callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
__weak void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_ResumeCallback could be implemented in the user file
   */
}

/**
  * @brief  Incomplete ISO OUT callback.
  * @param  hpcd: PCD handle
  * @param  epnum: endpoint number
  * @retval None
  */
__weak void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);
  UNUSED(epnum);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_ISOOUTIncompleteCallback could be implemented in the user file
   */
}

/**
  * @brief  Incomplete ISO IN callback.
  * @param  hpcd: PCD handle
  * @param  epnum: endpoint number
  * @retval None
  */
__weak void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);
  UNUSED(epnum);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_ISOINIncompleteCallback could be implemented in the user file
   */
}

/**
  * @brief  Connection event callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
__weak void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_ConnectCallback could be implemented in the user file
   */
}

/**
  * @brief  Disconnection event callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
__weak void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_DisconnectCallback could be implemented in the user file
   */
}

/**
  * @}
  */

/** @defgroup PCD_Exported_Functions_Group3 Peripheral Control functions
 *  @brief   management functions
 *
@verbatim
 ===============================================================================
                      ##### Peripheral Control functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the PCD data
    transfers.

@endverbatim
  * @{
  */

/**
  * @brief  Connect the USB device.
  * @param  hpcd: PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_DevConnect(PCD_HandleTypeDef *hpcd)
{
  __HAL_LOCK(hpcd);
  USB_DevConnect(hpcd->Instance);
  __HAL_UNLOCK(hpcd);
  return HAL_OK;
}

/**
  * @brief  Disconnect the USB device.
  * @param  hpcd: PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_DevDisconnect(PCD_HandleTypeDef *hpcd)
{
  __HAL_LOCK(hpcd);
  USB_DevDisconnect(hpcd->Instance);
  __HAL_UNLOCK(hpcd);
  return HAL_OK;
}

/**
  * @brief  Set the USB Device address.
  * @param  hpcd: PCD handle
  * @param  address: new device address
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_SetAddress(PCD_HandleTypeDef *hpcd, uint8_t address)
{
  __HAL_LOCK(hpcd);
  hpcd->USB_Address = address;
  USB_SetDevAddress(hpcd->Instance, address);
  __HAL_UNLOCK(hpcd);
  return HAL_OK;
}
/**
  * @brief  Open and configure an endpoint.
  * @param  hpcd: PCD handle
  * @param  ep_addr: endpoint address
  * @param  ep_mps: endpoint max packet size
  * @param  ep_type: endpoint type
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_EP_Open(PCD_HandleTypeDef *hpcd, uint8_t ep_addr, uint16_t ep_mps, uint8_t ep_type)
{
  HAL_StatusTypeDef  ret = HAL_OK;
  PCD_EPTypeDef *ep = NULL;

  if ((ep_addr & 0x80) == 0x80)
  {
    ep = &hpcd->IN_ep[ep_addr & 0x7F];
  }
  else
  {
    ep = &hpcd->OUT_ep[ep_addr & 0x7F];
  }
  ep->num   = ep_addr & 0x7F;

  ep->is_in = (0x80 & ep_addr) != 0;
  ep->maxpacket = ep_mps;
  ep->type = ep_type;

  __HAL_LOCK(hpcd);
  USB_ActivateEndpoint(hpcd->Instance , ep);
  __HAL_UNLOCK(hpcd);
  return ret;

}


/**
  * @brief  Deactivate an endpoint.
  * @param  hpcd: PCD handle
  * @param  ep_addr: endpoint address
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_EP_Close(PCD_HandleTypeDef *hpcd, uint8_t ep_addr)
{
  PCD_EPTypeDef *ep = NULL;

  if ((ep_addr & 0x80) == 0x80)
  {
    ep = &hpcd->IN_ep[ep_addr & 0x7F];
  }
  else
  {
    ep = &hpcd->OUT_ep[ep_addr & 0x7F];
  }
  ep->num   = ep_addr & 0x7F;

  ep->is_in = (0x80 & ep_addr) != 0;

  __HAL_LOCK(hpcd);
  USB_DeactivateEndpoint(hpcd->Instance , ep);
  __HAL_UNLOCK(hpcd);
  return HAL_OK;
}


/**
  * @brief  Receive an amount of data.
  * @param  hpcd: PCD handle
  * @param  ep_addr: endpoint address
  * @param  pBuf: pointer to the reception buffer
  * @param  len: amount of data to be received
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_EP_Receive(PCD_HandleTypeDef *hpcd, uint8_t ep_addr, uint8_t *pBuf, uint32_t len)
{
  PCD_EPTypeDef *ep = NULL;

  ep = &hpcd->OUT_ep[ep_addr & 0x7F];

  /*setup and start the Xfer */
  ep->xfer_buff = pBuf;
  ep->xfer_len = len;
  ep->xfer_count = 0;
  ep->is_in = 0;
  ep->num = ep_addr & 0x7F;

  if ((ep_addr & 0x7F) == 0 )
  {
    USB_EP0StartXfer(hpcd->Instance, ep, hpcd->Init.dma_enable);
  }
  else
  {
    USB_EPStartXfer(hpcd->Instance, ep, hpcd->Init.dma_enable);
  }

  return HAL_OK;
}

/**
  * @brief  Get Received Data Size.
  * @param  hpcd: PCD handle
  * @param  ep_addr: endpoint address
  * @retval Data Size
  */
uint16_t HAL_PCD_EP_GetRxCount(PCD_HandleTypeDef *hpcd, uint8_t ep_addr)
{
  return hpcd->OUT_ep[ep_addr & 0x7F].xfer_count;
}
/**
  * @brief  Send an amount of data.
  * @param  hpcd: PCD handle
  * @param  ep_addr: endpoint address
  * @param  pBuf: pointer to the transmission buffer
  * @param  len: amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_EP_Transmit(PCD_HandleTypeDef *hpcd, uint8_t ep_addr, uint8_t *pBuf, uint32_t len)
{
  PCD_EPTypeDef *ep = NULL;

  ep = &hpcd->IN_ep[ep_addr & 0x7F];

  /*setup and start the Xfer */
  ep->xfer_buff = pBuf;
  ep->xfer_len = len;
  ep->xfer_count = 0;
  ep->is_in = 1;
  ep->num = ep_addr & 0x7F;

  if ((ep_addr & 0x7F) == 0 )
  {
    USB_EP0StartXfer(hpcd->Instance,ep,  hpcd->Init.dma_enable);
  }
  else
  {
    USB_EPStartXfer(hpcd->Instance, ep,  hpcd->Init.dma_enable);
  }

  return HAL_OK;
}

/**
  * @brief  Set a STALL condition over an endpoint.
  * @param  hpcd: PCD handle
  * @param  ep_addr: endpoint address
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_EP_SetStall(PCD_HandleTypeDef *hpcd, uint8_t ep_addr)
{
  PCD_EPTypeDef *ep = NULL;

  if ((0x80 & ep_addr) == 0x80)
  {
    ep = &hpcd->IN_ep[ep_addr & 0x7F];
  }
  else
  {
    ep = &hpcd->OUT_ep[ep_addr];
  }

  ep->is_stall = 1;
  ep->num   = ep_addr & 0x7F;
  ep->is_in = ((ep_addr & 0x80) == 0x80);

  __HAL_LOCK(hpcd);
  USB_EPSetStall(hpcd->Instance , ep);
  if((ep_addr & 0x7F) == 0)
  {
    USB_EP0_OutStart(hpcd->Instance,  hpcd->Init.dma_enable, (uint8_t *)hpcd->Setup);
  }
  __HAL_UNLOCK(hpcd);

  return HAL_OK;
}

/**
  * @brief  Clear a STALL condition over in an endpoint.
  * @param  hpcd: PCD handle
  * @param  ep_addr: endpoint address
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_EP_ClrStall(PCD_HandleTypeDef *hpcd, uint8_t ep_addr)
{
  PCD_EPTypeDef *ep = NULL;

  if ((0x80 & ep_addr) == 0x80)
  {
    ep = &hpcd->IN_ep[ep_addr & 0x7F];
  }
  else
  {
    ep = &hpcd->OUT_ep[ep_addr];
  }

  ep->is_stall = 0;
  ep->num   = ep_addr & 0x7F;
  ep->is_in = ((ep_addr & 0x80) == 0x80);

  __HAL_LOCK(hpcd);
  USB_EPClearStall(hpcd->Instance , ep);
  __HAL_UNLOCK(hpcd);

  return HAL_OK;
}

/**
  * @brief  Flush an endpoint.
  * @param  hpcd: PCD handle
  * @param  ep_addr: endpoint address
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_EP_Flush(PCD_HandleTypeDef *hpcd, uint8_t ep_addr)
{
  __HAL_LOCK(hpcd);

  if ((ep_addr & 0x80) == 0x80)
  {
    USB_FlushTxFifo(hpcd->Instance, ep_addr & 0x7F);
  }
  else
  {
    USB_FlushRxFifo(hpcd->Instance);
  }

  __HAL_UNLOCK(hpcd);

  return HAL_OK;
}

/**
  * @brief  Activate remote wakeup signalling.
  * @param  hpcd: PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_ActivateRemoteWakeup(PCD_HandleTypeDef *hpcd)
{
  return(USB_ActivateRemoteWakeup(hpcd->Instance));
}

/**
  * @brief  De-activate remote wakeup signalling.
  * @param  hpcd: PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_DeActivateRemoteWakeup(PCD_HandleTypeDef *hpcd)
{
  return(USB_DeActivateRemoteWakeup(hpcd->Instance));
}
/**
  * @}
  */

/** @defgroup PCD_Exported_Functions_Group4 Peripheral State functions
 *  @brief   Peripheral State functions
 *
@verbatim
 ===============================================================================
                      ##### Peripheral State functions #####
 ===============================================================================
    [..]
    This subsection permits to get in run-time the status of the peripheral
    and the data flow.

@endverbatim
  * @{
  */

/**
  * @brief  Return the PCD handle state.
  * @param  hpcd: PCD handle
  * @retval HAL state
  */
PCD_StateTypeDef HAL_PCD_GetState(PCD_HandleTypeDef *hpcd)
{
  return hpcd->State;
}
/**
  * @}
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @addtogroup PCD_Private_Functions
  * @{
  */
#if defined (USB_OTG_FS)
/**
  * @brief  Check FIFO for the next packet to be loaded.
  * @param  hpcd: PCD handle
  * @param  epnum: endpoint number
  * @retval HAL status
  */
static HAL_StatusTypeDef PCD_WriteEmptyTxFifo(PCD_HandleTypeDef *hpcd, uint32_t epnum)
{
  USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;
  USB_OTG_EPTypeDef *ep = NULL;
  int32_t len = 0U;
  uint32_t len32b = 0;
  uint32_t fifoemptymsk = 0;

  ep = &hpcd->IN_ep[epnum];
  #if 0
  len = ep->xfer_len - ep->xfer_count;

  if (len > ep->maxpacket)
  {
    len = ep->maxpacket;
  }


  len32b = (len + 3) / 4;

  while  ( (USBx_INEP(epnum)->DTXFSTS & USB_OTG_DTXFSTS_INEPTFSAV) > len32b &&
          ep->xfer_count < ep->xfer_len &&
            ep->xfer_len != 0)
  {
    /* Write the FIFO */
    len = ep->xfer_len - ep->xfer_count;

    if (len > ep->maxpacket)
    {
      len = ep->maxpacket;
    }
    len32b = (len + 3) / 4;

    USB_WritePacket(USBx, ep->xfer_buff, epnum, len, hpcd->Init.dma_enable);

    ep->xfer_buff  += len;
    ep->xfer_count += len;
  }

  #else
  /* dpgeorge: modified above loop to:
   *    - allow to write the packet in the case it exactly fills the FIFO
   *    - recompute len32b before each check of this value against FIFO free space
   */
  for (;;)
  {
    len = ep->xfer_len - ep->xfer_count;
    if (len <= 0U)
    {
      /* Finished sending all data */
      break;
    }
    if (len > ep->maxpacket)
    {
      len = ep->maxpacket;
    }

    len32b = (len + 3U) / 4U;
    if  (len32b > (USBx_INEP(epnum)->DTXFSTS & USB_OTG_DTXFSTS_INEPTFSAV))
    {
      /* No room left in FIFO */
      break;
    }

    USB_WritePacket(USBx, ep->xfer_buff, epnum, len, hpcd->Init.dma_enable);
    ep->xfer_buff  += len;
    ep->xfer_count += len;
  }
  #endif

  if(len <= 0)
  {
    fifoemptymsk = 0x1 << epnum;
    USBx_DEVICE->DIEPEMPMSK &= ~fifoemptymsk;

  }

  return HAL_OK;
}
#endif /* USB_OTG_FS */

#if defined (USB)
/**
  * @brief  This function handles PCD Endpoint interrupt request.
  * @param  hpcd: PCD handle
  * @retval HAL status
  */
static HAL_StatusTypeDef PCD_EP_ISR_Handler(PCD_HandleTypeDef *hpcd)
{
  PCD_EPTypeDef *ep = NULL;
  uint16_t count = 0;
  uint8_t epindex = 0;
  __IO uint16_t wIstr = 0;
  __IO uint16_t wEPVal = 0;

  /* stay in loop while pending interrupts */
  while (((wIstr = hpcd->Instance->ISTR) & USB_ISTR_CTR) != 0)
  {
    /* extract highest priority endpoint number */
    epindex = (uint8_t)(wIstr & USB_ISTR_EP_ID);

    if (epindex == 0)
    {
      /* Decode and service control endpoint interrupt */

      /* DIR bit = origin of the interrupt */
      if ((wIstr & USB_ISTR_DIR) == 0)
      {
        /* DIR = 0 */

        /* DIR = 0      => IN  int */
        /* DIR = 0 implies that (EP_CTR_TX = 1) always  */
        PCD_CLEAR_TX_EP_CTR(hpcd->Instance, PCD_ENDP0);
        ep = &hpcd->IN_ep[0];

        ep->xfer_count = PCD_GET_EP_TX_CNT(hpcd->Instance, ep->num);
        ep->xfer_buff += ep->xfer_count;

        /* TX COMPLETE */
        HAL_PCD_DataInStageCallback(hpcd, 0);


        if((hpcd->USB_Address > 0)&& ( ep->xfer_len == 0))
        {
          hpcd->Instance->DADDR = (hpcd->USB_Address | USB_DADDR_EF);
          hpcd->USB_Address = 0;
        }

      }
      else
      {
        /* DIR = 1 */

        /* DIR = 1 & CTR_RX       => SETUP or OUT int */
        /* DIR = 1 & (CTR_TX | CTR_RX) => 2 int pending */
        ep = &hpcd->OUT_ep[0];
        wEPVal = PCD_GET_ENDPOINT(hpcd->Instance, PCD_ENDP0);

        if ((wEPVal & USB_EP_SETUP) != 0)
        {
          /* Get SETUP Packet*/
          ep->xfer_count = PCD_GET_EP_RX_CNT(hpcd->Instance, ep->num);
          USB_ReadPMA(hpcd->Instance, (uint8_t*)hpcd->Setup ,ep->pmaadress , ep->xfer_count);
          /* SETUP bit kept frozen while CTR_RX = 1*/
          PCD_CLEAR_RX_EP_CTR(hpcd->Instance, PCD_ENDP0);

          /* Process SETUP Packet*/
          HAL_PCD_SetupStageCallback(hpcd);
        }

        else if ((wEPVal & USB_EP_CTR_RX) != 0)
        {
          PCD_CLEAR_RX_EP_CTR(hpcd->Instance, PCD_ENDP0);
          /* Get Control Data OUT Packet*/
          ep->xfer_count = PCD_GET_EP_RX_CNT(hpcd->Instance, ep->num);

          if (ep->xfer_count != 0)
          {
            USB_ReadPMA(hpcd->Instance, ep->xfer_buff, ep->pmaadress, ep->xfer_count);
            ep->xfer_buff+=ep->xfer_count;
          }

          /* Process Control Data OUT Packet*/
           HAL_PCD_DataOutStageCallback(hpcd, 0);

          PCD_SET_EP_RX_CNT(hpcd->Instance, PCD_ENDP0, ep->maxpacket);
          PCD_SET_EP_RX_STATUS(hpcd->Instance, PCD_ENDP0, USB_EP_RX_VALID);
        }
      }
    }
    else
    {
      /* Decode and service non control endpoints interrupt  */

      /* process related endpoint register */
      wEPVal = PCD_GET_ENDPOINT(hpcd->Instance, epindex);
      if ((wEPVal & USB_EP_CTR_RX) != 0)
      {
        /* clear int flag */
        PCD_CLEAR_RX_EP_CTR(hpcd->Instance, epindex);
        ep = &hpcd->OUT_ep[epindex];

        /* OUT double Buffering*/
        if (ep->doublebuffer == 0)
        {
          count = PCD_GET_EP_RX_CNT(hpcd->Instance, ep->num);
          if (count != 0)
          {
            USB_ReadPMA(hpcd->Instance, ep->xfer_buff, ep->pmaadress, count);
          }
        }
        else
        {
          if (PCD_GET_ENDPOINT(hpcd->Instance, ep->num) & USB_EP_DTOG_RX)
          {
            /*read from endpoint BUF0Addr buffer*/
            count = PCD_GET_EP_DBUF0_CNT(hpcd->Instance, ep->num);
            if (count != 0)
            {
              USB_ReadPMA(hpcd->Instance, ep->xfer_buff, ep->pmaaddr0, count);
            }
          }
          else
          {
            /*read from endpoint BUF1Addr buffer*/
            count = PCD_GET_EP_DBUF1_CNT(hpcd->Instance, ep->num);
            if (count != 0)
            {
              USB_ReadPMA(hpcd->Instance, ep->xfer_buff, ep->pmaaddr1, count);
            }
          }
          PCD_FreeUserBuffer(hpcd->Instance, ep->num, PCD_EP_DBUF_OUT);
        }
        /*multi-packet on the NON control OUT endpoint*/
        ep->xfer_count+=count;
        ep->xfer_buff+=count;

        if ((ep->xfer_len == 0) || (count < ep->maxpacket))
        {
          /* RX COMPLETE */
          HAL_PCD_DataOutStageCallback(hpcd, ep->num);
        }
        else
        {
          HAL_PCD_EP_Receive(hpcd, ep->num, ep->xfer_buff, ep->xfer_len);
        }

      } /* if((wEPVal & EP_CTR_RX) */

      if ((wEPVal & USB_EP_CTR_TX) != 0)
      {
        ep = &hpcd->IN_ep[epindex];

        /* clear int flag */
        PCD_CLEAR_TX_EP_CTR(hpcd->Instance, epindex);

        /* IN double Buffering*/
        if (ep->doublebuffer == 0)
        {
          ep->xfer_count = PCD_GET_EP_TX_CNT(hpcd->Instance, ep->num);
          if (ep->xfer_count != 0)
          {
            USB_WritePMA(hpcd->Instance, ep->xfer_buff, ep->pmaadress, ep->xfer_count);
          }
        }
        else
        {
          if (PCD_GET_ENDPOINT(hpcd->Instance, ep->num) & USB_EP_DTOG_TX)
          {
            /*read from endpoint BUF0Addr buffer*/
            ep->xfer_count = PCD_GET_EP_DBUF0_CNT(hpcd->Instance, ep->num);
            if (ep->xfer_count != 0)
            {
              USB_WritePMA(hpcd->Instance, ep->xfer_buff, ep->pmaaddr0, ep->xfer_count);
            }
          }
          else
          {
            /*read from endpoint BUF1Addr buffer*/
            ep->xfer_count = PCD_GET_EP_DBUF1_CNT(hpcd->Instance, ep->num);
            if (ep->xfer_count != 0)
            {
              USB_WritePMA(hpcd->Instance, ep->xfer_buff, ep->pmaaddr1, ep->xfer_count);
            }
          }
          PCD_FreeUserBuffer(hpcd->Instance, ep->num, PCD_EP_DBUF_IN);
        }
        /*multi-packet on the NON control IN endpoint*/
        ep->xfer_count = PCD_GET_EP_TX_CNT(hpcd->Instance, ep->num);
        ep->xfer_buff+=ep->xfer_count;

        /* Zero Length Packet? */
        if (ep->xfer_len == 0)
        {
          /* TX COMPLETE */
          HAL_PCD_DataInStageCallback(hpcd, ep->num);
        }
        else
        {
          HAL_PCD_EP_Transmit(hpcd, ep->num, ep->xfer_buff, ep->xfer_len);
        }
      }
    }
  }
  return HAL_OK;
}
#endif /* USB */

/**
  * @}
  */

#endif /* STM32L432xx || STM32L433xx || STM32L442xx || STM32L443xx || */
       /* STM32L452xx || STM32L462xx || */
       /* STM32L475xx || STM32L476xx || STM32L485xx || STM32L486xx || */
       /* STM32L496xx || STM32L4A6xx */

#endif /* HAL_PCD_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/**
  ******************************************************************************
  * @file    stm32l4xx_hal_dcmi.c
  * @author  MCD Application Team
  * @version V1.7.2
  * @date    16-June-2017
  * @brief   DCMI HAL module driver
  *          This file provides firmware functions to manage the following
  *          functionalities of the Digital Camera Interface (DCMI) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral Control functions
  *           + Peripheral State and Error functions
  *
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
  [..]
      The sequence below describes how to use this driver to capture an image
      from a camera module connected to the DCMI Interface.
      This sequence does not take into account the configuration of the
      camera module, which should be made before configuring and enabling
      the DCMI.

    (#) Program the required configuration through the following parameters:
        horizontal and vertical polarity, pixel clock polarity, capture rate,
        synchronization mode, frame delimiter codes, data width, byte and line
        selection using HAL_DCMI_Init() function.

    (#) Optionally select JPEG mode; in that case, only the polarity
        and the capture mode parameters need to be set.

    (#) Capture mode can be either snapshot or continuous mode.

    (#) Configure the DMA_Handle to transfer data from DCMI DR
        register to the destination memory buffer.

    -@- In snapshot mode, the interface transfers a single frame through DMA. In
       continuous mode, the DMA must be set in circular mode to ensure a continuous
       flow of images data samples.

    (#) Program the transfer configuration through the following parameters:
        DCMI mode, destination memory buffer address and data length then
        enable capture using HAL_DCMI_Start_DMA() function.

    (#) Whether in continuous or snapshot mode, data length parameter must be
        equal to the frame size.

    (#) When the frame size is unknown beforehand (e.g. JPEG case), data length must
        be large enough to ensure the capture of a frame.

    (#) If the frame size is larger than the maximum DMA transfer length (i.e. 65535),
        (++) the DMA must be configured in circular mode, either for snapshot or continuous
             capture mode,
        (++) during capture, the driver copies the image data samples from DCMI DR register
             at the end of the final destination buffer used as a work buffer,
        (++) at each DMA half (respectively complete) transfer interrupt, the first
             (resp. second) half of the work buffer is copied to the final destination thru
             a second DMA channel.
        (++) Parameters of this second DMA channel are contained in the memory to memory DMA
             handle "DMAM2M_Handle", itself field of the DCMI handle structure.
        (++) This memory to memory transfer has length half that of the work buffer and is
             carried out in normal mode (not in circular mode).

    (#) Optionally, configure and enable the CROP feature to select a
        rectangular window from the received image using HAL_DCMI_ConfigCrop()
        and HAL_DCMI_EnableCrop() functions. Use HAL_DCMI_DisableCrop() to
        disable this feature.

    (#) The capture can be stopped with HAL_DCMI_Stop() function.

    (#) To control the DCMI state, use the function HAL_DCMI_GetState().

    (#) To read the DCMI error code, use the function HAL_DCMI_GetError().

    [..]
    (@) When the frame size is less than the maximum DMA transfer length (i.e. 65535)
        and when in snapshot mode, user must make sure the FRAME interrupt is disabled.
        This allows to avoid corner cases where the FRAME interrupt might be triggered
        before the DMA transfer completion interrupt. In this specific configuration,
        the driver checks the FRAME capture flag after the DMA transfer end and calls
        HAL_DCMI_FrameEventCallback() if the flag is set.

     *** DCMI HAL driver macros list ***
     =============================================
     [..]
       Below the list of most used macros in DCMI HAL driver.

      (+) __HAL_DCMI_ENABLE: Enable the DCMI peripheral.
      (+) __HAL_DCMI_DISABLE: Disable the DCMI peripheral.
      (+) __HAL_DCMI_GET_FLAG: Get the DCMI pending flags.
      (+) __HAL_DCMI_CLEAR_FLAG: Clear the DCMI pending flags.
      (+) __HAL_DCMI_ENABLE_IT: Enable the specified DCMI interrupts.
      (+) __HAL_DCMI_DISABLE_IT: Disable the specified DCMI interrupts.
      (+) __HAL_DCMI_GET_IT_SOURCE: Check whether the specified DCMI interrupt has occurred
          and that the interruption is enabled at the same time.

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

#ifdef HAL_DCMI_MODULE_ENABLED

#if defined(STM32L496xx) || defined(STM32L4A6xx)

/** @addtogroup STM32L4xx_HAL_Driver
  * @{
  */
/** @defgroup DCMI DCMI
  * @brief DCMI HAL module driver
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup DCMI_Private_Constants DCMI Private Constants
  * @{
  */

/** @defgroup DCMI_Stop_TimeOut DCMI Stop TimeOut
  * @{
  */
#define DCMI_TIMEOUT_STOP    ((uint32_t)1000)  /*!< 1s  */
/**
  * @}
  */

/** @defgroup DCMI_Shifts DCMI Shifts
  * @{
  */
#define DCMI_POSITION_CWSIZE_VLINE (uint32_t)POSITION_VAL(DCMI_CWSIZE_VLINE) /*!< Required left shift to set crop window vertical line count       */
#define DCMI_POSITION_CWSTRT_VST   (uint32_t)POSITION_VAL(DCMI_CWSTRT_VST)   /*!< Required left shift to set crop window vertical start line count */

#define DCMI_POSITION_ESCR_LSC     (uint32_t)POSITION_VAL(DCMI_ESCR_LSC)     /*!< Required left shift to set line start delimiter */
#define DCMI_POSITION_ESCR_LEC     (uint32_t)POSITION_VAL(DCMI_ESCR_LEC)     /*!< Required left shift to set line end delimiter   */
#define DCMI_POSITION_ESCR_FEC     (uint32_t)POSITION_VAL(DCMI_ESCR_FEC)     /*!< Required left shift to set frame end delimiter  */

#define DCMI_POSITION_ESUR_LSU     (uint32_t)POSITION_VAL(DCMI_ESUR_LSU)     /*!< Required left shift to set line start delimiter unmask */
#define DCMI_POSITION_ESUR_LEU     (uint32_t)POSITION_VAL(DCMI_ESUR_LEU)     /*!< Required left shift to set line end delimiter unmask   */
#define DCMI_POSITION_ESUR_FEU     (uint32_t)POSITION_VAL(DCMI_ESUR_FEU)     /*!< Required left shift to set frame end delimiter unmask  */
/**
  * @}
  */

#define NPRIME   16

/**
  * @}
  */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t PrimeArray[NPRIME] = { 1,  2,  3,  5,
                                7, 11, 13, 17,
                               19, 23, 29, 31,
                               37, 41, 43, 47};
/* Private function prototypes -----------------------------------------------*/
/** @addtogroup DCMI_Private_Functions DCMI Private Functions
  * @{
  */
static void       DCMI_DMAXferCplt(DMA_HandleTypeDef *hdma);
static void       DCMI_DMAHalfXferCplt(DMA_HandleTypeDef *hdma);
static void       DCMI_DMAError(DMA_HandleTypeDef *hdma);
static uint32_t   DCMI_TransferSize(uint32_t InputSize);
/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/

/** @defgroup DCMI_Exported_Functions DCMI Exported Functions
  * @{
  */

/** @defgroup DCMI_Exported_Functions_Group1 Initialization and Configuration functions
 *  @brief   Initialization and Configuration functions
 *
@verbatim
 ===============================================================================
                ##### Initialization and Configuration functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Initialize and configure the DCMI
      (+) De-initialize the DCMI

@endverbatim
  * @{
  */

/**
  * @brief  Initialize the DCMI according to the specified
  *         parameters in the DCMI_InitTypeDef and create the associated handle.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @note   By default, all interruptions are enabled (line end, frame end, overrun,
  *         VSYNC and embedded synchronization error interrupts).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMI_Init(DCMI_HandleTypeDef *hdcmi)
{
  /* Check the DCMI peripheral state */
  if(hdcmi == NULL)
  {
     return HAL_ERROR;
  }

  /* Check function parameters */
  assert_param(IS_DCMI_ALL_INSTANCE(hdcmi->Instance));
  assert_param(IS_DCMI_SYNCHRO(hdcmi->Init.SynchroMode));
  assert_param(IS_DCMI_PCKPOLARITY(hdcmi->Init.PCKPolarity));
  assert_param(IS_DCMI_VSPOLARITY(hdcmi->Init.VSPolarity));
  assert_param(IS_DCMI_HSPOLARITY(hdcmi->Init.HSPolarity));
  assert_param(IS_DCMI_CAPTURE_RATE(hdcmi->Init.CaptureRate));
  assert_param(IS_DCMI_EXTENDED_DATA(hdcmi->Init.ExtendedDataMode));
  assert_param(IS_DCMI_MODE_JPEG(hdcmi->Init.JPEGMode));

  assert_param(IS_DCMI_BYTE_SELECT_MODE(hdcmi->Init.ByteSelectMode));
  assert_param(IS_DCMI_BYTE_SELECT_START(hdcmi->Init.ByteSelectStart));
  assert_param(IS_DCMI_LINE_SELECT_MODE(hdcmi->Init.LineSelectMode));
  assert_param(IS_DCMI_LINE_SELECT_START(hdcmi->Init.LineSelectStart));

  if(hdcmi->State == HAL_DCMI_STATE_RESET)
  {
    /* Allocate lock resource and initialize it */
    hdcmi->Lock = HAL_UNLOCKED;
    /* Init the low level hardware */
    HAL_DCMI_MspInit(hdcmi);
  }

  /* Change the DCMI state */
  hdcmi->State = HAL_DCMI_STATE_BUSY;

  /* Disable DCMI IP before setting the configuration register */
  __HAL_DCMI_DISABLE(hdcmi);

  if (hdcmi->Init.ExtendedDataMode != DCMI_EXTEND_DATA_8B)
  {
    /* Byte select mode must be programmed to the reset value if the extended mode
      is not set to 8-bit data capture on every pixel clock */
    hdcmi->Init.ByteSelectMode = DCMI_BSM_ALL;
  }

  /* Set DCMI parameters */
  hdcmi->Instance->CR &= ~(DCMI_CR_PCKPOL | DCMI_CR_HSPOL  | DCMI_CR_VSPOL  | DCMI_CR_EDM_0 |\
                           DCMI_CR_EDM_1  | DCMI_CR_FCRC_0 | DCMI_CR_FCRC_1 | DCMI_CR_JPEG  |\
                           DCMI_CR_ESS | DCMI_CR_BSM_0 | DCMI_CR_BSM_1 | DCMI_CR_OEBS |\
                           DCMI_CR_LSM | DCMI_CR_OELS);

  hdcmi->Instance->CR |=  (uint32_t)(hdcmi->Init.SynchroMode | hdcmi->Init.CaptureRate |\
                                     hdcmi->Init.VSPolarity  | hdcmi->Init.HSPolarity  |\
                                     hdcmi->Init.PCKPolarity | hdcmi->Init.ExtendedDataMode |\
                                     hdcmi->Init.JPEGMode | hdcmi->Init.ByteSelectMode |\
                                     hdcmi->Init.ByteSelectStart | hdcmi->Init.LineSelectMode |\
                                     hdcmi->Init.LineSelectStart);

  if(hdcmi->Init.SynchroMode == DCMI_SYNCHRO_EMBEDDED)
  {
    hdcmi->Instance->ESCR = (((uint32_t)hdcmi->Init.SynchroCode.FrameStartCode)    |\
                             ((uint32_t)hdcmi->Init.SynchroCode.LineStartCode << DCMI_POSITION_ESCR_LSC)|\
                             ((uint32_t)hdcmi->Init.SynchroCode.LineEndCode << DCMI_POSITION_ESCR_LEC) |\
                             ((uint32_t)hdcmi->Init.SynchroCode.FrameEndCode << DCMI_POSITION_ESCR_FEC));
  }

  /* By default, enable all interrupts. The user may disable the unwanted ones
     in resorting to __HAL_DCMI_DISABLE_IT() macro before invoking HAL_DCMI_Start_DMA().
     Enabled interruptions are
     - end of line
     - end of frame
     - data reception overrun
     - frame synchronization signal VSYNC
     - synchronization error */
  __HAL_DCMI_ENABLE_IT(hdcmi, DCMI_IT_FRAME|DCMI_IT_OVR|DCMI_IT_ERR|DCMI_IT_VSYNC|DCMI_IT_LINE);

  /* Update error code */
  hdcmi->ErrorCode = HAL_DCMI_ERROR_NONE;

  /* Initialize the DCMI state*/
  hdcmi->State  = HAL_DCMI_STATE_READY;

  return HAL_OK;
}

/**
  * @brief  De-initialize the DCMI peripheral, reset control registers to
  *         their default values.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMI_DeInit(DCMI_HandleTypeDef *hdcmi)
{
  /* Before aborting any DCMI transfer, check
     first whether or not DCMI clock is enabled */
  if (__HAL_RCC_DCMI_IS_CLK_ENABLED())
  {
     if (HAL_DCMI_Stop(hdcmi) != HAL_OK)
     {
        /* Issue when stopping DCMI IP */
        return HAL_ERROR;
     }
  }

  /* DeInit the DCMI low level hardware */
  HAL_DCMI_MspDeInit(hdcmi);

  /* Reset DCMI control register */
  hdcmi->Instance->CR = 0;

  /* Update error code */
  hdcmi->ErrorCode = HAL_DCMI_ERROR_NONE;

  /* Initialize the DCMI state*/
  hdcmi->State = HAL_DCMI_STATE_RESET;

  /* Release Lock */
  __HAL_UNLOCK(hdcmi);

  return HAL_OK;
}

/**
  * @brief  Initialize the DCMI MSP.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @retval None
  */
__weak void HAL_DCMI_MspInit(DCMI_HandleTypeDef* hdcmi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdcmi);

  /* NOTE : This function should not be modified; when the callback is needed,
            the HAL_DCMI_MspInit() callback can be implemented in the user file
   */
}

/**
  * @brief  De-initialize the DCMI MSP.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @retval None
  */
__weak void HAL_DCMI_MspDeInit(DCMI_HandleTypeDef* hdcmi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdcmi);

  /* NOTE : This function should not be modified; when the callback is needed,
            the HAL_DCMI_MspDeInit() callback can be implemented in the user file
   */
}

/**
  * @}
  */

/** @defgroup DCMI_Exported_Functions_Group2 IO operation functions
 *  @brief   IO operation functions
 *
@verbatim
 ===============================================================================
                      #####  IO operation functions  #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Configure destination address and data length,
          enable DCMI DMA request and DCMI capture.
      (+) Stop DCMI capture.
      (+) Handle DCMI interrupt request.

    [..]  A set of callbacks is provided:
        (+) HAL_DCMI_ErrorCallback()
        (+) HAL_DCMI_LineEventCallback()
        (+) HAL_DCMI_VsyncEventCallback()
        (+) HAL_DCMI_FrameEventCallback()


@endverbatim
  * @{
  */

/**
  * @brief  Enable DCMI capture in DMA mode.
  * @param  hdcmi:     Pointer to a DCMI_HandleTypeDef structure that contains
  *                    the configuration information for DCMI.
  * @param  DCMI_Mode: DCMI capture mode snapshot or continuous grab.
  * @param  pData:     The destination memory buffer address.
  * @param  Length:    The length of capture to be transferred (in 32-bit words).
  * @note  In case of length larger than 65535 (0xFFFF is the DMA maximum transfer length),
  *        the API uses the end of the destination buffer as a work area: HAL_DCMI_Start_DMA()
  *        initiates a circular DMA transfer from DCMI DR to the ad-hoc work buffer and each
  *        half and complete transfer interrupt triggers a copy from the work buffer to
  *        the final destination pData thru a second DMA channel.
  * @note  Following HAL_DCMI_Init() call, all interruptions are enabled (line end,
  *        frame end, overrun, VSYNC and embedded synchronization error interrupts).
  *        User can disable unwanted interrupts thru __HAL_DCMI_DISABLE_IT() macro
  *        before invoking HAL_DCMI_Start_DMA().
  * @note  For length less than 0xFFFF (DMA maximum transfer length) and in snapshot mode,
  *        frame interrupt is disabled before DMA transfer. FRAME capture flag is checked
  *        in DCMI_DMAXferCplt callback at the end of the DMA transfer. If flag is set,
  *        HAL_DCMI_FrameEventCallback() API is called.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMI_Start_DMA(DCMI_HandleTypeDef* hdcmi, uint32_t DCMI_Mode, uint32_t pData, uint32_t Length)
{
  uint32_t circular_copy_length = 0;

  /* Check capture parameter */
  assert_param(IS_DCMI_CAPTURE_MODE(DCMI_Mode));

  /* Process Locked */
  __HAL_LOCK(hdcmi);

  /* Lock the DCMI peripheral state */
  hdcmi->State = HAL_DCMI_STATE_BUSY;

  /* Configure the DCMI Mode and enable the DCMI IP at the same time */
  MODIFY_REG(hdcmi->Instance->CR, (DCMI_CR_CM|DCMI_CR_ENABLE), (DCMI_Mode|DCMI_CR_ENABLE));

  /* Set the DMA conversion complete callback */
  hdcmi->DMA_Handle->XferCpltCallback = DCMI_DMAXferCplt;

  /* Set the DMA error callback */
  hdcmi->DMA_Handle->XferErrorCallback = DCMI_DMAError;

  if(Length <= 0xFFFF)
  {
    hdcmi->XferCount = 0; /* Mark as direct transfer from DCMI_DR register to final destination buffer */

    /* Enable the DMA channel */
    if (HAL_DMA_Start_IT(hdcmi->DMA_Handle, (uint32_t)&hdcmi->Instance->DR, (uint32_t)pData, Length) != HAL_OK)
    {
      /* Update error code */
      hdcmi->ErrorCode |= HAL_DCMI_ERROR_DMA;

      /* Set state back to Ready */
      hdcmi->State = HAL_DCMI_STATE_READY;

      /* Process Unlocked */
      __HAL_UNLOCK(hdcmi);

       return HAL_ERROR;
    }
  }
  else /* Capture length is longer than DMA maximum transfer size */
  {
     /* Set DMA in circular mode */
    hdcmi->DMA_Handle->Init.Mode = DMA_CIRCULAR;

    /* Set the DMA half transfer complete callback */
    hdcmi->DMA_Handle->XferHalfCpltCallback = DCMI_DMAHalfXferCplt;

    /* Initialize transfer parameters */
    hdcmi->XferSize = Length;  /* Store the complete transfer length in DCMI handle */
    hdcmi->pBuffPtr = pData;   /* Final destination buffer pointer */

    circular_copy_length = DCMI_TransferSize(Length);

    /* Check if issue in intermediate length computation */
    if (circular_copy_length == 0)
    {
      /* Set state back to Ready */
      hdcmi->State = HAL_DCMI_STATE_READY;

      /* Process Unlocked */
      __HAL_UNLOCK(hdcmi);

       return HAL_ERROR;
    }

    /* Store the number of half - intermediate buffer copies needed */
    hdcmi->XferCount = 2 * ((Length / circular_copy_length) - 1);
    /* Store the half-buffer copy length */
    hdcmi->HalfCopyLength = circular_copy_length / 2;

    /* DCMI DR samples in circular mode will be copied
       at the end of the final buffer.
       Now compute the circular buffer start address. */
    /* Start by pointing at the final buffer */
    hdcmi->pCircularBuffer = pData;
    /* Update pCircularBuffer in "moving" at the end of the final
       buffer, don't forger to convert in bytes to compute exact address */
   hdcmi->pCircularBuffer +=  4 * (((Length / circular_copy_length) - 1) * circular_copy_length);

    /* Initiate the circular DMA transfer from DCMI IP to final buffer end */
    if ( HAL_DMA_Start_IT(hdcmi->DMA_Handle, (uint32_t)&hdcmi->Instance->DR, (uint32_t)hdcmi->pCircularBuffer, circular_copy_length) != HAL_OK)
    {
      /* Update error code */
      hdcmi->ErrorCode |= HAL_DCMI_ERROR_DMA;

      /* Set state back to Ready */
      hdcmi->State = HAL_DCMI_STATE_READY;

      /* Process Unlocked */
      __HAL_UNLOCK(hdcmi);

       return HAL_ERROR;
    }
  }

  /* Enable Capture */
  SET_BIT(hdcmi->Instance->CR, DCMI_CR_CAPTURE);

  /* Release Lock */
  __HAL_UNLOCK(hdcmi);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Disable DCMI capture in DMA mode.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMI_Stop(DCMI_HandleTypeDef* hdcmi)
{
  uint32_t tickstart = 0;

  /* Process locked */
  __HAL_LOCK(hdcmi);

  /* Lock the DCMI peripheral state */
  hdcmi->State = HAL_DCMI_STATE_BUSY;

  /* Disable Capture */
  CLEAR_BIT(hdcmi->Instance->CR, DCMI_CR_CAPTURE);

  /* Get tick */
  tickstart = HAL_GetTick();

  /* Check if the DCMI capture is effectively disabled */
  while((hdcmi->Instance->CR & DCMI_CR_CAPTURE) != 0)
  {
    if((HAL_GetTick() - tickstart ) > DCMI_TIMEOUT_STOP)
    {
      /* Update error code */
      hdcmi->ErrorCode |= HAL_DCMI_ERROR_TIMEOUT;

      /* Change DCMI state */
      hdcmi->State = HAL_DCMI_STATE_READY;

      /* Process Unlocked */
      __HAL_UNLOCK(hdcmi);

      return HAL_TIMEOUT;
    }
  }

  /* Disable the DMA */
  HAL_DMA_Abort(hdcmi->DMA_Handle);

  /* Disable DCMI IP */
  __HAL_DCMI_DISABLE(hdcmi);

  /* Change DCMI state */
  hdcmi->State = HAL_DCMI_STATE_READY;

  /* Process Unlocked */
  __HAL_UNLOCK(hdcmi);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Suspend DCMI capture.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMI_Suspend(DCMI_HandleTypeDef* hdcmi)
{
  uint32_t tickstart = 0;

  /* Process locked */
  __HAL_LOCK(hdcmi);

  if(hdcmi->State == HAL_DCMI_STATE_BUSY)
  {
    /* Change DCMI state */
    hdcmi->State = HAL_DCMI_STATE_SUSPENDED;

    /* Disable Capture */
    CLEAR_BIT(hdcmi->Instance->CR, DCMI_CR_CAPTURE);

    /* Get tick */
    tickstart = HAL_GetTick();

    /* Check if the DCMI capture is effectively disabled */
    while((hdcmi->Instance->CR & DCMI_CR_CAPTURE) != 0)
    {
      if((HAL_GetTick() - tickstart ) > DCMI_TIMEOUT_STOP)
      {
        /* Update error code */
        hdcmi->ErrorCode |= HAL_DCMI_ERROR_TIMEOUT;

        /* Change DCMI state */
        hdcmi->State = HAL_DCMI_STATE_READY;

        /* Process Unlocked */
        __HAL_UNLOCK(hdcmi);

        return HAL_TIMEOUT;
      }
    }
  }

  /* Process Unlocked */
  __HAL_UNLOCK(hdcmi);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Resume DCMI capture.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMI_Resume(DCMI_HandleTypeDef* hdcmi)
{
  /* Process locked */
  __HAL_LOCK(hdcmi);

  if(hdcmi->State == HAL_DCMI_STATE_SUSPENDED)
  {
    /* Change DCMI state */
    hdcmi->State = HAL_DCMI_STATE_BUSY;

    /* Enable Capture */
    SET_BIT(hdcmi->Instance->CR, DCMI_CR_CAPTURE);
  }

  /* Process Unlocked */
  __HAL_UNLOCK(hdcmi);

  return HAL_OK;
}

/**
  * @brief  Handle DCMI interrupt request.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for the DCMI.
  * @retval None
  */
void HAL_DCMI_IRQHandler(DCMI_HandleTypeDef *hdcmi)
{
  uint32_t misflags = READ_REG(hdcmi->Instance->MISR);

  /* Synchronization error interrupt management *******************************/
  if ((misflags & DCMI_MIS_ERR_MIS) != RESET)
  {
    /* Clear the Synchronization error flag */
    __HAL_DCMI_CLEAR_FLAG(hdcmi, DCMI_FLAG_ERRRI);

    /* Update error code */
    hdcmi->ErrorCode |= HAL_DCMI_ERROR_SYNC;
  }

  /* Overflow interrupt management ********************************************/
  if ((misflags & DCMI_MIS_OVR_MIS) != RESET)
  {
    /* Clear the Overflow flag */
    __HAL_DCMI_CLEAR_FLAG(hdcmi, DCMI_FLAG_OVRRI);

    /* Update error code */
    hdcmi->ErrorCode |= HAL_DCMI_ERROR_OVR;
  }

  if (hdcmi->ErrorCode != HAL_DCMI_ERROR_NONE)
  {
    /* Change DCMI state */
    hdcmi->State = HAL_DCMI_STATE_READY;

    /* Set the overflow callback */
    hdcmi->DMA_Handle->XferAbortCallback = DCMI_DMAError;

    /* Abort the DMA Transfer */
    HAL_DMA_Abort_IT(hdcmi->DMA_Handle);

    /* Error Callback */
    HAL_DCMI_ErrorCallback(hdcmi);
  }

  /* Line Interrupt management ************************************************/
  if ((misflags & DCMI_MIS_LINE_MIS) != RESET)
  {
    /* Clear the Line interrupt flag */
    __HAL_DCMI_CLEAR_FLAG(hdcmi, DCMI_FLAG_LINERI);

    /* Line interrupt Callback */
    HAL_DCMI_LineEventCallback(hdcmi);
  }

  /* VSYNC interrupt management ***********************************************/
  if ((misflags & DCMI_MIS_VSYNC_MIS) != RESET)
  {
    /* Clear the VSYNC flag */
    __HAL_DCMI_CLEAR_FLAG(hdcmi, DCMI_FLAG_VSYNCRI);

    /* VSYNC Callback */
    HAL_DCMI_VsyncEventCallback(hdcmi);
  }

  /* End of Frame interrupt management ****************************************/
  if ((misflags & DCMI_MIS_FRAME_MIS) != RESET)
  {
    /* Disable the Line interrupt when using snapshot mode */
    if ((hdcmi->Instance->CR & DCMI_CR_CM) == DCMI_MODE_SNAPSHOT)
    {
      __HAL_DCMI_DISABLE_IT(hdcmi, DCMI_IT_LINE|DCMI_IT_VSYNC|DCMI_IT_ERR|DCMI_IT_OVR);
      /* Change the DCMI state */
      hdcmi->State = HAL_DCMI_STATE_READY;
    }

    /* Clear the End of Frame flag */
    __HAL_DCMI_CLEAR_FLAG(hdcmi, DCMI_FLAG_FRAMERI);

    /* End of Frame Callback */
    HAL_DCMI_FrameEventCallback(hdcmi);
  }
}

/**
  * @brief  Error DCMI callback.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @retval None
  */
__weak void HAL_DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdcmi);

  /* NOTE : This function should not be modified; when the callback is needed,
            the HAL_DCMI_ErrorCallback() callback can be implemented in the user file.
   */
}

/**
  * @brief  Line Event callback.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @retval None
  */
__weak void HAL_DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdcmi);

  /* NOTE : This function should not be modified; when the callback is needed,
            the HAL_DCMI_LineEventCallback() callback can be implemented in the user file.
   */
}

/**
  * @brief  VSYNC Event callback.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @retval None
  */
__weak void HAL_DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdcmi);

  /* NOTE : This function should not be modified; when the callback is needed,
            the HAL_DCMI_VsyncEventCallback() callback can be implemented in the user file.
   */
}

/**
  * @brief  Frame Event callback.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @retval None
  */
__weak void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdcmi);

  /* NOTE : This function should not be modified; when the callback is needed,
            the HAL_DCMI_FrameEventCallback() callback can be implemented in the user file.
   */
}
/**
  * @}
  */


/** @defgroup DCMI_Exported_Functions_Group3 Peripheral Control functions
 *  @brief    Peripheral Control functions
 *
@verbatim
 ===============================================================================
                    ##### Peripheral Control functions #####
 ===============================================================================
[..]  This section provides functions allowing to:
      (+) Configure the crop feature.
      (+) Enable/Disable the crop feature.
            (+) Configure the synchronization delimiters unmasks.
            (+) Enable/Disable user-specified DCMI interrupts.

@endverbatim
  * @{
  */

/**
  * @brief  Configure the DCMI crop window coordinates.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @param  X0:    DCMI window crop window X offset (number of pixels clocks to count before the capture).
  * @param  Y0:    DCMI window crop window Y offset (image capture starts with this line number, previous
  *                line data are ignored).
  * @param  XSize: DCMI crop window horizontal size (in number of pixels per line).
  * @param  YSize: DCMI crop window vertical size (in lines count).
  * @note For all the parameters, the actual value is the input data + 1 (e.g. YSize = 0x0 means 1 line,
  *       YSize = 0x1 means 2 lines, ...)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMI_ConfigCrop(DCMI_HandleTypeDef *hdcmi, uint32_t X0, uint32_t Y0, uint32_t XSize, uint32_t YSize)
{
  /* Check the parameters */
  assert_param(IS_DCMI_WINDOW_COORDINATE(X0));
  assert_param(IS_DCMI_WINDOW_HEIGHT(Y0));
  assert_param(IS_DCMI_WINDOW_COORDINATE(XSize));
  assert_param(IS_DCMI_WINDOW_COORDINATE(YSize));

  /* Process Locked */
  __HAL_LOCK(hdcmi);

  /* Lock the DCMI peripheral state */
  hdcmi->State = HAL_DCMI_STATE_BUSY;

  /* Configure CROP */
  MODIFY_REG(hdcmi->Instance->CWSIZER, (DCMI_CWSIZE_VLINE|DCMI_CWSIZE_CAPCNT), (XSize | (YSize << DCMI_POSITION_CWSIZE_VLINE)));
  MODIFY_REG(hdcmi->Instance->CWSTRTR, (DCMI_CWSTRT_VST|DCMI_CWSTRT_HOFFCNT), (X0 | (Y0 << DCMI_POSITION_CWSTRT_VST)));

  /* Initialize the DCMI state*/
  hdcmi->State  = HAL_DCMI_STATE_READY;

  /* Process Unlocked */
  __HAL_UNLOCK(hdcmi);

  return HAL_OK;
}

/**
  * @brief  Disable the crop feature.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMI_DisableCrop(DCMI_HandleTypeDef *hdcmi)
{
  /* Process Locked */
  __HAL_LOCK(hdcmi);

  /* Lock the DCMI peripheral state */
  hdcmi->State = HAL_DCMI_STATE_BUSY;

  /* Disable DCMI Crop feature */
  CLEAR_BIT(hdcmi->Instance->CR, DCMI_CR_CROP);

  /* Change the DCMI state*/
  hdcmi->State = HAL_DCMI_STATE_READY;

  /* Process Unlocked */
  __HAL_UNLOCK(hdcmi);

  return HAL_OK;
}

/**
  * @brief  Enable the crop feature.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMI_EnableCrop(DCMI_HandleTypeDef *hdcmi)
{
  /* Process Locked */
  __HAL_LOCK(hdcmi);

  /* Lock the DCMI peripheral state */
  hdcmi->State = HAL_DCMI_STATE_BUSY;

  /* Enable DCMI Crop feature */
  SET_BIT(hdcmi->Instance->CR, DCMI_CR_CROP);

  /* Change the DCMI state*/
  hdcmi->State = HAL_DCMI_STATE_READY;

  /* Process Unlocked */
  __HAL_UNLOCK(hdcmi);

  return HAL_OK;
}

/**
  * @brief  Set embedded synchronization delimiters unmasks.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @param  SyncUnmask: pointer to a DCMI_SyncUnmaskTypeDef structure that contains
  *                the embedded synchronization delimiters unmasks.
  * @retval HAL status
  */
HAL_StatusTypeDef  HAL_DCMI_ConfigSyncUnmask(DCMI_HandleTypeDef *hdcmi, DCMI_SyncUnmaskTypeDef *SyncUnmask)
{
  /* Process Locked */
  __HAL_LOCK(hdcmi);

  /* Lock the DCMI peripheral state */
  hdcmi->State = HAL_DCMI_STATE_BUSY;

  /* Write DCMI embedded synchronization unmask register */
    hdcmi->Instance->ESUR = (((uint32_t)SyncUnmask->FrameStartUnmask) |\
                             ((uint32_t)SyncUnmask->LineStartUnmask << DCMI_POSITION_ESUR_LSU)|\
                             ((uint32_t)SyncUnmask->LineEndUnmask << DCMI_POSITION_ESUR_LEU)|\
                             ((uint32_t)SyncUnmask->FrameEndUnmask << DCMI_POSITION_ESUR_FEU));

  /* Change the DCMI state*/
  hdcmi->State = HAL_DCMI_STATE_READY;

  /* Process Unlocked */
  __HAL_UNLOCK(hdcmi);

  return HAL_OK;
}




/**
  * @}
  */

/** @defgroup DCMI_Exported_Functions_Group4 Peripheral State functions
 *  @brief    Peripheral State functions
 *
@verbatim
 ===============================================================================
               ##### Peripheral State and Errors functions #####
 ===============================================================================
    [..]
    This subsection provides functions allowing to
      (+) Check the DCMI state.
      (+) Get the specific DCMI error flag.

@endverbatim
  * @{
  */

/**
  * @brief  Return the DCMI state.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @retval HAL state
  */
HAL_DCMI_StateTypeDef HAL_DCMI_GetState(DCMI_HandleTypeDef *hdcmi)
{
  return hdcmi->State;
}

/**
* @brief  Return the DCMI error code.
* @param  hdcmi : pointer to a DCMI_HandleTypeDef structure that contains
  *               the configuration information for DCMI.
* @retval DCMI Error Code
*/
uint32_t HAL_DCMI_GetError(DCMI_HandleTypeDef *hdcmi)
{
  return hdcmi->ErrorCode;
}

/**
  * @}
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup DCMI_Private_Functions DCMI Private Functions
  * @{
  */

  /**
  * @brief  DMA conversion complete callback.
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @note When the size of the frame being captured by the DCMI peripheral is
  *       larger than 0xFFFF (DMA maximum transfer length), this API initiates
  *       another DMA transfer to copy the second half of the work buffer
  *       associated to the DCMI handle to the final destination buffer.
  * @retval None
  */
static void DCMI_DMAXferCplt(DMA_HandleTypeDef *hdma)
{
  uint32_t loop_length       = 0;     /* transfer length  */
  uint32_t * tmpBuffer_Dest  = NULL;
  uint32_t * tmpBuffer_Orig  = NULL;

  DCMI_HandleTypeDef* hdcmi = ( DCMI_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;


  if(hdcmi->XferCount != 0)
  {
    /* Manage second half buffer copy in case of big transfer */

    /* Decrement half-copies counter */
    hdcmi->XferCount--;

    /* Point at DCMI final destination */
    tmpBuffer_Dest = (uint32_t *)hdcmi->pBuffPtr;

    /* Point at DCMI circular buffer mid-location */
    tmpBuffer_Orig = (uint32_t *)hdcmi->pCircularBuffer;
    tmpBuffer_Orig +=  hdcmi->HalfCopyLength;

    /* copy half the buffer size */
    loop_length = hdcmi->HalfCopyLength;

    /* Save next entry to write at next half DMA transfer interruption */
    hdcmi->pBuffPtr += (uint32_t) loop_length*4;
    hdcmi->XferSize -= hdcmi->HalfCopyLength;

    /* Data copy from work buffer to final destination buffer */
    /* Enable the DMA Channel */
    if (HAL_DMA_Start_IT(hdcmi->DMAM2M_Handle, (uint32_t) tmpBuffer_Orig, (uint32_t) tmpBuffer_Dest, loop_length) != HAL_OK)
    {
      /* Update error code */
      hdcmi->ErrorCode |= HAL_DCMI_ERROR_DMA;

      /* Change DCMI state */
      hdcmi->State = HAL_DCMI_STATE_READY;

      /* Process Unlocked */
      __HAL_UNLOCK(hdcmi);

      /* Error Callback */
      HAL_DCMI_ErrorCallback(hdcmi);
    }
  }
  else
  {
    /* if End of frame IT is disabled */
    if((hdcmi->Instance->IER & DCMI_IT_FRAME) == RESET)
    {
      /* If End of Frame flag is set */
      if(__HAL_DCMI_GET_FLAG(hdcmi, DCMI_FLAG_FRAMERI) != RESET)
      {
        /* Clear the End of Frame flag */
        __HAL_DCMI_CLEAR_FLAG(hdcmi, DCMI_FLAG_FRAMERI);

        /* When snapshot mode, disable Vsync, Error and Overrun interrupts */
        if((hdcmi->Instance->CR & DCMI_CR_CM) == DCMI_MODE_SNAPSHOT)
        {
          /* Disable the Vsync, Error and Overrun interrupts */
          __HAL_DCMI_DISABLE_IT(hdcmi, DCMI_IT_LINE | DCMI_IT_VSYNC | DCMI_IT_ERR | DCMI_IT_OVR);

          hdcmi->State = HAL_DCMI_STATE_READY;

          /* Process Unlocked */
          __HAL_UNLOCK(hdcmi);
        }

        /* FRAME Callback */
        HAL_DCMI_FrameEventCallback(hdcmi);
      }
    }
  }
}


  /**
  * @brief  DMA Half Transfer complete callback.
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @note When the size of the frame being captured by the DCMI peripheral is
  *       larger than 0xFFFF (DMA maximum transfer length), this API initiates
  *       another DMA transfer to copy the first half of the work buffer
  *       associated to the DCMI handle to the final destination buffer.
  * @retval None
  */
static void DCMI_DMAHalfXferCplt(DMA_HandleTypeDef *hdma)
{
  uint32_t loop_length       = 0;     /* transfer length  */
  uint32_t * tmpBuffer_Dest  = NULL;
  uint32_t * tmpBuffer_Orig  = NULL;

  DCMI_HandleTypeDef* hdcmi = ( DCMI_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;

  if(hdcmi->XferCount != 0)
  {
    /* Manage first half buffer copy in case of big transfer */

    /* Decrement half-copies counter */
    hdcmi->XferCount--;

    /* Point at DCMI final destination */
    tmpBuffer_Dest = (uint32_t *)hdcmi->pBuffPtr;

    /* Point at DCMI circular buffer start */
    tmpBuffer_Orig = (uint32_t *)hdcmi->pCircularBuffer;

    /* copy half the buffer size */
    loop_length = hdcmi->HalfCopyLength;

    /* Save next entry to write at next DMA transfer interruption */
    hdcmi->pBuffPtr += (uint32_t) loop_length*4;
    hdcmi->XferSize -= hdcmi->HalfCopyLength;

    /* Data copy from work buffer to final destination buffer */
    /* Enable the DMA Channel */
    if (HAL_DMA_Start_IT(hdcmi->DMAM2M_Handle, (uint32_t) tmpBuffer_Orig, (uint32_t) tmpBuffer_Dest, loop_length) != HAL_OK)
    {
      /* Update error code */
      hdcmi->ErrorCode |= HAL_DCMI_ERROR_DMA;

      /* Change DCMI state */
      hdcmi->State = HAL_DCMI_STATE_READY;

      /* Process Unlocked */
      __HAL_UNLOCK(hdcmi);

      /* Error Callback */
      HAL_DCMI_ErrorCallback(hdcmi);
    }
  }
}

/**
  * @brief  DMA error callback
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void DCMI_DMAError(DMA_HandleTypeDef *hdma)
{
    DCMI_HandleTypeDef* hdcmi = ( DCMI_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;

    /* Update error code */
    hdcmi->ErrorCode |= HAL_DCMI_ERROR_DMA;

    /* Change DCMI state */
    hdcmi->State = HAL_DCMI_STATE_READY;

    /* Error Callback */
    HAL_DCMI_ErrorCallback(hdcmi);
}

/**
  * @brief Sub-buffers transfer size computation.
  * @note In the case of a frame size larger than the maximum DMA transfer length (0xFFFF),
  *       the tranfer from DCMI DR register to the final output buffer is carried out by a sequence
  *       of intermediate sub-copies to temporary buffers of size less than 0xFFFF.
  *       To optimize the number of DMA transfers, the API computes the temporary buffer
  *       size so that the latter is an even number less than 0xFFFF, that divides the final
  *       buffer size and is as high as possible. The API implements a sub-optimum solution for
  *       complexity's sake.
  * @note InputSize MUST be even.
  * @param  InputSize: full buffer size (in 32-bit words)
  * @retval Transfer size (in 32-bit words)
  */
static uint32_t DCMI_TransferSize(uint32_t InputSize)
{
  uint32_t j = 1;
  uint32_t temp = InputSize;
  uint32_t aPrime[NPRIME] = {0};
  uint32_t output = 2; /* Want a result which is an even number */


  /* Develop InputSize in product of prime numbers */

  while (j < NPRIME)
  {
    if (temp < PrimeArray[j])
    {
      break;
    }
    while ((temp % PrimeArray[j]) == 0)
    {
      aPrime[j]++;
      temp /= PrimeArray[j];
    }
    j++;
  }

  /*  Search for the biggest even divisor less or equal to 0xFFFE = 65534 */
  aPrime[1] -= 1; /* output is initialized to 2, so don't count dividor 2 twice */

   /*  The algorithm below yields a sub-optimal solution
       but in an acceptable time.  */
    j =  NPRIME-1;
  while ((j > 0) &&  (output <= 0xFFFE))
  {
    while (aPrime[j] >0)
    {
      if (output * PrimeArray[j] > 0xFFFE)
      {
        break;
      }
      else
      {
        output *= PrimeArray[j];
        aPrime[j]--;
      }
    }
    j--;
  }



  return output;
}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* STM32L496xx || STM32L4A6xx */

#endif /* HAL_DCMI_MODULE_ENABLED */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


#include "../inc/usbh_hid_touch.h"
#include "../inc/usbh_hid_parser.h"

static USBH_StatusTypeDef USBH_HID_TouchDecode(USBH_HandleTypeDef *pHost);

// USBH_HID_TOUCH_Private_Variables
tagHID_TouchInfo    touchInfo;
u8 touchReportData[5];

// Structures defining how to access items in a HID mouse report
// Access button 1 state.
// TOPWAY、迪文
static HID_Report_ItemTypedef button={
    (uint8_t *)touchReportData+0,     // data
    8,                                 // size
    6,                                 // shift
    0,                                 // count (only for array items)
    0,                                 // signed?
    0,                                 // min value read can return
    0x1,                             // max value read can return
    0,                                 // min value device can report
    0xFF,                             // max value device can report
    1,                              // resolution
    0                                // 无效
};

// Access x coordinate change
static HID_Report_ItemTypedef axisX={
    (uint8_t *)touchReportData+1,     // data
    16,                                // size
    0,                                // shift
    0,                                 // count (only for array items)
    0,                                 // signed?
    0,                                 // min value read can return
    0xFFFF,                            // max value read can return*/
    0,                                 // min vale device can report
    0xFFFF,                            // max value device can report
    1,                              // resolution
    0                                // 无效
};

// Access y coordinate change.
static HID_Report_ItemTypedef axisY={
    (uint8_t *)touchReportData+3,     // data
    16,                             // size
    0,                                 // shift
    0,                                 // count (only for array items)
    0,                                 // signed?
    0,                                 // min value read can return
    0xFFFF,                            // max value read can return
    0,                                 // min vale device can report
    0xFFFF,                            // max value device can report
    1,                              // resolution
    0                                // 无效
};

// 步科
static HID_Report_ItemTypedef buttonKinco={
    (uint8_t *)touchReportData+0,     // data
    8,                                 // size
    0,                                 // shift
    0,                                 // count (only for array items)
    0,                                 // signed?
    0,                                 // min value read can return
    1,                                 // max value read can return
    0,                                 // min value device can report
    0xFF,                             // max value device can report
    1,                              // resolution
    0x1                                // 只使用数据的最低位
};

HID_Report_ItemTypedef *pButton, *pAxisX, *pAxisY;

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
USBH_StatusTypeDef USBH_HID_TouchInit(USBH_HandleTypeDef *pHost)
{
    HID_HandleTypeDef *HID_Handle =  (HID_HandleTypeDef *) pHost->pActiveClass->pData;
    USBH_StatusTypeDef res = USBH_OK;
    extern void USBH_SetTouch(void *pHost);

    touchInfo.wX = 0;
    touchInfo.wY = 0;
    touchInfo.bButton = 0;

    if(HID_Handle->length > sizeof(touchReportData))
    {
        HID_Handle->length = sizeof(touchReportData);
    }

    HID_Handle->pData = (uint8_t *)touchReportData; //
    fifo_init(&HID_Handle->fifo, pHost->device.Data, HID_QUEUE_SIZE * sizeof(touchReportData)); // FIFO的BUF指向驱动的缓冲

    if((0x6000 == pHost->device.DevDesc.idProduct) && (0x14E1 == pHost->device.DevDesc.idVendor)) // TOPWAY
    {
        pButton = &button;
        pAxisX = &axisX;
        pAxisY = &axisY;

    }
    else if((0x1F01 == pHost->device.DevDesc.idProduct) && (0xC251 == pHost->device.DevDesc.idVendor)) // 步科
    {
        pButton = &buttonKinco;
        pAxisX = &axisX;
        pAxisY = &axisY;
    }
    else
    {
        pButton = &button;
        pAxisX = &axisX;
        pAxisY = &axisY;
        printf("\r\n: erro : usbs%02x : HID touch device do not support, try the default configuration.", pHost->id);
        res = USBH_NOT_SUPPORTED;
    }

    USBH_SetTouch((void*)pHost);

    return (res);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
USBH_StatusTypeDef USBH_HID_TouchDeInit(USBH_HandleTypeDef *pHost)
{
    extern void USBH_ResetTouch(void *pHost);

    USBH_ResetTouch((void*)pHost);

    return USBH_OK;
}


// ============================================================================
// 功能：从FIFO中获取数据
// 参数：
// 返回：
// 备注：
// ============================================================================
tagHID_TouchInfo *_USBH_HID_TouchGetData(USBH_HandleTypeDef *pHost)
{
    if(USBH_HID_TouchDecode(pHost)== USBH_OK)
    {
        return (&touchInfo);
    }
    else
    {
        return (NULL);
    }
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static USBH_StatusTypeDef USBH_HID_TouchDecode(USBH_HandleTypeDef *pHost)
{
    HID_HandleTypeDef *HID_Handle = (HID_HandleTypeDef *) pHost->pActiveClass->pData;

    if(HID_Handle->length == 0)
    {
        return (USBH_FAIL);
    }

    // fill report
    if(fifo_read(&HID_Handle->fifo, &touchReportData, HID_Handle->length) == HID_Handle->length)
    {
        // decode report
        touchInfo.bButton = (u8)HID_ReadItem((HID_Report_ItemTypedef*)pButton, 0);
        touchInfo.wX = (u16)HID_ReadItem((HID_Report_ItemTypedef*)pAxisX, 0);
        touchInfo.wY = (u16)HID_ReadItem((HID_Report_ItemTypedef*)pAxisY, 0);

        return (USBH_OK);
    }

    return (USBH_FAIL);
}
// ============================================================================
// 功能：手动同步
// 参数：
// 返回：
// 备注：写入到FIFO
// ============================================================================
s32 USBH_HID_TouchManualSync(USBH_HandleTypeDef *pHost)
{
    USBH_URBStateTypeDef res;
    s64 time;
    u16 i, retrys = 0;
    HID_HandleTypeDef *handle =  (HID_HandleTypeDef *) pHost->pActiveClass->pData;


    time = DJY_GetSysTime();
    handle->timer = pHost->Timer;

    while(1)
    {
        i = 0;
        USBH_InterruptReceiveData(pHost,
                                  handle->pData,
                                  handle->length,
                                  handle->InPipe);

        while(i++ < 300) // TODO: 有一些器件的端点回复速度较慢，如果不给足等待时间，会导致逻辑上的额外中断
        {
            res = USBH_LL_GetURBState(pHost , handle->InPipe);

            if(USBH_URB_DONE == res)
            {
                #if 0 // 调试打印
                {
                    u8 k;

                    time = DJY_GetSysTime() - time;
                    printf("touch original:");
                    printf("<%dus> <%d> ", (u32)time, retrys);

                    printf("< ");
                    for(k = 0; k < handle->length; k++)
                    {
                        printf("%02x ", handle->pData[k]);
                    }
                    printf(">");
                    printf("   ");

                }
                #endif
                fifo_write(&handle->fifo, handle->pData, handle->length);
                return (0); // 成功

            }
            else if(USBH_URB_STALL == res)
            {
                printf("\r\n: erro : usbs%02x : HID touch sync stalled.", pHost->id);
                return (-1);
            }
        }

        // 一次读不到数据，多次读，至于为何如此，可能与中断型的端点有关系
        if(retrys < 10)
        {
            if((pHost->Timer - handle->timer) > handle->poll) // 校验逻辑，防止第一次读时间过长
            {
                time = DJY_GetSysTime() - time;
//                printf("\r\n: erro : usbs%02x : HID touch sync out of time(%d.us).", pHost->id, (u32)time);
                retrys = 10; // 再多尝试一次
            }

            retrys++;

        }
        else
        {
            time = DJY_GetSysTime() - time;
            // USBH_UsrLog ("USB #%04x stack service : debug : HID touch sync retry %d times <%dus>, but no data.", pHost->id, retrys, (u32)time);
            break;
        }
    }

    return (-1);
}
// ============================================================================
// 功能：获取触摸屏的数据
// 参数：
// 返回：-1 -- 没有数据； 0 -- 有数据；
// 备注：
// ============================================================================
s32 USBH_HID_TouchGetData(USBH_HandleTypeDef *pHost, u8 *pButton, u32 *pX, u32 *pY)
{
    tagHID_TouchInfo *touch;

    if((!pHost) || (!pButton) || (!pX) || (!pY))
        return (-1);


    if(USBH_HID_TouchManualSync(pHost))
        return (-1); // 没有数据

    touch = _USBH_HID_TouchGetData(pHost);
    if(!touch)
        return (-1);

    *pButton = touch->bButton;
    *pX = ((u32)touch->wX & 0x0000FFFF);
    *pY = ((u32)touch->wY & 0x0000FFFF);
    return (0);
}
// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================


#include "usb_record.h"
#include <stdio.h>
#include <string.h>
#include "./stm32_usb_host_library/config/usbh_conf.h"
tagInt intRecord;
u8 intoEndless = 0;

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
void RecordShow(void)
{
    USBH_UsrLog("\r\nUSB record:\r\n");
    USBH_UsrLog("  total: %d\r\n", intRecord.dwTotal);
    USBH_UsrLog("  INCOMPISOOUT: %d\r\n", intRecord.dwINCOMPISOOUT);
    USBH_UsrLog("  IISOIXFR: %d\r\n", intRecord.dwIISOIXFR);
    USBH_UsrLog("  PTXFE: %d\r\n", intRecord.dwPTXFE);
    USBH_UsrLog("  MMIS: %d\r\n", intRecord.dwMMIS);
    USBH_UsrLog("  DISCINT: %d\r\n", intRecord.dwDISCINT);
    USBH_UsrLog("  HPRTINT: %d\r\n", intRecord.dwHPRTINT);
    USBH_UsrLog("  SOF: %d\r\n", intRecord.dwSOF);
    USBH_UsrLog("  HCINT: %d\r\n", intRecord.dwHCINT);
    USBH_UsrLog("  RXFLVL: %d\r\n", intRecord.dwRXFLVL);

    USBH_UsrLog("  IN : total %d\r\n", intRecord.in.dwTotal);
    USBH_UsrLog("  IN : AHBERR %d\r\n", intRecord.in.dwAHBERR);
    USBH_UsrLog("  IN : ACK %d\r\n", intRecord.in.dwACK);
    USBH_UsrLog("  IN : STALL %d\r\n", intRecord.in.dwSTALL);
    USBH_UsrLog("  IN : DTERR %d\r\n", intRecord.in.dwDTERR);
    USBH_UsrLog("  IN : FRMOR %d\r\n", intRecord.in.dwFRMOR);
    USBH_UsrLog("  IN : XFRC %d\r\n", intRecord.in.dwXFRC);
    USBH_UsrLog("  IN : CHH %d\r\n", intRecord.in.dwCHH);
    USBH_UsrLog("  IN : TXERR %d\r\n", intRecord.in.dwTXERR);
    USBH_UsrLog("  IN : NAK %d\r\n", intRecord.in.dwNAK);

    USBH_UsrLog("  OUT: total %d\r\n", intRecord.out.dwTotal);
    USBH_UsrLog("  OUT: AHBERR %d\r\n", intRecord.out.dwAHBERR);
    USBH_UsrLog("  OUT: ACK %d\r\n", intRecord.out.dwACK);
    USBH_UsrLog("  OUT: NYET %d\r\n", intRecord.out.dwNYET);
    USBH_UsrLog("  OUT: FRMOR %d\r\n", intRecord.out.dwFRMOR);
    USBH_UsrLog("  OUT: XFRC %d\r\n", intRecord.out.dwXFRC);
    USBH_UsrLog("  OUT: STALL %d\r\n", intRecord.out.dwSTALL);
    USBH_UsrLog("  OUT: NAK %d\r\n", intRecord.out.dwNAK);
    USBH_UsrLog("  OUT: TXERR %d\r\n", intRecord.out.dwTXERR);
    USBH_UsrLog("  OUT: DTERR %d\r\n", intRecord.out.dwDTERR);
    USBH_UsrLog("  OUT: CHH %d\r\n", intRecord.out.dwCHH);
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
void RecordClean(void)
{
    memset(&intRecord, 0x0, sizeof(tagInt));
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
void RecordOn(void)
{
    intRecord.state = ON;
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
void RecordOff(void)
{
    intRecord.state = OFF;
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
void RecordControl(char *pOperation)
{
    if(!strcmp(pOperation, "clean"))
    {
        RecordClean();
    }

    if(!strcmp(pOperation, "on"))
    {
        RecordOn();
    }

    if(!strcmp(pOperation, "off"))
    {
        RecordOff();
    }

    if(!strcmp(pOperation, "show"))
    {
        RecordShow();
    }
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
void TEST_POINT(void)
{
    if(intoEndless)
        while(1);
}

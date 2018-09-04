
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
    printf("\r\n: info : usb   : driver's record >>>>");
    printf("\r\n                 total: %d\r\n", intRecord.dwTotal);
    printf("\r\n                 INCOMPISOOUT: %d\r\n", intRecord.dwINCOMPISOOUT);
    printf("\r\n                 IISOIXFR: %d\r\n", intRecord.dwIISOIXFR);
    printf("\r\n                 PTXFE: %d\r\n", intRecord.dwPTXFE);
    printf("\r\n                 MMIS: %d\r\n", intRecord.dwMMIS);
    printf("\r\n                 DISCINT: %d\r\n", intRecord.dwDISCINT);
    printf("\r\n                 HPRTINT: %d\r\n", intRecord.dwHPRTINT);
    printf("\r\n                 SOF: %d\r\n", intRecord.dwSOF);
    printf("\r\n                 HCINT: %d\r\n", intRecord.dwHCINT);
    printf("\r\n                 RXFLVL: %d\r\n", intRecord.dwRXFLVL);

    printf("\r\n                 IN : total %d\r\n", intRecord.in.dwTotal);
    printf("\r\n                 IN : AHBERR %d\r\n", intRecord.in.dwAHBERR);
    printf("\r\n                 IN : ACK %d\r\n", intRecord.in.dwACK);
    printf("\r\n                 IN : STALL %d\r\n", intRecord.in.dwSTALL);
    printf("\r\n                 IN : DTERR %d\r\n", intRecord.in.dwDTERR);
    printf("\r\n                 IN : FRMOR %d\r\n", intRecord.in.dwFRMOR);
    printf("\r\n                 IN : XFRC %d\r\n", intRecord.in.dwXFRC);
    printf("\r\n                 IN : CHH %d\r\n", intRecord.in.dwCHH);
    printf("\r\n                 IN : TXERR %d\r\n", intRecord.in.dwTXERR);
    printf("\r\n                 IN : NAK %d\r\n", intRecord.in.dwNAK);

    printf("\r\n                 OUT: total %d\r\n", intRecord.out.dwTotal);
    printf("\r\n                 OUT: AHBERR %d\r\n", intRecord.out.dwAHBERR);
    printf("\r\n                 OUT: ACK %d\r\n", intRecord.out.dwACK);
    printf("\r\n                 OUT: NYET %d\r\n", intRecord.out.dwNYET);
    printf("\r\n                 OUT: FRMOR %d\r\n", intRecord.out.dwFRMOR);
    printf("\r\n                 OUT: XFRC %d\r\n", intRecord.out.dwXFRC);
    printf("\r\n                 OUT: STALL %d\r\n", intRecord.out.dwSTALL);
    printf("\r\n                 OUT: NAK %d\r\n", intRecord.out.dwNAK);
    printf("\r\n                 OUT: TXERR %d\r\n", intRecord.out.dwTXERR);
    printf("\r\n                 OUT: DTERR %d\r\n", intRecord.out.dwDTERR);
    printf("\r\n                 OUT: CHH %d\r\n", intRecord.out.dwCHH);
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


#include "usb_record.h"
#include <stdio.h>
#include <string.h>

tagInt intRecord;

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
void RecordShow(void)
{
	printf("\r\nUSB record:\r\n");
	printf("  total: %d\r\n", intRecord.dwTotal);
	printf("  INCOMPISOOUT: %d\r\n", intRecord.dwINCOMPISOOUT);
	printf("  IISOIXFR: %d\r\n", intRecord.dwIISOIXFR);
	printf("  PTXFE: %d\r\n", intRecord.dwPTXFE);
	printf("  MMIS: %d\r\n", intRecord.dwMMIS);
	printf("  DISCINT: %d\r\n", intRecord.dwDISCINT);
	printf("  HPRTINT: %d\r\n", intRecord.dwHPRTINT);
	printf("  SOF: %d\r\n", intRecord.dwSOF);
	printf("  HCINT: %d\r\n", intRecord.dwHCINT);
	printf("  RXFLVL: %d\r\n", intRecord.dwRXFLVL);

	printf("  IN : total %d\r\n", intRecord.in.dwTotal);
	printf("  IN : AHBERR %d\r\n", intRecord.in.dwAHBERR);
	printf("  IN : ACK %d\r\n", intRecord.in.dwACK);
	printf("  IN : STALL %d\r\n", intRecord.in.dwSTALL);
	printf("  IN : DTERR %d\r\n", intRecord.in.dwDTERR);
	printf("  IN : FRMOR %d\r\n", intRecord.in.dwFRMOR);
	printf("  IN : XFRC %d\r\n", intRecord.in.dwXFRC);
	printf("  IN : CHH %d\r\n", intRecord.in.dwCHH);
	printf("  IN : TXERR %d\r\n", intRecord.in.dwTXERR);
	printf("  IN : NAK %d\r\n", intRecord.in.dwNAK);

	printf("  OUT: total %d\r\n", intRecord.out.dwTotal);
	printf("  OUT: AHBERR %d\r\n", intRecord.out.dwAHBERR);
	printf("  OUT: ACK %d\r\n", intRecord.out.dwACK);
	printf("  OUT: NYET %d\r\n", intRecord.out.dwNYET);
	printf("  OUT: FRMOR %d\r\n", intRecord.out.dwFRMOR);
	printf("  OUT: XFRC %d\r\n", intRecord.out.dwXFRC);
	printf("  OUT: STALL %d\r\n", intRecord.out.dwSTALL);
	printf("  OUT: NAK %d\r\n", intRecord.out.dwNAK);
	printf("  OUT: TXERR %d\r\n", intRecord.out.dwTXERR);
	printf("  OUT: DTERR %d\r\n", intRecord.out.dwDTERR);
	printf("  OUT: CHH %d\r\n", intRecord.out.dwCHH);
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
void RecordClean(void)
{
	memset(&intRecord, 0x0, sizeof(tagInt));
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
void RecordOn(void)
{
	intRecord.state = ON;
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
void RecordOff(void)
{
	intRecord.state = OFF;
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
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

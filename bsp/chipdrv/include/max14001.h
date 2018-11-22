// =============================================================================

// 文件名     ：Max14001.h
// 模块描述: 芯片Max14001的驱动
// 模块版本: V1.00
// 创建人员:YT
// 创建时间: 2017年11月8日
// =============================================================================

#ifndef _MAX14001_H_
#define _MAX14001_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "OS.h"
#include "spibus.h"

/*Register Map Define*/

#define Max14001_SPI_SPEED      (1*1000*1000)  //设置成5M时clk有问题

#define   ADC     (0x00)  //ADC reading
#define   FADC    (0x01)  //FADC reading
#define   FLAGS   (0x02)  //Latched flags indicate errors and why the FAULT pin was asserted
                          //if the fault is enabled in the FLTEN register.
#define   FLTEN   (0x03)  //Enables fault conditions to assert the FAULT signal.
#define   THL     (0x04)  //User-programmed lower comparator threshold. 低阈值
#define   THU     (0x05)  //User-programmed upper comparator threshold. 高阈值

#define   INRR    (0x06)  //Max14002 only!!
#define   INRT    (0x07)  //Max14002 only!!
#define   INRP    (0x08)  //Max14002 only!!

#define   CFG     (0x09)  //Configuration register controls functions within the MAX14001/MAX14002.
#define   ENBL    (0x0A)  //The ENA bit in the ENBL register enables the inrush and bias current.
#define   ACT     (0x0B)  //Immediate action register.
#define   WEN     (0x0C)  //Write enable register.
#define   FLTV    (0x13)  //FLTEN verification register.
#define   THLV    (0x14)  //THL verification register.
#define   THUV    (0x15)  //THU verification register.

#define   INRRV   (0x16)  //Max14002 only!!
#define   INRTV   (0x17)  //Max14002 only!!
#define   INRPV   (0x18)  //Max14002 only!!

#define   CFGV    (0x18)  //CFG verification register.
#define   ENBLV   (0x18)  //ENBL verification register.

bool_t ModuleInstall_Max14001(void);
bool_t Max14001_Config(u16 thl,u16 thu,struct SPI_Device Max14001_Dev);
u16 Get_Data(u32 port,u16 pin);
u16  Max14001_TxRx(u16 data,u8 addr,u8 rdflag,u32 timeout,struct SPI_Device Max14001_Dev);
void ADC_SAMPLE_UPDATA(void);

//bool_t ModuleInstall_Max14001(char *BusName);
//bool_t Max14001_Config(u16 thl,u16 thu);
//u16 Get_Data(u32 port,u16 pin);
//u16  Max14001_TxRx(u16 data,u8 addr,u8 rdflag,u32 timeout);
//void ADC_SAMPLE_UPDATA(void);


#ifdef __cplusplus
}
#endif

#endif

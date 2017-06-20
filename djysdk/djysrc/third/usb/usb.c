//-----------------------------------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

#include <stdio.h>
#include "./stm32_usb_host_library/core/inc/usbh_core.h"
#include "./stm32_usb_host_library/class/msc/inc/usbh_msc.h"
#include "./stm32_usb_host_library/class/cdc/inc/usbh_cdc.h"
#include "./stm32_usb_host_library/class/custom/inc/usbh_custom.h"
#include "cpu_peri_int_line.h"
#include <int.h>
#include "usb.h"

USBH_HandleTypeDef gUSBHost[2];

static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

//-----------------------------------------------------------------------------
//功能: 设备复位
//参数:
//返回:
//备注: 由驱动实现
//-----------------------------------------------------------------------------
__attribute__((weak)) void USB_DeviceReset(u8 bID)
{
	bID = bID;
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回: NULL -- 控制器不存在
//备注:
//-----------------------------------------------------------------------------
USBH_HandleTypeDef *USBH_HostHandle(u8 bController)
{
	USBH_HandleTypeDef *controller;

	switch(bController)
	{
	case 1 :
		controller = &gUSBHost[0];
		break;
	case 2 :
		controller = &gUSBHost[1];
		break;
	default :
		controller = NULL;
		break;
	}

	return (controller);
}

//-----------------------------------------------------------------------------
//功能: USB0协议栈服务线程
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
u32 USB_2_Thread(void)
{
	USBH_HandleTypeDef *controller;
	s64 time, comsumed;
	static u8 bootup = 0;

	controller = USBH_HostHandle(2);
	if(!controller)
	{
		printf("\r\nUSB #2 stack service QUIT <the controller is not initialized>\r\n");
		return (0);
	}

	// Add Supported Class
	USBH_RegisterClass(controller, USBH_MSC_CLASS); // 注册MSC类
	USBH_RegisterClass(controller, USBH_CDC_CLASS); // 注册CDC类
	USBH_RegisterClass(controller, USBH_CUSTOM_CLASS); // 注册CUSTOM类

	// Start Host Process
	USBH_Start(controller); // 开中断，VBUS上电

	printf("\r\nUSB #2 stack service start to run...\r\n");
	
	USB_DeviceReset(0);

	time = DjyGetSysTime();
	// Run Application (Blocking mode)
	while (1)
	{
		// USB Host Background task
		USBH_Process(controller);

		// 枚举过程中，如果一直未检测到设备，则每隔45秒复位一次设备
		comsumed = DjyGetSysTime() - time;
		if(!bootup)
		{
			if(HOST_DEV_ATTACHED == controller->gState)
			{
				bootup = 1; // 已检测到设备
			}
			else if(comsumed > 45000)
			{
				printf("\r\nUSB #2 stack service no device, try reset \r\n");
				USB_DeviceReset(0);
				time = DjyGetSysTime();
			}
		}
	}
}

//-----------------------------------------------------------------------------
//功能: USB1协议栈服务线程
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
u32 USB_1_Thread(void)
{
	USBH_HandleTypeDef *controller;

	controller = USBH_HostHandle(1);
	if(!controller)
	{
		printf("\r\nUSB #1 stack service QUIT <the controller is not initialized>\r\n");
		return (0);
	}

	// Add Supported Class
	USBH_RegisterClass(controller, USBH_MSC_CLASS); // 注册MSC类
	USBH_RegisterClass(controller, USBH_CDC_CLASS); // 注册CDC类
	USBH_RegisterClass(controller, USBH_CUSTOM_CLASS); // 注册CUSTOM类

	// Start Host Process
	USBH_Start(controller); // 开中断，VBUS上电

	printf("\r\nUSB #1 stack service start to run...\r\n");

	// Run Application (Blocking mode)
	while (1)
	{
		// USB Host Background task
		USBH_Process(controller);
	}
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static void USBH_UserProcess(USBH_HandleTypeDef *pHost, u8 bID)
{
	switch(bID)
	{
		case HOST_USER_SELECT_CONFIGURATION:
			USBH_UsrLog ("UserProcess:host user select configuration.");
			break;

		case HOST_USER_DISCONNECTION:
			USBH_UsrLog ("UserProcess:host user disconnection.");
			break;

		case HOST_USER_CLASS_SELECTED:
			USBH_UsrLog ("UserProcess:host user class selected.");
			break;

		case HOST_USER_CLASS_ACTIVE:
			USBH_UsrLog ("UserProcess:host user class active.");
			break;

		case HOST_USER_CONNECTION:
			USBH_UsrLog ("UserProcess:host user connection");
			break;

		default:
			break;
	}
}

//-----------------------------------------------------------------------------
//功能: 查询USB设备状态
//参数: bController -- USB设备号
//返回:  0 -- 设备已准备； 1 -- 设备未准备好；
//备注:
//-----------------------------------------------------------------------------
s32 USBH_DeviceQuery(u8 bIndex, tagDeviceUSB *pHead)
{
	USBH_HandleTypeDef *controller = NULL;
	tagDeviceUSB *member = pHead;
	u8 i = 0;

	while(member)
	{
		if(bIndex == i)
		{
			controller = member->pController;
			break;
		}

		member = member->pNext;
		i++;
	}

	if((controller) && (HOST_CLASS == controller->gState) &&
	   (USBH_OK == controller->pActiveClass->ClassState))
		return (0); // 设备已准备好

	return (1); // 设备未准备好
}

//-----------------------------------------------------------------------------
//功能: 注册一个USB设备
//参数: pHead -- USB设备链表； bController -- 控制器号；
//返回:  NULL -- 失败；非NULL -- 成功；
//备注:
//-----------------------------------------------------------------------------
tagDeviceUSB *USBH_NewDevice(tagDeviceUSB *pHead, u8 bController)
{
	USBH_HandleTypeDef *controller;
	tagDeviceUSB *tail = NULL;
	tagDeviceUSB *member = pHead;

	controller = USBH_HostHandle(bController);
	if(!controller)
		return (NULL); // 控制器不存在

	while(member) // 直至链表尾部
	{
		tail = member;
		member = member->pNext;
	}

	member = malloc(sizeof(*member));
	if(!member)
		return (NULL);

	member->pController = controller;
	member->pNext = NULL;
	if(tail)
		tail->pNext = member;

	return (member);
}

//-----------------------------------------------------------------------------
//功能:
//参数: bController
//返回: "0" -- 成功; "-1" -- 失败;
//备注: #1控制器用于MSC设备， #2控制器用于无线模块设备
//-----------------------------------------------------------------------------
s32 ModuleInstall_USB(u8 bController)
{
	u16 thread;
	s32 res;
	USBH_HandleTypeDef *controller;
	extern void USBH_LL_Register(u8 bController);
	extern void USB_ShellInstall(void);

	controller = USBH_HostHandle(bController);
	if(!controller)
		return (-1);

	USBH_LL_Register(bController); // 驱动注册
	USBH_Init(controller, USBH_UserProcess, bController); // 初始化

	if(1 == bController)
	{
		extern s32 USB_RegisterMSC_Device(u8 bController);
		res = USB_RegisterMSC_Device(bController);
		if(res)
		{
			printf("\r\nUSB has been registered.");
			return (-1);
		}

		thread = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0,
					USB_1_Thread, NULL, 0x2000, "USB #1 stack service");
		if(CN_EVTT_ID_INVALID != thread)
		{
			Djy_EventPop(thread, NULL, 0, 0, 0, 0);
			return (0);
		}
	}
	else
	{
		extern s32 USBH_RegisterCUSTOM_Device(u8 bController);
		res = USBH_RegisterCUSTOM_Device(bController);
		if(res)
		{
			printf("\r\nUSB has been registered.");
			return (-1);
		}

		USB_ShellInstall(); // install USB shell command

		thread = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0,
					USB_2_Thread, NULL, 0x2000, "USB #2 stack service");
		if(CN_EVTT_ID_INVALID != thread)
		{
			Djy_EventPop(thread, NULL, 0, 0, 0, 0);
			return (0);
		}
	}

	return (-1);
}



// =============================================================================
// Copyright (C) 2012-2020 	 All Rights Reserved
// FileDescription:This file define the cpu init,such as the IFC and some devices
// FileVersion    :
// Author         :
// Create Time    :2014/12/12
// =============================================================================
// Instructions:

// cpu basic init
void CPU_CpuInit(void)
{
	extern void CPU_P1020ElbcInit();
	CPU_P1020ElbcInit();	
	return;
}

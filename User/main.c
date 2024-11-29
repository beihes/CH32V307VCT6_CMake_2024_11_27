/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include "constHead.h"


/* Global typedef */

/* Global define */

/* Global Variable */


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	Delay_Init();
	USART_Printf_Init(115200);	
	printf("SystemClk:%ld\r\n",SystemCoreClock);
	printf( "ChipID:%08lx\r\n", DBGMCU_GetCHIPID() );
	printf("This is printf example\r\n");
	printf("\t-->JY61P<--\r\n");

	Led_GPIO_Init();
	JY61P_Init();

	while(1)
    {
		JY61P_Test();
	}
}


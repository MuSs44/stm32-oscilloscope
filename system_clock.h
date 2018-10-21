#ifndef __system_clock_h__
#define __system_clock_h__

void SystemClock_Config(void) {

	LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

	if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2) {
		//Error_Handler();
		while (1);
	}
	//LL_RCC_HSI_SetCalibTrimming(16);

	LL_RCC_HSE_Enable();

	/* Wait till HSI is ready */
	while(LL_RCC_HSE_IsReady() != 1);
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

	LL_RCC_PLL_Enable();

	/* Wait till PLL is ready */
	while(LL_RCC_PLL_IsReady() != 1);

	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);

	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

	LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_HSE_DIV128);

	/* Wait till System clock is ready */
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

	LL_Init1msTick(72000000);

	LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);

	LL_SetSystemCoreClock(72000000);

	/* SysTick_IRQn interrupt configuration */
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
}

#endif
#ifndef __gpio_h__
#define __gpio_h__

void setupGPIO() {
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
}


//led
void LED_PC13_ON() { //the led is active low
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
}

void LED_PC13_OFF() {
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
}

void setupLED_PC13() {
	LL_GPIO_InitTypeDef initgpio;

	LL_GPIO_StructInit(&initgpio);

	initgpio.Pin = LL_GPIO_PIN_13;
	initgpio.Mode = LL_GPIO_MODE_OUTPUT;
	initgpio.Speed = LL_GPIO_MODE_OUTPUT_2MHz;
	initgpio.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;

	LL_GPIO_Init(GPIOC, &initgpio);
}

//
#endif

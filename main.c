#include "stm32f1xx.h"
#include "main.h"

const char Startup_msg[] = "****************************************\n" \
                           "*             Oscilloscope             *\n" \
                           "*             v  1.0                   *\n" \
                           "****************************************\n\n";

void __libc_init_array(void) {
}

int main(void) {
	/*LL_RTC_TimeTypeDef jj;*/
	SystemClock_Config();
	setupSleep();
	setupGPIO();
	setupLED_PC13();
	LED_PC13_ON();
	setup_USART1();
	setupADC();

	s_send_len(Startup_msg, sizeof(Startup_msg)-1, 0);

	//Setup Timer 4
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);

	LL_TIM_InitTypeDef tim4_init_struct;
	LL_TIM_StructInit(&tim4_init_struct);

	tim4_init_struct.Prescaler = (72)-1;
	tim4_init_struct.CounterMode = LL_TIM_COUNTERMODE_UP;
	tim4_init_struct.Autoreload = (100)-1;
	tim4_init_struct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	LL_TIM_Init(TIM4, &tim4_init_struct);
	LL_TIM_EnableIT_UPDATE(TIM4);
	LL_TIM_EnableCounter(TIM4);
	LL_TIM_GenerateEvent_UPDATE(TIM4);

	NVIC_SetPriority(TIM4_IRQn, 0);
	//NVIC_EnableIRQ(TIM4_IRQn);

	LL_ADC_REG_StartConversionSWStart(ADC1);

	s_send_char(0x00, 0); //Begin receiving

	char op,err;

	err = 0;

	while (1) { //process serial inputs

		if (check_buf() != 0) {
			s_recv_char(&op, 0);
			switch (op) {
				case 0x01: //begin
					NVIC_EnableIRQ(TIM4_IRQn);
					break;

				case 0x02: //stop
					NVIC_DisableIRQ(TIM4_IRQn);
					break;

				case 0x10: //set to 1 KHZ
					LL_TIM_DisableCounter(TIM4);
					LL_TIM_SetPrescaler(TIM4, 72);
					LL_TIM_SetAutoReload(TIM4, 1000);
					LL_TIM_EnableCounter(TIM4);
					s_send_char(0x10, 0); //OK
					break;

				case 0x11: //set to 5Khz
					LL_TIM_DisableCounter(TIM4);
					LL_TIM_SetPrescaler(TIM4, 72);
					LL_TIM_SetAutoReload(TIM4, 200);
					LL_TIM_EnableCounter(TIM4);
					s_send_char(0x11, 0); //OK
					break;

				/* //needs fast serial
				case 0x12: //set to 10Khz
					LL_TIM_DisableCounter(TIM4);
					LL_TIM_SetPrescaler(TIM4, 72);
					LL_TIM_SetAutoReload(TIM4, 100);
					LL_TIM_EnableCounter(TIM4);
					break;

				case 0x13: //set to 50Khz
					LL_TIM_DisableCounter(TIM4);
					LL_TIM_SetPrescaler(TIM4, 72);
					LL_TIM_SetAutoReload(TIM4, 20);
					LL_TIM_EnableCounter(TIM4);
					break;
				*/

				default: //error
					err = 0x01; //unknown command
					break;
			}

			if (err != 0)
				s_send_char(err, 0); //ERR
				


		}

	}

}

//inturrupts section

//read and send adc value
__attribute__((optimize("O0"))) void TIM4_IRQHandler() {
	uint16_t val;

	LL_TIM_ClearFlag_UPDATE(TIM4);

	//Wait until the conversion is completed
	while (LL_ADC_IsActiveFlag_EOS(ADC1) == 0);

	//Read the value
	val = LL_ADC_REG_ReadConversionData12(ADC1);
	s_send_len((char *) &val, sizeof(val), 0);
}

#ifndef __serial_h__
#define __serial_h__

#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_usart.c"

#define max_sleep_loops 100

#define serial_buf_size 2

char serial_rx_buf[serial_buf_size];

uint32_t serial_rx_pos = 0xFFFFFFFF;
uint32_t serial_rx_size = 0;

void reset_buf() {
	reset_queue(&serial_rx_pos, &serial_rx_size);
}

uint32_t check_buf() {
	return serial_rx_size;
}

void __setup_USART1_inturrupts() {
	LL_USART_EnableIT_RXNE(USART1);
	//LL_USART_EnableIT_TXE(USART1);

	NVIC_SetPriority(USART1_IRQn, 0);
	NVIC_EnableIRQ(USART1_IRQn);
}

void __setup_USART1() {
	LL_USART_InitTypeDef usart_init;
	LL_GPIO_InitTypeDef usart_pin_init;

	LL_GPIO_StructInit(&usart_pin_init);
	usart_pin_init.Pin = LL_GPIO_PIN_9;
	usart_pin_init.Mode = LL_GPIO_MODE_ALTERNATE;
	usart_pin_init.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	usart_pin_init.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	usart_pin_init.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(GPIOA, &usart_pin_init);

	LL_GPIO_StructInit(&usart_pin_init);
	usart_pin_init.Pin = LL_GPIO_PIN_10;
	usart_pin_init.Mode = LL_GPIO_MODE_FLOATING;
	usart_pin_init.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	LL_GPIO_Init(GPIOA, &usart_pin_init);

	LL_USART_StructInit(&usart_init);
	usart_init.BaudRate = 115200;
	//usart_init.BaudRate = 256000;
	usart_init.DataWidth = LL_USART_DATAWIDTH_8B;
	usart_init.StopBits = LL_USART_STOPBITS_2;
	usart_init.Parity = LL_USART_PARITY_EVEN;
	usart_init.TransferDirection = LL_USART_DIRECTION_TX_RX;
	usart_init.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	if (LL_USART_Init(USART1, &usart_init) != 1) {
		//LedBlinkForever();
	}

	LL_USART_ConfigAsyncMode(USART1);

	LL_USART_Enable(USART1);
}

void setup_USART1() {
	//enable gpioA clock and USART1 clock
	if ( !(LL_APB2_GRP1_IsEnabledClock(LL_APB2_GRP1_PERIPH_GPIOA)) ) {
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	}
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

	//
	__setup_USART1();
	__setup_USART1_inturrupts();

	LL_USART_Enable(USART1);
}

// send/recieve functions
__attribute__((optimize("O0"))) int s_recv_char(char *chr, unsigned int timeout) {

	if (serial_rx_size == 0) {
		if (timeout == 0) {
			while (serial_rx_size == 0);
		} else {

			for (int32_t i=max_sleep_loops; 0 <= i; i--) {
				__delay(timeout/max_sleep_loops);
				if (serial_rx_size == 0) {
					if (i == 0)
						return -1;
				} else {
					break;	
				}

			}

		}
	}

	dequeue(serial_rx_buf, chr, &serial_rx_pos, &serial_rx_size, serial_buf_size);
	return 0;
}

__attribute__((optimize("O0"))) int s_send_char(const char chr, unsigned int timeout) {

	if (LL_USART_IsActiveFlag_TXE(USART1) == 0) {
		if (timeout == 0) {
			while (LL_USART_IsActiveFlag_TXE(USART1) == 0);
		} else {

			for (int32_t i=max_sleep_loops; 0 <= i; i--) {
				__delay(timeout/max_sleep_loops);
				if (LL_USART_IsActiveFlag_TXE(USART1) == 0) {
					if (i == 0)
						return -1;
				} else {
					break;	
				}
			}

		}
	}

	LL_USART_TransmitData8(USART1, chr);
	//while (LL_USART_IsActiveFlag_TC(USART1) == 0);

	return 0;
}

int s_recv_len(char *arr, int len, unsigned int timeout) {
	char chr;

	for (int i=0; i<len; i++) {
		if (s_recv_char(&chr, timeout) != 0)
			return i;
		arr[i] = chr;
	}

	return 0;

}

int s_send_len(const char *arr, int len, unsigned int timeout) {
	for (int i=0; i<len; i++)
		if (s_send_char(arr[i], timeout) != 0)
			return i;

	return 0;
}
//

__attribute__((optimize("O0"))) void USART1_IRQHandler() {
	LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
	if (LL_USART_IsActiveFlag_RXNE(USART1)) {
		enqueue_overwrite_shift(serial_rx_buf, LL_USART_ReceiveData8(USART1), &serial_rx_pos, &serial_rx_size, serial_buf_size);
	}
}

#endif

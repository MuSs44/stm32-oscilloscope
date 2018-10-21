#ifndef __sleep_h__
#define __sleep_h__

void setupSleep() {
	//LL_InitTick(SystemCoreClock, (SystemCoreClock/1000) );
	//LL_Init1msTick(SystemCoreClock);

	//in system_clock.h
}

void __delay(unsigned int time) {
	/*while (time > 0)
		time--;*/
	LL_mDelay(time);
}

#endif
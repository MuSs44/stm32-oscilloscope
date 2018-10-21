#ifndef __adc_h__
#define __adc_h__

void __setup_ADC() {
	LL_GPIO_InitTypeDef adc_pin_init;
	LL_ADC_REG_InitTypeDef adc_init_struct;

	//Setup the analog input pin
	LL_GPIO_StructInit(&adc_pin_init);
	adc_pin_init.Pin = LL_GPIO_PIN_0;
	adc_pin_init.Mode = LL_GPIO_MODE_ANALOG;
	adc_pin_init.Speed = LL_GPIO_MODE_OUTPUT_50MHz;
	LL_GPIO_Init(GPIOA, &adc_pin_init);

	//Setup the ADC
	adc_init_struct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
	adc_init_struct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
	adc_init_struct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
	adc_init_struct.ContinuousMode = LL_ADC_REG_CONV_CONTINUOUS;
	adc_init_struct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
	LL_ADC_REG_Init(ADC1, &adc_init_struct);
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_0);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_4, LL_ADC_SAMPLINGTIME_71CYCLES_5);

	//Enable and calibrate the ADC
	LL_ADC_Enable(ADC1);
	LL_ADC_StartCalibration(ADC1);
	while (LL_ADC_IsCalibrationOnGoing(ADC1));
	//LL_ADC_ClearFlag_ADRDY(ADC1);

}

void setupADC() {
	//Enable ADC clock
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

	__setup_ADC();
}

#endif
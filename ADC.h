/*
 * ADC.h
 *
 *  Created on: Apr 23, 2024
 *      Author: user
 */

#ifndef ADC_H_
#define ADC_H_

#define ADCVREF_VDDA 0x000
#define ADCVREF_INT  0x200

void ADC0_Init(uint32_t channel, uint32_t reference);

uint32_t ADC0_In(void);

void ADC1_Init(uint32_t channel, uint32_t reference);

uint32_t ADC1_In(void);

void ADC_InitDual(ADC12_Regs *adc12,uint32_t channel1,uint32_t channel2, uint32_t reference);

void ADC_InDual(ADC12_Regs *adc12,uint32_t *d1, uint32_t *d2);




#endif /* ADC_H_ */

/*
 * adc.h
 *
 *  Created on: Jun 18, 2019
 *      Author: colemata
 */

#ifndef ADC_H_
#define ADC_H_



void adc_init(void);

int adc_read(void);

int adc_do(int ir_distance);



#endif /* ADC_H_ */

/*
 * adc.c
 *
 *  Created on: Jun 18, 2019
 *      Author: colemata
 */

#include "adc.h"
#include "tm4c123gh6pm.h"
#include "lcd.h"
#include "math.h"

void adc_init(void) {

    SYSCTL_RCGCADC_R    |= 0b01;
    SYSCTL_RCGCGPIO_R   |= 0b000010;
    GPIO_PORTB_AFSEL_R  |= 0b00010000;
    GPIO_PORTB_DEN_R    &= 0b11101111;
    GPIO_PORTB_DIR_R    &= 0b11101111;
    GPIO_PORTB_AMSEL_R  |= 0b00010000;
    GPIO_PORTB_ADCCTL_R |= 0b00000000;
    ADC0_ACTSS_R        &= 0b1110;
    ADC0_EMUX_R         = 0b0;
    ADC0_SSMUX0_R       |= 0x0000000A;
    ADC0_SSCTL0_R       |= 0x00000006;
    ADC0_ACTSS_R        |= 0b0001;
}
int adc_read(void) {
    ADC0_PSSI_R = ADC_PSSI_SS0;
    while((ADC0_RIS_R & ADC_RIS_INR0) == 0){}
    ADC0_ISC_R = ADC_ISC_IN0;

    return ADC0_SSFIFO0_R;
}

int adc_do(int ir_distance) {
        int sample = 100;
        int x[100] = 0;
        int x_avg = 0;
        int i = 0;
                                                //adc_sac averages samples


            x_avg = 0;
            for (i = 0; i < sample; i++) {
                x[i] = adc_read();
                x_avg = x_avg + x[i];
            }
            x_avg = x_avg / sample;
            //y = 3000000*pow(x_avg,-1.559);
            ir_distance = 2648514*pow(x_avg,-1.57);       //cybot 13

            return ir_distance;
}

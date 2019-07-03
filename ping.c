/*
 * ping.c
 *
 *  Created on: Jun 20, 2019
 *      Author: colemata
 */

#include <timer.h>
#include "ping.h"
#include "tm4c123gh6pm.h"

volatile unsigned int time_start = 0;
volatile unsigned int time_stop = 0;
volatile int flag = 0;
volatile unsigned long *time_difference = 0;
int overflow = 0;

void ping_init(void) {
    SYSCTL_RCGCGPIO_R   |= 0b000010;
    timer_waitMillis(50);
    SYSCTL_RCGCTIMER_R |= 0b001000;
    timer_waitMillis(50);
    TIMER3_CTL_R        &= 0b111011111110;      //disables timers A and B
    TIMER3_CFG_R        |= 0x4;                 //sets timer to 16-bit
    TIMER3_CTL_R        |= 0b110000000000;      //configures timer to trigger an interrupt on both edges
    TIMER3_TBPR_R       |= 0xFF;
    TIMER3_TBMR_R       |= 0b00010111;          //puts timer in capture mode
    TIMER3_TBILR_R      =  0xFFFFFFFF;
    TIMER3_ICR_R        |= 0b010000000000;      //clears interrupt events
    TIMER3_IMR_R        |= 0b010000000000;      //enables the timer b interrupt

    NVIC_EN1_R          |= 0x10;                //sets interrupt for timer
    NVIC_PRI9_R         |= 0b00100000;          //sets priority for timer
    IntMasterEnable();
    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    TIMER3_CTL_R        |= 0b000100000000;      //enables timer B
}

void send_pulse(void) {
    GPIO_PORTB_PCTL_R   &= 0xFFFF8FFF;
    GPIO_PORTB_AFSEL_R  &= 0b11110111;
    GPIO_PORTB_DEN_R    |= 0b00001000;
    GPIO_PORTB_DIR_R    |= 0x08;
    GPIO_PORTB_DATA_R   = 0x0;
    GPIO_PORTB_DATA_R   |= 0x08;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R   &= 0xF7;
    GPIO_PORTB_AFSEL_R  |= 0b00001000;
    GPIO_PORTB_PCTL_R   &= 0xFFFF0FFF;
    GPIO_PORTB_PCTL_R   |= 0x00007000;
    GPIO_PORTB_DIR_R    &= 0xF7;
}

void ping_read(int *time_diff) {
    time_difference = time_diff;
    send_pulse();
}

void TIMER3B_Handler() {
    TIMER3_ICR_R |= 0b010000000000;                 //clears interrupt
    if (flag == 0) {
        time_start = TIMER3_TBR_R;                  //sets start time to time_start
        flag = 1;                                   //next interrupt that happens will go to else statement
    }
    else {
        time_stop = TIMER3_TBR_R;                   //sets stop time to time_stop
        *time_difference = time_stop - time_start;  //calculates the time difference between time_stop and time_start
        flag = 0;                                   //resets handler function to allow another start time to be recorded
    }
    if (time_start > time_stop) {
        overflow += (time_stop < time_start);
        *time_difference = ((unsigned long)overflow<<24) + time_stop - time_start;
    }
}







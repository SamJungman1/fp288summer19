/*
 * servo.c
 *
 *  Created on: Jun 24, 2019
 *      Author: colemata
 */

#include <timer.h>
#include "servo.h"
#include "lcd.h"

//int direction = 0;                                              //value of 0 is counterclockwise, value of 1 is clockwise

void gpio_portb_init()
{
    SYSCTL_RCGCGPIO_R |= 0b000010;
    timer_waitMillis(50);
    GPIO_PORTB_AFSEL_R |= 0b00100000;
    GPIO_PORTB_DEN_R |= 0b00100000;
    GPIO_PORTB_PCTL_R |= 0x00700000;
    GPIO_PORTB_DIR_R &= 0b11011111;
}

void gpio_porte_init()
{
    SYSCTL_RCGCGPIO_R |= 0b010000;
    timer_waitMillis(50);
    GPIO_PORTE_DIR_R &= 0b11000000;
    GPIO_PORTE_DEN_R |= 0b00111111;
}
void servo_init_lab8(void)
{
    SYSCTL_RCGCTIMER_R |= 0b000010;
    timer_waitMillis(50);
    TIMER1_CTL_R &= 0b111011111110;              //disables timers A and B
    TIMER1_CFG_R |= 0x4;                         //sets timer to 16-bit
    TIMER1_TBMR_R |= 0b00011010; //puts timer in PERIODIC and PWM mode, counts down
    TIMER1_TBILR_R = 0xE200;                     //lower 16 bits of the interval
    TIMER1_TBPR_R = 0x04;                 //set the upper 8 bits of the interval
    TIMER1_TBPMR_R = 0x4;
    TIMER1_TBMATCHR_R = 0x87A5;                      //zeros servo at 90 degrees
    TIMER1_CTL_R |= 0b000100000000;              //enables timer B
}

void servo_init(void)
{
    SYSCTL_RCGCTIMER_R |= 0b000010;
    timer_waitMillis(50);
    TIMER1_CTL_R &= 0b111011111110;              //disables timers A and B
    TIMER1_CFG_R |= 0x4;                         //sets timer to 16-bit
    TIMER1_TBMR_R |= 0b00011010;                 //puts timer in PERIODIC and PWM mode, counts down
    TIMER1_TBILR_R = 0xE200;                     //lower 16 bits of the interval
    TIMER1_TBPR_R = 0x04;                        //set the upper 8 bits of the interval
    TIMER1_TBPMR_R = 0x4;
    TIMER1_TBMATCHR_R = 0xBED8;                  //zeros servo at 0 degrees
    TIMER1_CTL_R |= 0b000100000000;              //enables timer B
}

void servo_reset(void)
{
    TIMER1_TBMATCHR_R = 0xBED8;
}
void servo_90(void)
{
    TIMER1_TBMATCHR_R = 0x87A5;
}

int servo_move_lab8(double degrees, int *direction)
{
    if (GPIO_PORTE_DATA_R <= 31)
    {                                                          //SWITCH 6: Move the servo to 180 degrees
        TIMER1_TBPMR_R = 0x4;                                  //value of 282738
        TIMER1_TBMATCHR_R = 0x5072;
        degrees = 180;
    }
    else if (GPIO_PORTE_DATA_R <= 47 & GPIO_PORTE_DATA_R >= 32)
    {                                                          //SWITCH 5: Move the servo to 0 degrees
        TIMER1_TBPMR_R = 0x4;                                  //value of 311000
        TIMER1_TBMATCHR_R = 0xBED8;
        degrees = 0;
    }
    else if (GPIO_PORTE_DATA_R <= 55 & GPIO_PORTE_DATA_R >= 48)
    {                                                          //SWITCH 4: Switch between clockwise and counterclockwise
        if (*direction == 0)
        {
            *direction = 1;
            timer_waitMillis(50);
        }
        else
        {
            *direction = 0;
            timer_waitMillis(50);
        }
    }
    else if (GPIO_PORTE_DATA_R <= 59 & GPIO_PORTE_DATA_R >= 56)
    {                                                          //SWITCH 3: Move the servo 5 degrees
        if (*direction == 0)
        {
            TIMER1_TBMATCHR_R = TIMER1_TBMATCHR_R - 785.05556;
            degrees = degrees + 5;

        }
        else if (*direction == 1)
        {
            TIMER1_TBMATCHR_R = TIMER1_TBMATCHR_R + 785.05556;
            degrees = degrees - 5;
        }
    }
    else if (GPIO_PORTE_DATA_R <= 61 & GPIO_PORTE_DATA_R >= 60)
    {                                                          //SWITCH 2: Move the servo 2.5 degrees
        if (*direction == 0)
        {
            TIMER1_TBMATCHR_R = TIMER1_TBMATCHR_R - 392.52778;
            degrees = degrees + 2.5;
        }
        else if (*direction == 1)
        {
            TIMER1_TBMATCHR_R = TIMER1_TBMATCHR_R + 392.52778;
            degrees = degrees - 2.5;
        }
    }
    else if (GPIO_PORTE_DATA_R == 62)
    {                                                         //SWITCH 1: Move the servo 1 degree
        if (*direction == 0)
        {
            TIMER1_TBMATCHR_R = TIMER1_TBMATCHR_R - 157.011111;
            degrees = degrees + 1;
        }
        else if (*direction == 1)
        {
            TIMER1_TBMATCHR_R = TIMER1_TBMATCHR_R + 157.011111;
            degrees = degrees - 1;
        }
    }
    if (degrees > 180)
    {
        degrees = 180;
    }
    if (degrees < 0)
    {
        degrees = 0;
    }
                                                                //    int x = TIMER1_TBMATCHR_R;
                                                                //    lcd_printf("%x", x);
    return degrees;
}

int servo_move(double degrees)
{
    TIMER1_TBMATCHR_R = TIMER1_TBMATCHR_R - 314.022222;
    degrees = degrees + 2;

    return degrees;
}

void servo_point(double degrees)
{
    TIMER1_TBMATCHR_R = 0xBED8 - (degrees * 157.011111);
}

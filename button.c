/*
 * button.c
 *
 *  Created on: Jul 18, 2016
 *      Author: Eric Middleton, Zhao Zhang, Chad Nelson, & Zachary Glanz.
 *
 *  @edit: Lindsey Sleeth and Sam Stifter on 02/04/2019
 *  @edit: Phillip Jone 05/30/2019: Mearged Spring 2019 version with Fall 2018
 */

//The buttons are on PORTE 5:0
// GPIO_PORTE_DATA_R -- Name of the memory mapped register for GPIO Port E, 
// which is connected to the push buttons
#include "button.h"

// Global varibles
volatile int *button_event_ptr;
volatile int *button_num_ptr;

/**
 * Initialize PORTE and configure bits 0-5 to be used as inputs for the buttons.
 */
void button_init()
{
    static uint8_t initialized = 0;

    //Check if already initialized
    if (initialized)
    {
        return;
    }

    // Reading: To initialize and configure GPIO PORTE, visit pg. 656 in the
    // Tiva datasheet.

    // Follow steps in 10.3 for initialization and configuration. Some steps
    // have been outlined below.

    // Ignore all other steps in initialization and configuration that are not
    // listed below. You will learn more about additional steps in a later lab.

    // 1) Turn on PORTE system clock, do not modify other clock enables
    SYSCTL_RCGCGPIO_R |= 0b010000;

    // 2) Set the buttons as inputs, do not modify other PORTE wires
    GPIO_PORTE_DIR_R &= 0b11000000;

    // 3) Enable digital functionality for button inputs,
    //    do not modify other PORTE enables
    GPIO_PORTE_DEN_R |= 0b11111111;

    initialized = 1;
}

/**
 * Interrupt handler -- executes when a hardware event occurs (a button is pressed)
 */
void init_button_interrupts(int *button_event_addr, int *button_num_addr)
{
    button_event_ptr = button_event_addr;
    button_num_ptr = button_num_addr;

    // In order to configure GPIO ports to detect interrupts, you will need to visit pg. 656 in the Tiva datasheet.
    // Notice that you already followed some steps in 10.3 for initialization and configuration of the GPIO ports in the function button_init().
    // Additional steps for setting up the GPIO port to detect interrupts have been outlined below.
    // TODO: Complete code below

    // 1) Mask the bits for pins 0-5
    GPIO_PORTE_IM_R &= 0b11000000;

    // 2) Set pins 0-5 to use edge sensing
    GPIO_PORTE_IS_R &= 0b11000000;

    // 3) Set pins 0-5 to use both edges. We want to update the LCD
    //    when a button is pressed, and when the button is released.
    GPIO_PORTE_IBE_R |= 0b00111111;

    // 4) Clear the interrupts
    GPIO_PORTE_ICR_R = 0b00111111;

    // 5) Unmask the bits for pins 0-5
    GPIO_PORTE_IM_R |= 0b00111111;

    // TODO: Complete code below
    // 6) Enable GPIO port E interrupt
    // interrupt #4
    NVIC_EN0_R |= 0x10;

    // Bind the interrupt to the handler.
    IntRegister(INT_GPIOE, gpioe_handler);
}

/**
 * Handles a hardware interrupt that occurs from a button being pressed
 */
void gpioe_handler()
{

    // Clear interrupt status register
    GPIO_PORTE_ICR_R = 0b11111111;
    // update
    *button_event_ptr = 1;

    *button_num_ptr = button_getButton();
}

/**
 * Returns the position of the leftmost button being pushed.
 * @return the position of the leftmost button being pushed. 6 is the leftmost button, 1 is the rightmost button.  0 indicates no button being pressed
 */
uint8_t button_getButton(int x)
{

    if (GPIO_PORTE_DATA_R <= 31) //bit 6 is 0
    {
        x = 6;
    }
    else if (GPIO_PORTE_DATA_R <= 47 & GPIO_PORTE_DATA_R >= 32) //bit 5 is 0
    {
        x = 5;
    }
    else if (GPIO_PORTE_DATA_R <= 55 & GPIO_PORTE_DATA_R >= 48) //bit 4 is 0
    {
        x = 4;
    }
    else if (GPIO_PORTE_DATA_R <= 59 & GPIO_PORTE_DATA_R >= 56) //bit 3 is 0
    {
        x = 3;
    }
    else if (GPIO_PORTE_DATA_R <= 61 & GPIO_PORTE_DATA_R >= 60) //bit 2 is 0
    {
        x = 2;
    }
    else if (GPIO_PORTE_DATA_R == 62) //bit 1 is 0
    {
        x = 1;
    }
    else if (GPIO_PORTE_DATA_R == 63) //all bits are 1
    {
        x = 0;
    }

    return x;
}


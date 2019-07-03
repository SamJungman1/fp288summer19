/*
 *
 *   uart.c
 *
 *
 *
 *
 *
 *   @author
 *   @date
 */

#include "uart.h"
#include "cyBot_uart.h"
#include "stdint.h"
#include "stdbool.h"
#include "tm4c123gh6pm.h"

void uart_init(void){
    //    cyBot_uart_init_clean(); // Clean UART initialization, before running your UART GPIO init code

    // Complete this code for configuring the  (GPIO) part of UART initialization
    SYSCTL_RCGCGPIO_R |= 0b110010;
    SYSCTL_RCGCUART_R |= 0b00000010;
    timer_waitMillis(1); // Small delay before accessing device after turning on clock

    GPIO_PORTB_DEN_R |= 0b00000011;
    GPIO_PORTB_DIR_R &= 0b11111110;      // Force 0's in the desired locations
    GPIO_PORTB_DIR_R |= 0b00000010;      // Force 1's in the desired locations
    //GPIO_PORTB_PCTL_R &= 0x00000011;     // Force 0's in the desired locations
    GPIO_PORTB_AFSEL_R |= 0b00000011;
    GPIO_PORTB_PCTL_R |= 0x00000011;     // Force 1's in the desired locations //went to page 1351, found specific function. Looked up pin number and also digital function number (column). Went to page 689 and forced the proper ports to the alternate function value.


    //        GPIO_PORTF_DEN_R = 0b00001111;
    //        GPIO_PORTF_DIR_R = 0b00001111;

    UART1_CTL_R &= 0xFFFFFFFE;
    UART1_IBRD_R = 8;
    UART1_FBRD_R = 44;
    UART1_LCRH_R |= 0x60;
    UART1_CC_R &= 0x0;
    UART1_CTL_R |= 0x1;

    //        cyBot_uart_init_last_half(); // Complete the UART device initialization part of configuration
}

void uart_sendChar(char data){
    while ((UART1_FR_R & 0b00100000) != 0);
    UART1_DR_R = data;
}

int uart_receive(void){
    //	uint32_t ret;
    //	char rdata;
    //
    //	while((UART1_FR_R & 0b00010000) != 0);
    //	ret = UART1_DR_R;
    //	if (ret & 0xF00) {                  //error occurred
    //	    GPIO_PORTF_DATA_R = 0xF;
    //	}
    //	else {
    //	    rdata = (char)(UART1_DR_R & 0xFF);
    //	}
    //	return rdata;

    char data = 0x0;

    while(UART1_FR_R & UART_FR_RXFE){
        // DO NOTHING
    }

    data = (char)(UART1_DR_R & 0xFF);
    return data;
}

void uart_sendStr(const char *data){
    int i = 0;
    for (i = 0; i < strlen(data); i++) {
        uart_sendChar(data[i]);
    }
}

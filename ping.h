/*
 * ping.h
 *
 *  Created on: Jun 20, 2019
 *      Author: colemata
 */

#ifndef PING_H_
#define PING_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"

void ping_init(void);

void send_pulse(void);

void ping_read();

void TIMER3B_Handler(void);



#endif /* PING_H_ */

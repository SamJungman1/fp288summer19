/*
 * servo.h
 *
 *  Created on: Jun 24, 2019
 *      Author: colemata
 */

#ifndef SERVO_H_
#define SERVO_H_

#include "stdint.h"
#include "inc/tm4c123gh6pm.h"
#include "stdbool.h"
#include "driverlib/interrupt.h"

void servo_init(void);

void servo_init_lab8(void);

void gpio_portb_init(void);

void gpio_porte_init(void);

int servo_move_lab8(double degrees, int *direction);

int servo_move(double degrees);

void servo_reset(void);

void servo_point(double degrees);

void servo_90(void);

#endif /* SERVO_H_ */

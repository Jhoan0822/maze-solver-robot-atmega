/*
 * PID.h
 *
 * Created: 20/05/2024 11:10:54 p. m.
 *  Author: Pablo
 */ 

#ifndef PID_H_
#define PID_H_

#include <avr/io.h>
#include <util/delay.h>

pinMode(volatile uint8_t *ddrRegister, char pin, char mode);
digitalRead(volatile uint8_t *pinRegister, char pin);
digitalWrite(volatile uint8_t *port, char pin, char value);
PID();
frenos();

#endif /* PID_H_ */
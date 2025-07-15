/*
 * QTRA.h
 *
 * Created: 18/05/2024 2:24:03 p. m.
 *  Author: Daniel
 */ 

#ifndef QTRA_H_
#define QTRA_H_

#include <avr/io.h>
#include <util/delay.h>

pinMode(volatile uint8_t *ddrRegister, char pin, char mode);
digitalRead(volatile uint8_t *pinRegister, char pin);
digitalWrite(volatile uint8_t *port, char pin, char value);
Qtrinit();
QtrRead(char channel);
fondos();
lineas();
promedio();
lectura();
QTRcalibrate();

#endif /* QTRA_H_ */
/*
 * tcrt5000.h
 *
 * Created: 8/06/2024 4:26:29 p. m.
 *  Author: henyd
 */ 

#ifndef TCRT5000_H
#define TCRT5000_H

#include <avr/io.h>
#include <util/delay.h>

// Prototipos de funciones
void initTCRT5000(void);
void giros(void);
void turnRight(void);
void turnLeft(void);

#endif // TCRT5000_H

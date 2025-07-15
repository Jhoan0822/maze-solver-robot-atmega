/*
 * tcrt5000.c
 *
 * Created: 8/06/2024 4:25:25 p. m.
 *  Author: henyd
 */ 
#include "tcrt5000.h"

int velaD=200;
int velaT=200;

void initTCRT5000(void) {
	// Configurar los pines PB0 y PB1 como entradas
	DDRB &= ~(1 << PB0); // Sensor derecho
	DDRB &= ~(1 << PB1); // Sensor izquierdo

	// Habilitar los pull-ups internos en PB0 y PB1
	PORTB |= (1 << PB0);
	PORTB |= (1 << PB1);
}

void giros(void) {
	// Leer el estado del sensor derecho
	if ((PINB & (1 << PB0))) { // Si el pin PB0 está bajo (se detecta objeto)
		turnRight();
	}

	// Leer el estado del sensor izquierdo
	if ((PINB & (1 << PB1))) { // Si el pin PB1 está bajo (se detecta objeto)
		//turnLeft();
	}

	// Esperar un tiempo antes de la siguiente lectura
	_delay_ms(100);
}

// Definiciones de las funciones turnRight y turnLeft
void turnRight(void) {
	//engines(velaD, -velaT);

	 engines(-velaT, velaD);
	 _delay_ms(200);
}

void turnLeft(void) {
	//engines(velaT,-velaD);
	engines(velaD, -velaT);
	_delay_ms(200);
}

/*
 * driver.c
 *
 * Created: 20/05/2024 8:52:17 p. m.
 *  Author: Pablo
 */ 

#include <avr/io.h>
#include <util/delay.h>

#include "tc0_pwm.h"
#include "QTRA.h"

void engines(volatile int izq,volatile int der)
{
	////// Left engine /////
	if (izq>=0){
		digitalWrite(&PORTC,0,0);
		digitalWrite(&PORTC,1,1);
	}else{
		digitalWrite(&PORTC,0,1);
		digitalWrite(&PORTC,1,0);
		izq=izq*(-1);		
	}
	OCR0=izq;
	
	////// Right engine //////
	if (der>=0){
		digitalWrite(&PORTC,2,0);
		digitalWrite(&PORTC,3,1);
	}else{
		digitalWrite(&PORTC,2,1);
		digitalWrite(&PORTC,3,0);
		der=der*(-1);
	}
	OCR2=der;
}






 
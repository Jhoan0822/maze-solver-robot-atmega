#include <avr/io.h>
//Configuracion del ATmega32A para el PWM
void configTC0()
{
	//Clock Source: clk_IO/8
	TCCR0 |= (1<<CS01);
	//TCCR2 |= (1<<CS21);
	TCCR2 |= (1<<CS01);
	
	//Operation Mode: Fast PWM
	TCCR0 |= (1<<WGM01) | (1<<WGM00);
	//TCCR2 |= (1<<WGM21) | (1<<WGM20);
	TCCR2 |= (1<<WGM01) | (1<<WGM00);
	
	//OC0 y OC2 Mode: Non inverting
	TCCR0 |= (1<<COM01);
	//TCCR2 |= (1<<COM21);
	TCCR2 |= (1<<COM01);
}


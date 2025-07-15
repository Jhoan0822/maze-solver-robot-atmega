/*
 * QTR8A.c
 *
 * Created: 19/05/2024 10:43:28 a. m.
 * Author : henry,daniel,alejandro,pablo,jhoan,jpshep
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcd.h"
#include "driver.h"
#include "tc0_pwm.h"

void configIO();
void configTC0();

void HCSR04_Init();
void HCSR04_Trigger();
uint32_t getDistance();

static volatile uint32_t first_reading = 0;
static volatile uint32_t second_reading = 0;
static volatile uint32_t duty_cycle = 0;
int posX;

int main(void){
	configIO();
	configTC0();
	
	engines(200,200);
	_delay_ms(1000);
	engines(0,0);
	
	DDRD &= ~(1 << 6); //PD6 as input (ICP1)
	uint32_t distancia = 0;
	HCSR04_Init();
	
	Qtrinit();
	_delay_ms(10);
	QTRcalibrate();
	
	digitalWrite(&PORTD,0,0);
	digitalWrite(&PORTD,3,1);
	
    while (1){
		HCSR04_Trigger(); // Trigger the ultrasonic sensor
		_delay_ms(60); // Wait for the echo pulse
		
		distancia = getDistance();
		if (distancia >= 10)
		{
			posX = lectura();
			if (posX>350)
			{
				digitalWrite(&PORTD,0,1);
			}
			if (posX<350)
			{
				digitalWrite(&PORTD,0,0);
				
			}
			PID(posX);
			frenos(posX);
		}
		else
		{
			engines(0,0);
			DDRB |= (1 << DDB7);
			PORTB &= ~(1 << PORTB7);
			engines(1,1);
			_delay_ms(200);
			engines(-1,1);
			_delay_ms(200);

		}
		
				
    }
	giros();
}

void configIO()
{
	DDRB=0b00011000;

	DDRC=0b00001111;
	DDRD=0b10001000;	
}

void HCSR04_Init(){
	cli(); //clear prior interrupts
	
	// Trigger pin configuration
	DDRB |= (1 << 4); //set PB3 as output

	// Input Capture configuration for Timer 1
	DDRD &= ~(1 << 6); //PD6 as input (ICP1)
	TCCR1B = (1 << ICNC1) | (1 << ICES1) | (1 << CS11); //noise canceling + positive edge detection for input capture and Prescaler = 8.
	
	sei(); //enable global interrupts
	TIMSK |= (1 << TICIE1); //enable timer1 input capture interrupt
}

void HCSR04_Trigger(){
	PORTB |= (1 << 4); //set trigger pin high
	_delay_us(10); //wait for 10 microseconds
	PORTB &= ~(1 << 4); //set trigger pin low
}

uint32_t getDistance(){
	static uint32_t echo_pulse_uS;
	static uint32_t distance_cm;

	// Timer 1 runs at F_CPU / 8 = 1 MHz, so each tick is 1 µs
	echo_pulse_uS = duty_cycle; // already in microseconds
	distance_cm = echo_pulse_uS * 0.034 / 2; // speed of sound = 340 m/s, divided by 2 for round trip

	return distance_cm;
}

ISR(TIMER1_CAPT_vect){
	if ((TCCR1B & (1 << ICES1)) == (1 << ICES1)){
		first_reading = ICR1;
		} else {
		second_reading = ICR1;
	}
	
	if (first_reading != 0 && second_reading != 0){
		duty_cycle = second_reading - first_reading;
		first_reading = 0;
		second_reading = 0;
	}
	
	TCCR1B ^= (1 << ICES1); //toggle edge detection bit
}


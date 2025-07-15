/*
 * PID.c
 *
 * Created: 20/05/2024 11:11:23 p. m.
 *  Author: Pablo
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "tc0_pwm.h"
#include "driver.h"

// Defines
float KP=0.8;//0.5
float KI=0.001;//0.01
float KD=8;
int vel=200;//150
int veladelante=200;
int velatras=200;

// Datos variable integral
int error1=0;
int error2=0;
int error3=0;
//int error4=0;
//int error5=0;
//int error6=0;

// Variables PID
int proporcional=0;
int integral=0;
int derivativo=0;
int diferencial=0;
int last_prop=0;
int setpoint=350;

void PID(volatile int valor){
	proporcional=valor-setpoint;
	derivativo= proporcional-last_prop;
	integral= error1+error2+error3;//+error4+error5+error6;
	last_prop=proporcional;
	//error6=error5;
	//error5=error4;
	//error4=error3;
	error3=error2;
	error2=error1;
	error1=proporcional;
	
	diferencial=(proporcional*KP)+(derivativo*KD)+(integral*KI);
	if (diferencial>vel){
		diferencial=vel;
	}else if (diferencial < (-1)*vel){
		diferencial=vel*(-1);
	}
	
	if (diferencial < 0){
		 engines(vel, vel - diferencial);
	}else{
		 engines(vel + diferencial, vel);
	 }
}

void frenos(int valor){
	if(valor<=50){
		engines(veladelante, -velatras);
	}
	if(valor>=700){
		engines(-velatras, veladelante);
	}
}
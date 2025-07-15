/*
 * QTRA.c
 *
 * Created: 18/05/2024 2:23:34 p. m.
 *  Author: Daniel
 */ 

#include <avr/io.h>
#include <util/delay.h>

volatile unsigned int L_fondos[8] = {0};
volatile unsigned int L_lineas[8] = {0};
volatile char QTR_S[8][5];
volatile int umbral[8];
volatile int digital[8];
volatile int sumap, suma, pos;
volatile int QTR[8];
volatile int digital[8];
volatile char linea = 0;
volatile int poslast;

void pinMode(volatile uint8_t *ddrRegister, char pin, char mode){
	if (mode == 1){
		*ddrRegister |= (1 << pin); // Configurar pin como salida
	}else{
		*ddrRegister &= ~(1 << pin); // Configurar pin como entrada
	}
}

void digitalWrite(volatile uint8_t *port, char pin, char value){
	if (value){
		*port |= (1 << pin);
	}else{
		*port &= ~(1 << pin);
	}
}

char digitalRead(volatile uint8_t *pinRegister, char pin){
	return (*pinRegister & (1 << pin)) ? 1 : 0;
}

void Qtrinit(){
	DDRA = 0x3E;
	pinMode(&DDRD,0,1);
	pinMode(&DDRD,1,1);
	pinMode(&DDRB,1,1);
	digitalWrite(&PORTB,1,1);
	
	digitalWrite(&PORTA,5,0);
	ADMUX = 0x00;
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

int QtrRead(char channel){
	int Ain,AinLow;
	
	digitalWrite(&PORTA,1,(channel&0x01));
	digitalWrite(&PORTA,2,(channel&0x02));
	digitalWrite(&PORTA,3,(channel&0x04));
	digitalWrite(&PORTA,4,(channel&0x08));

	ADCSRA |= (1<<ADSC);		/* Start conversion */
	while((ADCSRA&(1<<ADIF))==0);	/* Monitor end of conversion interrupt */
	
	_delay_us(10);
	AinLow = (int)ADCL;		/* Read lower byte*/
	Ain = (int)ADCH*256;		/* Read higher 2 bits and Multiply with weight */
	Ain = Ain + AinLow;				
	return(Ain);			/* Return digital value*/
}

void fondos(){
	for (int y = 0;y < 60; y++){
		for(int i = 0; i < 8; i++){
			L_fondos[i] += QtrRead(i);
		}
	}
	
	for(int z = 0; z < 8; z++){
		L_fondos[z] = L_fondos[z]/61;
	}
}

void lineas(){
	for (int y = 0;y < 60; y++){
		for(int i = 0; i < 8; i++){
			L_lineas[i] += QtrRead(i);
		}
	}
	
	for(int z = 0; z < 8; z++){
		L_lineas[z] = L_lineas[z]/61;
	}
}

void promedio(){
	for(int i = 0; i < 8; i++){
		umbral[i] = (L_lineas[i]+L_fondos[i])/2;
	}
}

int lectura(){
	for(int i = 0; i < 8; i++){
		QTR[i] = QtrRead(i);
		if (linea == 0){
			if (QTR[i] <= umbral[i]){
				digital[i] = 0;
			}else{
				digital[i] = 1;
			}
		}
		
		if (linea == 1){
			if (QTR[i] <= umbral[i]){
				digital[i] = 1;
			}else{
				digital[i] = 0;
			}
		}
	}
	
	sumap = digital[0]*700+digital[1]*600+digital[2]*500+digital[3]*400+digital[4]*300+digital[5]*200+digital[6]*100+digital[7]*0;
	suma = digital[0]+digital[1]+digital[2]+digital[3]+digital[4]+digital[5]+digital[6]+digital[7];
	pos = sumap/suma;
	if (poslast <= 100 && pos == -1)
		pos = 0;
	if (poslast >= 600 && pos == -1)
		pos = 700;
	poslast = pos;
	_delay_ms(10);
	return pos;
}

void QTRcalibrate(){
	digitalWrite(&PORTD,0,1);
	while(digitalRead(&PINA,7));
	while(digitalRead(&PINA,6))
	{
		digitalWrite(&PORTD,0,0);
		_delay_ms(200);
		fondos();
		digitalWrite(&PORTD,0,1);
		_delay_ms(200);
	}
	while(digitalRead(&PINA,7));
	while(digitalRead(&PINA,6))
	{
		digitalWrite(&PORTD,0,0);
		_delay_ms(200);
		lineas();
		digitalWrite(&PORTD,0,1);
		_delay_ms(200);
	}
	while(digitalRead(&PINA,7));
	promedio();
	digitalWrite(&PORTD,0,0);
}
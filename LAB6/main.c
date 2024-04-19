//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Programación de Microcontroladores
// Autor: Samuel Tortola - 22094
// Proyecto: Laboratorio 5
// Hardware: Atmega238P
// Creado: 19/04/2024
//Última modificación: 19/04/2024
//******************************************************************************

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>


//int dato;

//void initADC(void);
void initUART9600(void);
void writeUART(char Caracter);
void writeTextUART(char * Texto);
volatile uint8_t  datoTX;


int main(void)
{
	DDRB = 0b11111111;  //Salida hacia LEDs
	PORTB = 0b00000000;	//Iniciamos los leds apagados
		
	//initADC();
	initUART9600();
	
	sei(); //Activar interrupciones
	
	writeTextUART("UVG");   //Mostrar inicio 
	writeUART(10);
	writeUART(13);
	
	
	while (1)
	{
	//	ADCSRA |= (1 << ADSC);   //Leer puerto de ADC
		_delay_ms(10);  
		
	}
	
}

void initUART9600(void)
{
	//Paso 1: RX y TX como salida
	DDRD &= ~(1<<DDD0);
	DDRD |= (1<<DDD1);
	
	//Paso 2:Configurar UCSR0A
	UCSR0A = 0;
	
	//Paso 3: configurar UCSR0B donde se habilita ISR de recepcion y se habilita RX y TX
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	
	//Paso 4: Configurar UCSR0C, asincrono, paridad: none, 1 bit stop, data bits de 8 bits
	UCSR0C = 0;
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
	
	//Paso 5: COnfigurar velocidad de baudrate a 9600
	UBRR0 = 103;
	
}

/*
void initADC(){
	ADMUX = 6;
	ADMUX |= (1<<REFS0);  //Referencia AVCC = 5V
	ADMUX &= ~(1<<REFS1); //Justificación a la izquierda
	ADMUX |= (1<<ADLAR);
	ADCSRA = 0;
	ADCSRA |= (1<<ADIE);  //Habilitando interrupcion de ADC
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);  //Habilitando prescaler de 16M/128 frecuencia = 125Khz
	ADCSRA |= (1<<ADEN);   //Habilitando el ADC
}
*/


/*
ISR(ADC_vect)
{
	dato = ADCH;   //Contador general
	ADCSRA |= (1<<ADIF); //Se borra la bandera de interrupción
}
*/

void writeUART(char Caracter)
{
	while(!(UCSR0A & (1<<UDRE0)));  //hasta que la bandera este en 1
		UDR0 = Caracter;
		
}


void writeTextUART(char * Texto){
	uint8_t o;
	for(o = 0; Texto[o]!= '\0'; o++){
		while(!(UCSR0A & (1<<UDRE0)));
			UDR0 = Texto[o];
	}
}

ISR(USART_RX_vect)
{
	datoTX = UDR0;
	UDR0 = datoTX;
	writeUART(10);
	writeUART(13);
	
}
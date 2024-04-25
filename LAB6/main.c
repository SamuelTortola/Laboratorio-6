//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Programación de Microcontroladores
// Autor: Samuel Tortola - 22094
// Proyecto: Laboratorio 6
// Hardware: Atmega238P
// Creado: 19/04/2024
//Última modificación: 25/04/2024
//******************************************************************************

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>


int activa = 0, activa2 = 0;  //variables para entrar y salir de los mesnus y funciones
int dato;//Variable que almacena el valor del ADC

void initADC(void);
void initUART9600(void);
void writeUART(char Caracter);
void writeTextUART(char * Texto);
volatile uint8_t  datoTX;
volatile char receivedChar = 0;

char lista[10] = {'0','1','2','3','4','5','6','7','8','9'}; //Lista de numeros a mostrar
int main(void)
{
	DDRB = 0b00111111;  //Salida hacia LEDs  PB0 a PB5
	DDRD |= 0b11000000; // Configura PD6 y PD7 como salidas 
		
	initADC();
	initUART9600();
	
	sei(); //Activar interrupciones
	
	
	while (1)
	{
		
	ADCSRA |= (1 << ADSC);   //Leer puerto de ADC
		
	//************MENU*********************************
	if(activa == 0){
	writeTextUART("\n\r     **************Hola como esta****************");   //Mostrar inicio 
	writeUART(10);
	writeUART(13);
	writeUART(10);
	writeTextUART("          Coloque 1 para leer potenciometro");   //Mostrar inicio
	writeUART(10);
	writeUART(13);
	writeTextUART("          Coloque 2 para enviar ASCII\n\r");   //Mostrar inicio
	writeUART(10);
	writeUART(13);
	activa = 1;   //Salir del menu 
	}
	
	if(receivedChar != 0){      //Si la variable que hay en USART es diferente de cero
		switch(receivedChar)   //Switch para elegir que se desea, si pot o ASCII
		{
			case '1':
				writeTextUART("\n\r------------------------------------------\n\r");
				writeTextUART("Lectura actual de potenciometro es:");
				
				
				
				// Dividir el carácter recibido en dos partes
				uint8_t lower_bits = dato & 0b00111111; // Los 6 bits menos significativos
				uint8_t upper_bits = (dato >> 6) & 0b11; // Los 2 bits más significativos

				// Mostrar los 6 bits menos significativos en PORTB
				PORTB = lower_bits;

				// Mostrar los 2 bits más significativos en los pines PD6 y PD7 de PORTD
				PORTD = (PORTD & ~0b11000000) | (upper_bits << 6);
				
				
				
				//Obtener el dato del potenciometro descompuesto 
				int millares = dato/1000;
				int centenas = (dato-(millares*1000))/100;
				int decenas = (dato- (millares*1000 + centenas*100))/10;
				int unidades = dato-(millares*1000 + centenas*100 + decenas*10 );
				
				
				switch(centenas){   //Mostrar centenas
					case 0:
						writeUART(lista[0]);
						break;
					
					
					case 1:
						writeUART(lista[1]);
						break;
						
					case 2:
						writeUART(lista[2]);
						break;
						
					
					case 3:
						writeUART(lista[3]);
						break;
					
					case 4:
						writeUART(lista[4]);
						break;
					
					
					case 5:
						writeUART(lista[5]);
						break;
						
					
					case 6:
						writeUART(lista[6]);
						break;
					
					
					case 7:
						writeUART(lista[7]);
						break;
						
					case 8:
						writeUART(lista[8]);
						break;
						
					
					case 9:
						writeUART(lista[9]);
						break;
					
				}
				
				
				switch(decenas){   //Mostrar decenas
					case 0:
					writeUART(lista[0]);
					break;
					
					
					case 1:
					writeUART(lista[1]);
					break;
					
					case 2:
					writeUART(lista[2]);
					break;
					
					
					case 3:
					writeUART(lista[3]);
					break;
					
					case 4:
					writeUART(lista[4]);
					break;
					
					
					case 5:
					writeUART(lista[5]);
					break;
					
					
					case 6:
					writeUART(lista[6]);
					break;
					
					
					case 7:
					writeUART(lista[7]);
					break;
					
					case 8:
					writeUART(lista[8]);
					break;
					
					
					case 9:
					writeUART(lista[9]);
					break;
					
				}
				
				switch(unidades){   //Mostrar unidades
					case 0:
					writeUART(lista[0]);
					break;
					
					
					case 1:
					writeUART(lista[1]);
					break;
					
					case 2:
					writeUART(lista[2]);
					break;
					
					
					case 3:
					writeUART(lista[3]);
					break;
					
					case 4:
					writeUART(lista[4]);
					break;
					
					
					case 5:
					writeUART(lista[5]);
					break;
					
					
					case 6:
					writeUART(lista[6]);
					break;
					
					
					case 7:
					writeUART(lista[7]);
					break;
					
					case 8:
					writeUART(lista[8]);
					break;
					
					
					case 9:
					writeUART(lista[9]);
					break;
					
				}
				
				activa = 0;   //Ingresar a menu principal
				receivedChar  = 0;    //Salir del if 
				writeTextUART("\n\r------------------------------------------\n\r");
			break;
			
			case '2':
				receivedChar = 0;  //Por si se desea colocar un valor en UART
				writeTextUART("\n\rIngrese un valor\n\r");
				activa2 = 1;    // Entrar a la opcion de mostrar el valor en LEDS,en la interrupcion de USART
				break;
			
		}
	}
		
	
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


void initADC(){
	ADMUX = 6;  //ADC6 como entrada
	ADMUX |= (1<<REFS0);  //Referencia AVCC = 5V
	ADMUX &= ~(1<<REFS1); //Justificación a la izquierda
	ADMUX |= (1<<ADLAR);
	ADCSRA = 0;
	ADCSRA |= (1<<ADIE);  //Habilitando interrupcion de ADC
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);  //Habilitando prescaler de 16M/128 frecuencia = 125Khz
	ADCSRA |= (1<<ADEN);   //Habilitando el ADC
}




ISR(ADC_vect)
{
	dato = ADCH;   //Contador general
	ADCSRA |= (1<<ADIF); //Se borra la bandera de interrupción
	
}


//Funcion de escritura de un caracter
void writeUART(char Caracter)
{
	while(!(UCSR0A & (1<<UDRE0)));  //hasta que la bandera este en 1
	UDR0 = Caracter;
		
}

 //Función de escritura de una cadena de caracteres
void writeTextUART(char * Texto){
	uint8_t o;
	for(o = 0; Texto[o]!= '\0'; o++){
		while(!(UCSR0A & (1<<UDRE0)));
			UDR0 = Texto[o];
	}
}

ISR(USART_RX_vect)
{
	receivedChar = UDR0; // Almacena el carácter recibido
	
	if (activa2 == 1){    //Se se eligio enviar un caracter
		// Dividir el carácter recibido en dos partes
		uint8_t lower_bits = receivedChar & 0b00111111; // Los 6 bits menos significativos
		uint8_t upper_bits = (receivedChar >> 6) & 0b11; // Los 2 bits más significativos

		// Mostrar los 6 bits menos significativos en PORTB
		PORTB = lower_bits;

		// Mostrar los 2 bits más significativos en los pines PD6 y PD7 de PORTD
		PORTD = (PORTD & ~0b11000000) | (upper_bits << 6);
		activa2 = 0;   //Salir de este if
		activa = 0;  //Entrar al menu principal
	}
	
	while(!(UCSR0A & (1<<UDRE0)));    //Mientras haya caracteres
		UDR0 = receivedChar;
		
	

}
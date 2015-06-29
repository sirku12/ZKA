/*
 * main.c
 *
 *  Created on: 20 mar 2015
 *      Author: Michal
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#include "IR_DECODE/ir_decode.h"
#include "MKUART/mkuart.h"

const char tab[] PROGMEM = { 0x00, 0x20, 0x14, 0x14, 0x12, 0x12, 0x04,0x04, 0x02, 0x02, 0x08 };
volatile unsigned char i=4;
volatile unsigned char jii=0;
volatile unsigned char licznik=0;
volatile uint16_t licznik2=0;
volatile uint8_t  trybik = 0;
volatile unsigned char kopiaB;


uint16_t pomiar (uint8_t kanal);

#define REF_256 (1<<REFS1)|(1<<REFS0)
#define REF_VCC (1<<REFS0)

int main(void){

	//ustawienie ADC
	ADCSRA |= (1<<ADEN); // w³¹cz ADC
	ADCSRA |= (1<<ADPS2); // preskaler = 16
	ADMUX = REF_256; // ustawiamy wewn Ÿród³o odniesienia
	//koniec ustawieñ ADC

	//diody
	DDRB |= (1<<PB1);
	DDRB |= (1<<PB2);
	DDRB |= (1<<PB3);
	DDRB |= (1<<PB4);
	DDRB |= (1<<PB5);

	//adc
	DDRD |= (1<<PD4);
	//przyciski
	DDRD &= ~(1<<PD2);
	PORTD |= (1<<PD2);
	DDRD &= ~(1<<PD3);
	PORTD |= (1<<PD3);

	UART_Init( __UBRR );	// inicjalizacja UART
	ir_init();	/* inicjalizacja dekodowania IR */
	sei();

	//ustawienia timer0
	TIMSK |= (1<<TOIE0); //Przerwanie overflow (przepe³nienie timera)
	TCCR0 |= (1<<CS01);

	char zezwolenie = 0;

	while(1){

		if(Ir_key_press_flag) {	/* jeœli odebrano prawid³owe kody z pilota */
			if(IrData==2077){
				if(trybik>0){
					trybik--;
					if(jii<=25 && jii>=5){
						jii-=5;
					}
					if(trybik == 5){
						DDRB |= (1<<PB1);
						DDRB |= (1<<PB2);
						DDRB |= (1<<PB3);
						DDRB |= (1<<PB4);
						DDRB |= (1<<PB5);
						jii=25;
					}
				}
			}
			if(IrData==1033){
				if(trybik<11){
					trybik++;
					if(jii<=25 && jii>=0){
						jii+=5;
					}
					if(trybik == 6){
						DDRB &= ~(1<<PB1);
						DDRB &= ~(1<<PB2);
						DDRB &= ~(1<<PB3);
						DDRB &= ~(1<<PB4);
						DDRB &= ~(1<<PB5);
						jii=0;
					}
				}
			}
			if(IrData==5763){
				i=3;
				PORTB = pgm_read_byte(&(tab[i%11]));
			}
			if(IrData==1121){
				i=5;
				PORTB = pgm_read_byte(&(tab[i%11]));
			}
			if(IrData==1118){
				i=7;
				PORTB = pgm_read_byte(&(tab[i%11]));
			}
			if(IrData==1471){
				i=9;
				PORTB = pgm_read_byte(&(tab[i%11]));
			}
			if(IrData==4603){
				i=1;
				PORTB = pgm_read_byte(&(tab[i%11]));
			}
			if(IrData==1089){
				i=10;
				PORTB = pgm_read_byte(&(tab[i%11]));
			}
			if(IrData==4615){
				i=0;
				PORTB = pgm_read_byte(&(tab[i%11]));
			}
			if(IrData==5246){// przycisk uruchamiaj¹cy mryganie w danym stanie(który ma takie mo¿liwoœci)
				PORTB = pgm_read_byte(&(tab[i%11]));
				kopiaB=PORTB;
				if((i%11)==9){
					i--;
				}
				else if((i%11)==7){
					i--;
				}
				else if((i%11)==5){
					i--;
				}
				else if((i%11)==3){
					i--;
				}
			}
		}

		if(!( PIND & (1<<PD2) )){
			_delay_ms(20);
			if(!(PIND & (1<<PD2)) && zezwolenie==1){
				DDRB |= (1<<PB1);
				DDRB |= (1<<PB2);
				DDRB |= (1<<PB3);
				DDRB |= (1<<PB4);
				DDRB |= (1<<PB5);
				PORTD |= (1<<PD4);

				i=0;
				PORTB = pgm_read_byte(&(tab[i%11]));


				_delay_ms(50);
				PORTB=0;
				if(pomiar(4)>820){
				if(pomiar(4)>820){
					PORTB |= (1<<PB5);
				}
				if(pomiar(4)>830){
					PORTB |= (1<<PB4);
				}
				if(pomiar(4)>870){
					PORTB |= (1<<PB3);
				}
				if(pomiar(4)>900){
					PORTB |= (1<<PB2);
				}
				if(pomiar(4)>930){
					PORTB |= (1<<PB1);
				}
				PORTD ^= (1<<PD4);
				_delay_ms(1000);
				PORTB = 0;
				_delay_ms(400);
				}
			}
		}
		if(!( PIND & (1<<PD3) )){
			_delay_ms(20);
			if(!(PIND & (1<<PD3)) && zezwolenie==1){
			}
		}
		if((PIND & (1<<PD2))&&(PIND & (1<<PD3))){
				zezwolenie = 1;
		}
	}
}

uint16_t pomiar( uint8_t kanal ){
	ADMUX = (ADMUX & 0b11111000) | kanal;
	ADCSRA |= (1<<ADSC); //start konwersji
	//while(ADCSRA & (1<<ADSC));
	return ADCW;
}

ISR(TIMER0_OVF_vect){

		licznik++;

		if((i%11)==8 || (i%11)==4){
			if(licznik2==1500){
				PORTB = pgm_read_byte(&(tab[i%11]));
				PORTB ^= (1<<PB1);
				kopiaB=PORTB;
			}
			if(licznik2>=3000){
				licznik2=0;
				PORTB = pgm_read_byte(&(tab[i%11]));
				kopiaB=PORTB;
			}

			licznik2++;
			if(licznik%30>=jii){
				PORTB = kopiaB;
			}
			else{
				PORTB &= ~pgm_read_byte(&(tab[i%11]));
			}
		}
		else if((i%11)==6 || (i%11)==2){
			if(licznik2==1500){
				PORTB = pgm_read_byte(&(tab[i%11]));
				PORTB ^= (1<<PB2);
				kopiaB=PORTB;
			}
			if(licznik2>=3000){
				licznik2=0;
				PORTB = pgm_read_byte(&(tab[i%11]));
				kopiaB=PORTB;
			}

			licznik2++;
			if(licznik%30>=jii){
				PORTB = kopiaB;
			}
			else{
				PORTB &= ~pgm_read_byte(&(tab[i%11]));
			}
		}
		else{
			if(licznik%30>=jii){
				PORTB |= pgm_read_byte(&(tab[i%11]));
			}
			else{
				PORTB &= ~pgm_read_byte(&(tab[i%11]));
			}
		}
}

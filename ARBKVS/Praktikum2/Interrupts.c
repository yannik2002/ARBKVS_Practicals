/*
 * Interrupts.c
 *
 * Created: 06/10/2024 11:09:43
 * Author : ysint
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED_0 PIND0
#define LED_9 PIND1
#define Zustand_0 0
#define Zustand_1 1
#define Zustand_2 2
#define Zustand_3 3

// Start: Zustand_0 wie in meinem Zustandsautomaten
int currentMode = Zustand_0;

ISR(INT0_vect)
{
	switch(currentMode)
	{
		case Zustand_0:
			currentMode = Zustand_1;
			break;
			
		case Zustand_1:
			currentMode = Zustand_0;
			break;
			
		case Zustand_2:
			currentMode = Zustand_0;
			break;
			
		case Zustand_3:
			currentMode = Zustand_1;
			break;
			
		default:
			break;
	}
}

ISR(INT1_vect)
{
	switch (currentMode)
	{
		case Zustand_0:
			currentMode = Zustand_2;
			break;
			
		case Zustand_1:
			currentMode = Zustand_3;
			break;
			
		case Zustand_2:
			currentMode = Zustand_3;
			break;
			
		case Zustand_3:
			currentMode = Zustand_2;
			break;
			
		default:
			break;
	}
}

int main(void)
{
	DDRD = 0b00000011; // PD0 und PD1 auf Ausgang, rest auf Eingang
	PORTD = 0b11111100; // Ausgänge aus, Eingänge auf Pull-Up Widerstand
	
	// BLOCKDIAGRAMM S.13
	// PD3,PD2 -> INT[1:0]
	// INT1 (PD3), INT0 (PD2) sind standardisiert
	
	// S. 89/90 waren wichtig: 
	// External Interrupt Control Register A
	// INT1 1/0 (ISC11 and ISC10)
	// INT0 1/0 (ISC01 and ISC00)
	// Value 1/0 für interrupt request bei falling edge, also das erste der beiden Bits auf 1 stellen
	// Damit interrupt request entsteht, wenn Switch1/Switch2 gedrückt werden und von 1 auf 0 gehen (wg. Pull up Widerstand)
	EICRA |= (1 << ISC11) | (1 << ISC01);
	
	// External Interrupt Mask Register
	// Bit0 und Bit1 im EIMSK Register steuern, ob externe Interrupts an diesen Pins aktiv sind
	// hier werden dann beide aktiviert
	EIMSK |= (1<<INT0); // externer Interrupt für PIND2 aktivieren
	EIMSK |= (1<<INT1); // externer Interrupt für PIND3 aktivieren
	
	// Global interrupt enable
	sei();
	
	while (1) { adjustLEDs(); }
}

void adjustLEDs()
{
	switch(currentMode)
	{
		// LED_0 DAUER
		case Zustand_0:
			PORTD &= ~(1<<LED_9); // LED_9 aus
			PORTD |= (1<<LED_0); // LED_0 einschalten
			_delay_ms(200);
			break;
		
		// LED_0 TOGGLE
		case Zustand_1:
			PORTD &= ~(1<<LED_9); // LED_9 aus
			PORTD ^= (1<<LED_0); // LED_0 toggle
			_delay_ms(200);
			break;
		
		// LED_9 DAUER
		case Zustand_2:
			PORTD &= ~(1<<LED_0); // LED_0 aus
			PORTD |= (1<<LED_9); // LED_9 einschalten
			_delay_ms(200);
			break;
		
		// LED_9 TOGGLE
		case Zustand_3:
			PORTD &= ~(1<<LED_0); // LED_0 aus
			PORTD ^= (1<<LED_9); // LED_9 toggle
			_delay_ms(200);
			break;
		
		default:
			break;
	}
}


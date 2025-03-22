/*
 * Polling.c
 *
 * Created: 05/10/2024 12:50:42
 * Author : ysint
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#define LED_0 PIND0
#define LED_9 PIND1
#define Zustand_0 0
#define Zustand_1 1
#define Zustand_2 2
#define Zustand_3 3

// Start: Zustand_0 wie in meinem Zustandsautomaten
int currentMode = Zustand_0;

int main(void)
{
	DDRD = 0b00000011; // PD0 und PD1 auf Ausgang, rest auf Eingang
	PORTD = 0b11111100; // Ausgänge aus, Eingänge auf Pull-Up Widerstand
	
	while(1)
	{
		// prüft ob PIND2 = 0 (gedrückt), da active low
		if(!(PIND & (1<<PIND2))) 
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
		// prüft ob PIND3 = 0 (gedrückt), da active low
		else if(!(PIND & (1<<PIND3)))
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
		adjustLEDs();
	}
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

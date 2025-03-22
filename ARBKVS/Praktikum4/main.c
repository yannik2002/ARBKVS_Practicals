/*
 * Praktikum4.c
 *
 * Created: 16/10/2024 15:51:34
 * Author : ysint
 */ 

/*
Aufbau:
PD0 - a
PD1 - b
PD2 - c
PD3 - d
PD4 - e
PD5 - f
PD6 - g
PD7 wird nicht benutzt -> das größte Bit ist "egal"
7-Segment Anzeige ist active-Low
0 -> AN
1 -> AUS
PB0 - A, Umschalten zwischen 10er und 1er Stelle
Mindestens 50x pro Sekunde umschalten (Timer), hier 100x da kein Flackern 
SW2 - PC0, hochzählen
SW1 - PC1, runterzählen
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "sevenseg.h"
#include "keys.h"

int counter = 0; // Startzahl

static const int digitSegments[] = { // Konstant, active low
	// PD7, PD6, ..., PD1, PD0 (PD7 immer 0, da nicht gebraucht bzw. 0-6 schon a-g abdecken)
	// Hex Werte von 0-9 an den entsprechend gleichen Stellen im Array
	0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10 
};

void init_ports() {
	DDRD = 0xFF; // PD als Ausgang
	PORTD = 0x00; // Alle Ausgänge aus
	DDRB = 0xFF; // PB als Ausgang
	PORTB = 0x00; // Alle Ausgänge aus
	DDRC = 0x00; // PC als Eingang
	PORTC = 0xFF; // Pullups aktivieren
}

int main(void)
{
	init_ports();
	init_keys();
	init_sevenseg();
	sei(); // Aktivierung globaler Interrupts
	
	// Schleife bleibt leer, da alles über Interrupts gesteuert wird
	while(1) {}
	return 0;
}

// erhöht den Counter, wraparound bei 99
void increment_counter() {
	counter = (counter < 99) ? counter + 1 : 0;
}

// erniedrigt den Counter, wraparound bei 0
void decrement_counter() {
	counter = (counter > 0) ? counter - 1 : 99;
}

// ISR beim drücken von Switches
ISR(PCINT1_vect) {
	// SW2 gedrückt: hochzählen
	if (!(PINC & (1 << PINC0))) {
		increment_counter();
	}
	// SW1 gedrückt: runterzählen
	else if (!(PINC & (1 << PINC1))) {
		decrement_counter();
	}
}

// ISR für den Timer
ISR(TIMER1_COMPA_vect) {
	// Toggle zwischen 10er und 1er Stelle
	PORTB ^= (1 << PINB0);
	// Anzeige entsprechend der aktuellen Stelle aktualisieren
	if (PORTB == 0b00000000) {
		PORTD = digitSegments[counter % 10]; // 1er Stelle
		} else {
		PORTD = digitSegments[counter / 10]; // 10er Stelle (klappt wegen int division)
	}
}


/*
 * Praktikum3.c
 *
 * Created: 16/10/2024 12:26:00
 * Author : ysint
 */

//a)
//volatile stellt Compileroptimierung aus, somit wird der Wert bei jedem Zugriff neu abgefragt und nicht zwischengespeichert
//ms_counter ändert sich außerhalb der normalen Programmausführung, da der Zähler hier in der ISR inkrementiert wird
//b)
//uint32_t ist unsigned 32 bit, also bis max (2^32)-1 = 4.294.976.295
//somit dauert es (2^32)-1 ms -> 4294967,295 sek -> 71582,78825 min -> 49,710269618056 Tage (Google Rechner benutzt zum umrechnen)
//c und d)
//overflow verhindern durch wraparound bei max_ms_counter und nutzung von sek_counter/min_counter und reset vom vorgänger

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint32_t ms_counter = 0;

void init_timer() {
	// S.171-173
	// TCCR1A/B/C are 8-bit registers
	// Interrupts are masked with TIMSK1 register
	// Timer can be clocked internally via the prescaler 
	TCCR1B |= (1 << WGM12); // Clear Timer on Compare match (CTC Mode)
	// bei CTC zählt der timer immer bis OCR1A hoch und wird danach resetted
	// Prescaler bits, determine the timers clock relative to the system clock
	// CS12 | CS11 | CS10 | TOP
	//    0 |    1 |    1 | OCR1A
	TCCR1B |= (1 << CS11) | (1 << CS10); // Prescaler 64
	// 16.000.000 Takte pro sek / 64 = 250.000 Takte pro sek
	// -> 250.000 Takte pro sek / 1000 = 250 Takte pro ms
	// Zähle bis 250, um 1ms zu erreichen
	OCR1A = 250;
	// Timer-Interrupts (1) aktivieren S.184
	// Jede ms wird interrupt getriggered
	// Output Compare A Match Interrupt Enable
	TIMSK1 |= (1 << OCIE1A);
	
	sei();
}

void waitFor(uint32_t ms) {
	// target_time berechnen
	uint32_t target_time = ms_counter + ms;
	// warten, bis target_time erreicht wurde
	waitUntil(target_time);
}

void waitUntil(uint32_t ms) { 
	while (ms_counter < ms); // warten in while-schleife
}

// TIMER1 Compare Match A
ISR(TIMER1_COMPA_vect) {
	ms_counter++;
}

int main(void)
{
	DDRB = 0b00000001; // PB0 als Ausgang, einfach an LED D0 anschließen
	PORTB = 0b00000000; // Erstmal alles aus
	init_timer();
	
	// Ablauf: 5 sek an, 2.5 sek aus, 2.5 sek an, jede halbe sek toggle
	PORTB ^= (1 << PINB0);
	waitUntil(5000);
	PORTB ^= (1 << PINB0);
	waitUntil(7500);
	PORTB ^= (1 << PINB0);
	waitUntil(10000);
    while (1) {
		PORTB ^= (1 << PINB0); 
		waitFor(500);
    }
}


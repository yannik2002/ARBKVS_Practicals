/*
 * sevenseg.c
 *
 * Created: 16/10/2024 16:04:16
 *  Author: ysint
 */ 

#include "sevenseg.h"

void init_sevenseg() {
	// Timer gleich wie P3, nur größerer Wert für OCR1A
	TCCR1B |= (1 << WGM12); // CTC-Mode einstellen
	TCCR1B |= (1 << CS11) | (1 << CS10); // Prescaler 64
	TIMSK1 |= (1 << OCIE1A); // Timer Interrupt einschalten
	// 16.000.000 / 64 = 250.000 Takte pro Sekunde
	// 5000 := 50x in der Sekunde, 250.000 / 5.000 = 50 Hz
	// Flackert sichtbar, deshalb...
	// 2500 := 100x in der Sekunde, 250.000 / 2.500 = 100 Hz
	OCR1A = 2500;
}

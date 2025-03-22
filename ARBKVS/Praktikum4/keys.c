/*
 * keys.c
 *
 * Created: 16/10/2024 16:04:31
 *  Author: ysint
 */ 

#include "keys.h"

void init_keys() {
	// Sind die Taster an PC0 und PC1 nutzen wir PCINT8 und PCINT9, 
	// dementsprechend müssen die Register PCICR und PCMSK1 konfiguriert werden (Übung)
	// Pin Change Interrupt 1 is enabled
	PCICR |= (1 << PCIE1);
	// PC1 (PCINT9) und PC0 (PCINT8) (S.14)
	// selects whether pin change interrupt is enabled on the corresponding I/O (S.96)
	PCMSK1 |= (1 << PCINT8) | (1 << PCINT9);
}
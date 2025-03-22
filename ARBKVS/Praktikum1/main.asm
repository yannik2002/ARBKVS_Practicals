;
; Praktikum1.asm
;
; Created: 24/09/2024 13:17:37
; Author : ysint
;

.include "m328pdef.inc" ; Prozessordefinitionen laden (Controller Konstanten etc.)
.org 0x000				; Reset Vector setzen
rjmp init				

; Konstanten 
.equ PORTD_OUTPUT = 0b11111111
.equ PORTB_OUTPUT = 0b00000011
.equ LED_ALL_LOW = 0b00000000
.equ LED_FIRST = 0b00000001
.equ LED_LAST = 0b10000000

; Aliase
.def D_dir_left_done_comp = r17
.def D_dir_right_done_comp = r18
.def Laufrichtung = r19 ; 0 = links, 1 = rechts
.def Logisch_Eins = r20 

init:
	; Stackpointer initialisieren
	ldi r16, LOW(RAMEND)
	out SPL, r16
	ldi r16, HIGH(RAMEND)
	out SPH, r16

	; Ausgänge setzen
	ldi r16, PORTD_OUTPUT
	out DDRD, r16			; Alle Pins von Port D auf Ausgang (1) setzen (LED 0-7)
	ldi r16, PORTB_OUTPUT
	out DDRB, r16			; Ersten beiden Pins von B auf Ausgang (1) setzen (LED 8-9)

	; Register initialisieren
	ldi r16, LED_FIRST						; erste Leuchte setzen
	ldi D_dir_left_done_comp, LED_LAST		; Vergleichswert: D links einmal fertig durchlaufen
	ldi D_dir_right_done_comp, LED_FIRST	; Vergleichswert: D rechts einmal fertig durchlaufen
	ldi Laufrichtung, 0						; Laufrichtung 0: Links, Laufrichtung 1: Rechts
	ldi Logisch_Eins, 1						; Vergleichswert

; Blinkprogramm für Port D
D_blink:
	cp Laufrichtung, Logisch_Eins		; Laufrichtung = 0 -> dir_links, Laufrichtung = 1 -> dir_rechts
	breq dir_right						; falls Laufrichtung = 1 in dir_right springen, sonst dir_left

	; Durchlaufe Port D nach links
	dir_left:
		out PORTD, r16					; Ausgabe der ersten LED
		rcall delay	   
		cp r16, D_dir_left_done_comp	; falls D fertig nach links durchlaufen in B_blink springen
		breq B_blink
		LSL r16							; Linksshift
		rjmp dir_left					; So lange bis linksshift bis D nach links fertig durchlaufen wurde

	; Durchlaufe Port D nach rechts
	dir_right:
		cp r16, D_dir_right_done_comp	; Vergleich: wenn nach rechts fertig durchlaufen
		breq dir_right_done				; Sprung zu dir_right_done den ganzen Zyklus von vorne zu starten
		out PORTD, r16					; die linkeste LED von D zum leuchten bringen
		rcall delay
		LSR r16							; Rechtsshift
		rjmp dir_right					; So lange rechtsshift bis D nach rechts fertig durchlaufen wurde

	; Fertig nach rechts durchlaufen, Laufrichtung resetten und alles von vorne
	dir_right_done:
		ldi Laufrichtung, 0	; Laufrichtung = 0 für dir_left setzen
		rjmp D_blink


; Blinkprogramm für Port B		
B_blink:
	; Solange B durchlaufen wird alle LEDs von D ausschalten
	ldi r16, LED_ALL_LOW
	out PORTD, r16			
	
	; LEDs 8-9 mit Bitshifts durchlaufen
	ldi r16, LED_FIRST						
	out PORTB, r16
	rcall delay	
	LSL r16		
	out PORTB, r16
	rcall delay
	LSR r16	
	out PORTB, r16
	rcall delay
	ldi r16, LED_ALL_LOW	
	out PORTB, r16
	
	ldi r16, LED_LAST		; LED8 in r16 laden für den Rückweg
	ldi Laufrichtung, 1		; Laufrichtung = 1 setzen für Rückweg bzw. dir_rechts
	rjmp D_blink

delay:
; Assembly code auto-generated
; by utility from Bret Mulvey
; Delay 3 200 000 cycles
; 200ms at 16 MHz

    ldi  r22, 17
    ldi  r23, 60
    ldi  r24, 204
L1: dec  r24
    brne L1
    dec  r23
    brne L1
    dec  r22
    brne L1
	ret



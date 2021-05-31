#include "Buzzer.h"

#define LONG_TONE 1500
#define SHORT_TONE 500
#define OK_FREQ 2500
#define ERR_FREQ 250

Buzzer::Buzzer(int pin) {
	this->pin = pin;
	pinMode(pin,OUTPUT);
}

void Buzzer::genericOK() {
	tone(pin, OK_FREQ, SHORT_TONE);
	delay(SHORT_TONE);
}

void Buzzer::genericError() {
	tone(pin, ERR_FREQ, SHORT_TONE);
	delay(SHORT_TONE);
}

/**
 * 10 short beeps signalling disconnection from the station.
 */
void Buzzer::SMSSendingBeep() {
	for (byte i = 0; i < 3; i++) {
		tone(pin, OK_FREQ, SHORT_TONE);
		delay(SHORT_TONE + 200);
	}
}

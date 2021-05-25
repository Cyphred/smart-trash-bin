#include "Buzzer.h"

#define LONG_TONE 1500
#define SHORT_TONE 500
#define OK_FREQ 2500
#define ERR_FREQ 500

Buzzer::Buzzer(int pin) {
	this->pin = pin;
	pinMode(pin,OUTPUT);
}

void Buzzer::genericOK() {
	tone(pin, OK_FREQ, SHORT_TONE);
}

void Buzzer::genericError() {
	tone(pin, ERR_FREQ, SHORT_TONE);
}

/**
* 1 long beep upon successfully connecting to the station and initializing all variables.
*/
void Buzzer::initSuccess() {
	tone(pin, OK_FREQ, LONG_TONE);
}

/**
 * 10 short beeps signalling disconnection from the station.
 */
void Buzzer::disconnected() {
	for (byte i = 0; i < 10; i++) {
		tone(pin, OK_FREQ, SHORT_TONE);
		delay(SHORT_TONE + 200);
	}
}

/**
* 3 long buzzes with 200 ms in between.
* This means the transceiver is undetectable by the Arduino.
* This error requires manual inspection/diagnosis.
*/
void Buzzer::radioError() {
	for (byte i = 0; i < 3; i++) {
		tone(pin, ERR_FREQ, LONG_TONE);
		delay(LONG_TONE + 200);
	}
}

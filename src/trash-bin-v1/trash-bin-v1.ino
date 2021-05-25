/**
* Sketch for a prototype of a smart trash bin with a notification
* system for when it is full.
* =================================================================
* Written by Jeremy Andrews I. Zantua
* jeremyzantua@gmail.com
* 25 May 2021
* =================================================================
* IMPORTANT NOTES
* - This sketch has been used and tested only on the following hardware:
*   - A chinese clone of the Arduino UNO. It is the version running an ATMEGA 328PU
*   - SIM800L V1
*   - HC-SR04
*/

#include "gsm.h"
// Maps the SIM800L TX and RX pins to digital pins on the arduino
#define SIM800L_TX_PIN 8
#define SIM800L_RX_PIN 7
#define SIM800L_BAUD_RATE 9600
#define ACTION_TIMEOUT 5000
gsm sim800l(SIM800L_TX_PIN,SIM800L_RX_PIN, SIM800L_BAUD_RATE);

#include "sr04.h"
// Maps the HC-SR04
#define ECHO_PIN 5
#define TRIG_PIN 6
sr04 sensor(ECHO_PIN, TRIG_PIN);

void setup() {
	Serial.begin(115200);

	// Test if the GSM module is responding to basic AT command.
	if (! sim800l.isReady(ACTION_TIMEOUT))
		// TODO Hold and signal error

}

void loop() {
}

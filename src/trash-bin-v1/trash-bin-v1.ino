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
#define SIM800L_TX_PIN 7
#define SIM800L_RX_PIN 2
#define SIM800L_BAUD_RATE 9600
gsm sim800l(SIM800L_TX_PIN,SIM800L_RX_PIN, SIM800L_BAUD_RATE);

#include "sr04.h"
sr04 sensor_1(8, 9);
sr04 sensor_2(10, 11);
sr04 sensor_3(12, 13);

#include "Buzzer.h"
Buzzer buzzer(5);

#include "History.h"
#define FULL_THRESHOLD 30
#define MAX_STREAK_SIZE 10
History history(FULL_THRESHOLD, MAX_STREAK_SIZE);


void setup() {
	Serial.begin(115200);

	// Test if the GSM module is responding to basic AT command.
	if (sim800l.isReady(ACTION_TIMEOUT))
		buzzer.genericOK();

}

void loop() {
	int distance[3];
	delay(100);
	distance[0] = sensor_1.measure();
	delay(100);
	distance[1] = sensor_2.measure();
	delay(100);
	distance[2] = sensor_3.measure();

	Serial.print("1: ");
	Serial.print(distance[0]);
	Serial.print("cm\t\t2: ");
	Serial.print(distance[1]);
	Serial.print("cm\t\t3: ");
	Serial.print(distance[2]);
	Serial.println("cm");

	history.addPoint(distance);
	if (history.isFull())
		buzzer.genericError();
}

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
#define GSM_STATUS_CHECK_INTERVAL 5000

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

struct Status {
	unsigned long lastGsmStatusCheck = 0;
	bool gsmActive;
	bool gsmRegistered;
	int lastSignalStrength;
} status;

void setup() {
	Serial.begin(115200);
	gsm_initialize();
	//sim800l.debug();
}

void loop() {
	if ((millis() - status.lastGsmStatusCheck) >= GSM_STATUS_CHECK_INTERVAL) {
		Serial.println("Checking GSM Status...");
		status.gsmActive = sim800l.isReady(5000);

		if (status.gsmActive) {
			status.gsmRegistered = sim800l.isRegistered();
			status.lastSignalStrength = sim800l.getSignalStrength();
		}

		Serial.print("GSM ");
		printStatus(status.gsmActive);
		Serial.print("Registered ");
		printStatus(status.gsmRegistered);
		Serial.print("Signal ");
		Serial.print(status.lastSignalStrength);
		Serial.println("%");

		status.lastGsmStatusCheck = millis();
	}

	measurementRoutine();
}

void measurementRoutine() {
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

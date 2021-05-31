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
#define FULL_THRESHOLD 60
#define MAX_STREAK_SIZE 10
History history(FULL_THRESHOLD, MAX_STREAK_SIZE);

#define MAX_SMS_ALERTS 3
#define SMS_ALERT_INTERVAL 60000 // Interval in milliseconds between each SMS reminder.
#define SMS_RETRY_INTERVAL 10000 // Interval in milliseconds between each SMS retry attempt
struct Status {
	unsigned long lastGsmStatusCheck = 0;
	bool gsmActive;
	bool gsmRegistered;
	int lastSignalStrength;
	String SMSRecipient = "639267064383";
	bool full = false;
} status;

void setup() {
	Serial.begin(115200);
	gsm_initialize();

	// Uncomment this for manual debugging of GSM module
	//sim800l.debug();
}

void loop() {
	if (isTimeToCheckGSMStatus())
		checkGSMStatus();

	measurementRoutine();

	if (history.isFull()) {
		SMSAlertLoop();
	}
}

void SMSAlertLoop() {
	for (int i = 0; i < 3; i++) {
		if (SMSAlert()) {
			Serial.println("Waiting before sending next notif...");
			delay(SMS_ALERT_INTERVAL);
			continue;
		}

		// Retry loop
		while (true) {
			Serial.print("Waiting for retry");
			delay(SMS_RETRY_INTERVAL);
			bool rslt = SMSAlert();
			if (!rslt)
				break;
		}
	}

	Serial.println("Waiting to be cleared...");
	while (true) {
		;
	}
}

/**
* Routine for reading the proximity sensors.
*/
void measurementRoutine() {
	// Collect distance data from each sensor
	int distance[3];
	delay(100);
	distance[0] = sensor_1.measure();
	delay(100);
	distance[1] = sensor_2.measure();
	delay(100);
	distance[2] = sensor_3.measure();

	// Store distnace data
	history.addPoint(distance);

	// Display distance data
	Serial.print("1: ");
	Serial.print(distance[0]);
	Serial.print("cm\t\t2: ");
	Serial.print(distance[1]);
	Serial.print("cm\t\t3: ");
	Serial.print(distance[2]);
	Serial.println("cm");
}

bool SMSAlert() {
	buzzer.SMSSendingBeep();

	// Check if the GSM is ready to send an SMS
	if (!status.gsmActive || !status.gsmRegistered) {
		Serial.println("GSM not ready");
		buzzer.genericError();
		return false;
	}

	Serial.print("Sending SMS to ");
	Serial.print(status.SMSRecipient);
	Serial.print("...");
	bool result = sim800l.sendSMS(status.SMSRecipient, "NOTICE: Bin is full and requires emptying.");
	printStatus(result);

	if (result) {
		buzzer.genericOK();
		return true;
	}
	else {
		buzzer.genericError();
	}

	return false;
}

void checkGSMStatus() {
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

/**
* Checks if it is time to check the GSM module's status.
*
* @return is true if it is time. False if not.
*/
bool isTimeToCheckGSMStatus() {
	if ((millis() - status.lastGsmStatusCheck) >= GSM_STATUS_CHECK_INTERVAL)
		return true;
	return false;
}

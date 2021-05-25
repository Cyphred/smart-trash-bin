#include "sr04.h"

sr04::sr04(const int echoPin, const int trigPin) {
	this->echoPin = echoPin;
	this->trigPin = trigPin;
	pinMode(echoPin, INPUT);
	pinMode(trigPin, OUTPUT);
}

/**
 * Measures the distance detected by the sensor.
 *
 * @return is the distance in cm detected by the sensor.
 */
int sr04::measure() {
	// Clears the trigPin condition
	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);

	// Sets the trigPin HIGH (ACTIVE) for 10 microseconds
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);

	// Reads the echoPin, returns the sound wave travel time in microseconds
	long duration = pulseIn(echoPin, HIGH);

	// Calculating the distance
	int distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
	return distance;
}

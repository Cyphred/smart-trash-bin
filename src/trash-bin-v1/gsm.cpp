#include "gsm.h"

gsm::gsm(const int tx_pin, const int rx_pin, const int baud_rate) : gsm_(tx_pin, rx_pin) {
	gsm_.begin(baud_rate);
	gsmData = "";
}

bool gsm::isReady(const unsigned long timeout) {
	gsm_.println("AT");
	return waitForActionResult(timeout);
}

bool gsm::hasSIM() {
	return false;
}

bool gsm::isRegistered() {
	return false;
}

byte gsm::getSignalStrength() {
	return 0;
}

void gsm::debug() {
	while (true) {
		if (gsm_.available())
			Serial.write(gsm_.read());

		if (Serial.available())
			gsm_.write(Serial.read());
	}
}

/**
 * Checks if a character is human-readable/visible.
 *
 * @return true if character is human-readable. False if not.
 */
bool gsm::isReadableCharacter(const char character) {
	if (character >= 32 && character <= 127)
		return true;

	if (character == 13 || character == 10)
		return true;

	return false;
}

bool gsm::waitForActionResult(const unsigned long timeout) {
	unsigned long start = millis();
	while ((millis() - start) < timeout) {
		if (gsm_.available() > 0) {
			String temp = gsm_.readString();
			temp = temp.substring( (temp.length() - 4), (temp.length() - 2) );

			if (temp.equals("OK"))
				return true;
			else
				return false;
		}
	}

	return false;
}

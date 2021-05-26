#include "gsm.h"

gsm::gsm(const int tx_pin, const int rx_pin, const int baud_rate) : gsm_(tx_pin, rx_pin) {
	gsm_.begin(baud_rate);
}

bool gsm::initialize() {
	bool pass;
	for (int i = 0; i < 3; i++) {
		if (isReady(5000)) {
			pass = true;
			break;
		}
	}

	if (!pass) {
		Serial.println("GSM ERROR");
		return false;
	}

	if (!hasSIM()) {
		Serial.println("NO SIM");
		return false;
	}

	return true;
}

/**
 * Checks if the GSM module is responding to basic AT commands.
 *
 * @param timeout is the time in ms to wait for a response.
 * @return is true if a response is received within the allowed time.
 */
bool gsm::isReady(const unsigned long timeout) {
	gsm_.println("AT");
	waitForActionResult(timeout);
}

/**
 * Queries the GSM module if a valid SIM card is installed and recognized.
 *
 * @return is true if the GSM reports that a valid SIM card is installed.
 */
bool gsm::hasSIM() {
	gsm_.println("AT+CCID");
	return waitForActionResult(5000);
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

/**
 * Waits for GSM to respond to a command and interprets if its response
 * is either ERROR or OK.
 *
 * @param timeout is the time to wait for the response.
 * @return is true if the device responds with `OK` within the allowed time. Otherwise, returns false.
 */
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

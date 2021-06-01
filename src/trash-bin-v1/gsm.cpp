#include "gsm.h"

/**
 * Constructs a GSM object.
 *
 * @param tx_pin is the pin that is connected to the GSM module's TX pin
 * @param rx_pin is the pin that is connected to the GSM module's RX pin
 * @param baud_rate is the baud rate for the software serial communication between the Arduino and the GSM module.
 */
gsm::gsm(const int tx_pin, const int rx_pin, const int baud_rate) : gsm_(tx_pin, rx_pin) {
	gsm_.begin(baud_rate); // Initializes software serial with specified baud rate
}

/**
 * Checks if the GSM module is responding to a basic AT command.
 *
 * @param timeout is the time in ms to wait for a response.
 * @return is true if a response is received within the allowed time.
 */
bool gsm::isReady(const unsigned long timeout) {
	gsm_.println("AT");
	return waitForExpectedResponse(timeout, "OK");
}

/**
 * Queries the GSM module if a valid SIM card is installed and recognized.
 *
 * @return is true if the GSM reports that a valid SIM card is installed.
 */
bool gsm::hasSIM() {
	gsm_.println("AT+CCID");
	return waitForExpectedResponse(5000, "OK");
}

bool gsm::isRegistered() {
	gsm_.println("AT+CREG?");
	return waitForExpectedResponse(5000, "0,1");
}

/**
 * Queries for the signal strength and returns a computed percentage.
 *
 * @return is the signal strength in percentage relative to 30. Returns -1 if GSM returns an error.
 */
int gsm::getSignalStrength() {
	gsm_.println("AT+CSQ");
	String response = getResponseAsString(5000);

	// If response is empty, does not contain "OK", or contains "ERROR".
	if (response.length() == 0 || response.indexOf("OK") == -1 || response.indexOf("ERROR") > -1)
		return -1;

	// Trim response, leaving the first digit of the returned values,
	// then parses it as an integer to do arithmetic operations on it
	// in the next statement.
	int rawValue = response.substring(15, response.indexOf(',')).toInt(); 

	rawValue = ((float) rawValue / 30.0) * 100.0; // Percentage relative to 30, which is the highest value.
	return rawValue;
}

/**
 * Debug mode for manually sending commands to the GSM module.
 */
void gsm::debug() {
	while (true) {
		if (gsm_.available())
			Serial.write(gsm_.read());

		if (Serial.available())
			gsm_.write(Serial.read());
	}
}

/**
 * Waits for GSM to respond to a command and interprets if its response
 * contains an expected substring.
 *
 * @param timeout is the time to wait for the response.
 * @return is true if the device responds with `OK` within the allowed time. Otherwise, returns false.
 */
bool gsm::waitForExpectedResponse(const unsigned long timeout, const String expected) {
	unsigned long start = millis();
	while ((millis() - start) < timeout) {
		if (gsm_.available() > 0) {
			String temp = gsm_.readString();
			if (temp.indexOf(expected) > -1)
				return true;
			else
				return false;
		}
	}

	return false;
}

/**
 * Waits for GSM to respond to a command and return the
 * response as a string.
 *
 * @param timeout is the time to wait for the response.
 * @return is the string response.
 */
String gsm::getResponseAsString(const unsigned long timeout) {
	unsigned long start = millis();
	String temp = "";
	while ((millis() - start) < timeout) {
		if (gsm_.available() > 0) {
			temp += gsm_.readString();
			return temp;
		}
	}

	return temp;
}

/**
 * Sends an SMS to a mobile number.
 * This function blocks until it receives a response from the module declaring the message
 * has sent or failed.
 *
 * @param number is the phone number of the recipient.
 * @param message is the content of the message to be sent.
 * @return is true if the message has been successfully sent. False if not.
 */
bool gsm::sendSMS(String number, String message) {
	// Prepare GSM for sending SMS.
	gsm_.println("AT+CMGF=1");
	if (!waitForExpectedResponse(5000, "OK"))
		return false;

	number = "AT+CMGS=\"+" + number + "\"";
	gsm_.println(number); // Declare the recipient of the message.
	delay(5);
	gsm_.print(message); // Send the message contents.
	delay(5);

	// Mark the end of message contents and send.
	// byte 26 (CTRL-Z) is recognized by the GSM module as a signal to send an SMS.
	gsm_.write(26);

	// Loop until a response of `OK` or `ERROR` is received from the GSM module.
	while (true) {
		if (gsm_.available() > 0) {
			String temp = gsm_.readString();
			if (temp.indexOf("OK"))
				return true;

			if (temp.indexOf("ERROR"))
				return false;
		}
	}

	return false;
}

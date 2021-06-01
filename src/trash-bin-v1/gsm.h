#ifndef H_GSM
#define H_GSM

#include "Arduino.h"
#include <SoftwareSerial.h>	// For serial communication between the Arduino and the GSM module.

class gsm {
	public:
		gsm(const int tx_pin, const int rx_pin, const int baud_rate);
		bool isReady(unsigned long timeout);
		bool hasSIM();
		bool isRegistered();
		int getSignalStrength();
		void debug();
		bool sendSMS(String number, String message);
	private:
		SoftwareSerial gsm_;
		bool waitForExpectedResponse(const unsigned long timeout, const String expected);
		String getResponseAsString(const unsigned long timeout);
};

#endif

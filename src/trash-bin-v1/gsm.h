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
		byte getSignalStrength();
		void debug();
	private:
		SoftwareSerial gsm_;
		String gsmData;
		bool isReadableCharacter(const char character);
		bool waitForActionResult(const unsigned long timeout);
};

#endif

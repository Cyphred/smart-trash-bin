void gsm_initialize() {
	Serial.println("Initializing GSM...");
	Serial.print("BASIC...");
	bool pass;
	for (int i = 0; i < 3; i++) {
		if (sim800l.isReady(5000)) {
			pass = true;
			break;
		}
	}

	if (pass) {
		printStatus(true); buzzer.genericOK();
		status.gsmActive = true;
	}
	else {
		printStatus(false); buzzer.genericError();
	}

	Serial.print("SIM...");
	if (sim800l.hasSIM()) {
		printStatus(true); buzzer.genericOK();
	}
	else {
		printStatus(false); buzzer.genericError();
	}

	return true;
}

/**
 * Data-saving measure for repetitive string constants "OK" and "ERROR".
 *
 * @param status is the printed status.
 */
void printStatus(const bool status) {
	if (status)
		Serial.println("OK");
	else
		Serial.println("ERROR");
}

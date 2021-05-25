#ifndef H_SR04
#define H_SR04

#include "Arduino.h"

class sr04 {
	public:
		sr04(const int echoPin, const int trigPin);
		int measure();
	private:
		int echoPin;
		int trigPin;
};

#endif

#ifndef H_HISTORY
#define H_HISTORY

#include "Arduino.h"

class History {
	public:
		History(unsigned int defaultValues, unsigned int threshold);
		void addPoint(unsigned int point);
		bool dataViolatesThreshold();
	private:
		unsigned int data[20];
		byte currentPoint;
		unsigned int threshold;
};

#endif

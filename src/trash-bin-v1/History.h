#ifndef H_HISTORY
#define H_HISTORY

#include "Arduino.h"

class History {
	public:
		History(const unsigned int threshold, const int max_streak_size);
		void addPoint(int * values);
		bool isFull();
	private:
		unsigned int threshold;
		int max_streak_size;
		unsigned int current_streak_size;
};

#endif

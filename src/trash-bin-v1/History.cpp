#include "History.h"

History::History(const unsigned int threshold, const int max_streak_size) {
	this->threshold = threshold;
	this->max_streak_size = max_streak_size;
	current_streak_size = 0;
}

/**
 * Adds a data point to the history.
 *
 */
void History::addPoint(int * values) {
	// Check if all values exceed the threshold
	if (values[0] <= threshold && values[1] <= threshold && values[2] <= threshold) {
		current_streak_size++;
	}
	else {
		current_streak_size = 0;
	}
}

bool History::isFull() {
	if (current_streak_size >= max_streak_size)
		return true;
	return false;
}

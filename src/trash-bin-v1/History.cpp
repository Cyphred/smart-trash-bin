#include "History.h"

/**
 * Constructs a History object.
 *
 * @param threshold is the max threshold of the bin's contents' proximity
 * to the proximity sensors.
 * @param max_streak_size is the maximum number of consecutive sets of
 * proximity measurements that exceed the threshold.
 */
History::History(const unsigned int threshold, const int max_streak_size) {
	this->threshold = threshold;
	this->max_streak_size = max_streak_size;

	// The streak size is what will be compared to the max streak size
	// when determining the "fullness" of the bin. Incoming data sets
	// (sets of 3 proximity readings) that exceed the set threshold
	// will make this counter increment by one, but a data set that
	// does not exceed the threshold will reset this counter.
	current_streak_size = 0;
}

/**
 * Adds a data point to the history.
 *
 * @param *values is the pointer to an array containing a set of 3 readings
 * from each proximity sensor.
 */
void History::addPoint(int * values) {
	// Check if all values exceed the threshold
	if (values[0] <= threshold && values[1] <= threshold && values[2] <= threshold)
		current_streak_size++;
	else
		current_streak_size = 0;
}

/**
 * Checks if the bin is full based on the current size of the streak.
 *
 * @return is true if the streak has exceeded the max size.
 */
bool History::isFull() {
	if (current_streak_size >= max_streak_size)
		return true;
	return false;
}

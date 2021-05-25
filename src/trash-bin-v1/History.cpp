#include "History.h"

History::History(unsigned int defaultValue, unsigned int threshold) {
	currentPoint = 0;
	this->threshold = threshold;
	for (int i = 0; i < 20; i++)
		data[i] = defaultValue;
}

/**
 * Adds a data point to the history.
 *
 * @param point is the data point to be added.
 */
void History::addPoint(unsigned int point) {
	data[currentPoint] = point;
	currentPoint++;

	if (currentPoint == 20)
		currentPoint = 0;
}

/**
 * Checks if all values in data[] violates the distance threshold.
 *
 * @param threshold is the distance threshold.
 * @return true if all values violates the distance threshold. False if not.
 */
bool History::dataViolatesThreshold() {
	byte violations = 0;
	for (int i = 0; i < 20; i++) {
		if (data[i] <= threshold)
			violations++;
	}

	if (violations == 20)
		return true;

	return false;
}

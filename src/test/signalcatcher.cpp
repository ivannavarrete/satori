
#include "signalcatcher.h"


SignalCatcher signal_catcher;


void SignalCatcher::ClearSignalCounters() {
	string_signal = 0;
}


/**
 * Check whether any signale were received.
 */
bool SignalCatcher::SignalsReceived() {
	if (SumSignals() == 0)
		return false;

	return true;
}


/**
 * Check whether this signal was received n times.
 */
bool SignalCatcher::StringSignalsReceived(unsigned int n) {
	if ((string_signal == n) && (SumSignals() - string_signal == 0))
		return true;

	return false;
}


/**
 * Get the number of total signals received.
 */
unsigned int SignalCatcher::SumSignals() {
	unsigned int all_signals = string_signal;

	return all_signals;
}

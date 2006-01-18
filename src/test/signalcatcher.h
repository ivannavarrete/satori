
#ifndef SIGNALCATCHER_H
#define SIGNALCATCHER_H

#include <string>
#include <QObject>


/*
 * This class is used to receive and count Qt signals.
 */
class SignalCatcher : public QObject {
	Q_OBJECT

public:
	SignalCatcher() { ClearSignalCounters(); }
	void ClearSignalCounters();
	bool SignalsReceived();
	unsigned int SumSignals();

	bool StringSignalsReceived(unsigned int n);

public slots:
	void StringSlot(std::string str) { string_signal++; }

private:
	unsigned int string_signal;
};


#endif

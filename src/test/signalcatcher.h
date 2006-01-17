
#ifndef SIGNALCATCHER_H
#define SIGNALCATCHER_H

#include <iostream>

#include <string>
#include <QObject>

using namespace std;


/*
 * This class is used to receive and count boost signals.
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
	void StringSlot(string str) { cout << str << endl; string_signal++; }

private:

	unsigned int string_signal;
};


#endif

#include "Settings.h"

#ifndef OILCALCULATOR_H_
#define OILCALCULATOR_H_

class ChainOiler {

	public:
	ChainOiler(int pinPump);
	virtual ~ChainOiler();
	void init(Settings *settings);
	void processTick();
	void process();
	int getDistancePercent();

	int getSpeed() const {
		return _speed;
	}

	bool isSpeedPump() const {
		return _speedPump;
	}

	bool isSignalLost() const {
		return _signalLost;
	}

	void pumpOn();
	void pumpOff();

private:
	long _requiredOilTicks;
	long _remainingOilTicks;
	int _pinPump;
	bool _pumpActive;
	bool _startPump;
	bool _stopPump;
	bool _signalLost;
	unsigned long _lastTickMillis;
	int _speed;
	long _speedTickFactor;
	int _speedTicks;
	bool _speedPump;
	long _emergencyPumpInterval;
	int _speedIntervall;

	long calculateOilTicks(long tickPerRotation, long oilDistance, long rotationLength);
	void pumpOnce();
	void deactivatePumpPin();
	void activatePumpPin();
	void processPump();
	void calculateSpeed();
	void processSpeedPump();
};

#endif

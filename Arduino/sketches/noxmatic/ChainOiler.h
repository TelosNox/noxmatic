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
		return currentSpeed;
	}

	bool isSpeedPump() const {
		return speedPump;
	}

	bool isSignalLost() const {
		return signalLost;
	}

	void pumpOn();
	void pumpOff();

private:
	long requiredOilTicks;
	long remainingOilTicks;
	int pinPump;
	bool pumpActive;
	bool startPump;
	bool stopPump;
	bool signalLost;
	unsigned long lastTickMillis;
	int currentSpeed;
	long speedTickFactor;
	int speedTicks;
	bool speedPump;
	long emergencyPumpInterval;
	int speedIntervall;

	long calculateOilTicks(long tickPerRotation, long oilDistance, long rotationLength);
	void pumpOnce();
	void deactivatePumpPin();
	void activatePumpPin();
	void processPump();
	void calculateSpeed();
	void processSpeedPump();
};

#endif

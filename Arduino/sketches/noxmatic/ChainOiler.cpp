#include "ChainOiler.h"
#include "Arduino.h"

#define PUMP_ON_INTERVAL_MILLIS 750;
#define PUMP_OFF_INTERVAL_MILLIS 250;
#define SIGNAL_LOST_INTERVAL_MILLIS 600000;
#define SPEED_INTERVALL 1000;

ChainOiler::ChainOiler(int pinPump) {
	pumpActive = false;
	startPump = false;
	stopPump = true;
	this->pinPump = pinPump;
	pinMode(pinPump, OUTPUT);
	requiredOilTicks = 0;
	remainingOilTicks = 0;
	lastTickMillis = 0;
	signalLost = false;
	currentSpeed = 0;
	speedTickFactor = 0;
	speedTicks = 0;
	speedPump = false;
	emergencyPumpInterval = 600000;
	speedIntervall = SPEED_INTERVALL;
}

ChainOiler::~ChainOiler() {
}

void ChainOiler::init(Settings *settings) {
	long rotationLength = settings->getOilerRotationLength();
	long tickPerRotation = settings->getOilerTickPerRotation();
	long oilDistance = settings->getOilerDistance();
	emergencyPumpInterval = (long)settings->getOilerEmergencyInterval() * 1000;

	requiredOilTicks = calculateOilTicks(tickPerRotation, oilDistance, rotationLength);
	remainingOilTicks = requiredOilTicks / 2;
	speedTickFactor = rotationLength * 3600 / tickPerRotation;
	if (tickPerRotation > 1) {
		speedIntervall /= 2;
	}
}

long ChainOiler::calculateOilTicks(long tickPerRotation, long oilDistance, long rotationLength) {
	return tickPerRotation * oilDistance * 1000 / rotationLength;
}

void ChainOiler::processTick() {
	remainingOilTicks--;
	speedTicks++;
	if (remainingOilTicks < 1) {
		remainingOilTicks += requiredOilTicks;
		speedPump = true;
	}
	lastTickMillis = millis();
}

void ChainOiler::process() {
	static unsigned long nextEmergencyPump = 0;

	unsigned long currentMillis = millis();
	unsigned long signalLostMillis = lastTickMillis + SIGNAL_LOST_INTERVAL_MILLIS;
	if (currentMillis > signalLostMillis) {
		signalLost = true;
		//NOTLAUF
	} else {
		signalLost = false;
	}

	if (signalLost && currentMillis > nextEmergencyPump) {
		nextEmergencyPump = currentMillis + emergencyPumpInterval;
		pumpOnce();
	}

	processSpeedPump();
	processPump();
	calculateSpeed();
}

int ChainOiler::getDistancePercent() {
	unsigned long tmp = (requiredOilTicks - remainingOilTicks)* 100;
	return tmp / requiredOilTicks;
}

void ChainOiler::processPump() {
	static unsigned long pumpOnUntilMillis = 0;
	static unsigned long pumpOffUntilMillis = 0;

	unsigned long currentMillis = millis();

	if (pumpActive && pumpOnUntilMillis < currentMillis) {
		deactivatePumpPin();
		pumpOffUntilMillis = currentMillis + PUMP_OFF_INTERVAL_MILLIS;
	} else if (!pumpActive && startPump && pumpOffUntilMillis < currentMillis) {
		if (stopPump) {
			startPump = false;
		}
		pumpOnUntilMillis = currentMillis + PUMP_ON_INTERVAL_MILLIS;
		activatePumpPin();
	}
}

void ChainOiler::pumpOnce() {
	startPump = true;
	stopPump = true;
}

void ChainOiler::pumpOn() {
	startPump = true;
	stopPump = false;
}

void ChainOiler::pumpOff() {
	startPump = false;
	stopPump = true;
}

void ChainOiler::deactivatePumpPin() {
	pumpActive = false;
	digitalWrite(pinPump, LOW);
}

void ChainOiler::activatePumpPin() {
	pumpActive = true;
	digitalWrite(pinPump, HIGH);
}

void ChainOiler::calculateSpeed() {
	static unsigned long nextSpeedMillis = 0;
	static unsigned long lastCalcMillis = 0;
	static int lastSpeed = 0;

	unsigned long currentMillis = millis();
	if (currentMillis > nextSpeedMillis) {
		nextSpeedMillis = currentMillis + SPEED_INTERVALL;

		int currentTicks = speedTicks;
		unsigned long lastTick = lastTickMillis;
		long relevantMillis = lastTick - lastCalcMillis;
		lastCalcMillis = lastTick;

		long calcSpeed = ((long)currentTicks * speedTickFactor) / relevantMillis;
		calcSpeed /= 1000;
		currentSpeed = (calcSpeed + lastSpeed) / 2;

		speedTicks -= currentTicks;
		lastSpeed = calcSpeed;
	}
}

void ChainOiler::processSpeedPump() {
	if (speedPump) {
		if ((10 < currentSpeed && currentSpeed < 60) || getDistancePercent() > 10) {
			speedPump = false;
			pumpOnce();
		}
	}
}

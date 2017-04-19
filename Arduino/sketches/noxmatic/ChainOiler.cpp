#include "ChainOiler.h"
#include "Arduino.h"

#define PUMP_ON_INTERVAL_MILLIS 750;
#define PUMP_OFF_INTERVAL_MILLIS 250;
#define SIGNAL_LOST_INTERVAL_MILLIS 600000;
#define SPEED_INTERVALL 1000;

ChainOiler::ChainOiler(int pinPump) {
	_pumpActive = false;
	_startPump = false;
	_stopPump = true;
	_pinPump = pinPump;
	pinMode(_pinPump, OUTPUT);
	_requiredOilTicks = 0;
	_remainingOilTicks = 0;
	_lastTickMillis = 0;
	_signalLost = false;
	_speed = 0;
	_speedTickFactor = 0;
	_speedTicks = 0;
	_speedPump = false;
	_emergencyPumpInterval = 600000;
	_speedIntervall = SPEED_INTERVALL;
}

ChainOiler::~ChainOiler() {
}

void ChainOiler::init(Settings *settings) {
	long rotationLength = settings->getOilerRotationLength();
	long tickPerRotation = settings->getOilerTickPerRotation();
	long oilDistance = settings->getOilerDistance();
	_emergencyPumpInterval = (long)settings->getOilerEmergencyInterval() * 1000;

	_requiredOilTicks = calculateOilTicks(tickPerRotation, oilDistance, rotationLength);
	_remainingOilTicks = _requiredOilTicks / 2;
	_speedTickFactor = rotationLength * 3600 / tickPerRotation;
	if (tickPerRotation > 1) {
		_speedIntervall /= 2;
	}
}

long ChainOiler::calculateOilTicks(long tickPerRotation, long oilDistance, long rotationLength) {
	return tickPerRotation * oilDistance * 1000 / rotationLength;
}

void ChainOiler::processTick() {
	_remainingOilTicks--;
	_speedTicks++;
	if (_remainingOilTicks < 1) {
		_remainingOilTicks += _requiredOilTicks;
		_speedPump = true;
	}
	_lastTickMillis = millis();
}

void ChainOiler::process() {
	static unsigned long nextEmergencyPump = 0;

	unsigned long currentMillis = millis();
	unsigned long signalLostMillis = _lastTickMillis + SIGNAL_LOST_INTERVAL_MILLIS;
	if (currentMillis > signalLostMillis) {
		_signalLost = true;
		//NOTLAUF
	} else {
		_signalLost = false;
	}

	if (_signalLost && currentMillis > nextEmergencyPump) {
		nextEmergencyPump = currentMillis + _emergencyPumpInterval;
		pumpOnce();
	}

	processSpeedPump();
	processPump();
	calculateSpeed();
}

int ChainOiler::getDistancePercent() {
	unsigned long tmp = (_requiredOilTicks - _remainingOilTicks)* 100;
	return tmp / _requiredOilTicks;
}

void ChainOiler::processPump() {
	static unsigned long pumpOnUntilMillis = 0;
	static unsigned long pumpOffUntilMillis = 0;

	unsigned long currentMillis = millis();

	if (_pumpActive && pumpOnUntilMillis < currentMillis) {
		deactivatePumpPin();
		pumpOffUntilMillis = currentMillis + PUMP_OFF_INTERVAL_MILLIS;
	} else if (!_pumpActive && _startPump && pumpOffUntilMillis < currentMillis) {
		if (_stopPump) {
			_startPump = false;
		}
		pumpOnUntilMillis = currentMillis + PUMP_ON_INTERVAL_MILLIS;
		activatePumpPin();
	}
}

void ChainOiler::pumpOnce() {
	_startPump = true;
	_stopPump = true;
}

void ChainOiler::pumpOn() {
	_startPump = true;
	_stopPump = false;
}

void ChainOiler::pumpOff() {
	_startPump = false;
	_stopPump = true;
}

void ChainOiler::deactivatePumpPin() {
	_pumpActive = false;
	digitalWrite(_pinPump, LOW);
}

void ChainOiler::activatePumpPin() {
	_pumpActive = true;
	digitalWrite(_pinPump, HIGH);
}

void ChainOiler::calculateSpeed() {
	static unsigned long nextSpeedMillis = 0;
	static unsigned long lastCalcMillis = 0;
	static int lastSpeed = 0;

	unsigned long currentMillis = millis();
	if (currentMillis > nextSpeedMillis) {
		nextSpeedMillis = currentMillis + SPEED_INTERVALL;

		int currentTicks = _speedTicks;
		unsigned long lastTick = _lastTickMillis;
		long relevantMillis = lastTick - lastCalcMillis;
		lastCalcMillis = lastTick;

		long calcSpeed = ((long)currentTicks * _speedTickFactor) / relevantMillis;
		calcSpeed /= 1000;
		_speed = (calcSpeed + lastSpeed) / 2;

		_speedTicks -= currentTicks;
		lastSpeed = calcSpeed;
	}
}

void ChainOiler::processSpeedPump() {
	if (_speedPump) {
		if ((10 < _speed && _speed < 60) || getDistancePercent() > 10) {
			_speedPump = false;
			pumpOnce();
		}
	}
}

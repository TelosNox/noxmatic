#include "Heater.h"
#include <Arduino.h>

#define TEMPERATURE_INTERVAL 5000;
#define PWM_INTERVAL 1000;

Heater::Heater(int pinHeater, int pinTemperatureData) {
	_pinHeater = pinHeater;
	pinMode(_pinHeater, OUTPUT);

	_heatActive = false;
	_actualTemperature = 0;
	_heaterPower = 0;

	_startTemp = 0;
	_startHeat = 0;
	_maxTemp = 0;
	_maxHeat = 0;
  _oneWire = new OneWire(pinTemperatureData);
	_temperatureSensor = new DallasTemperature(_oneWire);
	_temperatureSensor->begin();
}

Heater::~Heater() {
}

void Heater::init(int startTemp, int startHeat, int maxTemp, int maxHeat) {
	_startTemp = startTemp;
	_startHeat = startHeat;
	_maxTemp = maxTemp;
	_maxHeat = maxHeat;
}

void Heater::process() {
	calculateTemperatureAndHeat();
	runPwm();
}

void Heater::calculateTemperatureAndHeat() {
	static unsigned long nextMillis = 0;

	unsigned long currentMillis = millis();

	if (nextMillis < currentMillis) {
		nextMillis = currentMillis + TEMPERATURE_INTERVAL;
		_temperatureSensor->requestTemperatures();
		float temp = _temperatureSensor->getTempCByIndex(0);
		_actualTemperature = temp * 10;
		calculateHeat();
	}
}

void Heater::calculateHeat() {
	int checkTemp = _actualTemperature;

	int heaterPower = 0;

	if (checkTemp > _startTemp) {
		_heaterPower = heaterPower;
		return;
	}

	if (checkTemp < _maxTemp) {
		heaterPower = _maxHeat;
	} else {
		heaterPower = map(checkTemp, _startTemp, _maxTemp, _startHeat, _maxHeat);
	}

	_heaterPower = heaterPower;
}

void Heater::runPwm() {
	static unsigned long nextMillis = 0;

	unsigned long currentMillis = millis();

	if (nextMillis < currentMillis) {
		if (!_heatActive && _heaterPower > 0) {
			activateHeaterPin();
			nextMillis = currentMillis + calculateOnMillis();
		} else {
			deactivateHeaterPin();
			nextMillis = currentMillis + calculateOffMillis();
		}
	}
}

void Heater::activateHeaterPin() {
	_heatActive = true;
	digitalWrite(_pinHeater, HIGH);
}

void Heater::deactivateHeaterPin() {
	_heatActive = false;
	digitalWrite(_pinHeater, LOW);
}

long Heater::calculateOffMillis() {
	return 1000 - _heaterPower * 10;
}

long Heater::calculateOnMillis() {
	return _heaterPower * 10;
}

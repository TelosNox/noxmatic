#include "Heater.h"
#include <Arduino.h>

#define TEMPERATURE_INTERVAL 10000;
#define PWM_INTERVAL 1000;

Heater::Heater(int pinHeater, int pinTemperatureData) {
	this->pinHeater = pinHeater;
	pinMode(pinHeater, OUTPUT);

	heatActive = false;
	actualTemperature = 0;
	heaterPower = 0;

	startTemp = 0;
	startHeat = 0;
	maxTemp = 0;
	maxHeat = 0;
  oneWire = new OneWire(pinTemperatureData);
	temperatureSensor = new DallasTemperature(oneWire);
	temperatureSensor->begin();
}

Heater::~Heater() {
}

void Heater::init(int startTemp, int startHeat, int maxTemp, int maxHeat) {
	this->startTemp = startTemp;
	this->startHeat = startHeat;
	this->maxTemp = maxTemp;
	this->maxHeat = maxHeat;
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
		temperatureSensor->requestTemperatures();
		float temp = temperatureSensor->getTempCByIndex(0);
		actualTemperature = temp * 10;
    Serial.println(actualTemperature);
		calculateHeat();
	}
}

void Heater::calculateHeat() {
	int checkTemp = actualTemperature;

	if (checkTemp > startTemp) {
		heaterPower = 0;
		return;
	}

	if (checkTemp < maxTemp) {
		heaterPower = maxHeat;
	} else {
		heaterPower = map(checkTemp, startTemp, maxTemp, startHeat, maxHeat);
	}
}

void Heater::runPwm() {
	static unsigned long nextMillis = 0;

	unsigned long currentMillis = millis();

	if (nextMillis < currentMillis) {
		if (!heatActive && heaterPower > 0) {
			activateHeaterPin();
			nextMillis = currentMillis + calculateOnMillis();
		} else {
			deactivateHeaterPin();
			nextMillis = currentMillis + calculateOffMillis();
		}
	}
}

void Heater::activateHeaterPin() {
	heatActive = true;
	digitalWrite(pinHeater, HIGH);
}

void Heater::deactivateHeaterPin() {
	heatActive = false;
	digitalWrite(pinHeater, LOW);
}

long Heater::calculateOffMillis() {
	return 1000 - heaterPower * 10;
}

long Heater::calculateOnMillis() {
	return heaterPower * 10;
}

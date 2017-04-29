#include "Heater.h"

#define TEMPERATURE_INTERVAL 5000;
#define PWM_INTERVAL 1000;

Heater::Heater(int pinHeater1, int pinHeater2, int pinTemperatureData) {
	this->pinHeater1 = pinHeater1;
  this->pinHeater2 = pinHeater2;
	pinMode(pinHeater1, OUTPUT);
  pinMode(pinHeater2, OUTPUT);
  deactivateHeaterPin();
  
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
  temperatureSensor->setWaitForConversion(false);
  temperatureSensor->setResolution(9);
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
		float temp = temperatureSensor->getTempCByIndex(0);
    temperatureSensor->requestTemperatures();
		actualTemperature = temp * 10;
    actualTemperature -= 150;
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
	digitalWrite(pinHeater1, HIGH);
  digitalWrite(pinHeater2, HIGH);
}

void Heater::deactivateHeaterPin() {
	heatActive = false;
	digitalWrite(pinHeater1, LOW);
  digitalWrite(pinHeater2, LOW);
}

long Heater::calculateOffMillis() {
	return 1000 - heaterPower * 10;
}

long Heater::calculateOnMillis() {
	return heaterPower * 10;
}

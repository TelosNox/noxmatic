#include <OneWire.h>
#include <DallasTemperature.h>

#ifndef HEATER_H_
#define HEATER_H_

class Heater {
public:
	Heater(int pinHeater, int pinTemperatureData);
	virtual ~Heater();
	void init(int startTemp, int startHeat, int maxTemp, int maxHeat);
	void process();

	int getActualTemperature() {
		return actualTemperature;
	}

	int getHeaterPower() {
		return heaterPower;
	}

private:
	int pinHeater;
	int actualTemperature;
	int heaterPower;
	bool heatActive;

	int startTemp;
	int startHeat;
	int maxTemp;
	int maxHeat;

	void calculateTemperatureAndHeat();
	void calculateHeat();
	void runPwm();
	void activateHeaterPin();
	void deactivateHeaterPin();
	long calculateOffMillis();
	long calculateOnMillis();
	DallasTemperature* temperatureSensor;
	OneWire* oneWire;
};

#endif /* HEATER_H_ */

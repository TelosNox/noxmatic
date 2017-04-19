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

	int getActualTemperature() const {
		return _actualTemperature;
	}

	int getHeaterPower() const {
		return _heaterPower;
	}

private:
	int _pinHeater;
	int _actualTemperature;
	int _heaterPower;
	bool _heatActive;

	int _startTemp;
	int _startHeat;
	int _maxTemp;
	int _maxHeat;

	void calculateTemperatureAndHeat();
	void calculateHeat();
	void runPwm();
	void activateHeaterPin();
	void deactivateHeaterPin();
	long calculateOffMillis();
	long calculateOnMillis();
	DallasTemperature* _temperatureSensor;
	OneWire* _oneWire;
};

#endif /* HEATER_H_ */

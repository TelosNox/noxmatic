#include <OneWire.h>
#include <DallasTemperature.h>

#ifndef HEATER_H_
#define HEATER_H_

#define TEMPERATURE_INTERVAL 5000;
#define PWM_INTERVAL 1000;

class Heater {
public:
  Heater(int pinHeater1, int pinHeater2, int pinTemperatureData) {
    this->pinHeater1 = pinHeater1;
    this->pinHeater2 = pinHeater2;
    pinMode(pinHeater1, OUTPUT);
    pinMode(pinHeater2, OUTPUT);
    
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
  
  ~Heater() {
  }
  
  void init(int startTemp, int startHeat, int maxTemp, int maxHeat) {
    deactivateHeaterPin();
    this->startTemp = startTemp;
    this->startHeat = startHeat;
    this->maxTemp = maxTemp;
    this->maxHeat = maxHeat;
    temperatureSensor->setWaitForConversion(false);
    temperatureSensor->setResolution(9);
  }
  
  void process() {
    calculateTemperatureAndHeat();
    runPwm();
  }

	int getActualTemperature() {
		return actualTemperature;
	}

	int getHeaterPower() {
		return heaterPower;
	}

private:
  DallasTemperature* temperatureSensor;
  OneWire* oneWire;
	int pinHeater1;
  int pinHeater2;
	int actualTemperature;
	int heaterPower;
	bool heatActive;

	int startTemp;
	int startHeat;
	int maxTemp;
	int maxHeat;

  void calculateTemperatureAndHeat() {
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
  
  void calculateHeat() {
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
  
  void runPwm() {
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
  
  void activateHeaterPin() {
    heatActive = true;
    digitalWrite(pinHeater1, HIGH);
    digitalWrite(pinHeater2, HIGH);
  }
  
  void deactivateHeaterPin() {
    heatActive = false;
    digitalWrite(pinHeater1, LOW);
    digitalWrite(pinHeater2, LOW);
  }
  
  long calculateOffMillis() {
    return 1000 - heaterPower * 10;
  }
  
  long calculateOnMillis() {
    return heaterPower * 10;
  }
};

#endif /* HEATER_H_ */

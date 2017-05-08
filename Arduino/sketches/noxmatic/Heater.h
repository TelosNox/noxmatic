#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMPERATURE_INTERVAL 5000;
#define PWM_INTERVAL 1000;

class Heater {
public:
  Heater(int pinHeater1, int pinHeater2, int pinTemperatureData, Settings *settings, Information *information) {
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

    this->startTemp = settings->getHeaterStartTemp();
    this->startHeat = settings->getHeaterStartPower();
    this->maxTemp = settings->getHeaterMaxTemp();
    this->maxHeat = settings->getHeaterMaxPower();
    temperatureSensor->setWaitForConversion(false);
    temperatureSensor->setResolution(9);
    this->information = information;
  }
  
  ~Heater() {
  }

  void init() {
    deactivateHeaterPin();
  }
  
  void process() {
    calculateTemperatureAndHeat();
    runPwm();
  }

private:
  DallasTemperature* temperatureSensor;
  OneWire* oneWire;
  Information *information;
  
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
      information->temperature = actualTemperature;
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
    information->heaterPower = heaterPower;
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

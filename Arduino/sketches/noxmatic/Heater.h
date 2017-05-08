#include <OneWire.h>
#include <DallasTemperature.h>
#include "Pwm.h"

#define HEATER_INTERVAL 5000;

class Heater {
public:
  Heater(int pinHeater1, int pinHeater2, Settings *settings, Information *information) {
    this->pwmHeater1 = new Pwm(pinHeater1);
    this->pwmHeater2 = new Pwm(pinHeater2);
    
    this->startTemp = settings->getHeaterStartTemp();
    this->startHeat = settings->getHeaterStartPower();
    this->maxTemp = settings->getHeaterMaxTemp();
    this->maxHeat = settings->getHeaterMaxPower();
    int balance = settings->getHeaterBalance();
    if (balance > 100) {
      balance = 100;
    }
    this->heaterBalance = balance;
    this->information = information;
  }
  
  ~Heater() {
  }

  void init() {
    pwmHeater1->init();
    pwmHeater2->init();
  }
  
  void process() {
    static unsigned long nextMillis = 0;
    unsigned long currentMillis = millis();
    if (nextMillis < currentMillis) {
      nextMillis = currentMillis + HEATER_INTERVAL;
      calculateHeat();
    }
    runPwm();
  }

private:
  Information *information;
  
	Pwm *pwmHeater1;
  Pwm *pwmHeater2;

	int startTemp;
	int startHeat;
	int maxTemp;
	int maxHeat;
  int heaterBalance;

  void calculateHeat() {
    int checkTemp = information->temperature;
    int heaterPower = 0;
    if (checkTemp < maxTemp) {
      heaterPower = maxHeat;
    } else if (checkTemp < startTemp) {
      heaterPower = map(checkTemp, startTemp, maxTemp, startHeat, maxHeat);
    }
    information->heaterPower = heaterPower;
    int heaterPower2 = heaterPower * heaterBalance / 100;
    pwmHeater1->setPower(heaterPower);
    pwmHeater2->setPower(heaterPower2);
  }
  
  void runPwm() {
    pwmHeater1->process();
    pwmHeater2->process();
  }
};

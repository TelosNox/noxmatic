#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMPERATURE_INTERVAL 5000;

class TemperatureCalculator {
public:
  TemperatureCalculator(int pinTemperatureData, Information *information) {
    actualTemperature = 0;
    oneWire = new OneWire(pinTemperatureData);
    temperatureSensor = new DallasTemperature(oneWire);
    temperatureSensor->begin();
    temperatureSensor->setWaitForConversion(false);
    temperatureSensor->setResolution(9);
    this->information = information;
    temperatureSensor->requestTemperatures();
  }
  
  ~TemperatureCalculator() {
  }

  void process() {
    calculateTemperature();
  }

private:
  DallasTemperature* temperatureSensor;
  OneWire* oneWire;
  Information *information;
  
  int actualTemperature;

  void calculateTemperature() {
    static unsigned long nextMillis = 0;
  
    unsigned long currentMillis = millis();
  
    if (nextMillis < currentMillis) {
      nextMillis = currentMillis + TEMPERATURE_INTERVAL;
      float temp = temperatureSensor->getTempCByIndex(0);
      temperatureSensor->requestTemperatures();
      actualTemperature = temp * 10 - 150;
      information->temperature = actualTemperature;
    }
  }
};

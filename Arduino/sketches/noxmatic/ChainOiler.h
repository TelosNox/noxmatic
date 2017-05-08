#include "Settings.h"
#include "Information.h"

#ifndef OILCALCULATOR_H_
#define OILCALCULATOR_H_

#define PUMP_ON_INTERVAL_MILLIS 750;
#define PUMP_OFF_INTERVAL_MILLIS 250;

class ChainOiler {
public:
  ChainOiler(int pinPump, Settings *settings, Information *information) {
    this->information = information;
    pumpActive = false;
    startPump = false;
    stopPump = true;
    this->pinPump = pinPump;
    pinMode(pinPump, OUTPUT);
    setPumpPending(false);
    emergencyPumpInterval = 600000;
    long oilDistance = settings->getOilerDistance();
    emergencyPumpInterval = (long)settings->getOilerEmergencyInterval() * 1000;
    requiredOilDistance = oilDistance * 1000;
    remainingOilDistance = requiredOilDistance /2;
  }

  ~ChainOiler() {
  }

  void init() {
    deactivatePumpPin();
  }

  void processDistance(long distance) {
    remainingOilDistance -= distance;
    if (remainingOilDistance < 1) {
      remainingOilDistance += requiredOilDistance;
      setPumpPending(true);
    }
    information->distancePercent = getDistancePercent();    
  }

  void setPumpPending(bool pending) {
    pumpPending = pending;
    information->pumpPending = pumpPending;
  }

  void process() {
    static unsigned long nextEmergencyPump = 0;
    
    unsigned long currentMillis = millis();
    boolean signalLost = information->speedSignalLost;
    if (signalLost && currentMillis > nextEmergencyPump) {
      nextEmergencyPump = currentMillis + emergencyPumpInterval;
      pumpOnce();
    }
    
    processSpeedPump();
    processPump();
  }

  void pumpOn() {
    startPump = true;
    stopPump = false;
  }
  
  void pumpOff() {
    startPump = false;
    stopPump = true;
  }

private:
  long requiredOilDistance;
  long remainingOilDistance;
	int pinPump;
	bool pumpActive;
	bool startPump;
	bool stopPump;
	bool pumpPending;
	long emergencyPumpInterval;
  Information *information;

	long calculateOilTicks(long tickPerRotation, long oilDistance, long rotationLength) {
    if (rotationLength == 0) {
      return 0;
    }
    return tickPerRotation * oilDistance * 1000 / rotationLength;
  }

	void pumpOnce() {
    startPump = true;
    stopPump = true;
  }

  void deactivatePumpPin() {
    pumpActive = false;
    digitalWrite(pinPump, LOW);
  }
  
  void activatePumpPin() {
    pumpActive = true;
    digitalWrite(pinPump, HIGH);
  }

  int getDistancePercent() {
    int percent = 0;
    if(requiredOilDistance > 0 && remainingOilDistance > 0) {
      unsigned long tmp = (requiredOilDistance - remainingOilDistance)* 100;
      percent = tmp / requiredOilDistance;
    }
    return percent;
  }

	void processPump() {
    static unsigned long pumpOnUntilMillis = 0;
    static unsigned long pumpOffUntilMillis = 0;
  
    unsigned long currentMillis = millis();
    if (pumpActive && pumpOnUntilMillis < currentMillis) {
      deactivatePumpPin();
      pumpOffUntilMillis = currentMillis + PUMP_OFF_INTERVAL_MILLIS;
    } else if (!pumpActive && startPump && pumpOffUntilMillis < currentMillis) {
      if (stopPump) {
        startPump = false;
      }
      pumpOnUntilMillis = currentMillis + PUMP_ON_INTERVAL_MILLIS;
      activatePumpPin();
    }
  }

  void processSpeedPump() {
    if (pumpPending) {
      int currentSpeed = information->speed;
      if ((10 < currentSpeed && currentSpeed < 60) || getDistancePercent() > 20) {
        setPumpPending(false);
        pumpOnce();
      }
    }
  }
  
};

#endif

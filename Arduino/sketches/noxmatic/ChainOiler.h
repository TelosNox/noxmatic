#include "Settings.h"
#include "Information.h"
#include "Pump.h"

#ifndef OILCALCULATOR_H_
#define OILCALCULATOR_H_

class ChainOiler {
public:
  ChainOiler(Pump *pump, Settings *settings, Information *information) {
    this->information = information;
    this->pump = pump;
    setPumpPending(false);
    emergencyPumpInterval = 600000;
    long oilDistance = settings->getOilerDistance();
    emergencyPumpInterval = (long)settings->getOilerEmergencyInterval() * 1000;
    requiredOilDistance = oilDistance * 1000;
    remainingOilDistance = requiredOilDistance /2;
  }

  ~ChainOiler() {
  }

  void processDistance(long distance) {
    remainingOilDistance -= distance;
    if (remainingOilDistance < 1) {
      remainingOilDistance += requiredOilDistance;
      setPumpPending(true);
    }
    information->distancePercent = getDistancePercent();    
  }

  void process() {
    static unsigned long nextEmergencyPump = 0;
    
    unsigned long currentMillis = millis();
    boolean signalLost = information->speedSignalLost;
    if (signalLost && currentMillis > nextEmergencyPump) {
      nextEmergencyPump = currentMillis + emergencyPumpInterval;
      pump->pump();
    }
    processSpeedPump();
  }

private:
  long requiredOilDistance;
  long remainingOilDistance;
	Pump *pump;
	bool pumpPending;
	long emergencyPumpInterval;
  Information *information;

  int getDistancePercent() {
    int percent = 0;
    if(requiredOilDistance > 0 && remainingOilDistance > 0) {
      unsigned long tmp = (requiredOilDistance - remainingOilDistance)* 100;
      percent = tmp / requiredOilDistance;
    }
    return percent;
  }

  void setPumpPending(bool pending) {
    pumpPending = pending;
    information->pumpPending = pumpPending;
  }

  void processSpeedPump() {
    if (pumpPending) {
      int currentSpeed = information->speed;
      if ((10 < currentSpeed && currentSpeed < 60) || getDistancePercent() > 20) {
        setPumpPending(false);
        pump->pump();
      }
    }
  }
};
#endif

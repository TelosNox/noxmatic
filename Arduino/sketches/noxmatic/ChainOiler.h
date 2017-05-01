#include "Settings.h"

#ifndef OILCALCULATOR_H_
#define OILCALCULATOR_H_

#define PUMP_ON_INTERVAL_MILLIS 750;
#define PUMP_OFF_INTERVAL_MILLIS 250;
#define SIGNAL_LOST_INTERVAL_MILLIS 600000;
#define SPEED_INTERVALL 1000;

class ChainOiler {
public:
  ChainOiler(int pinPump) {
    pumpActive = false;
    startPump = false;
    stopPump = true;
    this->pinPump = pinPump;
    pinMode(pinPump, OUTPUT);
    requiredOilTicks = 0;
    remainingOilTicks = 0;
    lastTickMillis = 0;
    signalLost = false;
    currentSpeed = 0;
    speedTickFactor = 0;
    speedTicks = 0;
    speedPump = false;
    emergencyPumpInterval = 600000;
    speedIntervall = SPEED_INTERVALL;
  }

  ~ChainOiler() {
  }

  void init(Settings *settings) {
    long rotationLength = settings->getOilerRotationLength();
    long tickPerRotation = settings->getOilerTickPerRotation();
    long oilDistance = settings->getOilerDistance();
    emergencyPumpInterval = (long)settings->getOilerEmergencyInterval() * 1000;
    requiredOilTicks = calculateOilTicks(tickPerRotation, oilDistance, rotationLength);
    remainingOilTicks = requiredOilTicks / 2;
    if (tickPerRotation != 0) {
      speedTickFactor = rotationLength * 3600 / tickPerRotation;
    }
    if (tickPerRotation > 1) {
      speedIntervall /= 2;
    }
  }

	void processTick() {
   remainingOilTicks--;
    speedTicks++;
    if (remainingOilTicks < 1) {
      remainingOilTicks += requiredOilTicks;
      speedPump = true;
    }
    lastTickMillis = millis();
  }

  void process() {
    static unsigned long nextEmergencyPump = 0;
    
    unsigned long currentMillis = millis();
    unsigned long signalLostMillis = lastTickMillis + SIGNAL_LOST_INTERVAL_MILLIS;
    if (currentMillis > signalLostMillis) {
      signalLost = true;
      //NOTLAUF
    } else {
      signalLost = false;
    }
    
    if (signalLost && currentMillis > nextEmergencyPump) {
      nextEmergencyPump = currentMillis + emergencyPumpInterval;
      pumpOnce();
    }
    
    processSpeedPump();
    processPump();
    calculateSpeed();
  }

	int getDistancePercent() {
    if(requiredOilTicks == 0) {
      return 0;
    }
   unsigned long tmp = (requiredOilTicks - remainingOilTicks)* 100;
    return tmp / requiredOilTicks;
  }


	int getSpeed() {
		return currentSpeed;
	}

	bool isSpeedPump() {
		return speedPump;
	}

	bool isSignalLost() {
		return signalLost;
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
	long requiredOilTicks;
	long remainingOilTicks;
	int pinPump;
	bool pumpActive;
	bool startPump;
	bool stopPump;
	bool signalLost;
	unsigned long lastTickMillis;
	int currentSpeed;
	long speedTickFactor;
	int speedTicks;
	bool speedPump;
	long emergencyPumpInterval;
	int speedIntervall;

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

  void calculateSpeed() {
    static unsigned long nextSpeedMillis = 0;
    static unsigned long lastCalcMillis = 0;
    static int lastSpeed = 0;
  
    unsigned long currentMillis = millis();
    if (currentMillis > nextSpeedMillis) {
      nextSpeedMillis = currentMillis + SPEED_INTERVALL;
  
      int currentTicks = speedTicks;
      unsigned long lastTick = lastTickMillis;
      long relevantMillis = lastTick - lastCalcMillis;
      lastCalcMillis = lastTick;
      long calcSpeed = 0;
      if (relevantMillis != 0) {
        calcSpeed = ((long)currentTicks * speedTickFactor) / relevantMillis;
      }
      calcSpeed /= 1000;
      currentSpeed = (calcSpeed + lastSpeed) / 2;
  
      speedTicks -= currentTicks;
      lastSpeed = calcSpeed;
    }
  }

  void processSpeedPump() {
    if (speedPump) {
      if ((10 < currentSpeed && currentSpeed < 60) || getDistancePercent() > 10) {
        speedPump = false;
        pumpOnce();
      }
    }
  }
  
};

#endif

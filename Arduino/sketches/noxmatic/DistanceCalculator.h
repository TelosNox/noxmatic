#include "Settings.h"
#include "Information.h"

#define SIGNAL_LOST_INTERVAL_MILLIS 600000;
#define SPEED_INTERVALL 500;

class DistanceCalculator {
public:
  DistanceCalculator(Settings *settings, Information *information, ChainOiler *chainOiler) {
    this->information = information;
    this->chainOiler = chainOiler;
    lastTickMillis = 0;
    setSignalLost(false);
    currentSpeed = 0;
    speedTickFactor = 0;
    speedTicks = 0;
    distancePerTick = 0;
    long rotationLength = settings->getOilerRotationLength();
    long tickPerRotation = settings->getOilerTickPerRotation();
    if (tickPerRotation != 0) {
      speedTickFactor = rotationLength * 3600 / tickPerRotation;
      distancePerTick = (float)rotationLength / tickPerRotation;
    }
  }

  ~DistanceCalculator() {
  }

  void processTick() {
    speedTicks++;
    lastTickMillis = millis();
  }

  void process() {
    unsigned long currentMillis = millis();
    unsigned long signalLostMillis = lastTickMillis + SIGNAL_LOST_INTERVAL_MILLIS;
    if (currentMillis > signalLostMillis) {
      setSignalLost(true);
      //NOTLAUF
    } else {
      setSignalLost(false);
    }
    calculateSpeed();
  }

private:
  unsigned long lastTickMillis;
  int currentSpeed;
  long speedTickFactor;
  int speedTicks;
  float distancePerTick;
  Information *information;
  ChainOiler *chainOiler;

  void setSignalLost(bool lost) {
    information->speedSignalLost = lost;
  }

  void calculateSpeed() {
    static unsigned long nextSpeedMillis = 0;
    static unsigned long lastCalcMillis = 0;
    static int lastSpeed = 0;
  
    unsigned long currentMillis = millis();
    if (currentMillis > nextSpeedMillis) {
      nextSpeedMillis = currentMillis + SPEED_INTERVALL;
  
      long currentTicks = (long)speedTicks;
      unsigned long lastTick = lastTickMillis;
      long relevantMillis = lastTick - lastCalcMillis;
      lastCalcMillis = lastTick;
      
      long calcSpeed = 0;
      if (relevantMillis != 0) {
        calcSpeed = (currentTicks * speedTickFactor) / relevantMillis;
      }
      calcSpeed /= 1000;
      currentSpeed = (calcSpeed + lastSpeed) / 2;
      information->speed = currentSpeed;

      long distance = currentTicks * distancePerTick;
      chainOiler->processDistance(distance);
  
      speedTicks -= currentTicks;
      lastSpeed = calcSpeed;
    }
  }
};

#ifndef PUMP_H_
#define PUMP_H_

#define PUMP_ON_INTERVAL_MILLIS 750;

class Pump {
public:
  Pump(int pinPump) {
    pumpActive = false;
    this->pinPump = pinPump;
    pinMode(pinPump, OUTPUT);
    pumpDeactivateMillis = 0;
  }

  ~Pump() {
  }

  void init() {
    deactivatePumpPin();
  }

  void process() {
    processPump();
  }

  void pump() {
    activatePumpPin();
  }

private:
  int pinPump;
  bool pumpActive;
  long pumpDeactivateMillis;

  void deactivatePumpPin() {
    pumpActive = false;
    digitalWrite(pinPump, LOW);
  }
  
  void activatePumpPin() {
    pumpActive = true;
    digitalWrite(pinPump, HIGH);
    pumpDeactivateMillis = millis() + PUMP_ON_INTERVAL_MILLIS;
  }

  void processPump() {
    unsigned long currentMillis = millis();
    if (pumpActive && pumpDeactivateMillis < currentMillis) {
      deactivatePumpPin();
    }
  }
};
#endif

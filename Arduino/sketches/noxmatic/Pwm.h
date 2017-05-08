class Pwm {
public:
  Pwm(int pinPwm) {
    this->pinPwm = pinPwm;
    pinMode(pinPwm, OUTPUT);
    pinActive = false;
    power = 0;
    nextMillis = 0;
  }

  ~Pwm() {
  }

  void setPower(int power) {
    this->power = power;
  }

  void init() {
    deactivatePin();
  }
  
  void process() {
    runPwm();
  }

private:
  int pinPwm;
  bool pinActive;
  int power;
  unsigned long nextMillis;

  void runPwm() {
    unsigned long currentMillis = millis();
  
    if (nextMillis < currentMillis) {
      if (!pinActive && power > 0) {
        activatePin();
        nextMillis = currentMillis + calculateOnMillis();
      } else {
        deactivatePin();
        nextMillis = currentMillis + calculateOffMillis();
      }
    }
  }
  
  void activatePin() {
    pinActive = true;
    digitalWrite(pinPwm, HIGH);
  }
  
  void deactivatePin() {
    pinActive = false;
    digitalWrite(pinPwm, LOW);
  }
  
  long calculateOffMillis() {
    return 1000 - power * 10;
  }
  
  long calculateOnMillis() {
    return power * 10;
  }
};

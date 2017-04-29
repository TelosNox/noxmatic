#include "ChainOiler.h"
#include "Settings.h"
#include "Heater.h"
#include "Display.h"
#include "CommunicationESP.h"

#define PIN_SPEED_SIGNAL D6
#define PIN_PUMP D8

#define PIN_TEMPERATURE_DATA D2
#define PIN_HEATER1 D3
#define PIN_HEATER2 D4

ChainOiler chainOiler(PIN_PUMP);
Heater heater(PIN_HEATER1, PIN_HEATER2, PIN_TEMPERATURE_DATA);
Settings settings;
Display display(&heater, &chainOiler, &settings);
CommunicationESP communication(&settings);

void setup() {
  Serial.begin(115200);
  initSettings();
  communication.connectWifi();
  pinMode(PIN_SPEED_SIGNAL, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_SPEED_SIGNAL), speedSignalTrigger, FALLING);
}

void initSettings() {
  chainOiler.init(&settings);
  heater.init(settings.getHeaterStartTemp(), settings.getHeaterStartPower(), settings.getHeaterMaxTemp(), settings.getHeaterMaxPower());
}

void loop() {
  communication.process();
  chainOiler.process();
  heater.process();
  display.process();
}

void speedSignalTrigger() {
  chainOiler.processTick();
}

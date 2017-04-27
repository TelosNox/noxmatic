#include "ChainOiler.h"
#include "Settings.h"
#include "Heater.h"
#include "Display.h"
#include "CommunicationESP.h"

#define PIN_SPEED_SIGNAL D8
#define PIN_PUMP D6

#define PIN_TEMPERATURE_DATA D2
#define PIN_HEATER D3

ChainOiler chainOiler(PIN_PUMP);
Heater heater(PIN_HEATER, PIN_TEMPERATURE_DATA);
Settings settings;
Display display(&heater, &chainOiler, &settings);
CommunicationESP communication(&settings);

void setup() {
  Serial.begin(115200);
  pinMode(PIN_SPEED_SIGNAL, INPUT);
  digitalWrite(PIN_SPEED_SIGNAL, HIGH);
  attachInterrupt(PIN_SPEED_SIGNAL, speedSignalTrigger, FALLING);
  initSettings();

  communication.connectWifi();
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

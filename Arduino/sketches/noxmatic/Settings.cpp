#include "Settings.h"
#include "EEPROM.h"

const int OILER_ROTATIONLENGTH_ADDRESS = 0;
const int OILER_TICKPERROTATION_ADDRESS = 1;
const int OILER_DISTANCE_ADDRESS = 2;
const int OILER_EMERGENCYINTERVAL_ADDRESS = 3;

const int HEATER_STARTTEMP_ADDRESS = 4;
const int HEATER_STARTPOWER_ADDRESS = 5;
const int HEATER_MAXTEMP_ADDRESS = 6;
const int HEATER_MAXPOWER_ADDRESS = 7;


Settings::Settings() {
	oilerRotationLength = EEPROM.read(OILER_ROTATIONLENGTH_ADDRESS) * 10;
	oilerTickPerRotation = EEPROM.read(OILER_TICKPERROTATION_ADDRESS);
	oilerDistance = EEPROM.read(OILER_DISTANCE_ADDRESS) * 100;
	oilerEmergencyInterval = EEPROM.read(OILER_EMERGENCYINTERVAL_ADDRESS) * 10;

	heaterStartTemp = EEPROM.read(HEATER_STARTTEMP_ADDRESS);
	heaterStartPower = EEPROM.read(HEATER_STARTPOWER_ADDRESS);
	heaterMaxTemp = EEPROM.read(HEATER_MAXTEMP_ADDRESS);
	heaterMaxPower = EEPROM.read(HEATER_MAXPOWER_ADDRESS);
}

Settings::~Settings() {
}

void Settings::persist() {
	EEPROM.write(OILER_ROTATIONLENGTH_ADDRESS, (oilerRotationLength / 10));
	EEPROM.write(OILER_TICKPERROTATION_ADDRESS, oilerTickPerRotation);
	EEPROM.write(OILER_DISTANCE_ADDRESS, (oilerDistance / 100));
	EEPROM.write(OILER_EMERGENCYINTERVAL_ADDRESS, (oilerEmergencyInterval / 10));

	EEPROM.write(HEATER_STARTTEMP_ADDRESS, heaterStartTemp);
	EEPROM.write(HEATER_STARTPOWER_ADDRESS, heaterStartPower);
	EEPROM.write(HEATER_MAXTEMP_ADDRESS, heaterMaxTemp);
	EEPROM.write(HEATER_MAXPOWER_ADDRESS, heaterMaxPower);
}

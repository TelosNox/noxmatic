/*
 * Settings.cpp
 *
 *  Created on: 13.06.2015
 *      Author: Homer
 */

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
	_oilerRotationLength = EEPROM.read(OILER_ROTATIONLENGTH_ADDRESS) * 10;
	_oilerTickPerRotation = EEPROM.read(OILER_TICKPERROTATION_ADDRESS);
	_oilerDistance = EEPROM.read(OILER_DISTANCE_ADDRESS) * 100;
	_oilerEmergencyInterval = EEPROM.read(OILER_EMERGENCYINTERVAL_ADDRESS) * 10;

	_heaterStartTemp = EEPROM.read(HEATER_STARTTEMP_ADDRESS);
	_heaterStartPower = EEPROM.read(HEATER_STARTPOWER_ADDRESS);
	_heaterMaxTemp = EEPROM.read(HEATER_MAXTEMP_ADDRESS);
	_heaterMaxPower = EEPROM.read(HEATER_MAXPOWER_ADDRESS);
}

Settings::~Settings() {
}

void Settings::setValue(int address, int value) {
	switch(address) {
	case OILER_ROTATIONLENGTH_ADDRESS:
		_oilerRotationLength = value;
		break;
	case OILER_TICKPERROTATION_ADDRESS:
		_oilerTickPerRotation = value;
		break;
	case OILER_DISTANCE_ADDRESS:
		_oilerDistance = value;
		break;
	case OILER_EMERGENCYINTERVAL_ADDRESS:
		_oilerEmergencyInterval = value;
		break;
	case HEATER_STARTTEMP_ADDRESS:
		_heaterStartTemp = value;
		break;
	case HEATER_STARTPOWER_ADDRESS:
		_heaterStartPower = value;
		break;
	case HEATER_MAXTEMP_ADDRESS:
		_heaterMaxTemp = value;
		break;
	case HEATER_MAXPOWER_ADDRESS:
		_heaterMaxPower = value;
		break;
	}
}

void Settings::persist() {
	EEPROM.write(OILER_ROTATIONLENGTH_ADDRESS, (_oilerRotationLength / 10));
	EEPROM.write(OILER_TICKPERROTATION_ADDRESS, _oilerTickPerRotation);
	EEPROM.write(OILER_DISTANCE_ADDRESS, (_oilerDistance / 100));
	EEPROM.write(OILER_EMERGENCYINTERVAL_ADDRESS, (_oilerEmergencyInterval / 10));

	EEPROM.write(HEATER_STARTTEMP_ADDRESS, _heaterStartTemp);
	EEPROM.write(HEATER_STARTPOWER_ADDRESS, _heaterStartPower);
	EEPROM.write(HEATER_MAXTEMP_ADDRESS, _heaterMaxTemp);
	EEPROM.write(HEATER_MAXPOWER_ADDRESS, _heaterMaxPower);
}

String Settings::toString() {
	String settingString;

	settingString += keyValue(OILER_ROTATIONLENGTH_ADDRESS, _oilerRotationLength);
	settingString += "&";
	settingString += keyValue(OILER_TICKPERROTATION_ADDRESS, _oilerTickPerRotation);
	settingString += "&";
	settingString += keyValue(OILER_DISTANCE_ADDRESS, _oilerDistance);
	settingString += "&";
	settingString += keyValue(OILER_EMERGENCYINTERVAL_ADDRESS, _oilerEmergencyInterval);
	settingString += "&";
	settingString += keyValue(HEATER_STARTTEMP_ADDRESS, _heaterStartTemp);
	settingString += "&";
	settingString += keyValue(HEATER_STARTPOWER_ADDRESS, _heaterStartPower);
	settingString += "&";
	settingString += keyValue(HEATER_MAXTEMP_ADDRESS, _heaterMaxTemp);
	settingString += "&";
	settingString += keyValue(HEATER_MAXPOWER_ADDRESS, _heaterMaxPower);

	return settingString;
}

String Settings::keyValue(int key, int value) {
	String result;
	result += key;
	result += "=";
	result += value;
	return result;
}

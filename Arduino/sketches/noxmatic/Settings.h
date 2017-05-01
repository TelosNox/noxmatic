#include <EEPROM.h>

#ifndef SETTINGS_H_
#define SETTINGS_H_

const int OILER_ROTATIONLENGTH_ADDRESS = 0;
const int OILER_TICKPERROTATION_ADDRESS = 1;
const int OILER_DISTANCE_ADDRESS = 2;
const int OILER_EMERGENCYINTERVAL_ADDRESS = 3;

const int HEATER_STARTTEMP_ADDRESS = 4;
const int HEATER_STARTPOWER_ADDRESS = 5;
const int HEATER_MAXTEMP_ADDRESS = 6;
const int HEATER_MAXPOWER_ADDRESS = 7;

class Settings {

public:
  Settings() {
    EEPROM.begin(512);
    oilerRotationLength = EEPROM.read(OILER_ROTATIONLENGTH_ADDRESS) * 10;
    oilerTickPerRotation = EEPROM.read(OILER_TICKPERROTATION_ADDRESS);
    oilerDistance = EEPROM.read(OILER_DISTANCE_ADDRESS) * 100;
    oilerEmergencyInterval = EEPROM.read(OILER_EMERGENCYINTERVAL_ADDRESS) * 10;
  
    heaterStartTemp = EEPROM.read(HEATER_STARTTEMP_ADDRESS);
    heaterStartPower = EEPROM.read(HEATER_STARTPOWER_ADDRESS);
    heaterMaxTemp = EEPROM.read(HEATER_MAXTEMP_ADDRESS);
    heaterMaxPower = EEPROM.read(HEATER_MAXPOWER_ADDRESS);
  }
  
  ~Settings() {
  }
  
  void persist() {
    EEPROM.write(OILER_ROTATIONLENGTH_ADDRESS, (oilerRotationLength / 10));
    EEPROM.write(OILER_TICKPERROTATION_ADDRESS, oilerTickPerRotation);
    EEPROM.write(OILER_DISTANCE_ADDRESS, (oilerDistance / 100));
    EEPROM.write(OILER_EMERGENCYINTERVAL_ADDRESS, (oilerEmergencyInterval / 10));
  
    EEPROM.write(HEATER_STARTTEMP_ADDRESS, heaterStartTemp);
    EEPROM.write(HEATER_STARTPOWER_ADDRESS, heaterStartPower);
    EEPROM.write(HEATER_MAXTEMP_ADDRESS, heaterMaxTemp);
    EEPROM.write(HEATER_MAXPOWER_ADDRESS, heaterMaxPower);
    EEPROM.commit();
  }

	int getHeaterMaxPower() {
		return heaterMaxPower;
	}

  void setHeaterMaxPower(int heaterMaxPower) {
    this->heaterMaxPower = heaterMaxPower;
  }
  
	int getHeaterMaxTemp() {
		return heaterMaxTemp;
	}

  void setHeaterMaxTemp(int heaterMaxTemp) {
    this->heaterMaxTemp = heaterMaxTemp;
  }

	int getHeaterStartPower() {
		return heaterStartPower;
	}

  void setHeaterStartPower(int heaterStartPower) {
    this->heaterStartPower = heaterStartPower;
  }

	int getHeaterStartTemp() {
		return heaterStartTemp;
	}

  void setHeaterStartTemp(int heaterStartTemp) {
    this->heaterStartTemp = heaterStartTemp;
  }

	int getOilerDistance() {
		return oilerDistance;
	}

  void setOilerDistance(int oilerDistance) {
    this->oilerDistance = oilerDistance;
  }

	int getOilerRotationLength() {
		return oilerRotationLength;
	}

  void setOilerRotationLength(int oilerRotationLength) {
    this->oilerRotationLength = oilerRotationLength;
  }

	int getOilerTickPerRotation() {
		return oilerTickPerRotation;
	}

  void setOilerTickPerRotation(int oilerTickPerRotation) {
    this->oilerTickPerRotation = oilerTickPerRotation;
  }

	int getOilerEmergencyInterval() {
		return oilerEmergencyInterval;
	}

  void setOilerEmergencyInterval(int oilerEmergencyInterval) {
    this->oilerEmergencyInterval = oilerEmergencyInterval;
  }

private:
	int oilerRotationLength;
	int oilerTickPerRotation;
	int oilerDistance;
	int oilerEmergencyInterval;
	int heaterStartTemp;
	int heaterStartPower;
	int heaterMaxTemp;
	int heaterMaxPower;
};

#endif /* SETTINGS_H_ */

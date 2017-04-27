#include "Arduino.h"

#ifndef SETTINGS_H_
#define SETTINGS_H_

class Settings {

public:
	Settings();
	virtual ~Settings();

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

	void persist();

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

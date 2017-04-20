#include "Arduino.h"

#ifndef SETTINGS_H_
#define SETTINGS_H_

class Settings {

public:
	Settings();
	virtual ~Settings();

	int getHeaterMaxPower() const {
		return heaterMaxPower;
	}

  void setHeaterMaxPower(int heaterMaxPower) {
    this->heaterMaxPower = heaterMaxPower;
  }
  
	int getHeaterMaxTemp() const {
		return heaterMaxTemp;
	}

  void setHeaterMaxTemp(int heaterMaxTemp) {
    this->heaterMaxTemp = heaterMaxTemp;
  }

	int getHeaterStartPower() const {
		return heaterStartPower;
	}

  void setHeaterStartPower(int heaterStartPower) {
    this->heaterStartPower = heaterStartPower;
  }

	int getHeaterStartTemp() const {
		return heaterStartTemp;
	}

  void setHeaterStartTemp(int heaterStartTemp) {
    this->heaterStartTemp = heaterStartTemp;
  }

	int getOilerDistance() const {
		return oilerDistance;
	}

  void setOilerDistance(int oilerDistance) {
    this->oilerDistance = oilerDistance;
  }

	int getOilerRotationLength() const {
		return oilerRotationLength;
	}

  void setOilerRotationLenght(int oilerRotationLength) {
    this->oilerRotationLength = oilerRotationLength;
  }

	int getOilerTickPerRotation() const {
		return oilerTickPerRotation;
	}

  void setOilerTickPerRoatation(int oilerTickPerRotation) {
    this->oilerTickPerRotation = oilerTickPerRotation;
  }

	int getOilerEmergencyInterval() const {
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

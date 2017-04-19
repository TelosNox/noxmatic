#include "Arduino.h"

#ifndef SETTINGS_H_
#define SETTINGS_H_

class Settings {

public:
	Settings();
	virtual ~Settings();

	int getHeaterMaxPower() const {
		return _heaterMaxPower;
	}

	int getHeaterMaxTemp() const {
		return _heaterMaxTemp;
	}

	int getHeaterStartPower() const {
		return _heaterStartPower;
	}

	int getHeaterStartTemp() const {
		return _heaterStartTemp;
	}

	int getOilerDistance() const {
		return _oilerDistance;
	}

	int getOilerRotationLength() const {
		return _oilerRotationLength;
	}

	int getOilerTickPerRotation() const {
		return _oilerTickPerRotation;
	}

	int getOilerEmergencyInterval() const {
		return _oilerEmergencyInterval;
	}

	String toString();

	void setValue(int address, int value);

	void persist();

private:
	int _oilerRotationLength;
	int _oilerTickPerRotation;
	int _oilerDistance;
	int _oilerEmergencyInterval;
	int _heaterStartTemp;
	int _heaterStartPower;
	int _heaterMaxTemp;
	int _heaterMaxPower;

	String keyValue(int key, int value);
};

#endif /* SETTINGS_H_ */

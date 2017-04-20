#ifndef DISPLAY_H_
#define DISPLAY_H_
#include <U8g2lib.h>
#include "Heater.h"
#include "ChainOiler.h"
#include "Settings.h"

class Display {
public:
	Display(Heater *heater, ChainOiler *chainOiler, Settings *settings);
	virtual ~Display();
	void process();
 
	void setPumpRunning(bool pumpRunning) {
		_pumpRunning = pumpRunning;
	}

private:
	void processRefresh();
	void drawNormal();
	U8G2 *_u8g;
	Heater *_heater;
	ChainOiler *_chainOiler;
	Settings *_settings;
	bool _pumpRunning;
	void printTemperature(int temperature);
	void drawHeatpower(int heaterPower);
	void drawDistance(int distancePercent, bool blink);
	void drawSpeed(int speed);
};

#endif
#include "Display.h"
#include "Arduino.h"

#define REFRESH_INTERVAL_MILLIS 500;
#define SETUP_SWITCH_INTERVAL_MILLIS 5000;

const int DISPLAY_WIDTH = 102;

Display::Display(Heater *heater, ChainOiler *chainOiler, Settings *settings) {
  _u8g = new U8G2_UC1701_EA_DOGS102_F_4W_SW_SPI(0, 9, 8, 12, 10 , 11);
	_u8g->setColorIndex(1);
	_heater = heater;
	_chainOiler = chainOiler;
	_settings = settings;
	_pumpRunning = false;
	_u8g->setContrast(45);
}

Display::~Display() {
}

void Display::process() {
	processRefresh();
}

void Display::processRefresh() {
	static unsigned long nextRefreshMillis = 0;

	unsigned long currentMillis = millis();
	if (currentMillis > nextRefreshMillis) {
		nextRefreshMillis = currentMillis + REFRESH_INTERVAL_MILLIS;
    drawNormal();
	}
}

void Display::drawNormal() {
	static bool distanceVisible = true;

	int temperature = _heater->getActualTemperature();
	int heaterPower = _heater->getHeaterPower();
	int distancePercent = _chainOiler->getDistancePercent();
	bool signalLost = _chainOiler->isSignalLost();
	int speed = _chainOiler->getSpeed();
	bool speedPump = _chainOiler->isSpeedPump();

	if (speedPump) {
		distanceVisible = !distanceVisible;
	}

	_u8g->firstPage();
	do {
		printTemperature(temperature);
		drawHeatpower(heaterPower);

		if (signalLost) {
			_u8g->setFont(u8g_font_6x13);
			_u8g->setCursor(0, 63);
			_u8g->print("-Notlauf!-");
		} else {
			drawSpeed(speed);
			if (!speedPump || distanceVisible) {
				drawDistance(distancePercent, true);
			}
		}
	} while (_u8g->nextPage());
}

void Display::printTemperature(int temperature) {
	_u8g->setFont(u8g2_font_fub25_tn);
	int xRef = DISPLAY_WIDTH / 2 - 40;
	int posX = xRef;
	int posY = 31;

	bool drawMinus = false;
	if (temperature < 0) {
		int offset = _u8g->getStrWidth("-0") - _u8g->getStrWidth("0");
		posX -= offset;
		temperature *= -1;
		drawMinus = true;
	}

	if (temperature < 100) {
		int offset = _u8g->getStrWidth("00") - _u8g->getStrWidth("0");
		posX += offset;
	}

	_u8g->setCursor(posX, posY);

	if (drawMinus) {
		_u8g->print("-");
	}

	_u8g->print(temperature / 10);
	_u8g->print(",");
	_u8g->print(temperature % 10);
	_u8g->drawCircle(xRef + 72, 7, 3);
}

void Display::drawHeatpower(int heaterPower) {
	int xRef = DISPLAY_WIDTH - 11;
	_u8g->drawVLine(xRef + 10, 0, 50);
	for (int i = 0; i <= 50; i += 10) {
		int y = i;
		_u8g->drawHLine(xRef + 8, y, 3);
	}
	int heaterYPos = map(_heater->getHeaterPower(), 0, 100, 0, 51);
	int y = 50 - heaterYPos;
	_u8g->drawHLine(xRef, y, 9);
	_u8g->drawHLine(xRef, y + 1, 7);
	_u8g->drawHLine(xRef, y - 1, 7);
	_u8g->drawHLine(xRef, y + 2, 5);
	_u8g->drawHLine(xRef, y - 2, 5);
	_u8g->drawHLine(xRef, y + 3, 3);
	_u8g->drawHLine(xRef, y - 3, 3);
	_u8g->setFont(u8g_font_6x13);
	int posX = xRef - 6;
	if (heaterPower > 0) {
		if (heaterPower < 10) {
			int offset = _u8g->getStrWidth("00") - _u8g->getStrWidth("0");
			posX += offset;
		}
		_u8g->setCursor(posX, 63);
		_u8g->print(heaterPower);
		_u8g->print("%");
	} else {
		_u8g->setCursor(posX, 63);
		_u8g->print("aus");
	}
}

void Display::drawDistance(int distancePercent, bool blink) {
	const int length = 71;

	int distanceX = map(distancePercent, 0, 100, 0, length);

	int xPos = 3;
	_u8g->drawHLine(xPos, 63, length);
	for (int i = 0; i <= length - 1; i += 14) {
		int x = xPos + i;
		_u8g->drawVLine(x, 59, 4);
	}
	_u8g->drawHLine(xPos, 58, length);

	_u8g->drawBox(xPos - 3, 58, 3, 6);
	_u8g->drawBox(xPos + length, 58, 3, 6);

	_u8g->drawBox(xPos, 60, distanceX, 2);
}

void Display::drawSpeed(int speed) {
	_u8g->drawFrame(0, 43, 32, 16);
	_u8g->setFont(u8g_font_9x15B);
	int xPos = 3;
	if (speed < 100) {
		int offset = _u8g->getStrWidth("00") - _u8g->getStrWidth("0");
		xPos += offset;
		if (speed < 10) {
			xPos += offset;
		}
	}
	_u8g->setCursor(xPos, 56);
	_u8g->print(speed);
	_u8g->setFont(u8g_font_4x6);
	_u8g->setCursor(33, 50);
	_u8g->print("km");
	_u8g->setCursor(35, 57);
	_u8g->print("h");
	_u8g->drawHLine(33, 51, 7);
}

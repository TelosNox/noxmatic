#ifndef DISPLAY_H_
#define DISPLAY_H_
#include <U8g2lib.h>
#include "Heater.h"
#include "ChainOiler.h"
#include "Settings.h"
#include <SPI.h>
#include <Wire.h>

#define REFRESH_INTERVAL_MILLIS 500;
#define SETUP_SWITCH_INTERVAL_MILLIS 5000;

const int DISPLAY_WIDTH = 102;

class Display {
public:
  Display(Heater *heater, ChainOiler *chainOiler, Settings *settings) {
    u8g = new U8G2_UC1701_EA_DOGS102_F_4W_HW_SPI(U8G2_R0, D1, D0, D3);
    u8g->setColorIndex(1);
    this->heater = heater;
    this->chainOiler = chainOiler;
    this->settings = settings;
    pumpRunning = false;
    u8g->begin();
  
    u8g->clearBuffer();
    u8g->setFont(u8g_font_6x13);
    u8g->setCursor(0, 63);
    u8g->print("-WIFI Connect!-");
    u8g->sendBuffer();
  }
  
  ~Display() {
  }
  
  void process() {
    processRefresh();
  }
 
	void setPumpRunning(bool pumpRunning) {
		this->pumpRunning = pumpRunning;
	}

private:
  U8G2 *u8g;
  Heater *heater;
  ChainOiler *chainOiler;
  Settings *settings;
  bool pumpRunning;

  void processRefresh() {
    static unsigned long nextRefreshMillis = 0;
  
    unsigned long currentMillis = millis();
    if (currentMillis > nextRefreshMillis) {
      nextRefreshMillis = currentMillis + REFRESH_INTERVAL_MILLIS;
      drawNormal();
    }
  }
  
  void drawNormal() {
    static bool distanceVisible = true;
  
    int temperature = heater->getActualTemperature();
    
    int heaterPower = heater->getHeaterPower();
    int distancePercent = chainOiler->getDistancePercent();
    bool signalLost = chainOiler->isSignalLost();
    int speed = chainOiler->getSpeed();
    bool speedPump = chainOiler->isSpeedPump();
  
    if (speedPump) {
      distanceVisible = !distanceVisible;
    }
  
    u8g->clearBuffer();
  
    printTemperature(temperature);
    drawHeatpower(heaterPower);
  
    if (signalLost) {
      u8g->setFont(u8g_font_6x13);
      u8g->setCursor(0, 63);
      u8g->print("-Notlauf!-");
    } else {
      drawSpeed(speed);
      if (!speedPump || distanceVisible) {
        drawDistance(distancePercent, true);
      }
    }
    u8g->sendBuffer();
  }
  
  void printTemperature(int temperature) {
    u8g->setFont(u8g2_font_fub25_tn);
    int xRef = DISPLAY_WIDTH / 2 - 40;
    int posX = xRef;
    int posY = 31;
  
    bool drawMinus = false;
    if (temperature < 0) {
      int offset = u8g->getStrWidth("-0") - u8g->getStrWidth("0");
      posX -= offset;
      temperature *= -1;
      drawMinus = true;
    }
  
    if (temperature < 100) {
      int offset = u8g->getStrWidth("00") - u8g->getStrWidth("0");
      posX += offset;
    }
  
    u8g->setCursor(posX, posY);
  
    if (drawMinus) {
      u8g->print("-");
    }
  
    u8g->print(temperature / 10);
    u8g->print(",");
    u8g->print(temperature % 10);
    u8g->drawCircle(xRef + 72, 7, 3);
  }
  
  void drawHeatpower(int heaterPower) {
    int xRef = DISPLAY_WIDTH - 11;
    u8g->drawVLine(xRef + 10, 0, 50);
    for (int i = 0; i <= 50; i += 10) {
      int y = i;
      u8g->drawHLine(xRef + 8, y, 3);
    }
    int heaterYPos = map(heaterPower, 0, 100, 0, 51);
    int y = 50 - heaterYPos;
    u8g->drawHLine(xRef, y, 9);
    u8g->drawHLine(xRef, y + 1, 7);
    u8g->drawHLine(xRef, y - 1, 7);
    u8g->drawHLine(xRef, y + 2, 5);
    u8g->drawHLine(xRef, y - 2, 5);
    u8g->drawHLine(xRef, y + 3, 3);
    u8g->drawHLine(xRef, y - 3, 3);
    u8g->setFont(u8g_font_6x13);
    int posX = xRef - 6;
    if (heaterPower > 0) {
      if (heaterPower < 10) {
        int offset = u8g->getStrWidth("00") - u8g->getStrWidth("0");
        posX += offset;
      }
      u8g->setCursor(posX, 63);
      u8g->print(heaterPower);
      u8g->print("%");
    } else {
      u8g->setCursor(posX, 63);
      u8g->print("aus");
    }
  }
  
  void drawDistance(int distancePercent, bool blink) {
    const int length = 71;
  
    int distanceX = map(distancePercent, 0, 100, 0, length);
  
    int xPos = 3;
    u8g->drawHLine(xPos, 63, length);
    for (int i = 0; i <= length - 1; i += 14) {
      int x = xPos + i;
      u8g->drawVLine(x, 59, 4);
    }
    u8g->drawHLine(xPos, 58, length);
  
    u8g->drawBox(xPos - 3, 58, 3, 6);
    u8g->drawBox(xPos + length, 58, 3, 6);
  
    u8g->drawBox(xPos, 60, distanceX, 2);
  }
  
  void drawSpeed(int speed) {
    u8g->drawFrame(0, 43, 32, 16);
    u8g->setFont(u8g_font_9x15B);
    int xPos = 3;
    if (speed < 100) {
      int offset = u8g->getStrWidth("00") - u8g->getStrWidth("0");
      xPos += offset;
      if (speed < 10) {
        xPos += offset;
      }
    }
    u8g->setCursor(xPos, 56);
    u8g->print(speed);
    u8g->setFont(u8g_font_4x6);
    u8g->setCursor(33, 50);
    u8g->print("km");
    u8g->setCursor(35, 57);
    u8g->print("h");
    u8g->drawHLine(33, 51, 7);
  }
};

#endif

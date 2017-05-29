#include "Settings.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "Display.h"
#include <ESP8266HTTPUpdateServer.h>

#define PUMP_INTERVAL_MILLIS 1000;

const char* config_ssid = "daisy";
const char* config_password = "Arschloch1";

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

class CommunicationESP {

public:
  CommunicationESP(Settings *settings, Pump *pump, Display *display) {
      this->settings = settings;
      this->pump = pump;
      this->display = display;
      pumping = false;
  }
  
  virtual ~CommunicationESP() {    
  }

  String connectWifi() {
    WiFi.disconnect(); 
    WiFi.mode(WIFI_STA);
    WiFi.begin(config_ssid, config_password);
    int retry = 0;
    int progress = 0;
    while (WiFi.status() != WL_CONNECTED) {
      retry++;
      progress += 5;
      display->drawConnectProgress(progress);
      if (retry > 20) {
        WiFi.disconnect(); 
        return "";
      }
      delay(500);
    }
    httpUpdater.setup(&server);
    server.begin();
    server.on("/", std::bind(&CommunicationESP::sendHtml, this));

    return WiFi.localIP().toString();
  }
  
  void process() {
    static unsigned long nextMillis = 0;

    if (pumping && nextMillis < millis()) {
      nextMillis = millis() + PUMP_INTERVAL_MILLIS;
      pump->pump();
    }
    
    server.handleClient();
  }

private:
  Settings *settings;
  Pump *pump;
  Display *display;
  bool pumping;

  void sendHtml() {
    String action = server.arg("action");
    String add = "";
    if (action == "save") {
      settings->setHeaterStartTemp(getValue("heaterStartTemp", 10));
      settings->setHeaterMaxTemp(getValue("heaterMaxTemp", 10));
      settings->setHeaterStartPower(getValue("heaterStartPower", 1));
      settings->setHeaterMaxPower(getValue("heaterMaxPower", 1));
      settings->setHeaterBalance(getValue("heaterBalance", 1));
      settings->setOilerTickPerRotation(getValue("oilerTickPerRotation", 1));
      settings->setOilerRotationLength(getValue("oilerRotationLength", 1));
      settings->setOilerDistance(getValue("oilerDistance", 1));
      settings->setOilerEmergencyInterval(getValue("oilerEmergencyInterval", 1));
      settings->persist();
    } else if (action == "pump") {
      pumping = !pumping;
    }

    String temp = "<html><body><form action=\"\" method=\"post\"><input type=\"hidden\" name=\"action\" id=\"action\" value=\"save\"><table border=\"1\" cellspacing=\"0\" cellpadding=\"5\">";
    temp += "<thead><tr><th>Setting</th><th>Aktuell</th><th>Neu</th></tr></thead><tbody>";
    temp += "<tr><td colspan=\"3\"></td></tr>";
    temp += "<tr><td colspan=\"3\"><b>Heater</b></td></tr>";
    temp += buildRow("heaterStartTemp", "Ein Temperatur", settings->getHeaterStartTemp(), "0.5", 10, 0, 25);
    temp += buildRow("heaterStartPower", "Ein Leistung", settings->getHeaterStartPower(), "5", 1, 0, 100);
    temp += buildRow("heaterMaxTemp", "Max Temperatur", settings->getHeaterMaxTemp(), "0.5", 10, 0, 25);
    temp += buildRow("heaterMaxPower", "Max Leistung", settings->getHeaterMaxPower(), "5", 1, 0, 100);
    temp += buildRow("heaterBalance", "Gas Anteil", settings->getHeaterBalance(), "5", 1, 0, 100);
  
    temp += "<tr><td colspan=\"3\"></td></tr>";
    temp += "<tr><td colspan=\"3\"><b>Oiler</b></td></tr>";
    temp += buildRow("oilerTickPerRotation", "Radticks", settings->getOilerTickPerRotation(), "1", 1, 1, 255);
    temp += buildRow("oilerRotationLength", "Radumfang", settings->getOilerRotationLength(), "10", 1, 10, 2550);
    temp += buildRow("oilerDistance", "Distanz", settings->getOilerDistance(), "100", 1, 100, 25500);
    temp += buildRow("oilerEmergencyInterval", "Notintervall", settings->getOilerEmergencyInterval(), "10", 1, 10, 2550);
    temp += "</tbody></table><button type=\"submit\">Speichern</button></form>";
    temp += "<form action=\"\" method=\"post\"><input type=\"hidden\" name=\"action\" id=\"action\" value=\"pump\"><button type=\"submit\">Pumpe</button> ist ";
    if (pumping) {
      temp += "an";
    } else {
      temp += "aus";
    }
    temp += "</form>";
    temp += "</body></html>";
  
    temp += add;
    server.send(200, "text/html", temp);
  }
  
  String buildRow(String property, String text, int value, String step, int factor, int min, int max) {
    float floatVal = (float) value / factor;
    String row = "<tr><td>";
    row += text;
    row += "</td><td><input readonly type=\"number\" step=\"";
    row += step;
    row += "\" lang=\"en-150\" value=\"";
    row += floatVal;
    row += "\" /></td><td><input type=\"number\" step=\"";
    row += step;
    row += "\" lang=\"en-150\" value=\"";
    row += floatVal;
    row += "\" name=\"";
    row += property;
    row += "\" min=\"";
    row += min;
    row += "\" max=\"";
    row += max;
    row += "\" /></td></tr>";
    return row;
  }
  
  int getValue(String property, int factor) {
    String value = server.arg(property);
    return value.toFloat() * factor;
  }
};

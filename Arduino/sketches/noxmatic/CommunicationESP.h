#include "Settings.h"

class CommunicationESP {

public:
  CommunicationESP(Settings *settings);
  virtual ~CommunicationESP();

  bool connectWifi();
  void process();

private:
  Settings *settings;

  void sendHtml();
  String buildRow(String property, int value, String step, int factor, int min, int max);
  int getValue(String property, int factor);
};


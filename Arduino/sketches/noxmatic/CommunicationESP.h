#include "Settings.h"

class CommunicationESP {

public:
  CommunicationESP(Settings *settings);
  virtual ~CommunicationESP();

private:
  Settings *settings;
};


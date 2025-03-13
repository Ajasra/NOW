#include "Now.h"
#include "settings.h"

WatchyNow watchy(settings);

void setup(){
  // Serial.begin(115200);
  watchy.init();
}

void loop(){}
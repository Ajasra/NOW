#ifndef WATCHY_NOW_H
#define WATCHY_NOW_H

#include <Watchy.h>
#include <Timelib.h>
#include "monofonto28pt7b.h"
#include "monofonto10pt7b.h"
#include "monofonto8pt7b.h"
#include "monofonto6pt7b.h"
#include "signature.h"

RTC_DATA_ATTR int face = 0;
RTC_DATA_ATTR int cur_face = 2;
int reset_time = 0;
const int FACES_N = 1;

#define DARKMODE false
#define STEPSGOAL 7000
#define UPDATE_EVERY 3

class WatchyNow : public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace();
        void drawWrapText(String text);
        void drawCentered(String text, int y2);
        void drawFaceNow(int light, int batt, int face, int cur_face);
        // void drawTime();
        // void drawDate();
        // void drawSteps();
        // void drawLife();
        // void drawWeather();
        // void drawBattery();
        virtual void handleButtonPress(); //Must be virtual in Watchy.h too
};

#include "FaceNow.h"

void WatchyNow::handleButtonPress() {
  if (guiState == WATCHFACE_STATE) {
    //Up and Down switch watch faces
    uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();

    if (wakeupBit & UP_BTN_MASK) {
      face--;
      if (face < 0 ) { 
        face = FACES_N;
      }
      RTC.read(currentTime);
      reset_time = currentTime.Minute + 1;
      showWatchFace(true);
      return;
    }
    if (wakeupBit & DOWN_BTN_MASK) {
      face++;
      if (face > FACES_N ) { 
        face = 0;
        randomSeed(currentTime.Minute);
        cur_face = random(0, 3);
      }
      RTC.read(currentTime);
      reset_time = currentTime.Minute + 1;
      showWatchFace(true);
      return;
    }

    if (wakeupBit & MENU_BTN_MASK) {
      Watchy::handleButtonPress();
      return;
    }

  } else {Watchy::handleButtonPress();}
  return;
}


void WatchyNow::drawWatchFace() {
  // ** UPDATE **
  //resets step counter at midnight everyday
  if (currentTime.Hour == 00 && currentTime.Minute == 00) {
    sensor.resetStepCounter();
  }

  if (currentTime.Minute % UPDATE_EVERY == 0){
    randomSeed(currentTime.Minute);
    cur_face = random(0, 3);
  }

  if (currentTime.Minute > reset_time){
    face = 0;
    randomSeed(currentTime.Minute);
    cur_face = random(0, 3);
  }

  // ** GET BATTERY **
  int8_t batteryLevel = 0;
  float VBAT = getBatteryVoltage();
  if(VBAT > 4.1){
      batteryLevel = 3;
  }
  else if(VBAT > 3.95 && VBAT <= 4.1){
      batteryLevel = 2;
  }
  else if(VBAT > 3.80 && VBAT <= 3.95){
      batteryLevel = 1;
  }
  else if(VBAT <= 3.80){
      batteryLevel = 0;
  }
  
  drawFaceNow(1, batteryLevel, face, cur_face);
}

#endif

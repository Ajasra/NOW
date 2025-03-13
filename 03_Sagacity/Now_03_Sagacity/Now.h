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

RTC_DATA_ATTR int check_count = 0;

#define MAX_EVENTS 40

RTC_DATA_ATTR unsigned long eventTimes[MAX_EVENTS];

#define DARKMODE false
#define STEPSGOAL 7000
#define UPDATE_EVERY 1

class WatchyNow : public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace();
        void drawWrapText(String text);
        void drawCentered(String text, int y2);
        void drawFaceNow(int light, int batt, int face, int cur_face, int count);
        // void drawTime();
        // void drawDate();
        // void drawSteps();
        // void drawLife();
        // void drawWeather();
        // void drawBattery();
        virtual void handleButtonPress(); //Must be virtual in Watchy.h too
};

#include "FaceNow.h"

void addEventTime(unsigned long data) {
  if (check_count < MAX_EVENTS) {
    eventTimes[check_count] = data;
    check_count++;
  }
}

void printEventTime(){
  Serial.print("all events :");
  Serial.println(check_count);
  for (int i = 0; i < check_count; i++) {
    Serial.println(eventTimes[i]);
  }
  Serial.println("------------");
}

// Function to clean up old event times
void cleanupOldEvents(unsigned long threshold ) {
  int newIndex = 0;
  // Serial.println(threshold);
  // Serial.println("cleaning");
  for (int i = 0; i < check_count; i++) {
    // Serial.println(eventTimes[i]);
    if (eventTimes[i] >= threshold) {
      eventTimes[newIndex] = eventTimes[i];
      newIndex++;
      // Serial.println(i);
    }
  }
  check_count = newIndex;
}

void WatchyNow::handleButtonPress() {
  if (guiState == WATCHFACE_STATE) {
    //Up and Down switch watch faces
    uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();

    if (wakeupBit & UP_BTN_MASK) {
      RTC.read(currentTime);
      face--;
      if (face < 0 ) { 
        face = FACES_N;
        unsigned long tm = (currentTime.Year * 365 + currentTime.Month * 30 + currentTime.Day) * 24 + currentTime.Hour + 24;
        // unsigned long tmt = tm + 24;
        // Serial.println("current");
        // Serial.println(tm);
        addEventTime(tm);
        // printEventTime();
        // cleanupOldEvents(tm);
      }
      
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
    RTC.read(currentTime);
    unsigned long tm = (currentTime.Year * 365 + currentTime.Month * 30 + currentTime.Day) * 24 + currentTime.Hour;
    // Serial.println(tm);
    cleanupOldEvents(tm);
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
  
  drawFaceNow(1, batteryLevel, face, cur_face, check_count);
}

#endif

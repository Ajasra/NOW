#include <math.h>
#include "faces.h"

long remap(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float f(float x, float y) {
  if(x < y){
    return -log(12-x)/log(y);
  }else{
    return log(x-12)/log(y);
  }
}

// Custom map function (similar to p5.js map)
float mapf(float value, float start1, float stop1, float start2, float stop2) {
  return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

float getY(float x, float power, float x1, float x2) {
  float tmp_y = 100.0;
  
  float offset1 = cos(mapf(x, x1, x2, -1.5, 1.5)) * power * 80.0;
  float power2 = 1.0 - pow(abs(mapf(x, x1, x2, -1.0, 1.0)), 2);
  float offset2 = power2 * power * 80.0;
  
  tmp_y = tmp_y + (offset1 * 0.5 + offset2 * 0.5) * pow(power2, 3);
  
  return tmp_y;
}



void WatchyNow::drawFaceNow(int variant, int batt, int face, int cur_face, int count) {

  display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);

  float target_check = 14.0;
  int cur_time =  currentTime.Hour * 60 + currentTime.Minute;

  float coeff = f(count, target_check);
  // be sure there no errors and infinite values
  if (isnan(coeff)) coeff = 0;
  if(coeff > 2.1) coeff = 0;
  
  int time_offset = coeff * 60;
  cur_time = cur_time - time_offset;

  int show_h = int(cur_time / 60.0);
  int show_m = cur_time - show_h * 60;
  if(show_h > 24){
    show_h = show_h - 24;
  }
  if(show_m < 0){
    show_h = 23;
    show_m = show_m + 60;
  }

  if(face == 0){

    if(cur_face == 0){

      display.setFont(&monofonto28pt7b);
      display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
      display.setCursor(60, 120);
      display.print("NOW");

    }else{
      
      float power = coeff;

      // Draw the curve
      for( int x=20; x < 180; x++){
        display.drawPixel(x, getY(float(x),sin(power), 20, 180), DARKMODE ? GxEPD_WHITE: GxEPD_BLACK );
      }
      // Draw the circles
      display.fillCircle(100, 100 + int(sin(power) * 80.0), 17, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
      for(int i = 0; i < 4; i++){
        int offset = int(sin(power) * 80 * sin(float(i) / 3.0));
        int y = 100 + offset;
        int r = int(14 * sin(float(i) / 3.0)) + 1;
        display.drawCircle(100, y, r, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
      }

      display.setFont(&monofonto8pt7b);
      display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
      display.setCursor(20, 94);
      String count_str = "";
      if(count < 10) count_str = "0" + String(count);
      else count_str = String(count);
      display.print(count_str);

    }

  }else if(face == 1){

    // real time
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setFont(&monofonto28pt7b);
    display.setCursor(70, 85);
    int displayHour;
    if(HOUR_12_24==12){
      displayHour = ((show_h+11)%12)+1;
    } else {
      displayHour = show_h;
    }
    if(displayHour < 10){
        display.print("0");
    }
    display.print(displayHour);
    display.setCursor(70, 135);
    if(show_m < 10){
        display.print("0");
    }
    display.print(show_m);
    display.setFont(&monofonto8pt7b);
    display.setCursor(90, 160);
    display.print(currentTime.Hour < 11 ? "AM" : "PM");

    display.setFont(&monofonto6pt7b);
    display.setCursor(170, 190);
    if(batt == 0){
      display.print("---");
    }else if(batt == 1){
      display.print("--0");
    }else if(batt == 2){
      display.print("-00");
    }else if(batt == 3){
      display.print("000");
    }

    display.setFont(&monofonto8pt7b);
    display.setCursor(10, 190);
    display.print(currentTime.Day);
    display.print(".");
    display.print(currentTime.Month);
    display.print(".");
    display.print(tmYearToCalendar(currentTime.Year));
    display.print(" ");
    display.print(currentTime.Hour);
    display.print(":");
    display.print(currentTime.Minute);
    display.print(" ");
    display.print(-time_offset);

    uint32_t signature = generateSignature();
    char signatureHex[11];
    sprintf(signatureHex, "%08X", signature);

    display.setCursor(10, 20);
    display.print("S#ID: ");
    display.print(signatureHex);

    // String deviceId = getDeviceId();
    // display.setCursor(10, 40);
    // display.print(deviceId);
  }

}
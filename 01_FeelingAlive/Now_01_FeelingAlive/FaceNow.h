#define BIRTHDAY_Y 1988
#define BIRTHDAY_M 5
#define BIRTHDAY_D 10
#define TARGET_WEEKS 4000
#define TARGET_Y 2024
#define TARGET_M 8
#define TARGET_D 15

void WatchyNow::drawFaceNow(int variant, int batt, int face, int cur_face) {

  display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);

  int d_y = (tmYearToCalendar(currentTime.Year) - BIRTHDAY_Y) * 365;
  int d_m = (currentTime.Month - BIRTHDAY_M) * 30;
  int d_d = currentTime.Day - BIRTHDAY_D;
  int passed = (d_y + d_m + d_d);

  if(face == 0){

    if(cur_face == 0){

      display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
      display.setFont(&monofonto28pt7b);
      display.setCursor(60, 120);
      display.print("NOW");

    }else if(cur_face == 1){

      // Calculate the difference in days
      int max_days = TARGET_WEEKS * 7;
      double progress = (double)((passed  * 100.0) / max_days);
      progress = progress > 100 ? 100 : progress;

      display.setFont(&monofonto28pt7b);
      display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
      display.setCursor(50, 70);
      display.print("LIFE");
      display.setCursor(15, 120);
      display.print(String(progress) + "%");
      display.setCursor(15, 170);
      display.print("PASSED");

    }else if(cur_face == 2){
      
      int max_weeks = 4000;
      int left = max_weeks - int(passed / 7);
      display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
      display.setFont(&monofonto28pt7b);
      display.setCursor(35, 70);
      display.print("WEEKS");
      display.setCursor(45, 120);
      display.print(String(left));
      display.setCursor(50, 170);
      display.print("LEFT");
    }

    

  }else if(face == 1){
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setFont(&monofonto28pt7b);
    display.setCursor(70, 85);
    int displayHour;
    if(HOUR_12_24==12){
      displayHour = ((currentTime.Hour+11)%12)+1;
    } else {
      displayHour = currentTime.Hour;
    }
    if(displayHour < 10){
        display.print("0");
    }
    display.print(displayHour);
    display.setCursor(70, 135);
    if(currentTime.Minute < 10){
        display.print("0");
    }
    display.print(currentTime.Minute);
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
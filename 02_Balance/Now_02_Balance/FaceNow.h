#define PI 3.14159265

void WatchyNow::drawFaceNow(int variant, int batt, int face, int cur_face) {

  display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);

  if(face == 0){

    int screen = 100;
    int size = 80;

    int rot = 0;
    int part = 0;

    int h = currentTime.Hour;
    int m = currentTime.Minute;

    float darkPercent = (float(m) / 60.0) * 0.8 + .1;
    float whitePercent = 1 - darkPercent;
    int offset1 = int(size * darkPercent);
    int offset2 = int(size * whitePercent);

    float scale1 = 0;
    float scale2 = 0;

    if( darkPercent > 0.5){
      scale1 = 1.0;
      scale2 = 0.7;
    }else{
      scale1 = 0.7;
      scale2 = 1.0;
    }

    float min_size = .5;

    rot = h / 3;
    part = h % 3;

    display.setRotation(rot);
    switch(part){
      case 0:
        display.drawBitmap(0, 0, base_base1, 200, 200, GxEPD_WHITE);
        break;
      case 1:
        display.drawBitmap(0, 0, base_base2, 200, 200, GxEPD_WHITE);
        break;
      case 2:
        display.drawBitmap(0, 0, base_base3, 200, 200, GxEPD_WHITE);
        break;
      default:
        display.drawBitmap(0, 0, base_base1, 200, 200, GxEPD_WHITE);
        break;
    }

    float x_coeff = cos(float(part) / 3.0 * (PI / 2.0) - (PI/2.0));
    float y_coeff = sin(float(part) / 3.0 * (PI / 2.0) - (PI/2.0));

    // // big circles for hours
    display.fillCircle(screen - int(offset2 * x_coeff), screen - int(offset2 * y_coeff), int(size * darkPercent), GxEPD_WHITE);
    display.fillCircle(screen + int(offset1 * x_coeff), screen + int(offset1 * y_coeff), int(size * whitePercent)-1, GxEPD_BLACK);

    // small circles for minutes
    display.fillCircle(screen - int(offset2 * x_coeff), screen - int(offset2 * y_coeff), int(size * darkPercent * min_size * scale1), GxEPD_BLACK);
    display.fillCircle(screen + int(offset1 * x_coeff), screen + int(offset1 * y_coeff), int(size * whitePercent * min_size * scale2), GxEPD_WHITE);

    display.drawCircle(screen,screen,size,GxEPD_WHITE);

  }else if(face == 1){
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


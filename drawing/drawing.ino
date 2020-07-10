#define LILYGO_TWATCH_2020_V1        // If you are using T-Watch-2020 version, please open this macro definition

#include <TTGO.h>

TTGOClass *ttgo;

lv_obj_t *pos = nullptr;
char buf[128];


void setup() {
    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setTextFont(2);
    ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->eTFT->drawString("T-Watch Touch Test", 62, 90);
    draw_timer();

}

void loop() {
  // put your main code here, to run repeatedly:

}


void draw_timer() {
  ttgo->eTFT->fillCircle(120, 120, 100, TFT_RED);
  ttgo->eTFT->fillCircle(120, 120, 80, TFT_BLACK);
  ttgo->eTFT->fillRect(80,40,80,80,TFT_BLUE);
}


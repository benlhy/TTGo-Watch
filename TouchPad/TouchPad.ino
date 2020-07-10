#define LILYGO_TWATCH_2020_V1        // If you are using T-Watch-2020 version, please open this macro definition

#include <TTGO.h>

TTGOClass *ttgo;

lv_obj_t *pos = nullptr;
char buf[128];

void setup()
{
    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->lvgl_begin();
    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setTextFont(2);
    ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->eTFT->drawString("T-Watch Touch Test", 62, 90);
    ttgo->eTFT->fillCircle(20, 20, 20, TFT_RED);
}

void loop()
{
    if (ttgo->touch->touched()) {
        TP_Point p =  ttgo->touch->getPoint();
        Serial.println(p.x);
        int newX = (p.x - 240)*-1;
        Serial.println(newX);
        
        sprintf(buf, "x:%03d  y:%03d", p.x, p.y);
        ttgo->eTFT->setTextColor(TFT_BLACK, TFT_BLACK);
        ttgo->eTFT->drawString("Center",80,140);
        ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);
        if(newX>75 && newX<150) {
          ttgo->eTFT->drawString("Center",80,140);
        }
        else if(newX <75) {
          ttgo->eTFT->drawString("Left",80,140);
        }
        else {
          ttgo->eTFT->drawString("Right",80,140);
        }
        ttgo->eTFT->drawString(buf, 80, 118);
    }
    delay(5);
}

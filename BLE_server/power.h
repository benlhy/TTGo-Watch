#ifndef POWER_H
#define POWER_H


#include <TTGO.h>

bool power_irq = false;


void toggle_backlight(TTGOClass* ttgo);
void toggle_low_power(TTGOClass* ttgo);

void setup_power(TTGOClass* ttgo) {
    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, [] {
        power_irq = true;
    }, FALLING);

    //!Clear IRQ unprocessed  first
    ttgo->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_IRQ, true);
    ttgo->power->clearIRQ();

}

void process_power_irq(TTGOClass* ttgo) {
    if (power_irq) {
        power_irq = false;
        ttgo->power->readIRQ();
        if (ttgo->power->isVbusPlugInIRQ()) {
            ttgo->eTFT->fillRect(20, 100, 200, 85, TFT_BLACK);
            ttgo->eTFT->drawString("Power Plug In", 25, 100);
        }
        if (ttgo->power->isVbusRemoveIRQ()) {
            ttgo->eTFT->fillRect(20, 100, 200, 85, TFT_BLACK);
            ttgo->eTFT->drawString("Power Remove", 25, 100);
        }
        if (ttgo->power->isPEKShortPressIRQ()) {
            //ttgo->eTFT->fillRect(20, 100, 200, 85, TFT_BLACK);
            //ttgo->eTFT->drawString("PowerKey Press", 25, 100);
            toggle_backlight(ttgo);
        }
        ttgo->power->clearIRQ();
    }
}

void toggle_low_power(TTGOClass *ttgo) {

}

void toggle_backlight(TTGOClass *ttgo) {
  if (ttgo->bl->isOn()) {
    ttgo->closeBL();
  }
  else {
    ttgo->openBL();

  }
}
#endif
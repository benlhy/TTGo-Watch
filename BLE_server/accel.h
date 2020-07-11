#ifndef ACCEL_H
#define ACCEL_H


#include <TTGO.h>



bool irq = false;
bool bma_init_okay = false;


void setup_bma(TTGOClass* ttgo) {
    
    pinMode(BMA423_INT1, INPUT);
    attachInterrupt(BMA423_INT1, [] {
        irq = 1;
    }, RISING);

    bma_init_okay = ttgo->bma->begin();
    ttgo->bma->attachInterrupt();
    
}


void count_steps(TTGOClass* ttgo) {
    
    char buf[128];
    if (irq) {
        irq = 0;
        bool  rlst;
        do {
            rlst =  ttgo->bma->readInterrupt();
        } while (!rlst);

        if (ttgo->bma->isStepCounter()) {
            Serial.println(ttgo->bma->getCounter());
            ttgo->eTFT->setTextColor(random(0xFFFF), TFT_BLACK);
            snprintf(buf, sizeof(buf), "StepCount: %u", ttgo->bma->getCounter());
            ttgo->eTFT->drawString(buf, 45, 118, 4);
        }
    }
}
#endif
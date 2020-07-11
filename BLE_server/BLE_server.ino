/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#define LILYGO_TWATCH_2020_V1        // If you are using T-Watch-2020 version, please open this macro definition

#include <TTGO.h>
#include "time.h"
#include "touchscreen.h"
#include "accel.h"
#include "power.h"
#include "bluetooth.h"

TTGOClass *ttgo;

void setup() {
  ttgo_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(5);
  get_touch(ttgo);
  draw_screen(ttgo);
  process_power_irq(ttgo);
}


void ttgo_init() {
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->openBL();
  ttgo->lvgl_begin();
  setup_rtc(ttgo);
  setup_bma(ttgo);
  setup_power(ttgo);
  setup_ble();
}



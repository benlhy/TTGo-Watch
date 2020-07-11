#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include "time.h"
#include "accel.h"
#include "power.h"
#include "bluetooth.h"

#include <TTGO.h>


enum SCREEN {
  DEBUG,
  TIME,
  STEP,
  RED,
};

int8_t current_page = 0;
uint8_t num_pages = 4;
bool first_update_flag = 0;
extern bool bma_init_okay;

char buf[128];

void draw_screen(TTGOClass *ttgo) {
  uint8_t line = 0;    // this var is to track the line number to print on, each line is 16.
  switch(current_page) {
    case DEBUG:
      if (first_update_flag == 1){
        ttgo->eTFT->fillScreen(TFT_BLACK);
        ttgo->eTFT->setTextFont(2);
        ttgo->eTFT->setTextColor(TFT_GREEN, TFT_BLACK);
        line = print_compile_time(ttgo,line);
        #ifdef TIME_H
        bool rtc_valid = ttgo->rtc->isVaild();
        if (rtc_valid){
          ttgo->eTFT->drawString("RTC [OK]",0,line);
        }
        else{
          ttgo->eTFT->drawString("RTC [Err]",0,line);
        }
        
        line = line+16;
        #endif
        #ifdef TOUCHSCREEN_H
        ttgo->eTFT->drawString("Touchscreen [OK]",0,line);
        line = line+16;
        #endif
        #ifdef ACCEL_H
        if (bma_init_okay){
          ttgo->eTFT->drawString("Accel [OK]",0,line);
        }
        else {
          ttgo->eTFT->drawString("Accel [Err]",0,line);
        }
        
        line = line+16;
        #endif
        #ifdef BLUETOOTH_H
        ttgo->eTFT->drawString("Bluetooth [OK]",0,line);
        line = line+16;
        #endif
        ttgo->eTFT->drawString("Init Complete [OK]",0,line); // x y
        first_update_flag = 0;
      }
      break;
    case TIME:
      if(first_update_flag == 1){
        ttgo->eTFT->fillScreen(TFT_BLACK);
        ttgo->eTFT->setTextFont(2);
        ttgo->eTFT->setTextColor(TFT_GREEN, TFT_BLACK);
        ttgo->eTFT->drawString("Time",0,0);
        first_update_flag = 0;
        draw_time(ttgo);
      }
      update_time(ttgo);
      break;
    case STEP:
      if(first_update_flag == 1){
        ttgo->eTFT->fillScreen(TFT_BLACK);
        ttgo->eTFT->setTextFont(2);
        ttgo->eTFT->setTextColor(TFT_GREEN, TFT_BLACK);
        ttgo->eTFT->drawString("Step",0,0);
        first_update_flag = 0;
      }
      


      break;



      break;
    case RED:
      ttgo->eTFT->fillScreen(TFT_RED);

      break;
    

  }
}

void update_screen(){
  switch(current_page){
    case DEBUG:

    break;
  }
}



void get_touch(TTGOClass *ttgo) {
    if (ttgo->touch->touched()) {
        TP_Point p =  ttgo->touch->getPoint();
        Serial.println(p.x);
        int newX = (p.x - 240)*-1;
        int newY = (p.y - 240)*-1;
        Serial.println(newX);
        if (ttgo->bl->isOn()) {
          // we will only process screen area presses when the backlight is on.
          /*
          sprintf(buf, "x:%03d  y:%03d", newX, newY);
          ttgo->eTFT->drawString(buf, 80, 118);
          ttgo->eTFT->fillScreen(TFT_BLACK);
          ttgo->eTFT->setTextFont(2);
          ttgo->eTFT->setTextColor(TFT_BLACK, TFT_BLACK);
          ttgo->eTFT->drawString("Center",80,140);
          ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);*/
          if(newX>75 && newX<150) {
            //ttgo->eTFT->drawString("Center",80,140);
            toggle_backlight(ttgo);

          }
          else if(newX <75) {
            //ttgo->eTFT->drawString("Left",80,140);
            current_page = (current_page+(num_pages-1))%num_pages;
            first_update_flag = 1;
          }
          else {
            //ttgo->eTFT->drawString("Right",80,140);
            current_page = (current_page+1)%num_pages;
            first_update_flag = 1;
          }
        }
        else {
          //if the screen is off, touching anywhere on the screen will toggle it.
          toggle_backlight(ttgo);
        }
        
        delay(100); // debounce
        
    }
    //return p; // this is a TP_Point, with x and y
}




#endif
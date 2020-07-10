#ifndef TIME_H
#define TIME_H

#include <TTGO.h>

uint8_t print_compile_time(TTGOClass* ttgo, uint8_t line) {
    ttgo->eTFT->drawString("Compile:",0,line);
    ttgo->eTFT->setCursor(55, line);
    ttgo->eTFT->print(__DATE__);
    ttgo->eTFT->setCursor(145, line);
    ttgo->eTFT->print(__TIME__);
    line = line + 16;
    return line;
}

static uint8_t conv2d(const char *p)
{
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time

uint32_t targetTime = 0;       // for next 1 second timeout

byte omm = 99;
byte oss = 99;
boolean initial = 1;
byte xcolon = 0;
unsigned int colour = 0;



void setup_rtc(TTGOClass* ttgo){
    char buf[128];
    ttgo->rtc->setDateTime(RTC_Date(__DATE__,__TIME__)); // set time to compile time.

    //RTC_Date now = ttgo->rtc->getDateTime(); // y m d h mm s
    //sprintf(buf, "x:%03d  y:%03d", now.year, now.month);
    //ttgo->eTFT->drawString(buf,0,100);
    targetTime = millis() + 1000;
}

void draw_time(TTGOClass* ttgo) {
    byte xpos = 6;
    byte ypos = 0;
    char buf[128];

    ttgo->eTFT->setTextColor(0x39C4, TFT_BLACK);  
    ttgo->eTFT->drawString("88:88", xpos, ypos, 7); // Overwrite the text to clear it
    ttgo->eTFT->setTextColor(0xFBE0, TFT_BLACK); // Orange
    

    snprintf(buf, sizeof(buf), "%s", ttgo->rtc->formatDateTime());
    ttgo->eTFT->drawString(buf, 5, 118, 7);

    
    RTC_Date now = ttgo->rtc->getDateTime();

    hh = now.hour;
    mm = now.minute;
    ss = now.second;
    
    
    // Update digital time

    if (hh < 10) xpos += ttgo->eTFT->drawChar('0', xpos, ypos, 7);
    xpos += ttgo->eTFT->drawNumber(hh, xpos, ypos, 7);
    xcolon = xpos;
    xpos += ttgo->eTFT->drawChar(':', xpos, ypos, 7);
    if (mm < 10) xpos += ttgo->eTFT->drawChar('0', xpos, ypos, 7);
    ttgo->eTFT->drawNumber(mm, xpos, ypos, 7);

}


void update_time(TTGOClass* ttgo) {
    byte xpos = 6;
    byte ypos = 0;
    /*
    snprintf(buf, sizeof(buf), "%s", ttgo->rtc->formatDateTime());
    ttgo->eTFT->drawString(buf, 5, 118, 7);*/

    RTC_Date now = ttgo->rtc->getDateTime();
    hh = now.hour;
    mm = now.minute;
    ss = now.second;


    if (omm!=mm) {
        omm = mm;
        //if (oss != ss) {
        // Uncomment ONE of the next 2 lines, using the ghost image demonstrates text overlay as time is drawn over it
        ttgo->eTFT->setTextColor(0x39C4, TFT_BLACK);  // Leave a 7 segment ghost image, comment out next line!
        //ttgo->eTFT->setTextColor(TFT_BLACK, TFT_BLACK); // Set font colour to black to wipe image
        // Font 7 is to show a pseudo 7 segment display.
        // Font 7 only contains characters [space] 0 1 2 3 4 5 6 7 8 9 0 : .
        ttgo->eTFT->drawString("88:88", xpos, ypos, 7); // Overwrite the text to clear it
        ttgo->eTFT->setTextColor(0xFBE0, TFT_BLACK); // Orange


        if (hh < 10) xpos += ttgo->eTFT->drawChar('0', xpos, ypos, 7);
        xpos += ttgo->eTFT->drawNumber(hh, xpos, ypos, 7);
        xcolon = xpos;
        xpos += ttgo->eTFT->drawChar(':', xpos, ypos, 7);
        if (mm < 10) xpos += ttgo->eTFT->drawChar('0', xpos, ypos, 7);
        ttgo->eTFT->drawNumber(mm, xpos, ypos, 7);
    }

    if (ss % 2) { // Flash the colon
        ttgo->eTFT->setTextColor(0x39C4, TFT_BLACK);
        xpos += ttgo->eTFT->drawChar(':', xcolon, ypos, 7);
        ttgo->eTFT->setTextColor(0xFBE0, TFT_BLACK);
    } 
    else {
        ttgo->eTFT->drawChar(':', xcolon, ypos, 7);
        colour = random(0xFFFF);
        

    }
    /*
    if (targetTime < millis()) {
    targetTime = millis() + 1000;
    oss = ss;
    ss++;              // Advance second
    if (ss == 60) {
        ss = 0;
        omm = mm;
        mm++;            // Advance minute
        if (mm > 59) {
            mm = 0;
            hh++;          // Advance hour
            if (hh > 23) {
                hh = 0;
            }
        }
    }

    // Update digital time
    byte xpos = 6;
    byte ypos = 0;
    if (omm != mm) { // Only redraw every minute to minimise flicker
    //if (oss != ss) {
        // Uncomment ONE of the next 2 lines, using the ghost image demonstrates text overlay as time is drawn over it
        ttgo->eTFT->setTextColor(0x39C4, TFT_BLACK);  // Leave a 7 segment ghost image, comment out next line!
        //ttgo->eTFT->setTextColor(TFT_BLACK, TFT_BLACK); // Set font colour to black to wipe image
        // Font 7 is to show a pseudo 7 segment display.
        // Font 7 only contains characters [space] 0 1 2 3 4 5 6 7 8 9 0 : .
        ttgo->eTFT->drawString("88:88", xpos, ypos, 7); // Overwrite the text to clear it
        ttgo->eTFT->setTextColor(0xFBE0, TFT_BLACK); // Orange
        omm = mm;
        oss = ss;

        if (hh < 10) xpos += ttgo->eTFT->drawChar('0', xpos, ypos, 7);
        xpos += ttgo->eTFT->drawNumber(hh, xpos, ypos, 7);
        xcolon = xpos;
        xpos += ttgo->eTFT->drawChar(':', xpos, ypos, 7);
        if (mm < 10) xpos += ttgo->eTFT->drawChar('0', xpos, ypos, 7);
        ttgo->eTFT->drawNumber(mm, xpos, ypos, 7);
    }

    if (ss % 2) { // Flash the colon
        ttgo->eTFT->setTextColor(0x39C4, TFT_BLACK);
        xpos += ttgo->eTFT->drawChar(':', xcolon, ypos, 7);
        ttgo->eTFT->setTextColor(0xFBE0, TFT_BLACK);
    } 
    else {
        ttgo->eTFT->drawChar(':', xcolon, ypos, 7);
        colour = random(0xFFFF);
        // Erase the old text with a rectangle, the disadvantage of this method is increased display flicker
        
        ttgo->eTFT->fillRect (0, 64, 160, 20, TFT_BLACK);
        ttgo->eTFT->setTextColor(colour);
        ttgo->eTFT->drawRightString("Colour", 75, 64, 4); // Right justified string drawing to x position 75
        String scolour = String(colour, HEX);
        scolour.toUpperCase();
        char buffer[20];
        scolour.toCharArray(buffer, 20);
        ttgo->eTFT->drawString(buffer, 82, 64, 4);
    }
}*/

}

#endif
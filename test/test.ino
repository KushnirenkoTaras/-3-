
#include "config.h"
#include "myLib.h"

Com com;
//Com *com_ = new Com();

unsigned long cTimer;
unsigned int cnt = 0;

void setup() {
  com.receivedFlag = false;
  com.enableInterrupt = true;
  #ifdef DEBUG 
    Serial.begin(115200);
    Serial.print(__DATE__);
    Serial.print("   ");
    Serial.println(__TIME__);
  #endif
  #if (OUT == 2) 
    com.lcd->init();
    com.lcd->backlight();
    com.lcd->setCursor(0, 0);
    com.lcd->print(F("CC1101 Init ..."));
  #endif
  // initialize CC1101 with default settings
  #ifdef DEBUG 
    Serial.print(F("[CC1101] Initializing ... "));
  #endif
  while (com.begin() != 0);

}

void loop() {
  unsigned long cTime = millis();
  // check if the flag is set
  if(com.receivedFlag) {
    if (com.readData() == RADIOLIB_ERR_NONE) {
      
    }
  }
  if(cTimer < cTime) { // Task
    cTimer += 1000; 
    com.data.from = MY_ADDR;
    com.data.to = _A9;
    com.data.cmd = _CT;
    com.data.data1 = cnt++;
    com.data.end = _CR;
    if (com.transmit() == RADIOLIB_ERR_NONE) {
      #if (OUT == 2) 
        com.lcd->setCursor(0, 0);
        com.lcd->print(++cnt);
      #endif
    }
  }

}

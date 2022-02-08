
#include "config.h"
#include "myLib.h"
#if (MODULE != 0)
  //  #include <OneButton.h>
  #include <EncButton2.h>
#endif

Com com;
//Com *com_ = new Com();

unsigned long cTimer = 0; // Last time
#if (MODULE == 0)
  unsigned long lPing = 0; // Last ping
#else
  unsigned long lPing[SLAVES]; // Last ping
  EncButton2<EB_BTN> btn[BUTTONS];
#endif
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
  
  #if (MODULE == 0) // Init Last ping array
    for (size_t i = 0; i < SLAVES; i++) {
      lPing[i] = 0;
    }
  #endif
  #if (MODULE != 0) // Init Buttons
    btn[0].setPins(INPUT_PULLUP, A1);
    btn[1].setPins(INPUT_PULLUP, A2);
    btn[2].setPins(INPUT_PULLUP, A3);
    btn[3].setPins(INPUT_PULLUP, A6);
    btn[4].setPins(INPUT_PULLUP, 3);
    btn[5].setPins(INPUT_PULLUP, 4);
    btn[6].setPins(INPUT_PULLUP, 5);
    btn[7].setPins(INPUT_PULLUP, 6);
  #endif
}

void loop() {
  unsigned long cTime = millis();
  // check if the flag is set
  if(com.receivedFlag) {
    if (com.readData() == RADIOLIB_ERR_NONE) {
      
    }
  }
  #if (MODULE != 0) // Check Buttons
    for (int i = 0; i < BUTTONS; i++) btn[i].tick();
    for (int i = 0; i < BUTTONS; i++) {
      if (btn[i].press()) {
        #ifdef DEBUG 
          Serial.print("press btn: ");
          Serial.println(i);
        #endif
      }
    }  
  #endif
  
  if(cTimer < cTime) { // Task
    cTimer += PING; 
    com.data.from = MY_ADDR;
    com.data.to = _A9_;
    com.data.cmd = _C_PING_;
    com.data.data1 = cnt++;
    com.data.end = _CR_;
    if (com.transmit() == RADIOLIB_ERR_NONE) {
      #if (OUT == 2) 
        com.lcd->setCursor(0, 0);
        com.lcd->print(++cnt);
      #endif
    }
  }
}

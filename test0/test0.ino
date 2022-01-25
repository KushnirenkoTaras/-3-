
#include "config.h"
#include <RadioLib.h>
//CC1101 radio = new Module(10, 2, RADIOLIB_NC, 3);

#define MY_ADDR _A0

volatile bool receivedFlag;     // flag to indicate that a packet was received
volatile bool enableInterrupt;  // disable interrupt when it's not needed
//byte byteArr[64];
pack data;
unsigned long curTimer;
unsigned int cnt = 0;
// ===============================================================
void setup() {
  receivedFlag = false;     // flag to indicate that a packet was received
  enableInterrupt = true;   // disable interrupt when it's not needed
  radio = new Module(10, 2, RADIOLIB_NC);
  #ifdef DEBUG 
    Serial.begin(115200);
    Serial.print(__DATE__);
    Serial.print("   ");
    Serial.println(__TIME__);
  #endif

  #if (OUT == 2) 
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print(F("CC1101 Init ..."));
  #endif
  // initialize CC1101 with default settings
  #ifdef DEBUG 
    Serial.print(F("[CC1101] Initializing ... "));
  #endif
  int state = radio.begin();
  if (state == RADIOLIB_ERR_NONE) {
    success();
  } else {
    error(state);
    while (true);
  }

  radio.setGdo0Action(setFlag);  // set the function that will be called when new packet is received

  // start listening for packets
  #ifdef DEBUG 
    Serial.print(F("[CC1101] Starting to listen ... "));
  #endif
  #if (OUT == 2) 
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Wait ..."));
    lcd.setCursor(0, 1);
  #endif
  state = radio.startReceive();
  if (state == RADIOLIB_ERR_NONE) {
    success();
  } else {
    error(state);
    while (true);
  }

  // if needed, 'listen' mode can be disabled by calling
  // any of the following methods:
  //
  // radio.standby()
  // radio.sleep()
  // radio.transmit();
  // radio.receive();
  // radio.readData();
  curTimer = millis()+1000;
} // setup()
// ===============================================================
void loop() {
  unsigned long cTime = millis();
  // check if the flag is set
  if(receivedFlag) {
    readData();
  }
  if(curTimer < cTime) { // Task
    curTimer += 1000; 
    data.from = MY_ADDR;
    data.to = _A9;
    data.cmd = _CT;
    data.data1 = cnt++;
    data.end = _CR;
    int state = radio.transmit((byte*)&data, sizeof(pack));
    
    #if (OUT == 2) 
      lcd.setCursor(0, 0);
      lcd.print(++cnt);
    #endif
  }

} //loop()
// ===============================================================

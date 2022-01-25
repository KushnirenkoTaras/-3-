
#include "config.h"
#include <RadioLib.h>
//CC1101 radio = new Module(10, 2, RADIOLIB_NC, 3);
CC1101 radio = new Module(10, 2, RADIOLIB_NC);

  //***
  // 0: Serial-Console (Your PC); 
  // 1: No released!!! Serial-LCD (2nd Arduino with SimpleSerialDisplay)
  // 2: I2C-LCD
#define OUT 2
#define DEBUG 1

#if (OUT == 2)
  #include <microLiquidCrystal_I2C.h>
  LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F or 0x27 for a 16 chars and 2 line display
  char toLCD1[20] = "";
  char toLCD2[20] = "";
  char saveLCD1[20] = "";
  char saveLCD2[20] = "";
#endif

#ifdef DEBUG
  char toSerial[180] = "";
  char saveSerial[180] = "";
#endif

volatile bool receivedFlag = false;     // flag to indicate that a packet was received
volatile bool enableInterrupt = true;   // disable interrupt when it's not needed
//byte byteArr[64];
pack data;
unsigned long curTimer;
unsigned int cnt = 0;
// ===============================================================
void setup() {
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
    lcd.setCursor(0, 0);
    lcd.print(++cnt);
  }

} //loop()
// ===============================================================

void setFlag(void) {
  if(!enableInterrupt) {   // check if the interrupt is enabled
    return;
  }
  receivedFlag = true; // we got a packet, set the flag
}
void readData() {
    enableInterrupt = false;  // disable the interrupt service routine while processing the data
    receivedFlag = false;     // reset flag

//    int state = radio.readData(byteArr, sizeof(pack));
    int state = radio.readData((byte*)&data, sizeof(pack));

    if (state == RADIOLIB_ERR_NONE) {
      // packet was successfully received
      float rssi = radio.getRSSI();
      int lqi = radio.getLQI();
      char buf[16] = "";
      #if (OUT == 2)
        lcd.clear();
        lcd.setCursor(0, 0);
//        lcd.print(F("Ok "));
        lcd.print(rssi);
        lcd.print(F("dBm "));
        lcd.print(lqi);
        lcd.setCursor(0, 1);
        sprintf(buf, "%04x%04x%04x%04x", data.addr, data.cmd, data.data1, data.data2);
        lcd.print(buf);
//        lcd.setCursor(3, 1);
//        lcd.print(data.cmd);
       #endif
      #ifdef DEBUG 
        Serial.println(F("[CC1101] Received packet!"));
        Serial.print(F("[CC1101] Data:\t\t"));  // print data of the packet
        Serial.print("Addr: ");
        Serial.print(data.addr, HEX);
        Serial.print(" Cmd: ");
        Serial.print(data.cmd, HEX);
        Serial.print(" Data: ");
        Serial.print(data.data1, HEX);
        Serial.print(data.data2, HEX);
        Serial.print(" End: ");
        Serial.print(data.end, HEX);
        Serial.println();

        // print RSSI (Received Signal Strength Indicator)
        // of the last received packet
        Serial.print(F("[CC1101] RSSI:\t\t"));
        Serial.print(rssi);
        Serial.println(F(" dBm"));
  
        // print LQI (Link Quality Indicator)
        // of the last received packet, lower is better
        Serial.print(F("[CC1101] LQI:\t\t"));
        Serial.println(lqi);
      #endif

    } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
      #ifdef DEBUG 
        Serial.println(F("CRC error!"));  // packet was received, but is malformed
      #endif
    } else {
      #ifdef DEBUG 
        // some other error occurred
        Serial.print(F("failed, code "));
        Serial.println(state);
      #endif

    }
    radio.startReceive();    // put module back to listen mode
    enableInterrupt = true;    // we're ready to receive more packets, enable interrupt service routine
} //readData()

void success(){
  #if (OUT == 2)
    lcd.setCursor(0, 1);
    lcd.print(F("Success!"));
  #endif
  #ifdef DEBUG 
    Serial.println(F("success!"));
  #endif
} //success()

void error(int16_t state){
  #if (OUT == 2)
    lcd.setCursor(0, 1);
    lcd.print(F("Failed, code "));
    lcd.print(state);
  #endif
  #ifdef DEBUG 
    Serial.print(F("failed, code "));
    Serial.println(state);
  #endif
} //error()

void displayLCD(){
  #ifdef DEBUG 
    if (strcmp(toSerial,saveSerial)){
      Serial.println(toSerial);
      strcpy(saveSerial, toSerial);
    }
  #endif
  #if (OUT == 2) 
    if (strcmp(toLCD1,saveLCD1) || strcmp(toLCD2,saveLCD2)){
      lcd.clear();
      lcd.backlight();
      lcd.setCursor(0, 0);
      lcd.print(toLCD1);
      lcd.setCursor(0, 1);
      lcd.print(toLCD2);
      strcpy(saveLCD1, toLCD1);
      strcpy(saveLCD2, toLCD2);
    }
  #endif
} // displayLCD()

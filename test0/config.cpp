#include "config.h"

void setFlag(void) {
  if(!enableInterrupt) {   // check if the interrupt is enabled
    return;
  }
  receivedFlag = true; // we got a packet, set the flag
}

int readData() {
    enableInterrupt = false;  // disable the interrupt service routine while processing the data
    receivedFlag = false;     // reset flag
  int ret = 0;
//    int state = radio.readData(byteArr, sizeof(pack));
    int state = radio.readData((byte*)&data, sizeof(pack));

    if (state == RADIOLIB_ERR_NONE) {
      // packet was successfully received
      #ifdef DEBUG 
        float rssi = radio.getRSSI();
        int lqi = radio.getLQI();
       #endif
      #if (OUT == 2)
        char buf[16] = "";
        lcd.clear();
        lcd.setCursor(0, 0);
//        lcd.print(F("Ok "));
        lcd.print(rssi);
        lcd.print(F("dBm "));
        lcd.print(lqi);
        lcd.setCursor(0, 1);
        sprintf(buf, "%04x%04x%04x%04x", data.from, data.cmd, data.data1, data.data2);
        lcd.print(buf);
//        lcd.setCursor(3, 1);
//        lcd.print(data.cmd);
       #endif
      #ifdef DEBUG 
        Serial.println(F("[CC1101] Received packet!"));
        Serial.print(F("[CC1101] Data:\t\t"));  // print data of the packet
        Serial.print("From: ");
        Serial.print(data.from, HEX);
        Serial.print(" To: ");
        Serial.print(data.to, HEX);
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
    return ret;
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

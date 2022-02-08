#include "config.h"
#include "myLib.h"

// реализация методов
Com::Com() { // конструктор
  receivedFlag = false;     // flag to indicate that a packet was received
  enableInterrupt = true;   // disable interrupt when it's not needed
  #if (OUT == 2)
  (*lcd).clear();
  lcd->clear();
  #endif
}
int Com::begin() {
  _state = _radio.startReceive();
  if (_state == RADIOLIB_ERR_NONE) {
    success();
  } else {
    error(_state);
  }
  return _state;
}
int Com::transmit() {
  _state = _radio.transmit((byte*)&data, sizeof(pack));
  if (_state == RADIOLIB_ERR_NONE) {
    packetTxcOk++;
  } else {
    packetTxError++;
  }
  return _state;
}
int Com::readData() {
    enableInterrupt = false;  // disable the interrupt service routine while processing the data
    receivedFlag = false;     // reset flag
  int ret = 0;
    _state = _radio.readData((byte*)&data, sizeof(pack));

    if (_state == RADIOLIB_ERR_NONE) {
      // packet was successfully received
      if (data[(data[_P_CMD_]>> 4)+_P_END_] != _CR_) {
        packetRecBad++;
        return RECEIVE_ERR_PACKET;
      }
      if ((data[_P_TO_] != _A_MULTI_) || (data[_P_TO_] != MY_ADDR)) {
        packetRecBad++;
        return RECEIVE_ERR_PACKET;
      }
      packetRecOk++;
      #ifdef DEBUG 
        float rssi = _radio.getRSSI();
        int lqi = _radio.getLQI();
       #endif
      #if (OUT == 2)
        char buf[16] = "";
        lcd->clear();
        lcd->setCursor(0, 0);
        lcd->print(rssi);
        lcd->print(F("dBm "));
        lcd->print(lqi);
        lcd->setCursor(0, 1);
//        sprintf(buf, "%04x%04x%04x%04x", data[_P_FROM_], data[_P_CMD_], data.data1, data.data2);
        sprintf(buf, "%04x%04x", data[_P_FROM_], data[_P_CMD_]);
        lcd->print(buf);
//        lcd.setCursor(3, 1);
//        lcd.print(data.cmd);
       #endif
      #ifdef DEBUG 
        Serial.println(F("[CC1101] Received packet!"));
        Serial.print(F("[CC1101] Data:\t\t"));  // print data of the packet
        Serial.print("From: ");
        Serial.print(data[_P_FROM_], HEX);
        Serial.print(" To: ");
        Serial.print(data[_P_TO_], HEX);
        Serial.print(" Cmd: ");
        Serial.print(data[_P_CMD_], HEX);
//        Serial.print(" Data: ");
//        Serial.print(data.data1, HEX);
//        Serial.print(data.data2, HEX);
        Serial.print(" End: ");
        Serial.print(data[(data[_P_CMD_]>> 4)+_P_END_], HEX);
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
    } else if (_state == RADIOLIB_ERR_CRC_MISMATCH) {
      packetRecError++;
      #ifdef DEBUG 
        Serial.println(F("CRC error!"));  // packet was received, but is malformed
      #endif
    } else {
      packetRecError++;
      #ifdef DEBUG 
        // some other error occurred
        Serial.print(F("failed, code "));
        Serial.println(_state);
      #endif

    }
    _radio.startReceive();    // put module back to listen mode
    enableInterrupt = true;    // we're ready to receive more packets, enable interrupt service routine
    return ret;
} //readData()

void Com::setFlag(void) {
  if(!enableInterrupt) {   // check if the interrupt is enabled
    return;
  }
  receivedFlag = true; // we got a packet, set the flag
}

void Com::success(){
  #if (OUT == 2)
    lcd->setCursor(0, 1);
    lcd->print(F("Ok!"));
  #endif
  #ifdef DEBUG 
    Serial.println(F("success!"));
  #endif
} //success()

void Com::error(int16_t state){
  #if (OUT == 2)
    lcd->setCursor(0, 1);
    lcd->print(F("Err "));
    lcd->print(state);
  #endif
  #ifdef DEBUG 
    Serial.print(F("failed, code "));
    Serial.println(state);
  #endif
} //error()

void Com::displayLCD(){
  #ifdef DEBUG 
    if (strcmp(toSerial,_saveSerial)){
      Serial.println(toSerial);
      strcpy(_saveSerial, toSerial);
    }
  #endif
  #if (OUT == 2) 
    if (strcmp(toLCD1,_saveLCD1) || strcmp(toLCD2,_saveLCD2)){
      lcd->clear();
      lcd->backlight();
      lcd->setCursor(0, 0);
      lcd->print(toLCD1);
      lcd->setCursor(0, 1);
      lcd->print(toLCD2);
      strcpy(_saveLCD1, toLCD1);
      strcpy(_saveLCD2, toLCD2);
    }
  #endif
} // displayLCD()

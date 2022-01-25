#pragma once
#include "config.h"
#include <RadioLib.h>

#if (OUT == 2)
  #include <microLiquidCrystal_I2C.h>
#endif


  #define _A0 0x4130
  #define _A1 0x4131
  #define _A2 0x4132
  #define _A9 0x4139

  #define _CT 0x4354
  #define _C1 0x4331
  #define _C2 0x4332

  #define _CR 0x0d
  #define RECEIVE_ERR_PACKET  (-1001)
  
  struct pack {
    uint16_t from;
    uint16_t to;
    uint16_t cmd;
    uint16_t data1;
    uint16_t data2;
    uint8_t  end;
  };

// описание класса
class Com {
public:
  Com();
  volatile bool receivedFlag;     // flag to indicate that a packet was received
  volatile bool enableInterrupt;  // disable interrupt when it's not needed
  pack data;

  #if (OUT == 2)
    LiquidCrystal_I2C *lcd = new LiquidCrystal_I2C(0x27,16,2);  // set the LCD address to 0x3F or 0x27 for a 16 chars and 2 line display
    char toLCD1[20] = "";
    char toLCD2[20] = "";
  #endif
  #ifdef DEBUG
    char toSerial[180] = "";
  #endif

  void setFlag(void);
  void displayLCD();
  unsigned long packetRecError = 0;
  unsigned long packetRecBad = 0;
  unsigned long packetRecOk = 0;
  unsigned long packetTxError = 0;
  unsigned long packetTxcOk = 0;
  
  int begin();
  int readData();
  int transmit();

private:
  CC1101 _radio = new Module(10, 2, RADIOLIB_NC, 3);

  int _state;
  #if (OUT == 2)
    char _saveLCD1[20] = "";
    char _saveLCD2[20] = "";
  #endif
  #ifdef DEBUG
    char _saveSerial[180] = "";
  #endif

  void success();
  void error(int16_t state);
};

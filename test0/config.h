#if !defined(_CONFIG_H)
  #define _CONFIG_H

  #include <stdint.h>
  #include <RadioLib.h>
  CC1101 radio = new Module(10, 2, RADIOLIB_NC);

  //***
  // 0: Serial-Console (Your PC); 
  // 1: No released!!! Serial-LCD (2nd Arduino with SimpleSerialDisplay)
  // 2: I2C-LCD
//#define OUT 2
#define DEBUG 1

#if (OUT == 2)
  #include <microLiquidCrystal_I2C.h>
  LiquidCrystal_I2C _lcd = new LiquidCrystal_I2C(0x27,16,2);  // set the LCD address to 0x3F or 0x27 for a 16 chars and 2 line display
  char toLCD1[20] = "";
  char toLCD2[20] = "";
  char saveLCD1[20] = "";
  char saveLCD2[20] = "";
#endif

#ifdef DEBUG
  char toSerial[180] = "";
  char saveSerial[180] = "";
#endif

  #define _A0 0x4130
  #define _A1 0x4131
  #define _A2 0x4132
  #define _A9 0x4139

  #define _CT 0x4354
  #define _C1 0x4331
  #define _C2 0x4332

  #define _CR 0x0d

  struct pack {
    uint16_t from;
    uint16_t to;
    uint16_t cmd;
    uint16_t data1;
    uint16_t data2;
    uint8_t  end;
  };

  extern volatile bool receivedFlag;     // flag to indicate that a packet was received
  extern volatile bool enableInterrupt;  // disable interrupt when it's not needed

  extern pack data;

  void setFlag(void);
  int readData();
  int transmit();
  void success();
  void error(int16_t state);
  void displayLCD();

#endif

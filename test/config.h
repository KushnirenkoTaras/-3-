#pragma once
#include "Arduino.h"

//#define MODULE 0 // Main module
#define MODULE 1 // Slave 1 module
//#define MODULE 0 // Slave 2 module

#if (MODULE == 0)
  #define MY_ADDR 0x00
#elif (MODULE == 1)
  #define MY_ADDR 0x01
#elif (MODULE == 2)
  #define MY_ADDR 0x02
#else
  #error Invalid Module type!
#endif
#define SLAVES 2
#define BUTTONS 3



#define PING 500 // Ping interval 0.5 sec
#define PING_E 700 // Ping interval 0.7 sec

// Display settings
#define OUT 2    // Display on the LCD_I2C
#define DEBUG 1  // Debug on Serial

#define INIT_BUTTONS btn[0].setPins(INPUT_PULLUP, A1); \
    btn[1].setPins(INPUT_PULLUP, A2); \
    btn[2].setPins(INPUT_PULLUP, A3); \
    btn[3].setPins(INPUT_PULLUP, A6); \
    btn[4].setPins(INPUT_PULLUP, 3); \
    btn[5].setPins(INPUT_PULLUP, 4); \
    btn[6].setPins(INPUT_PULLUP, 5); \
    btn[7].setPins(INPUT_PULLUP, 6);
    


#define MODULE 0 // Main module
//#define MODULE 1 // Slave 1 module
//#define MODULE 0 // Slave 2 module

#if (MODULE == 0)
  #define MY_ADDR 0x4130
#elif (MODULE == 1)
  #define MY_ADDR 0x4131
#elif (MODULE == 2)
  #define MY_ADDR 0x4132
#else
  #error Invalid Module type!
#endif

#define PING 500 // Ping interval 0.5 sec
#define PING_E 700 // Ping interval 0.7 sec

// Display settings
#define OUT 2    // Display on the LCD_I2C
#define DEBUG 1  // Debug on Serial

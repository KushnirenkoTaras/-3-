#pragma once
#include "config.h"
#include <RadioLib.h>

#if (OUT == 2)
//  #include <microLiquidCrystal_I2C.h>
  #include <LiquidCrystal_I2C.h>
#endif

/* Пакет данных
0x00 - Адрес получателя
0x01 - Адрес отправителя
0x00 - Команда
Данные 0..n байт
0x0d - Cr - end packet

   Комманды. Старшие 4 бита - длина данных. Макс длина данных 15 байт
4 байта данные
0x43 - Режим рабочий - подготовительное время
0x44 - Режим рабочий - рабочее время
0x46 - Время
*/
  // Byte Positions
  #define _P_TO_      0 // Адрес получателя
  #define _P_FROM_    1 // Адрес отправителя
  #define _P_CMD_     2 // Command
  #define _P_END_     3 // End packet + data len

  // Адреса
  #define _A_MAIN_  0x00 // Основной блок
  #define _A_L_A_   0x01 // Address A1
  #define _A_L_B_   0x02 // Address A1
  #define _A_G_A_   0x10 // 0x1n - устройства линии A. n = 0 - блок концентратор. n = 1..E - индивидуальные
  #define _A_G_B_   0x20 // 0x2n - устройства линии B. n = 0 - блок концентратор. n = 1..E - индивидуальные
  #define _A_G_S_   0x30 // 0x3n - устройства секундомеры S. n = n = 1..E - индивидуальные
  
  #define _A_MULTI_ 0xFF // широковещательный пакет - всем
  #define _A_MUL_G_ 0xF0 // широковещательный пакет группе (A,B,S) n n = 1..E макс 15 групп
  
  // Комманды. Старшие 4 бита - длина данных. Макс длина данных 15 байт
  // Команды без данных
  #define _C_PING_   0x00  // Ping - ждем Pong
  #define _C_PONG_   0x01  // Pong - ответ на Ping
  #define _C_STOP_   0x02  // Режим стоп
  #define _C_PREP_   0x02  // Режим рабочий - подготовительное время
  #define _C_WORK_   0x04  // Work time
  #define _C_W_END_  0x05  // End Work time

  #define _C_M_CONF_ 0x07  // Confirm change mode
  #define _C_S_ENA_  0x08  // Кнопка линии S разрешена
  #define _C_S_DIS_  0x09  // Кнопка линии S запрещена
  #define _C_A_ENA_  0x0A  // Кнопка линии A разрешена
  #define _C_A_DIS_  0x0B  // Кнопка линии A запрещена
  #define _C_B_ENA_  0x0C  // Кнопка линии B разрешена
  #define _C_B_DIS_  0x0D  // Кнопка линии B запрещена
  #define _C_BUT_P_  0x0E  // Кнопка нажата
  #define _C_BUT_C_  0x0F  // Подтверждение - принято Кнопка нажата

  #define _CR_ 0x0d   // Cr - end packet
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
  //pack data;
  byte data[20];
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

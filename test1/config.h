#if !defined(_CONFIG_H)
  #define _CONFIG_H

  #define _A0 0x4130
  #define _A1 0x4131
  #define _A2 0x4132
  #define _A9 0x4139

  #define _CT 0x4354
  #define _C1 0x4331
  #define _C2 0x4332

  #define _CR 0x0d

  struct pack {
    uint16_t addr;
    uint16_t cmd;
    uint16_t data1;
    uint16_t data2;
    uint8_t  end;
  };
  
#endif

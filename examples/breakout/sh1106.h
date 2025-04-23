
#ifndef SH1106_H
#define SH1106_H

#include "armory/i2c.h"
#define SH1106_ADDR 0x3C

#define SH1106_WIDTH  128 
#define SH1106_HEIGHT 64
#define SH1106_PAGE_COUNT 8


void oledInit(I2C_TypeDef *i2c);
void oledUpdate(void);

void oledDrawPixel(uint8_t x, uint8_t y, bool on);
void oledDrawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool on); 

#endif // !SH1106_H

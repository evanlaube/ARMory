
#include "sh1106.h"
#include "armory/i2c.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static uint8_t frameBuffer[SH1106_WIDTH * SH1106_HEIGHT / 8];

static uint8_t dirtyPages = 0xFF;
#define DIRTY_SET(page)     (dirtyPages |= (1 << (page)))
#define DIRTY_CLEAR(page)   (dirtyPages &= ~(1 << (page)))
#define DIRTY_IS_SET(page)  (dirtyPages & (1 << (page)))

static I2C_TypeDef *i2c = NULL;

void oledInit(I2C_TypeDef *i2cInit) {
    i2c = i2cInit;

    static uint8_t init[] = {
        0x00,       // Control byte for commands
        0xAE,       // Display OFF
        0xA1,       // Segment remap (optional)
        0xC8,       // COM scan direction: remapped
        0xA8, 0x3F, // Multiplex ratio (1/64)
        0xD3, 0x00, // Display offset
        0x40,       // Display start line
        0xA6,       // Normal display
        0xA4,       // Entire display ON from RAM
        0xD5, 0xF0, // Display clock
        0xD9, 0xF1, // Pre-charge
        0xDA, 0x12, // COM pins
        0xDB, 0x40, // VCOM detect
        //0x8D, 0x14, // Charge pump
        0x81, 0x7F,       // Contrast
        0xAD, 0x8B,       // Charge pump ON (if supported)
        0x2E,             // Deactivate scroll (important!)
        0xAF        // Display ON
    };
    i2cWriteBytes(i2c, SH1106_ADDR, init, sizeof(init));

    // Initialize the display to all zero
    for(int i = 0; i < sizeof(frameBuffer); i++) {
        frameBuffer[i] = 0;
    }
}

void oledUpdate(void) {
    // Don't draw anything if there are no dirty pages
    if(dirtyPages == 0x00) {
        return;
    }

    static uint8_t buffer[SH1106_WIDTH + 1];
    buffer[0] = 0x40;

    for(uint8_t page = 0; page < 8; page++) {
        // Only draw if the page has been marked to draw on
        if(!DIRTY_IS_SET(page)) {
            continue;
        }

        uint8_t setPage[] = {
            0x00,
            (uint8_t)(0xB0 | page),
            0x02,
            0x10
        };
        i2cWriteBytes(i2c, SH1106_ADDR, setPage, sizeof(setPage));

        for(int i = 0; i < SH1106_WIDTH; i++) {
            buffer[i+1] = frameBuffer[page * SH1106_WIDTH + i];
        }

        i2cWriteBytes(i2c, SH1106_ADDR, buffer, sizeof(buffer));
        DIRTY_CLEAR(page);
    }
}

void oledDrawPixel(uint8_t x, uint8_t y, bool on) {
    if(x >= SH1106_WIDTH || y >= SH1106_HEIGHT) {
        return;
    }

    uint16_t index = (y/8) * SH1106_WIDTH + x;
    uint8_t mask = 1 << (y & 0x07);

    if(on) {
        if(frameBuffer[index] & mask){
            return;
        }
        frameBuffer[index] |= mask;
    } else {
        if(!(frameBuffer[index] & mask)) {
            return;
        }
        frameBuffer[index] &= (~mask);
    }

    // y/8
    DIRTY_SET(y >> 3);
}

void oledDrawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool on) {
    if(x >= SH1106_WIDTH || y >= SH1106_HEIGHT) {
        return;
    }

    if(x + w >= SH1106_WIDTH) {
        w = SH1106_WIDTH - x - 1;
    }

    if(y + h >= SH1106_HEIGHT) {
        h = SH1106_HEIGHT - y - 1;
    }

    for(int dx = 0; dx < w; dx ++) {
        for(int dy = 0; dy < h; dy++) {
            oledDrawPixel(x+dx, y+dy, on);
        }
    }
}

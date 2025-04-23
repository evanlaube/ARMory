
//#include "gpio.h"
//#include "i2c.h"
//#include <stdint.h>
//
//#define SH1106_ADDR 0x3C  // 7-bit I2C address
//
//#define RED_LED     A8
//#define GREEN_LED   A9
//#define BLUE_LED   A10
//
//#define JOY_X    A6 
//#define JOY_Y    A7 
//#define JOY_SW   B5
//
//#define JOY_DEADZONE 400
//#define JOY_RAW_MIN  500
//#define JOY_RAW_MAX  3000
//#define JOY_CENTER   2048
//#define JOY_OUTPUT_MAX 127
//
//#define OLED_SCL B6
//#define OLED_SDA B7
//
//int16_t absVal(int16_t a) {
//    if(a < 0) {
//        return -a;
//    }
//    return a;
//}
//
//volatile int16_t deltaX = 0;
//volatile int16_t deltaY = 0;
//volatile PinState joystickPressed = HIGH;
//
//
//static int16_t scaleJoystick(int raw) {
//    int centered = raw - JOY_CENTER;
//
//    if (centered > JOY_DEADZONE) {
//        return ((centered - JOY_DEADZONE) * JOY_OUTPUT_MAX) / (JOY_RAW_MAX - JOY_CENTER - JOY_DEADZONE);
//    } else if (centered < -JOY_DEADZONE) {
//        return ((centered + JOY_DEADZONE) * JOY_OUTPUT_MAX) / (JOY_CENTER - JOY_RAW_MIN - JOY_DEADZONE);
//    } else {
//        return 0;
//    }
//}
//
//void readJoystick(void) {
//    int rawX = adcReadPin(JOY_X);
//    int rawY = adcReadPin(JOY_Y);
//
//    deltaX = scaleJoystick(rawX);
//    deltaY = scaleJoystick(rawY);
//
//    joystickPressed = !gpioDigitalRead(JOY_SW); // active low
//}
//
//void oledInit(I2C_TypeDef *i2c) {
//    static uint8_t init[] = {
//        0x00,       // Control byte for commands
//        0xAE,       // Display OFF
//        0xA1,       // Segment remap (optional)
//        0xC8,       // COM scan direction: remapped
//        0xA8, 0x3F, // Multiplex ratio (1/64)
//        0xD3, 0x00, // Display offset
//        0x40,       // Display start line
//        0xA6,       // Normal display
//        0xA4,       // Entire display ON from RAM
//        0xD5, 0xF0, // Display clock
//        0xD9, 0xF1, // Pre-charge
//        0xDA, 0x12, // COM pins
//        0xDB, 0x40, // VCOM detect
//        //0x8D, 0x14, // Charge pump
//        0x81, 0x7F,       // Contrast
//        0xAD, 0x8B,       // Charge pump ON (if supported)
//        0x2E,             // Deactivate scroll (important!)
//        0xAF        // Display ON
//    };
//    i2cWriteBytes(i2c, SH1106_ADDR, init, sizeof(init));
//}
//
//void oledFillStripes(I2C_TypeDef *i2c) {
//    static uint8_t buffer[129];
//
//    if(buffer[0] != 0x40) {
//        buffer[0] = 0x40; // Control byte for data
//        for (int i = 1; i < 129; i++) {
//            buffer[i] = (i % 2) ? 0xAA : 0x55;
//        }
//    } else {
//        for(int i = 1; i < 129; i++) {
//            buffer[i] = ~buffer[i];
//        }
//    }
//    for (uint8_t page = 0; page < 8; page++) {
//        uint8_t setPage[] = {
//            0x00,
//            (uint8_t)(0xB0 | page),     // Set page address
//            0x00,                       // Set lower column
//            0x10                        // Set higher column
//        };
//        i2cWriteBytes(i2c, SH1106_ADDR, setPage, sizeof(setPage));
//
//        i2cWriteBytes(i2c, SH1106_ADDR, buffer, sizeof(buffer));
//    }
//}
//
//void oledClear(I2C_TypeDef *i2c) {
//    uint8_t clear[132];
//    for (uint8_t page = 0; page < 8; page++) {
//        uint8_t setPage[] = {
//            0x00,
//            (uint8_t)(0xB0 | page),
//            0x00, // low col
//            0x10  // high col
//        };
//        i2cWriteBytes(i2c, SH1106_ADDR, setPage, sizeof(setPage));
//
//        clear[0] = 0x40; // Control byte for data
//        for (int i = 1; i < 132; i++) clear[i] = 0x00;
//
//        i2cWriteBytes(i2c, SH1106_ADDR, clear, sizeof(clear));
//    }
//}
//
//void delay(volatile int count) {
//    while(count-- > 0);
//}
//
//#define DWT_CTRL    (*(volatile uint32_t*)0xE0001000)
//#define DWT_CYCCNT  (*(volatile uint32_t*)0xE0001004)
//#define DEMCR       (*(volatile uint32_t*)0xE000EDFC)
//
//void delay_ms(uint32_t ms) {
//    // Enable DWT
//    DEMCR |= (1 << 24);       // Enable TRCENA
//    DWT_CTRL |= 1;            // Enable CYCCNT
//    uint32_t start = DWT_CYCCNT;
//
//    uint32_t target = ms * 84000;  // 84 MHz = 84k cycles per ms
//
//    while ((DWT_CYCCNT - start) < target);
//}
//
//int main() {
//    i2cInit(I2C1);
//    oledInit(I2C1);
//    gpioInitAll();
//    adcInit();
//
//    pwmInitPin(RED_LED);
//    gpioPinMode(GREEN_LED, OUTPUT);
//    pwmInitPin(BLUE_LED);
//
//    gpioPinMode(JOY_X,  ANALOG);
//    gpioPinMode(JOY_Y,  ANALOG);
//    gpioPinMode(JOY_SW, INPUT);
//
//    gpioSetPull(JOY_X, NO_PULL);
//    gpioSetPull(JOY_Y, NO_PULL);
//    gpioSetPull(JOY_SW, PULL_UP);
//   
//    uint8_t frameCount = 0;
//
////    while(1) {
////    gpioWrite(GREEN_LED, HIGH);
////    oledClear(I2C1);
////    gpioWrite(GREEN_LED, LOW);
////    oledFillStripes(I2C1);
////    frameCount++;
////    if(frameCount % 60 == 0) {
////        frameCount = 0;
////        gpioWrite(GREEN_LED, HIGH);
////        delay_ms(100);
////        gpioWrite(GREEN_LED, LOW);
////    }
////    }
//
//    while(1) {
////        gpioWrite(GREEN_LED, HIGH);
//        delay_ms(500);
////        gpioWrite(GREEN_LED, LOW);
//        delay_ms(500);
//    }
//
//    //while(1) {
//    //    readJoystick();
//    //    gpioWrite(GREEN_LED, joystickPressed);
//
//    //    uint8_t redBrightness  = absVal(deltaX);
//    //    uint8_t blueBrightness = absVal(deltaY);
//
//    //    pwmWrite(RED_LED, redBrightness);
//    //    pwmWrite(BLUE_LED, blueBrightness);
//    //}
//    
//}

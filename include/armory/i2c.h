
#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"

#define I2C_TIMEOUT_TIME 10000

#define I2C_CR1_PE    ( 1 <<  0 )
#define I2C_SR1_RXNE  ( 1 <<  6 )
#define I2C_CR1_START ( 1 <<  8 )
#define I2C_CR1_STOP  ( 1 <<  9 )
#define I2C_CR1_ACK   ( 1 << 10 )
#define I2C_CR1_SWRST ( 1 << 15 )

#define I2C_SR1_SB    ( 1 <<  0 )
#define I2C_SR1_ADDR  ( 1 <<  1 )
#define I2C_SR1_BTF   ( 1 <<  2 )
#define I2C_SR1_AF    ( 1 << 10 )

#define I2C_SR2_BUSY  ( 1 <<  1 )

typedef struct {
    volatile uint32_t CR1;       // 0x00: Control register 1
    volatile uint32_t CR2;       // 0x04: Control register 2
    volatile uint32_t OAR1;      // 0x08: Own address register 1
    volatile uint32_t OAR2;      // 0x0C: Own address register 2
    volatile uint32_t DR;        // 0x10: Data register
    volatile uint32_t SR1;       // 0x14: Status register 1
    volatile uint32_t SR2;       // 0x18: Status register 2
    volatile uint32_t CCR;       // 0x1C: Clock control register
    volatile uint32_t TRISE;     // 0x20: TRISE register
    volatile uint32_t FLTR;      // 0x24: Filter register
} I2C_TypeDef;

#define I2C1 ((I2C_TypeDef *)0x40005400)
#define I2C2 ((I2C_TypeDef *)0x40005800)
#define I2C3 ((I2C_TypeDef *)0x40005C00)

typedef struct {
    I2C_TypeDef *instance;

    Pin sclPin;
    AlternateFunction sclAf;

    Pin sdaPin;
    AlternateFunction sdaAf;
} I2CMap;

// I2C pin mappings:
// I2C1:
//  SCL->PB6
//  SDA->PB7
// I2C2:
//  SCL->B10
//  SDA->B11
//
//  I2C3 remains unmapped for now
//
//  NOTE: These can be mapped to multiple different pins. There were
//  arbitrarily chosen and are not set in stone
static const I2CMap i2cPinMap[] = {
    { I2C1, B6,  AF4, B7,  AF4 },
    { I2C2, B10, AF4, B11, AF4 }
    //{ I2C3,  }
};

typedef enum {
    I2C_OK,
    I2C_TIMEOUT,
    I2C_NACK,
    I2C_ERROR
} I2CResult;

/**
 * @brief Gets the I2CMap for the given I2C instance.
 * 
 * @param i2c Pointer to the I2C instance to get the mapping for.
 * @return Pointer to the I2CMap for given I2C instance.
 */
const I2CMap *getI2CMap(I2C_TypeDef *i2c);

/**
 * @brief Initializes the I2C peripheral.
 * 
 * Enables the I2C clock, configures the GPIO pins for SCL and SDA,
 * and sets the I2C clock speed.
 *
 * @param i2c Pointer to the I2C instance to initialize.
 */
void i2cInit(I2C_TypeDef *i2c);

/**
 * @brief Starts the I2C communication.
 *
 * Issues a START condition on the I2C bus.
 *
 * @param i2c Pointer to the I2C instance to start communication on.
 */
void i2cStart(I2C_TypeDef *i2c);

/**
 * @brief Stops the I2C communication.
 *
 * Issues a STOP condition on the I2C bus.
 *
 * @param i2c Pointer to the I2C instance to stop communication on.
 */
void i2cStop(I2C_TypeDef *i2c);

/**
 * @brief Sends the device address on the I2C bus.
 *
 * Sends the device address shifted left by 1 bit, setting the LSB to the 
 * read bit. If read is true, the LSB is set to 1, indicating a read operation.
 *
 * @param i2c Pointer to the I2C instance to send the address on.
 * @param addr The device address to send.
 * @param read True if following with a read operation, false for write.
 *
 * @returns I2CResult indicating the result of the operation.
 */
I2CResult i2cSendAddr(I2C_TypeDef *i2c, uint8_t addr, bool read);

/**
 * @brief Sends a single data byte on the I2C bus.
 *
 * Sends a single byte of data on the specified I2C bus. The byte has no device
 * addressing or register addressing, it is simply a raw byte.
 *
 * @param i2c Pointer to the I2C instance to send the data on.
 * @param data The data byte to send.
 *
 * @returns I2CResult indicating the result of the operation.
 *
 * @note This function is intended to only be called inside of other I2C functions,
 *       but is exposed for convenience.
 */
I2CResult i2cSendData(I2C_TypeDef *i2c, uint8_t data);

/**
 * @brief Writes a byte to a specific register of a device on the I2C bus.
 *
 * Sends the device address byte, followed by the register address byte, 
 * followed by a single byte of data to the specified I2C bus. This is a 
 * complete I2C transaction.
 *
 * @param i2c Pointer to the I2C instance to send the data on.
 * @param devAddr The device address to send the data to.
 * @param regAddr The register address to write to.
 * @param data The data byte toe send.
 *
 * @returns I2CResult indicating the result of the operation.
 */
I2CResult i2cWriteByte(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t regAddr, uint8_t data);

/**
 * @brief Writes a raw byte of data to a device on the I2C bus.
 *
 * Sends the device address byte, followed by a single byte of data to the
 * specified I2C bus.
 *
 * @param i2c Pointer to the I2C instance to send the data on.
 * @param devAddr The device address to send the data to.
 * @param data The data byte to send.
 *
 * @returns I2CResult indicating the result of the operation.
 */
I2CResult i2cWriteRaw(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t data);

/**
 * @brief Writes multiple bytes of data to a device on the I2C bus.
 *
 * Sends the address byte, followed by the bytes in the data array to the
 * specified I2C bus.
 *
 * @param i2c Pointer to the I2C instance to send the data on.
 * @param devAddr The device address to send the data bytes to.
 * @param data Pointer to the array of data bytes to send.
 * @param n The number of bytes in the data array
 *
 * @returns I2CResult indicating the result of the operation.
 *
 * @note This function does not send a register address. If needed, it should
 *       be the first byte of the data array.
 */
I2CResult i2cWriteBytes(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t *data, uint16_t n);

/**
 * @brief Reads a byte from a specific register of a device on the I2C bus.
 *
 * Sends the device address byte, followed by the register address byte, then
 * issues a repeated start condition, followed by sending the device address 
 * byte again with the read bit set. Then receives the data byte from the device.
 * The data byte is stored in the passed data pointer.
 *
 * @param i2c Pointer to the I2C instance to read the data from.
 * @param devAddr The device address to read the data from.
 * @param regAddr The register address to read from.
 * @param data Pointer to the variable to store the received data byte in.
 *
 * @return I2cResult indicating the result of the operation.
 */
I2CResult i2cReadByte(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t regAddr, uint8_t *data);

/**
 * @brief Reads a raw byte of data from a device on the I2C bus.
 *
 * Sends the device address byte with the read bit set, the receives the data
 * byte from the device. The data byte is stored in the passed data pointer.
 *
 * @param i2c Pointer to the I2C instance to read the data from.
 * @param devAddr The device address to read the data from.
 * @param data Pointer to the variable to store the reveived data byte it.
 */
I2CResult i2cReadRaw(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t *data);

/**
 * @brief Reads multiple bytes of data from a device on the I2C bus.
 *
 * Sends the device address byte, followed by the register address byte, then
 * issues a repeated start condition, followed by sending the device address 
 * byte again with the read bit set. Then, receives the data bytes from the
 * device.
 *
 * @param i2c Pointer to the I2C instance to read the data from.
 * @param devAddr The device address to read the data from.
 * @param regAddr The register address to read from
 * @param buffer Pointer to the buffer array to store the received data bytes in.
 * @param len The number of bytes to read.
 *
 * @return I2CResult indicating the result of the operation.
 */
I2CResult i2cReadBytes(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t regAddr, 
        uint8_t *buffer, uint16_t len);

/**
 * @brief Reads multiple bytes of raw data from a device on the I2C bus.
 *
 * Sendds the device address byte with the read bit set, then receives the 
 * specified number of data bytes from the device and stores the at the passed 
 * data pointer.
 *
 * @param i2c Pointer to the I2C instance to read the data from.
 * @param devAddr The device address to read the data from.
 * @param buffer Pointer to the buffer array to store the received data bytes in.
 * @param len The number of bytes to read.
 */
I2CResult i2cReadRawBytes(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t *buffer, 
        uint16_t len);


#endif // !I2C_H

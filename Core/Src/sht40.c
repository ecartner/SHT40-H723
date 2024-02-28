/*
 * sht40.c
 *
 *  Created on: Feb 22, 2024
 *      Author: ecartner
 */
#include "sht40.h"

// SHT-40 Commands
enum
{
    SHT40_HIGH_PRECISION_MEASUREMENT = 0xFD,
    SHT40_MEDIUM_PRECISION_MEASUREMENT = 0xF6,
    SHT40_LOW_PRECISION_MEASUREMENT = 0xE0,
    SHT40_READ_SERIAL_NUMBER = 0x89,
    SHT40_SOFT_RESET = 0x94
} Sht40Command;

// Internal constants (not needed by the outside world)
enum
{
    SHT40_I2C_ADDRESS = 0x44 << 1,
    SHT40_MAX_LOW_PRECISION_READ_MICROS = 1600,
    SHT40_MAX_MEDIUM_PRECISION_READ_MICROS = 4500,
    SHT40_MAX_HIGH_PRECISION_READ_MICROS = 8300,
    SHT40_READ_LENGTH = 6
} Sht40InternalConstant;

static inline HAL_StatusTypeDef send_command(SHT40 *device, uint8_t command)
{
    return HAL_I2C_Master_Transmit(device->i2c, SHT40_I2C_ADDRESS, &command, 1, HAL_MAX_DELAY);
}

void sht40_init(SHT40 *device, I2C_HandleTypeDef *i2cHandle)
{
    device->i2c = i2cHandle;
}

HAL_StatusTypeDef sht40_soft_reset(SHT40 *device)
{
    return send_command(device, SHT40_SOFT_RESET);
}

HAL_StatusTypeDef sht40_start_measurement(SHT40 *device)
{
    return send_command(device, SHT40_HIGH_PRECISION_MEASUREMENT);
}

HAL_StatusTypeDef sht40_read(SHT40 *device)
{
    return HAL_I2C_Master_Receive(device->i2c, SHT40_I2C_ADDRESS, device->buffer, SHT40_READ_LENGTH, HAL_MAX_DELAY);
}

HAL_StatusTypeDef sht40_dma_read(SHT40 *device)
{
    return HAL_I2C_Master_Receive_DMA(device->i2c, SHT40_I2C_ADDRESS, device->buffer, SHT40_READ_LENGTH);
}

float sht40_temperatureF(const SHT40 *device)
{
    uint16_t st = (uint16_t)(device->buffer[0]) << 8;
    st |= device->buffer[1];
    float f = st;

    return -49 + 315.0f * f / 65535.0f;
}

float sht40_relative_humidity(const SHT40 *device)
{
    uint16_t sh = (uint16_t)(device->buffer[3]) << 8;
    sh |= device->buffer[4];
    float rh = sh;
    return -6 + 125.0f * rh / 65535.0f;
}

/*
 * sht40.h
 *
 *  Created on: Feb 22, 2024
 *      Author: ecartner
 */

#ifndef INC_SHT40_H_
#define INC_SHT40_H_

#include <stdint.h>
#include "stm32h7xx_hal.h"

typedef struct
{
    I2C_HandleTypeDef *i2c;
    uint8_t buffer[6];
} SHT40;

void sht40_init(SHT40 *device, I2C_HandleTypeDef *i2cHandle);
HAL_StatusTypeDef sht40_soft_reset(SHT40 *device);
HAL_StatusTypeDef sht40_start_measurement(SHT40 *device);
HAL_StatusTypeDef sht40_read(SHT40 *device);
HAL_StatusTypeDef sht40_dma_read(SHT40 *device);
float sht40_temperatureF(const SHT40 *device);
float sht40_relative_humidity(const SHT40 *device);

#endif /* INC_SHT40_H_ */

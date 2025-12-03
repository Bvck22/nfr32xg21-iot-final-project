/**
 * @file       aht20.h
 * @brief      AHT20 Driver for Silicon Labs EFR32xG21
 */

#ifndef INC_AHT20_H_
#define INC_AHT20_H_

#include <stdbool.h>
#include <stdint.h>

/* Silicon Labs GSDK Includes */
#include "sl_i2cspm.h"
#include "sl_status.h"

/* Public defines ----------------------------------------------------- */
/**
 * @brief Default I2C address of AHT20 (Shifted for SL 8-bit format)
 */
#define AHT20_I2C_ADDR (0x38 << 1)

/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief Error codes for AHT20 driver
 */
typedef enum
{
  AHT20_OK = 0,      /**< Operation successful */
  AHT20_ERR_PARAM,   /**< Invalid argument */
  AHT20_ERR_COMM,    /**< I2C communication error */
  AHT20_ERR_TIMEOUT, /**< Timeout waiting for device */
  AHT20_ERR_CALIB,   /**< Calibration failed */
  AHT20_ERR_UNKNOWN  /**< Unknown error */
} aht20_err_t;

/**
 * @brief AHT20 device handle for EFR32
 */
typedef struct
{
  sl_i2cspm_t *i2c_inst; /**< Pointer to I2C SPM instance (e.g., sl_i2cspm_sensor) */
  uint8_t      addr;     /**< I2C Device Address (8-bit) */
} aht20_t;

/* Public function prototypes ----------------------------------------- */

/**
 * @brief  Initialize AHT20 sensor
 * @param[in]  me  Pointer to device object
 * @return     AHT20_OK on success
 */
aht20_err_t aht20_init(aht20_t *me);

/**
 * @brief  Check if sensor is ready on I2C bus
 * @param[in]  me  Pointer to device object
 * @return     AHT20_OK if device responds
 */
aht20_err_t aht20_is_ready(aht20_t *me);

/**
 * @brief  Read temperature and humidity
 * @param[in]   me           Pointer to device object
 * @param[out]  temperature  Pointer to store temperature in °C
 * @param[out]  humidity     Pointer to store relative humidity in %RH
 * @return      AHT20_OK on success
 */
aht20_err_t aht20_read_data(aht20_t *me, float *temperature, float *humidity);

#endif /* INC_AHT20_H_ */

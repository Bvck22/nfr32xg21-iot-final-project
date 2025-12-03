/**
 * @file       aht20.c
 * @brief      AHT20 Driver implementation for EFR32xG21
 */

#include "aht20.h"
#include "sl_sleeptimer.h"
#include "em_i2c.h"

/* Private defines ---------------------------------------------------- */
#define AHT20_CMD_SOFT_RESET    (0xBA)
#define AHT20_STATUS_CALIBRATED (0x08)
#define AHT20_STATUS_BUSY       (0x80)

/* Private function prototypes ---------------------------------------- */
static aht20_err_t aht20_write(aht20_t *me, const uint8_t *data, uint16_t len);
static aht20_err_t aht20_read(aht20_t *me, uint8_t *data, uint16_t len);
static aht20_err_t aht20_wait_ready(aht20_t *me, uint32_t timeout_ms);
static aht20_err_t aht20_read_status(aht20_t *me, uint8_t *status);

/* Function definitions ----------------------------------------------- */

aht20_err_t aht20_init(aht20_t *me)
{
  if (!me || !me->i2c_inst)
    return AHT20_ERR_PARAM;

  // Check device presence
  if (aht20_is_ready(me) != AHT20_OK)
    return AHT20_ERR_COMM;

  // Soft reset
  uint8_t reset_cmd = AHT20_CMD_SOFT_RESET;
  if (aht20_write(me, &reset_cmd, 1) != AHT20_OK)
    return AHT20_ERR_COMM;

  sl_sleeptimer_delay_millisecond(40);

  // Send calibration command
  uint8_t cal_cmd[] = { 0xBE, 0x08, 0x00 };
  if (aht20_write(me, cal_cmd, sizeof(cal_cmd)) != AHT20_OK)
    return AHT20_ERR_COMM;

  // Verify calibration flag
  uint8_t status;
  sl_sleeptimer_delay_millisecond(10); // Wait a bit before checking

  if (aht20_read_status(me, &status) != AHT20_OK)
    return AHT20_ERR_COMM;

  if (!(status & AHT20_STATUS_CALIBRATED))
    return AHT20_ERR_CALIB;

  return AHT20_OK;
}

aht20_err_t aht20_is_ready(aht20_t *me)
{
  if (!me || !me->i2c_inst) return AHT20_ERR_PARAM;

  // Attempt a zero-length write to check ACK
  I2C_TransferSeq_TypeDef seq;
  seq.addr = me->addr;
  seq.flags = I2C_FLAG_WRITE;
  seq.buf[0].data = NULL;
  seq.buf[0].len = 0;

  I2C_TransferReturn_TypeDef ret = I2C_TransferInit(me->i2c_inst, &seq);
  return (ret == i2cTransferDone) ? AHT20_OK : AHT20_ERR_COMM;
}

aht20_err_t aht20_read_data(aht20_t *me, float *temperature, float *humidity)
{
  if (!me || !temperature || !humidity) return AHT20_ERR_PARAM;

  // Trigger measurement
  uint8_t cmd[] = { 0xAC, 0x33, 0x00 };
  if (aht20_write(me, cmd, sizeof(cmd)) != AHT20_OK)
    return AHT20_ERR_COMM;

  // Wait for measurement to complete (at least 80ms per datasheet)
  if (aht20_wait_ready(me, 100) != AHT20_OK)
    return AHT20_ERR_TIMEOUT;

  uint8_t raw[6];
  if (aht20_read(me, raw, 6) != AHT20_OK)
    return AHT20_ERR_COMM;

  // Conversion logic
  uint32_t raw_hum_val  = ((uint32_t) raw[1] << 12) | ((uint32_t) raw[2] << 4) | (raw[3] >> 4);
  uint32_t raw_temp_val = (((uint32_t) raw[3] & 0x0F) << 16) | ((uint32_t) raw[4] << 8) | raw[5];

  *humidity    = ((float) raw_hum_val / 1048576.0f) * 100.0f;
  *temperature = ((float) raw_temp_val / 1048576.0f) * 200.0f - 50.0f;

  return AHT20_OK;
}

/* Private Helper Functions ------------------------------------------- */

static aht20_err_t aht20_write(aht20_t *me, const uint8_t *data, uint16_t len)
{
  I2C_TransferSeq_TypeDef seq;
  seq.addr = me->addr;
  seq.flags = I2C_FLAG_WRITE;
  seq.buf[0].data = (uint8_t*)data;
  seq.buf[0].len = len;

  // Using I2C_TransferInit directly allows generic I2C usage,
  // sl_i2cspm wraps this usually but exposes setup.
  I2C_TransferReturn_TypeDef ret = I2C_TransferInit(me->i2c_inst, &seq);

  return (ret == i2cTransferDone) ? AHT20_OK : AHT20_ERR_COMM;
}

static aht20_err_t aht20_read(aht20_t *me, uint8_t *data, uint16_t len)
{
  I2C_TransferSeq_TypeDef seq;
  seq.addr = me->addr;
  seq.flags = I2C_FLAG_READ;
  seq.buf[0].data = data;
  seq.buf[0].len = len;

  I2C_TransferReturn_TypeDef ret = I2C_TransferInit(me->i2c_inst, &seq);

  return (ret == i2cTransferDone) ? AHT20_OK : AHT20_ERR_COMM;
}

static aht20_err_t aht20_read_status(aht20_t *me, uint8_t *status)
{
  return aht20_read(me, status, 1);
}

static aht20_err_t aht20_wait_ready(aht20_t *me, uint32_t timeout_ms)
{
  uint8_t  status;
  uint32_t elapsed = 0;
  const uint32_t step_ms = 5;

  while (elapsed < timeout_ms)
  {
    if (aht20_read_status(me, &status) == AHT20_OK)
    {
      if (!(status & AHT20_STATUS_BUSY))
      {
        return AHT20_OK;
      }
    }
    sl_sleeptimer_delay_millisecond(step_ms);
    elapsed += step_ms;
  }
  return AHT20_ERR_TIMEOUT;
}

#include "bsp_aht20.h"
#include "sl_i2cspm_instances.h"

static aht20_t aht20_handle;

aht20_err_t bsp_aht20_init(void)
{

    aht20_handle.i2c_inst = sl_i2cspm_sensor;


    aht20_handle.addr = AHT20_I2C_ADDR;


    return aht20_init(&aht20_handle);
}

aht20_err_t bsp_aht20_read(float *temp, float *humi)
{

    return aht20_read_data(&aht20_handle, temp, humi);
}

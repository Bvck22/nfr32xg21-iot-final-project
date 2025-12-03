#ifndef BSP_AHT20_H_
#define BSP_AHT20_H_

#include "aht20.h"


aht20_err_t bsp_aht20_init(void);

aht20_err_t bsp_aht20_read(float *temp, float *humi);

#endif

/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include "app.h"
#include "bsp_aht20.h"
#include "bsp_usart.h"
#include "sl_sleeptimer.h"
#include <stdio.h>
#include "sl_i2cspm_instances.h"

#include "sl_board_control.h"
#include "em_assert.h"
#include "glib.h"
#include "dmd.h"
/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/

aht20_t h_aht20 = {
  .i2c_inst = NULL,
  .addr     = AHT20_I2C_ADDR
};

void app_init(void)
{
  //h_aht20.i2c_inst = sl_i2cspm_sensor;
  //aht20_err_t err = aht20_init();
  memlcd_app_init();
  bsp_usart_init();


}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(int time)
{
    bsp_usart_print("\n Me may beo 123\n\r");
    aht20_err_t err = bsp_aht20_init();
    char lmao[30];
    sprintf(lmao, "\n Err: %d\n\r", err);
    bsp_usart_print(lmao);
    float t, h;
    bsp_aht20_read(&t, &h);
    memlcd_app_process_action(&t, &h, time);
    char output[50];
    sprintf(output, "T: %d.%02d H: %d.%02d\n\r", (int)t,(int)((t - (int)t) * 100), (int)h, (int)((h - (int)h) * 100));

    bsp_usart_print(output);
    sl_sleeptimer_delay_millisecond(time);
}

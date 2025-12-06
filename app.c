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
/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/

aht20_t h_aht20 = {
  .i2c_inst = NULL,
  .addr     = AHT20_I2C_ADDR
};

void app_init(void)
{
  h_aht20.i2c_inst = sl_i2cspm_sensor;
  //aht20_err_t err = aht20_init();
  bsp_usart_init();
  bsp_usart_print("\n Me may beo 123\n\r");
  aht20_err_t err = aht20_init(&h_aht20);
  char lmao[30];
  sprintf(lmao, "\n Err: %d\n\r", err);
  bsp_usart_print(lmao);
  sl_sleeptimer_delay_millisecond(100);
  float t, h;
  aht20_read_data(&h_aht20, &t, &h);
  char output[50];
  sprintf(output, "T: %.2f\n", t);
  bsp_usart_print(output);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  float temperature, humidity;
  char output[256];
      aht20_err_t err = bsp_aht20_read(&temperature, &humidity);

      if (err == AHT20_OK) {

          sprintf(output, "T: %.2f C, H: %.2f %%\n", temperature, humidity);
          bsp_usart_print(output);
      } else {
          printf("Read Error\n");
      }

      sl_sleeptimer_delay_millisecond(1000);
}

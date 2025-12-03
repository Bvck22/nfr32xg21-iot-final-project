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
/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/


void app_init(void)
{
  //aht20_err_t err = aht20_init();
  bsp_usart_init();
  bsp_usart_print("me may beo 123");
  if (aht20_init(AHT20_I2C_ADDR)) {
          bsp_usart_print("AHT20 Init Success!\n");
      } else {
          bsp_usart_print("AHT20 Init cc!\n");
          //printf("AHT20 Init Failed code: %d\n", err);
      }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  float temperature, humidity;
  char output;
      aht20_err_t err = bsp_aht20_read(&temperature, &humidity);

      if (err == AHT20_OK) {

          sprintf(output, "T: %.2f C, H: %.2f %%\n", temperature, humidity);
          bsp_usart_print(output);
      } else {
          printf("Read Error\n");
      }

      sl_sleeptimer_delay_millisecond(1000);
}

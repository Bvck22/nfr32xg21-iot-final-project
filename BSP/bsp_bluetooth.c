/*
 * bsp_bluetooth.c
 *
 *  Created on: Dec 6, 2025
 *      Author: ADMIN
 */
#include "bsp_bluetooth.h"
#include <string.h>
#include "stdio.h"

void fill_adv_packet(CustomAdv_t *pData, uint8_t flags, uint16_t companyID, uint16_t temp, uint8_t hum, char *name)
{
  int n;

  pData->len_flags = 0x02;
  pData->type_flags = 0x01;
  pData->val_flags = flags;

  pData->len_manuf = 6;  // 1+2+4 bytes for type, company ID and the payload
  pData->type_manuf = 0xFF;
  pData->company_LO = companyID & 0xFF;
  pData->company_HI = (companyID >> 8) & 0xFF;

  pData->temp_L = temp & 0xFF;
  pData->temp_H = (temp >> 8) & 0xFF;
  pData ->hum = hum;


  // Name length, excluding null terminator
  n = strlen(name);
  if (n > NAME_MAX_LENGTH) {
    // Incomplete name
    pData->type_name = 0x08;
  } else {
    pData->type_name = 0x09;
  }

  strncpy(pData->name, name, NAME_MAX_LENGTH);

  if (n > NAME_MAX_LENGTH) {
    n = NAME_MAX_LENGTH;
  }

  pData->len_name = 1 + n; // length of name element is the name string length + 1 for the AD type

  // Calculate total length of advertising data
  pData->data_size = 3 + (1 + pData->len_manuf) + (1 + pData->len_name);
}
void start_adv(CustomAdv_t *pData, uint8_t advertising_set_handle)
{
  sl_status_t sc;
  // Set custom advertising payload
  sc = sl_bt_legacy_advertiser_set_data(advertising_set_handle, 0, pData->data_size, (const uint8_t *)pData);
  app_assert(sc == SL_STATUS_OK,
                "[E: 0x%04x] Failed to set advertising data\n",
                (int)sc);

  // Start advertising using custom data
  sc = sl_bt_legacy_advertiser_start(advertising_set_handle, sl_bt_legacy_advertiser_connectable);
  app_assert(sc == SL_STATUS_OK,
                  "[E: 0x%04x] Failed to start advertising\n",
                  (int)sc);
}
void update_adv_data(CustomAdv_t *pData, uint8_t advertising_set_handle, uint16_t temp, uint8_t hum)
{
  sl_status_t sc;
  // Update the variable fields in the custom advertising packet
  pData->temp_L = temp & 0xFF;
  pData->temp_H = (temp >> 8) & 0xFF;
  pData->hum = hum;


  // Set custom advertising payload
  sc = sl_bt_legacy_advertiser_set_data(advertising_set_handle, 0, pData->data_size, (const uint8_t *)pData);
  app_assert(sc == SL_STATUS_OK,
                  "[E: 0x%04x] Failed to set advertising data\n",
                  (int)sc);
}

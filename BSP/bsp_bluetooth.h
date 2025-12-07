/*
 * bsp_bluetooth.h
 *
 *  Created on: Dec 6, 2025
 *      Author: ADMIN
 */

#ifndef BSP_BSP_BLUETOOTH_H_
#define BSP_BSP_BLUETOOTH_H_

#include "sl_bt_api.h"
#include "app_assert.h"
#define NAME_MAX_LENGTH 14

 //define for packet
#define FLAG  0x06
#define COMPANY_ID  0x02FF

typedef struct
{
  //3 bytes for Flag type
  uint8_t len_flags;
  uint8_t type_flags;
  uint8_t val_flags;

  //Then use type for Manufacturer Specific Data
  uint8_t len_manuf;
  uint8_t type_manuf;
  // First two bytes must contain the manufacturer ID (little-endian order)
  uint8_t company_LO;
  uint8_t company_HI;

  //Data for Manufacturer specific data
  uint8_t temp_L;
  uint8_t temp_H;
  uint8_t hum;


  // length of the name AD element is variable, adding it last to keep things simple
  uint8_t len_name;
  uint8_t type_name;

  // NAME_MAX_LENGTH must be sized so that total length of data does not exceed 31 bytes
  char name[NAME_MAX_LENGTH];

  // These values are NOT included in the actual advertising payload, just for bookkeeping
  char dummy;        // Space for null terminator
  uint8_t data_size; // Actual length of advertising data
} CustomAdv_t;
void fill_adv_packet(CustomAdv_t *pData, uint8_t flags, uint16_t companyID,  uint16_t temp, uint8_t hum,char *name);
void start_adv(CustomAdv_t *pData, uint8_t advertising_set_handle);
void update_adv_data(CustomAdv_t *pData, uint8_t advertising_set_handle, uint16_t temp, uint8_t hum);
#endif /* BSP_BSP_BLUETOOTH_H_ */

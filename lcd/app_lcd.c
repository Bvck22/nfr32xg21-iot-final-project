/*
 * lcd.c
 *
 *  Created on: Nov 4, 2024
 *      Author: Phat_Dang
 */
#include <stdio.h>

#include "sl_board_control.h"
#include "em_assert.h"
#include "glib.h"
#include "dmd.h"

#ifndef LCD_MAX_LINES
#define LCD_MAX_LINES      11
#endif

/*******************************************************************************
 ***************************  LOCAL VARIABLES   ********************************
 ******************************************************************************/
static GLIB_Context_t glibContext;
static int currentLine = 0;

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * Initialize example.
 ******************************************************************************/
void memlcd_app_init(void)
{
  uint32_t status;

  /* Enable the memory lcd */
  status = sl_board_enable_display();
  EFM_ASSERT(status == SL_STATUS_OK);

  /* Initialize the DMD support for memory lcd display */
  status = DMD_init(0);
  EFM_ASSERT(status == DMD_OK);

  /* Initialize the glib context */
  status = GLIB_contextInit(&glibContext);
  EFM_ASSERT(status == GLIB_OK);

  glibContext.backgroundColor = White;
  glibContext.foregroundColor = Black;

  /* Fill lcd with background color */
  GLIB_clear(&glibContext);

  /* Use Narrow font */
  GLIB_setFont(&glibContext, (GLIB_Font_t *) &GLIB_FontNarrow6x8);

  /* Draw text on the memory lcd display*/
  GLIB_drawStringOnLine(&glibContext,
                        "BaoKhanhGaming",
                        currentLine,
                        GLIB_ALIGN_LEFT,
                        5,
                        5,
                        true);

  DMD_updateDisplay();
}


/***************************************************************************//**
 * Ticking function.
 ******************************************************************************/
void memlcd_app_process_action(float *t, float *h, int time)
{

  char output[50], period[50];
  sprintf(output, "T: %d.%02d H: %d.%02d", (int)*t,(int)((*t - (int)*t) * 100), (int)*h, (int)((*h - (int)*h) * 100));
  //sprintf(output, "T: %.2f", t);
  GLIB_drawStringOnLine(&glibContext,
                        output,
                        0,
                        GLIB_ALIGN_CENTER,
                        5,
                        20,
                        true);
  sprintf(period, "Period: %d ms", time);
  GLIB_drawStringOnLine(&glibContext,
                        period,
                        0,
                        GLIB_ALIGN_CENTER,
                        5,
                        40,
                        true);
  DMD_updateDisplay();
  return;
}

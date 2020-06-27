/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once


 /**
  * lcd/dogm/TenlogLCD.h
  */

  /**
   * Marlin 3D Printer Firmware
   * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
   *
   * Based on Sprinter and grbl.
   * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
   *
   * This program is free software: you can redistribute it and/or modify
   * it under the terms of the GNU General Public License as published by
   * the Free Software Foundation, either version 3 of the License, or
   * (at your option) any later version.
   *
   * This program is distributed in the hope that it will be useful,
   * but WITHOUT ANY WARRANTY; without even the implied warranty of
   * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   * GNU General Public License for more details.
   *
   * You should have received a copy of the GNU General Public License
   * along with this program.  If not, see <http://www.gnu.org/licenses/>.
   *
   */

   /**
    * lcd/dogm/TenlogLCD.cpp
    *
    * Implementation of the LCD display routines for a Tenlog boards that use the AI HMI T8 chip.

    */
#include <WString.h>   
#include "../../inc/MarlinConfigPre.h"
#include "../../module/temperature.h"
#include "../../module/motion.h"
#include "../../sd/cardreader.h"


#define NUM_AXIS 4 // The axis order in all axis related arrays is X, Y, Z, E

extern float Tenlog_current_position[NUM_AXIS];
extern int Tenlog_i_print_page_id;
extern int Tenlog_tenlog_status_update_delay;
extern int Tenlog_tenlogScreenUpdate;

void TenlogScreen_begin(int boud);
void TenlogScreen_println(char const* s);
void TenlogScreen_print(char const* s);
void TenlogScreen_printend();
void TenlogScreen_printEmptyend();
bool MSerial2_available();
char MSerial2_read();
void tenlog_screen_update(CardReader card);
void tenlog_status_screen(CardReader card);
//Get Data From Commport
String getSerial2Data();
void Init_TenlogScreen();
void CSDI_TLS();

void sdcard_tlcontroller();


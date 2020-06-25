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

#ifdef TENLOG_LCD_CONTROLLER

#include "../../inc/MarlinConfigPre.h"

bool MSerial2_available();
char MSerial2_read();
void get_command_1();

void tenlog_status_screen();

void TenlogScreen_begin(int boud);
void TenlogScreen_println(const char s[]);
void TenlogScreen_print(const char s[]);
void TenlogScreen_printend();
void TenlogScreen_printEmptyend();

void sdcard_tlcontroller();

#endif

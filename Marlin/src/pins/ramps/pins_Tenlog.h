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
 * Velleman K8800 (Vertex)
 */

#if !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__)
  #error "Oops! Select 'Arduino/Genuino Mega or Mega 2560' in 'Tools > Board.'"
#endif

#define BOARD_INFO_NAME      "Tenlog"
#define DEFAULT_MACHINE_NAME "Hero"

//#define LCD_SCREEN_ROT_180

//
// Limit Switches
//
//#define X_STOP_PIN                             3
//#define Y_STOP_PIN                            14

//
// Steppers
//
#define X_STEP_PIN                            54
#define X_DIR_PIN                             55
#define X_ENABLE_PIN                          38
#define X_MAX_PIN                             2
#define X_MIN_PIN                             3

#define X2_STEP_PIN                           36
#define X2_DIR_PIN                            34
#define X2_ENABLE_PIN                         30

#define Y_STEP_PIN                            60
#define Y_DIR_PIN                             61
#define Y_ENABLE_PIN                          56
#define Y_MAX_PIN                             15
#define Y_MIN_PIN                             14

#define Z_STEP_PIN                            46
#define Z_DIR_PIN                             48
#define Z_ENABLE_PIN                          62
#define Z_MIN_PIN                             18
#define Z_MAX_PIN                             19

#define Z2_STEP_PIN                           65
#define Z2_DIR_PIN                            66
#define Z2_ENABLE_PIN                         64
//#define Z2_MIN_PIN                            19	


#define E1_STEP_PIN        26
#define E1_DIR_PIN         28
#define E1_ENABLE_PIN      24

#define E0_STEP_PIN        57 
#define E0_DIR_PIN         58 
#define E0_ENABLE_PIN      59 

//
// Temperature Sensors
//
#define TEMP_1_PIN                            13  
#define TEMP_0_PIN                            15	
#define TEMP_BED_PIN                          14 

//
// Heaters / Fans
//
#define HEATER_1_PIN                          10   
#define HEATER_0_PIN                          11
#define HEATER_BED_PIN                        8	
#define FAN_PIN                               9
#define FAN2_PIN                              5//5//63
//#define CONTROLLER_FAN_PIN                    9

//
// Misc. Functions
//
#define SDSS                                  53
#define LED_PIN                               13
#define PS_ON_PIN                             40 

#define FIL_RUNOUT_PIN                        69  // PK7
#define KILL_PIN                              32  // PD1
//#define PHOTOGRAPH_PIN                        23

//
// LCD / Controller
//
#define SD_DETECT_PIN                         49  // PD0
//#define LCD_SDSS                              53
//#define DOGLCD_CS                             29
//#define DOGLCD_A0                             27


#if ENABLED(NEWPANEL)
  #define BTN_EN1                             17
  #define BTN_EN2                             16
  #define BTN_ENC                             23
#endif

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

#include <string.h>
#include "TenlogLCD.h"
#include "../../inc/MarlinConfigPre.h"
#include "../../HAL/AVR/HAL.h"
#include "../../module/temperature.h"
#include "../../module/motion.h"
#include "../../sd/cardreader.h"

float Tenlog_current_position[NUM_AXIS];
int Tenlog_i_print_page_id;
int Tenlog_tenlog_status_update_delay;
int Tenlog_tenlogScreenUpdate;
unsigned long starttime = 0;

char Myconv[8];

char chrEnd = 0xFF;

float Y2_OFFSET = 4.5;
float Z2_OFFSET = 2.0;

float axis_steps_per_unit[] = DEFAULT_AXIS_STEPS_PER_UNIT;

char* itostr2(const uint8_t& x)
{

  //sprintf(Myconv,"%5.1f",x);
  int xx = x;
  Myconv[0] = (xx / 10) % 10 + '0';
  Myconv[1] = (xx) % 10 + '0';
  Myconv[2] = 0;
  return Myconv;
}

void TenlogScreen_begin(int boud) {
  MYSERIAL1.begin(boud);
}

String getSplitValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {
    0, -1
  };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
bool strISAscii(String str)
{
  bool bOK = true;
  int iFNL = str.length();
  char cFN[iFNL];
  str.toCharArray(cFN, iFNL);
  for (int i = 0; i < iFNL - 1; i++)
  {
    if (!isAscii(cFN[i]))
    {
      //ZYF_DEBUG_PRINT_LN((int)cFN[i]);
      bOK = false;
      break;
    }
  }
  return bOK;
}

void TenlogScreen_println(char const* s) {
  MYSERIAL1.print(s);
  MYSERIAL1.write(chrEnd);
  MYSERIAL1.write(chrEnd);
  MYSERIAL1.write(chrEnd);
}

void TenlogScreen_print(char const* s) {
  MYSERIAL1.print(s);
}


void TenlogScreen_printend() {
  MYSERIAL1.write(chrEnd);
  MYSERIAL1.write(chrEnd);
  MYSERIAL1.write(chrEnd);
}

void TenlogScreen_printEmptyend() {
  MYSERIAL1.write((char)0x00);
  MYSERIAL1.write(chrEnd);
  MYSERIAL1.write(chrEnd);
  MYSERIAL1.write(chrEnd);
}

bool MSerial2_available() {
  return MYSERIAL1.available();
}

char MSerial2_read() {
  return MYSERIAL1.read();
}

void tenlog_screen_update(CardReader card)
{
  if (starttime == 0) starttime = millis();
  String strAll = "main.sStatus.txt=\"";
  long lN = Tenlog_current_position[X_AXIS] * 10.0; //1
  String sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = Tenlog_current_position[Y_AXIS] * 10.0;     //2
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = Tenlog_current_position[Z_AXIS] * 10.0;     //3
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = Tenlog_current_position[E_AXIS] * 10.0;     //4
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = int(thermalManager.degTargetHotend(0) + 0.5);     //5
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = int(thermalManager.degHotend(0) + 0.5);           //6
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = int(thermalManager.degTargetHotend(1) + 0.5);     //7
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = int(thermalManager.degHotend(1) + 0.5);           //8
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = int(thermalManager.degTargetBed() + 0.5);         //9
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = int(thermalManager.degBed() + 0.5);               //10
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = int(thermalManager.scaledFanSpeed(0));                          //11
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = feedrate_percentage;                      //12
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  if (card.isPrinting)                     //13
  {
    strAll = strAll + "1|";
    lN = card.percentDone();
    sSend = String(lN);					//14
    strAll = strAll + sSend + "|";
  }
  else// if (!card.isPrinting)
  {
    strAll = strAll + "0|0|";
  }


  lN = active_extruder;                     //15
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = dual_x_carriage_mode;                //16
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  //lN=dual_x_carriage_mode;                //17 time
  if (card.isPrinting) {
    uint16_t time = millis() / 60000 - starttime / 60000;
    sSend = String(itostr2(time / 60)) + ":" + String(itostr2(time % 60));
    strAll = strAll + sSend + "|";
  }
  else {
    strAll = strAll + "00:00|";
  }

  if (card.isFileOpen()) {              //18 is file open
    strAll = strAll + "1|";
  }
  else {
    strAll = strAll + "0|";
  }

  if (thermalManager.isHeatingHotend(0)) {              //19 is heating nozzle 0
    strAll = strAll + "1|";
  }
  else {
    strAll = strAll + "0|";
  }

  if (thermalManager.isHeatingHotend(1)) {              //20 is heating nozzle 1
    strAll = strAll + "1|";
  }
  else {
    strAll = strAll + "0|";
  }

  if (thermalManager.isHeatingBed()) {              //21 is heating Bed
    strAll = strAll + "1|";
  }
  else {
    strAll = strAll + "0|";
  }

  strAll = strAll + "\"";
  const char* strAll0 = strAll.c_str();
  TenlogScreen_println(strAll0);
  delay(50);
  TenlogScreen_println("click btReflush,0");
}

void tenlog_status_screen(CardReader card)
{
  if (Tenlog_tenlog_status_update_delay)
    Tenlog_tenlog_status_update_delay--;
  else
    Tenlog_tenlogScreenUpdate = 1;
  if (Tenlog_tenlogScreenUpdate)
  {
    Tenlog_tenlogScreenUpdate = 0;
    tenlog_screen_update(card);
    Tenlog_tenlog_status_update_delay = 5000;   /* redraw the main screen every second. This is easier then trying keep track of all things that change on the screen */
  }
}

//Get Data From Commport
String getMYSERIAL1Data() {
  String strSerialdata = "";
  while (MSerial2_available() > 0) {
    strSerialdata += char(MSerial2_read());
    delay(2);
  }
  return strSerialdata;

}

void Init_TenlogScreen()
{
  _delay_ms(20);
  TenlogScreen_print("main.vLanguageID.val=");
  TenlogScreen_print(String(0).c_str());
  TenlogScreen_printend();
  _delay_ms(20);

  long iSend = X2_MAX_POS * 100.0;
  TenlogScreen_print("setting.xX2.val=");
  TenlogScreen_print(String(iSend).c_str());
  TenlogScreen_printend();
  _delay_ms(20);

  iSend = Y2_OFFSET * 100.0;
  TenlogScreen_print("setting.xY2.val=");
  TenlogScreen_print(String(iSend).c_str());
  TenlogScreen_printend();
  _delay_ms(20);

  iSend = Z2_OFFSET * 100.0;
  TenlogScreen_print("setting.xZ2.val=");
  TenlogScreen_print(String(iSend).c_str());
  TenlogScreen_printend();
  _delay_ms(20);

  iSend = axis_steps_per_unit[0] * 100.0;
  TenlogScreen_print("setting.xXs.val=");
  TenlogScreen_print(String(iSend).c_str());
  TenlogScreen_printend();
  _delay_ms(20);

  iSend = axis_steps_per_unit[1] * 100.0;
  TenlogScreen_print("setting.xYs.val=");
  TenlogScreen_print(String(iSend).c_str());
  TenlogScreen_printend();
  _delay_ms(20);

  iSend = axis_steps_per_unit[2] * 100.0;
  TenlogScreen_print("setting.xZs.val=");
  TenlogScreen_print(String(iSend).c_str());
  TenlogScreen_printend();
  _delay_ms(20);

  iSend = axis_steps_per_unit[3] * 100.0;
  TenlogScreen_print("setting.xEs.val=");
  TenlogScreen_print(String(iSend).c_str());
  TenlogScreen_printend();
  _delay_ms(20);

#ifdef FAN2_CONTROL
  iSend = FAN2_VALUE;
  TenlogScreen_print("setting.nF2s.val=");
  TenlogScreen_print(String(iSend).c_str());
  TenlogScreen_printend();
#else
  TenlogScreen_println("setting.nF2s.val=0");
#endif
  _delay_ms(20);

#ifdef FAN2_CONTROL
  iSend = FAN2_START_TEMP;
  TenlogScreen_print("setting.nF2t.val=");
  TenlogScreen_print(String(iSend).c_str());
  TenlogScreen_printend();
#else
  TenlogScreen_println("setting.nF2t.val=0");
#endif
  _delay_ms(20);

  iSend = X2_MAX_POS * 10.0;
  TenlogScreen_print("main.vXMax.val=");
  TenlogScreen_print(String(iSend).c_str());
  TenlogScreen_printend();
  _delay_ms(20);

  iSend = Y_MAX_POS * 10.0;
  TenlogScreen_print("main.vYMax.val=");
  TenlogScreen_print(String(iSend).c_str());
  TenlogScreen_printend();
  _delay_ms(20);

  iSend = Z_MAX_POS * 10.0;
  TenlogScreen_print("main.vZMax.val=");
  TenlogScreen_print(String(iSend).c_str());
  TenlogScreen_printend();
  _delay_ms(20);

#ifdef POWER_LOSS_TRIGGER_BY_PIN
  TenlogScreen_println("main.vPFR.val=1");
  TenlogScreen_printend();
  _delay_ms(20);

  iSend = AUTO_OFF;
  TenlogScreen_print("setting.cAutoOff.val=");
  TenlogScreen_print(String(iSend).c_str());
  TenlogScreen_printend();
  _delay_ms(20);
#endif //POWER_LOSS_TRIGGER_BY_PIN

  iSend = 0;// SLEEP_TIME
  TenlogScreen_print("setting.nSleep.val=");
  TenlogScreen_print(String(iSend).c_str());
  TenlogScreen_printend();
  _delay_ms(20);
  TenlogScreen_println("sleep=0");
  _delay_ms(20);
}

unsigned long lScreenStart = 0;
void CSDI_TLS()
{
  lScreenStart = millis();
  TenlogScreen_printEmptyend();
  TenlogScreen_println("connect");
  bool bCheckDone = false;
  while (!bCheckDone)
  {
    String strMYSERIAL1 = getMYSERIAL1Data();
    if (strMYSERIAL1 != "")
    {
      strMYSERIAL1.replace("\r", "");
      strMYSERIAL1.replace("\n", "");
      String strDI = getSplitValue(strMYSERIAL1, ',', 5);
      bCheckDone = true;
      TenlogScreen_print("loading.sDI.txt=\"");
      TenlogScreen_print(strDI.c_str());
      TenlogScreen_print("\"");
      TenlogScreen_printend();
      _delay_ms(20);
      TenlogScreen_println("click btA,0");
    }
    else {
      _delay_ms(10);
    }
    if (millis() - lScreenStart > 1000)
    {
      TenlogScreen_printEmptyend();
      TenlogScreen_println("connect");
      lScreenStart = millis();
    }
  }
}

void sdcard_tlcontroller()
{
  uint16_t fileCnt = card.get_num_Files();
  card.getWorkDirName();
  if (card.filename[0] == '/')
  {
  }
  else {
  }

  if (Tenlog_i_print_page_id == 0)
  {
    TenlogScreen_println("vis btUp,0");
    TenlogScreen_println("vis picUp,0");
  }
  else
  {
    TenlogScreen_println("vis btUp,1");
    TenlogScreen_println("vis picUp,1");
  }

  int iFileID = 0;
  //Clear the boxlist
  for (int i = 1; i < 7; i++)
  {
    TenlogScreen_print("select_file.tL");
    TenlogScreen_print(String(i).c_str());
    TenlogScreen_print(".txt=\"\"");
    TenlogScreen_printend();

    TenlogScreen_print("select_file.sL");
    TenlogScreen_print(String(i).c_str());
    TenlogScreen_print(".txt=\"\"");
    TenlogScreen_printend();
  }

  for (uint16_t i = 0; i < fileCnt; i++)
  {
    card.getfilename_sorted(fileCnt - 1 - i); //card.getfilename(i);   // card.getfilename(fileCnt-1-i); //By Zyf sort by time desc
    String strFN = String(card.filename);// + " | " + String(card.filename);

    if (strFN.length() > 0)
    {
      if (strISAscii(strFN))
      {
        strFN = String(card.longFilename);
        strFN.toLowerCase();
        String strLFN = strFN;
        iFileID++;
        if (iFileID >= (Tenlog_i_print_page_id) * 6 + 1 && iFileID <= (Tenlog_i_print_page_id + 1) * 6)
        {
          strFN = String(card.filename);
          strFN.toLowerCase();

          //DEBUG_PRINT_LN(strLFN);
          if (strLFN == "") strLFN = strFN;

          int iFTemp = iFileID - (Tenlog_i_print_page_id) * 6;
          TenlogScreen_print("select_file.tL");
          TenlogScreen_print(String(iFTemp).c_str());
          TenlogScreen_print(".txt=\"");
          strLFN.toLowerCase();
          TenlogScreen_print(strLFN.c_str());
          TenlogScreen_print("\"");
          TenlogScreen_printend();

          TenlogScreen_print("select_file.sL");
          TenlogScreen_print(String(iFTemp).c_str());
          TenlogScreen_print(".txt=\"");
          TenlogScreen_print(strFN.c_str());
          TenlogScreen_print("\"");
          TenlogScreen_printend();
          //DEBUG_PRINT_LN(strFN);
        }
        //MENU_ITEM(sdfile, MSG_CARD_MENU, card.filename, card.longFilename);
      }
    }
  }

  TenlogScreen_print("select_file.vPageID.val=");
  TenlogScreen_print(String(Tenlog_i_print_page_id).c_str());
  TenlogScreen_printend();

  if ((Tenlog_i_print_page_id + 1) * 6 >= iFileID)
  {
    TenlogScreen_println("vis btDown,0");
    TenlogScreen_println("vis picDown,0");
  }
  else
  {
    TenlogScreen_println("vis btDown,1");
    TenlogScreen_println("vis picDown,1");
  }
}


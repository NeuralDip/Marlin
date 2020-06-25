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

#ifdef TENLOG_LCD_CONTROLLER

#include "../../inc/MarlinConfigPre.h"


int i_print_page_id;
int tenlog_status_update_delay;
int tenlogScreenUpdate;


void TenlogScreen_begin(int boud) {
  Serial2.begin(boud);
}

char chrEnd = 0xFF;
void TenlogScreen_println(const char s[]) {
  Serial2.print(s);
  Serial2.write(chrEnd);
  Serial2.write(chrEnd);
  Serial2.write(chrEnd);
}

void TenlogScreen_print(const char s[]) {
  Serial2.print(s);
}


void TenlogScreen_printend() {
  Serial2.write(chrEnd);
  Serial2.write(chrEnd);
  Serial2.write(chrEnd);
}

void TenlogScreen_printEmptyend() {
  Serial2.write(0x00);
  Serial2.write(chrEnd);
  Serial2.write(chrEnd);
  Serial2.write(chrEnd);
}

bool MSerial2_available() {
  return Serial2.available();
}

char MSerial2_read() {
  return Serial2.read();
}

void tenlog_screen_update()
{
  String strAll = "main.sStatus.txt=\"";
  long lN = current_position[X_AXIS] * 10.0; //1
  String sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = current_position[Y_AXIS] * 10.0;     //2
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = current_position[Z_AXIS] * 10.0;     //3
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = current_position[E_AXIS] * 10.0;     //4
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = int(degTargetHotend(0) + 0.5);     //5
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = int(degHotend(0) + 0.5);           //6
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = int(degTargetHotend(1) + 0.5);     //7
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = int(degHotend(1) + 0.5);           //8
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = int(degTargetBed() + 0.5);         //9
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = int(degBed() + 0.5);               //10
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = fanSpeed;                          //11
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = feedmultiply;                      //12
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  if (card.sdprinting == 1)                     //13
  {
    strAll = strAll + "1|";
    lN = card.percentDone();
    sSend = String(lN);					//14
    strAll = strAll + sSend + "|";
  }
  else if (card.sdprinting == 0)
  {
    strAll = strAll + "0|0|";
  }
  else if (card.sdprinting == 2)
  {
    strAll = strAll + "2|0|";
  }

  lN = active_extruder;                     //15
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  lN = dual_x_carriage_mode;                //16
  sSend = String(lN);
  strAll = strAll + sSend + "|";

  //lN=dual_x_carriage_mode;                //17 time
  if (IS_SD_PRINTING) {
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

  if (isHeatingHotend(0)) {              //19 is heating nozzle 0
    strAll = strAll + "1|";
  }
  else {
    strAll = strAll + "0|";
  }

  if (isHeatingHotend(1)) {              //20 is heating nozzle 1
    strAll = strAll + "1|";
  }
  else {
    strAll = strAll + "0|";
  }

  if (isHeatingBed()) {              //21 is heating Bed
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

void tenlog_status_screen()
{
  if (tenlog_status_update_delay)
    tenlog_status_update_delay--;
  else
    tenlogScreenUpdate = 1;
  if (tenlogScreenUpdate)
  {
    tenlogScreenUpdate = 0;
    tenlog_screen_update();
    tenlog_status_update_delay = 5000;   /* redraw the main screen every second. This is easier then trying keep track of all things that change on the screen */
  }
}

//Get Data From Commport
String getSerial2Data() {
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
  TenlogScreen_print(String(languageID).c_str());
  TenlogScreen_printend();
  _delay_ms(20);

  long iSend = zyf_X2_MAX_POS * 100.0;
  TenlogScreen_print("setting.xX2.val=");
  TenlogScreen_print(String(iSend).c_str());
  TenlogScreen_printend();
  _delay_ms(20);

  iSend = zyf_Y2_OFFSET * 100.0;
  TenlogScreen_print("setting.xY2.val=");
  TenlogScreen_print(String(iSend).c_str());
  TenlogScreen_printend();
  _delay_ms(20);

  iSend = zyf_Z2_OFFSET * 100.0;
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
  iSend = zyf_FAN2_VALUE;
  TenlogScreen_print("setting.nF2s.val=");
  TenlogScreen_print(String(iSend).c_str());
  TenlogScreen_printend();
#else
  TenlogScreen_println("setting.nF2s.val=0");
#endif
  _delay_ms(20);

#ifdef FAN2_CONTROL
  iSend = zyf_FAN2_START_TEMP;
  TenlogScreen_print("setting.nF2t.val=");
  TenlogScreen_print(String(iSend).c_str());
  TenlogScreen_printend();
#else
  TenlogScreen_println("setting.nF2t.val=0");
#endif
  _delay_ms(20);

  iSend = zyf_X2_MAX_POS * 10.0;
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

  iSend = zyf_AUTO_OFF;
  TenlogScreen_print("setting.cAutoOff.val=");
  TenlogScreen_print(String(iSend).c_str());
  TenlogScreen_printend();
  _delay_ms(20);
#endif //POWER_LOSS_TRIGGER_BY_PIN

  iSend = zyf_SLEEP_TIME;
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
    String strSerial2 = getSerial2Data();
    if (strSerial2 != "")
    {
      strSerial2.replace("\r", "");
      strSerial2.replace("\n", "");
      String strDI = getSplitValue(strSerial2, ',', 5);
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
  uint16_t fileCnt = card.getnrfilenames();
  card.getWorkDirName();
  if (card.filename[0] == '/')
  {
  }
  else {
  }

  if (i_print_page_id == 0)
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
    card.getfilename(fileCnt - 1 - i); //card.getfilename(i);   // card.getfilename(fileCnt-1-i); //By Zyf sort by time desc
    String strFN = String(card.filename);// + " | " + String(card.filename);

    if (!card.filenameIsDir && strFN.length() > 0)
    {
      if (strISAscii(strFN))
      {
        strFN = String(card.longFilename);
        strFN.toLowerCase();
        String strLFN = strFN;
        iFileID++;
        if (iFileID >= (i_print_page_id) * 6 + 1 && iFileID <= (i_print_page_id + 1) * 6)
        {
          strFN = String(card.filename);
          strFN.toLowerCase();

          //ZYF_DEBUG_PRINT_LN(strLFN);
          if (strLFN == "") strLFN = strFN;

          int iFTemp = iFileID - (i_print_page_id) * 6;
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
          //ZYF_DEBUG_PRINT_LN(strFN);
        }
        //MENU_ITEM(sdfile, MSG_CARD_MENU, card.filename, card.longFilename);
      }
    }
  }

  TenlogScreen_print("select_file.vPageID.val=");
  TenlogScreen_print(String(i_print_page_id).c_str());
  TenlogScreen_printend();

  if ((i_print_page_id + 1) * 6 >= iFileID)
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

#endif // TENLOG_LCD_CONTROLLER

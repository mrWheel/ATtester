/*
***************************************************************************  
**  Program  : displayHelp (part of ATtester.ino)
**  Copyright (c) 2021 Willem Aandewiel 
**
**  TERMS OF USE: MIT License. See bottom of file.                                                            
***************************************************************************      
*/

void displayHelp(void)
{
  Serial.println(F("\r\nEnter command in the following form:"));
  Serial.println(F(" <ATcommand> | <OKreply> | <ERRORreply> | <Time Out>"));
  Serial.println(F("\r\nSpecial Commands:"));
  Serial.println(F(" - RESET    - resets the modem"));
  Serial.println(F(" - GETAPI   - sends a restAPI GET command to the server"));
  Serial.println(F(" - POSTAPI  - sends a restAPI POST command to the server"));
  Serial.println(F(" - HELP     - the help screen\r\n"));
}


/***************************************************************************
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the
* following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
* OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
* THE USE OR OTHER DEALINGS IN THE SOFTWARE.
* 
****************************************************************************
*/

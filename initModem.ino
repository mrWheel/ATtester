/*
***************************************************************************  
**  Program  : initModem (part of ATtester.ino)
**  Copyright (c) 2021 Willem Aandewiel 
**
**  TERMS OF USE: MIT License. See bottom of file.                                                            
***************************************************************************      
*/

void initModem()
{
  rc = sendAtCmnd("ATZ", 1000);
  rc = sendAtCmnd("ATI", 1000); 
  rc = sendAtCmnd("ATE1", 5000); 
  //-- test CPIN for READY
  rc = sendAtCmnd("AT+CPIN?", "READY", "ERROR", 2000, true);
  if (rc != 1)
  {
    rc = sendAtCmnd("AT+CPIN=\"0000\"", "READY", "ERROR", 2000, true);
  }
  rc = sendAtCmnd("AT+CFUN=1", 2000);
  //-- this depends on your SIM card ------vvvvvvvvvvvvvvv
  rc = sendAtCmnd("AT+CGDCONT=1, \"IP\", \"iot.tmobile.nl\"", 20000);  
  rc = sendAtCmnd("AT+CGDCONT?", 5000);
  rc = sendAtCmnd("AT+CMNB=1", 2000);
  rc = sendAtCmnd("AT+CNMP=2", 2000);
  rc = sendAtCmnd("AT+CGATT=1", 2000);
  rc = sendAtCmnd("AT+CNTPCID=1", 2000);
  rc = sendAtCmnd("AT+GCAP", 5000);
  rc = sendAtCmnd("AT+CSQ", 5000);
  rc = sendAtCmnd("AT+CGPADDR=1", 5000);

} // initModem();



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

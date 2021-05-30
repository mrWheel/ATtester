/*
***************************************************************************  
**  Program  : sendSMS (part of ATtester.ino)
**  Copyright (c) 2021 Willem Aandewiel 
**
**  TERMS OF USE: MIT License. See bottom of file.                                                            
***************************************************************************      
*/

/* THIS FUNCTION IS NOT TESTED, SO IT PROBABLY DOESN'T WORK */

void sendSMS()
{
  rc = sendAtCmnd("AT+CPMS=\"SM\",\"SM\",\"SM\"", "OK", "ERROR", 10000);
  showReturnCode(rc);
  rc = sendAtCmnd("AT+CMGF=1", "OK", "ERROR", 10000);
  showReturnCode(rc);
  rc = sendAtCmnd("AT+CMGF?", "OK", "ERROR", 10000); 
  showReturnCode(rc);
  rc = sendAtCmnd("AT+CPMS=\"Hello World!\"", "OK", "ERROR", 10000);
  showReturnCode(rc);
  if (rc != 1)  return;
  rc = sendAtCmnd("AT+CMGW=1,\"mem1\"", "OK", "ERROR", 10000);    
  showReturnCode(rc);
  if (rc != 1)  return;
  rc = sendAtCmnd("AT+CMGS=\"0653123456\"", "OK", "ERROR", 10000);  
  showReturnCode(rc);
  if (rc != 1)  return;
  rc = sendAtCmnd("AT+CMGF?", "OK", "ERROR", 10000);  
  showReturnCode(rc);

} //  sendSMS();



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

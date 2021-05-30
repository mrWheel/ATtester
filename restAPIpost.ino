/*
***************************************************************************  
**  Program  : restAPIpost (part of ATtester.ino)
**  Copyright (c) 2021 Willem Aandewiel 
**
**  TERMS OF USE: MIT License. See bottom of file.                                                            
***************************************************************************      
*/


//========================================================================
void postRestApiData() 
{
  char    myData[70] = {};
  int8_t  postState = 0, maxStates = 22;
  bool    errorState = false;

  Serial.println(F("postRestAPI .."));

  snprintf(replyBuffer, sizeof(replyBuffer), "AT+HTTPPARA=\"URL\",\"%s\"", _POSTURL_);

  snprintf(myData, sizeof(myData), "{\"simple\":\"json\"}");

  uint8_t myDataLen = strlen(myData);
  
  //sendAtCmnd("AT+SAPBR?", 1000);
  //sendAtCmnd("AT+SAPBR=1,1", 1000);
  //sendAtCmnd("AT+HTTPTERM", 1000);
  sendAtCmnd("AT+CSQ", "+CSQ:", "OK", 2000);
 
  while ( (postState < 90) && !errorState)
  {
    rc = 9;
    postState++;
    switch(postState)
    {
      case  1:  //sendAtCmnd("AT+SAPBR=1,1", 2000);
                break;
                
      case  2:  //sendAtCmnd("AT+HTTPTERM", 5000);
                break;

      case  3:  rc = sendAtCmnd("AT+HTTPINIT", 5000);
                if (rc == 0) postState = 90;
                break;

      case  4:  rc = sendAtCmnd(replyBuffer, 2000);
                break;

      case  5:  rc = sendAtCmnd("AT+HTTPPARA=\"CID\",1", 1000);
                break;
                
      case  6:  rc = sendAtCmnd("AT+HTTPPARA=\"CONTENT\",\"application/json\"", 1000);
    //case  6:  rc = sendAtCmnd("AT+HTTPPARA=\"CONTENT\",\"plain/text\"", 1000);
                break;

      case  7:  modemSS.print("AT+HTTPDATA=");
                modemSS.print(myDataLen);
                modemSS.println(",10000");
                rc = handleInFromSim("DOWNLOAD", "", 2000);  // wait for "DOWNLOAD"
                break;

      case  8:  modemSS.println(myData);
                //modemSS.flush();
                modemSS.print("\r\n");
                Serial.print("---> [");
                Serial.print(myData);
                Serial.println("]");
                handleInFromSim(1000);
                break;
                
      case  9:  delay(1000);
                rc = sendAtCmnd("AT+HTTPACTION=1", ",200,", ",40", 5000); // 0 = get, 1 = post
                if (rc == 0) { postState = 90; }
                break;

      case 10:  rc = sendAtCmnd("AT+HTTPREAD=0,500", "OK", "ERROR", 5000);
                break;
                
      default:  postState = 99;
    
    } // switch postState
    
    if (rc != 9)
    {
      showReturnCode(rc);
      if (rc == -1) errorState = true;
      handleInFromSim(100);
    }
    handleInFromSim(100);
    
  } // while postState && !errorState
  
  rc = sendAtCmnd("AT+HTTPTERM", 1000);
  
  if (errorState) Serial.println(F("\r\n==>> Error(s)..\r\n"));    
  else            Serial.println(F("\r\n==>> Done..\r\n"));
    
} //  postRestApiData()



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

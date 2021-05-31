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

  //getRestApiData();
  
  snprintf(replyBuffer, sizeof(replyBuffer), "AT+HTTPPARA=\"URL\",\"%s\"", _POSTURL_);

  snprintf(myData, sizeof(myData), "{\"simple\":\"json\"}");

  uint8_t myDataLen = strlen(myData);
  
  //-- not sure of the next commands are necessary
  sendAtCmnd("AT+HTTPREAD=0,500", 1000);
  sendAtCmnd("AT+SAPBR=1,1", 1000);
  sendAtCmnd("AT+HTTPTERM", 1000);
 
  while ( (postState < 90) && !errorState)
  {
    rc = 9;
    postState++;
    switch(postState)
    {
      case  1:  rc = sendAtCmnd("AT+HTTPINIT", 5000);
                if (rc == 0) postState = 90;
                break;

      case  2:  rc = sendAtCmnd(replyBuffer, 2000);
                break;

      case  3:  rc = sendAtCmnd("AT+HTTPPARA=\"CID\",1", 1000);
                break;
                
      case  4:  rc = sendAtCmnd("AT+HTTPPARA=\"CONTENT\",\"application/json\"", 1000);
    //case  4:  rc = sendAtCmnd("AT+HTTPPARA=\"CONTENT\",\"plain/text\"", 1000);
                break;

      case  5:  modemSS.print("AT+HTTPDATA=");
                modemSS.print(myDataLen);
                modemSS.println(",10000");
                rc = handleInFromSim("DOWNLOAD", "", 2000, true);  // wait for "DOWNLOAD"
                break;

      case  6:  modemSS.println(myData);
                modemSS.print("\r\n");
                Serial.print("---> [");
                Serial.print(myData);
                Serial.println("]");
                rc = handleInFromSim("OK", "ERROR", 5000, true);
                if (rc == 0) { postState = 90; }
                break;
                
      case  7:  delay(100);
                rc = sendAtCmnd("AT+HTTPACTION=1", ",200,", ",40", 5000, true); // 0 = get, 1 = post
                if (rc == 0) { postState = 90; }
                if (rc == 1)
                {
                  Serial.println();
                  Serial.println(F("============================================================"));
                  Serial.print(F("Data is send to and received by "));
                  Serial.println(_POSTURL_);
                  Serial.println(F("============================================================"));
                }
                break;

      case  8:  rc = sendAtCmnd("AT+HTTPREAD=0,500", "+HTTPREAD:", "ERROR", 5000);
                lastReplyLen = 0;
                handleInFromSim("OK", "ERROR", 1000, true);
                Serial.print(F("\r\nlastReply["));
                //-- only the first MAX_REPLY_BUFF chars are saved in lastReply
                Serial.print(lastReply);
                Serial.println(F("]"));
                break;
                
      default:  postState = 99;
    
    } // switch postState
    
    if (rc != 9)
    {
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

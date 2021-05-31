/*
***************************************************************************  
**  Program  : restAPIget (part of ATtester.ino)
**  Copyright (c) 2021 Willem Aandewiel 
**
**  TERMS OF USE: MIT License. See bottom of file.                                                            
***************************************************************************      
*/

//========================================================================
void getRestApiData() 
{
  int8_t  postState = 0, maxStates = 22;
  bool    errorState = false;

  Serial.println(F("getRestAPI .."));
  snprintf(replyBuffer, sizeof(replyBuffer), "AT+HTTPPARA=\"URL\",\"%s\"", _GETURL_);

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
      case  1:  rc = sendAtCmnd("AT+HTTPINIT", 2000);
                break;

      case  2:  rc = sendAtCmnd(replyBuffer, 2000);
                break;

      case  3:  rc = sendAtCmnd("AT+HTTPPARA=\"CID\",1", 1000);
                break;

      case  4:  rc = sendAtCmnd("AT+HTTPACTION=0", ",200,", ",60", 3000); // 0 = get, 1 = post
                //-- +HTTPACTION:0,601,0
                //--  An AT response code (601) for HTTP session start indicates that 
                //--  there is a network error. Make sure that the PDP context is setup
                //--  properly.
                //-- +HTTPACTION:0,200,4
                //--  Above HTTP GET request is sucessful and it returned 4 bytes.
                break;

      case  5:  rc = sendAtCmnd("AT+HTTPREAD=0,500", "+HTTPREAD:", "ERROR", 5000);
                lastReplyLen = 0;
                handleInFromSim("OK", "ERROR", 1000, true);
                Serial.print(F("\r\nlastReply["));
                //-- only the first MAX_REPLY_BUFF chars are saved and displayed
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
    
} //  getRestApiData()



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

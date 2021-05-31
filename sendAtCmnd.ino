/*
***************************************************************************  
**  Program  : sendAtCmnd (part of ATtester.ino)
**  Copyright (c) 2021 Willem Aandewiel 
**
**  TERMS OF USE: MIT License. See bottom of file.                                                            
***************************************************************************      
*/

//========================================================================
void showReturnCode(int8_t rc) 
{
  switch(rc)
  {
    case -1:  Serial.println(F(" (error state)")); 
              break;
    case  0:  Serial.println(F(" (command timed Out)")); 
              break;
    case  1:  Serial.println(F(" (true state)")); 
              break;
    default:  Serial.println(F(" (unknown state)")); 
              
  } // switch ..
  
} //  showReturnCode()


//========================================================================
int8_t sendAtCmnd(const char *cmnd, const char *trueString
                                  , const char *falseString
                                  , uint32_t wait4Response
                                  , bool showRC) 
//-- returns "1" if "trueString" condition
//--        "-1" if "falseString" condition
//--         "0" if timedOut
{
  uint32_t  waitTimer;
  char      cIn;
  lastReplyLen = 0;
  lastReply[lastReplyLen] = 0;
  
  waitTimer=millis()+wait4Response;
  Serial.print(F("---> ")); 
  Serial.println(cmnd);
  Serial.flush();
  modemSS.println(cmnd);
  modemSS.flush();
  Serial.print(F("<--- ")); 
  skipNewLine = true;
  
  while((int)(millis() - waitTimer) < 0)
  {
    while(modemSS.available())
    {
      waitTimer=millis()+wait4Response;
      cIn = modemSS.read();
      addChar2Ring(cIn);
      if ((cIn >= ' ')&&(lastReplyLen < MAX_REPLY_BUFF)) lastReply[lastReplyLen++] = cIn;
      if (cIn < ' ' && cIn != '\n'&& cIn != '\r') cIn = '?';
      if (cIn == '\n') 
      {
        if (!skipNewLine) Serial.println(); 
        skipNewLine = true;
        break;
      }
      if (cIn != '\r')  skipNewLine = false;
      Serial.print(cIn);
      if (strlen(trueString) > 0)
      {
        if (isStringInBuff(trueString)) 
        {
          modemSS.flush();
          lastReply[lastReplyLen-strlen(trueString)] = 0;
          if (showRC) showReturnCode(1);
          else        Serial.println();
          return 1;
        }
      }
      if (strlen(falseString) > 0)
      {
        if (isStringInBuff(falseString)) 
        {
          modemSS.flush();
          lastReply[lastReplyLen-strlen(falseString)] = 0;
          if (showRC) showReturnCode(-1);
          else        Serial.println();
          return -1;
        }
      }
    } //  while chars available from sim7000
  }

  modemSS.flush();
  lastReply[lastReplyLen] = 0;
  if (showRC) showReturnCode(0);
  else        Serial.println();

  return 0;
  
} //  sendAtCmnt()


//========================================================================
int8_t sendAtCmnd(const char *cmnd, const char *trueString
                                  , const char *falseString
                                  , uint32_t wait4Response)
//-- returns "1" if "trueString" condition
//--        "-1" if "falseString" condition
//--         "0" if timedOut
{
  return sendAtCmnd(cmnd, trueString, falseString, wait4Response, false);
  
} //  sendAtCmnd(char*, char*, char*, uint32_t)


//========================================================================
int8_t sendAtCmnd(const char *cmnd, uint32_t wait4Response, bool showRC) 
//-- returns "1" if "trueString" condition
//--        "-1" if "falseString" condition
//--         "0" if timedOut
{
  return sendAtCmnd(cmnd, "OK", "ERROR", wait4Response, showRC);
  
} //  sendAtCmnd(cmd, timeout, wait, showRC);


//========================================================================
int8_t sendAtCmnd(const char *cmnd, uint32_t wait4Response) 
//-- returns "1" if "trueString" condition
//--        "-1" if "falseString" condition
//--         "0" if timedOut
{
  return sendAtCmnd(cmnd, "OK", "ERROR", wait4Response, false);
  
} //  sendAtCmnd(cmd, timeout, wait)


//========================================================================
int8_t splitAndSendAtCmnd(char *cArray) 
{
  char      cmnd[40] = {};
  char      cArg2[MAX_RING_SIZE] = {};
  char      cArg3[MAX_RING_SIZE] = {};
  char      cArg4[6]  = {};
  uint8_t   pos = 0, mode = 0;
  uint16_t  iTime;

  for(int i=0; i<strlen(cArray); i++)
  {
    if (cArray[i] == '|')
    {
      pos = 0;
      mode++;
    }
    else
    {
      switch(mode)
      {
        case 0: if ((pos < sizeof(cmnd)-1) && cArray[i] != ' ')
                { 
                  cmnd[pos++]  = cArray[i];  
                  cmnd[pos]    = 0;
                }
                break;
        case 1: if ((pos < sizeof(cArg2)-1) && cArray[i] != ' ')
                {
                  cArg2[pos++] = cArray[i]; 
                  cArg2[pos]   = 0; 
                }
                break;
        case 2: if ((pos < sizeof(cArg3)-1) && cArray[i] != ' ') 
                {
                  cArg3[pos++] = cArray[i];  
                  cArg3[pos]   = 0;
                }
                break;
        case 3: if ((pos < sizeof(cArg4)-1) && cArray[i] != ' ')  
                {
                  cArg4[pos++]  = cArray[i];
                  cArg4[pos]    = 0;  
                }
                break;
      } //  switch(mode)
    }
  } //  for ...
  if (strlen(cArg4) == 0)   { strcpy(cArg4, "1000"); }
  if (strlen(cArg3) == 0)   { strcpy(cArg3, "ERROR"); }
  else if (atoi(cArg3) > 0) { strcpy(cArg4, cArg3); strcpy(cArg3, "ERROR"); }
  if (strlen(cArg2) == 0)   { strcpy(cArg2, "OK"); }
  else if (atoi(cArg2) > 0) { strcpy(cArg4, cArg2); strcpy(cArg2, "OK"); }
  iTime = atoi(cArg4);
  return sendAtCmnd(cmnd, cArg2, cArg3, iTime, true);
  
} //  splitAndSendAtCmnd()



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

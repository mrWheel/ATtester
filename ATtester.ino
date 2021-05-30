/*
***************************************************************************  
**  Program  : ATtester (modem tester)
*/
#define _FW_VERSION "v1.0 (30-05-2021)"
/*
**  Copyright (c) 2021 Willem Aandewiel
**
**  TERMS OF USE: MIT License. See bottom of file.                                                            
***************************************************************************      
*/

#include "ATtester.h"

//========================================================================
void showReturnCode(int8_t rc) 
{
  switch(rc)
  {
    case -1:  Serial.println(F("(error state)")); 
              break;
    case  0:  Serial.println(F("(command timed Out)")); 
              break;
    case  1:  Serial.println(F("(true state)")); 
              break;
    default:  Serial.println(F("(unknown state)")); 
              
  } // switch ..
  
} //  showReturnCode()


//========================================================================
void setBaud9600() 
{
  uint32_t  waitTimer;

  Serial.println(F("Set Baudrate modem.."));

  Serial.println(F("test 115200 .."));
  modemSS.begin(115200);
  //sendAtCmnd("ATE1", "OK", "", 20000);
  rc = sendAtCmnd("AT+IPR?", "OK", "", 1000); // test baud rate
  showReturnCode(rc);
  if (rc == 1)
  {
    Serial.println(F("its 115200 ..!"));
    rc = sendAtCmnd("AT+IPR=9600", 1000);
  }
  
  Serial.println(F("test 9600 .."));
  modemSS.begin(9600);
  //sendAtCmnd("ATE1", "OK", "", 1000);
  rc = sendAtCmnd("AT+IPR?", "OK", "", 1000); // test baud rate
  showReturnCode(rc);
  if (rc == 1)
  {
    Serial.println(F("its 9600 ..!"));
  }

} //  setBaud9600()


//========================================================================
void resetModem() 
{
  uint32_t  waitTimer;

  Serial.println(F("Reset Modem .."));
  
  digitalWrite(MODEM_DTR, LOW);
  digitalWrite(MODEM_RST, HIGH);
  delay(500);
  digitalWrite(MODEM_RST, LOW);

  setBaud9600();

  rc = sendAtCmnd("AT+CFUN=1,1", "RDY", "", 10000);  // reset board?
  showReturnCode(rc);

  handleInFromSim(1000) ;

  Serial.println("\r\nSET PIN ..");
  rc = sendAtCmnd("AT+CPIN?", "READY", "ERROR", 10000);  // Set PIN code
  showReturnCode(rc);
  if (rc != 1)
  {
    rc = sendAtCmnd("AT+CPIN=\"0000\"", "READY", "ERROR", 10000);  // Set PIN code
    showReturnCode(rc);
  }
  
  handleInFromSim(1000);

  Serial.println(F("\r\nContinue..\r\n"));
  
} //  resetModem()


//========================================================================
//-- returns "1" on "isTrue" condition
//--        "-1" on "isFalse" condition
//--         "0" on time-Out
int8_t  handleInFromSim(const char *isTrue, const char *isFalse, uint32_t maxWait) 
{
  uint32_t waitFor = millis()+maxWait;

  while((int)(millis()-waitFor) < 0)
  {
    while (modemSS.available())
    {
      //waitFor = millis()+50;
      char c=modemSS.read();
      if (c == '\r')  addChar2Ring(c); 
      if (c >= ' ')  
      {
        Serial.print(c); 
        addChar2Ring(c); 
        skipNewLine = false;
      }
      if (c == '\n') { skipNewLine = true; Serial.println(); }
      if (strlen(isTrue) > 0)
      {
        if (isStringInBuff(isTrue)) 
        {
          modemSS.flush();
          handleInFromSim();
          //Serial.flush();
          return 1;
        }
      }
      if (strlen(isFalse) > 0)
      {
        if (isStringInBuff(isFalse)) 
        {
          modemSS.flush();
          handleInFromSim();
          //Serial.flush();
          return -1;
        }
      }

    }
  }
  handleInFromSim();
  return 0;
  
} //  handleInFromSim()


//========================================================================
void handleInFromSim(uint32_t maxWait) 
{
  uint32_t waitFor = millis()+maxWait;

  while((int)(millis()-waitFor) < 0)
  {
    while (modemSS.available())
    {
      //waitFor = millis()+50;
      char c=modemSS.read();
      if (c == '\r')  addChar2Ring(c); 
      if (c >= ' ')  
      {
        Serial.print(c); 
        addChar2Ring(c); 
        skipNewLine = false;
      }
      if (c == '\n') { skipNewLine = true; Serial.println(); }
    }
  }
  
} //  handleInFromSim()


//========================================================================
void handleInFromSim() 
{
  handleInFromSim(50);
  
} //  handleInFromSim()


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
  return sendAtCmnd(cmnd, cArg2, cArg3, iTime);
  
} //  splitAndSendAtCmnd()


//========================================================================
int8_t removerTrailingCntrChar(char *cArray) 
{
  for(int i=strlen(cArray); i>=0; i--)
  {
    if (cArray[i] <= ' ') 
    {
      cArray[i] = 0;
    }
    else return strlen(cArray);
  }

  return strlen(cArray);

} //  removerTrailingCntrChars()


//========================================================================
int8_t sendAtCmnd(const char *cmnd, const char *isTrue, const char *isFalse, uint32_t wait4Response = 0) 
//-- returns "1" if "isTrue" condition
//--        "-1" if "isFalse" condition
//--         "0" if timedOut
{
  uint32_t  waitTimer;
  char      cIn;

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
      if (cIn < ' ' && cIn != '\n'&& cIn != '\r') cIn = '?';
      if (cIn == '\n') 
      {
        if (!skipNewLine) Serial.println(); 
        skipNewLine = true;
        break;
      }
      if (cIn != '\r')  skipNewLine = false;
      Serial.print(cIn);
      //Serial.flush();
      if (strlen(isTrue) > 0)
      {
        if (isStringInBuff(isTrue)) 
        {
          modemSS.flush();
          handleInFromSim();
          //Serial.println();
          //Serial.flush();
          return 1;
        }
      }
      if (strlen(isFalse) > 0)
      {
        if (isStringInBuff(isFalse)) 
        {
          modemSS.flush();
          handleInFromSim();
          //Serial.println();
          //Serial.flush();
          return -1;
        }
      }
    } //  while chars available from sim7000
  }

  modemSS.flush();
  handleInFromSim();
  //Serial.println();
  //Serial.flush();
  return 0;
  
} //  sendAtCmnt()


//========================================================================
int8_t sendAtCmnd(const char *cmnd, uint32_t wait4Response = 0) 
//-- returns "1" if "isTrue" condition
//--        "-1" if "isFalse" condition
//--         "0" if timedOut
{
  return sendAtCmnd(cmnd, "OK", "ERROR", wait4Response);
  
} //  sendAtCmnd(cmd, timeout);


//========================================================================
void setup() 
{
  Serial.begin(38400);
  while(!Serial) { delay(100); }
  Serial.println(F("\r\nAnd than it begins ...\r\n"));
  
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_DTR, OUTPUT);

  digitalWrite(MODEM_DTR, HIGH);
  delay(100);
  digitalWrite(MODEM_DTR, LOW);

  Serial.println(F("Initializing....(May take 5 seconds)"));
  Serial.flush();

  //testRingBuffer();
  
  setBaud9600();

  initModem();
  
  //sendAtCmnd("ATI", 500);
  //sendAtCmnd("AT+IPR=9600", 500);

  displayHelp();
  Serial.println("Now enter commands in the top input field..\r\n");
  
} //  setup()


//========================================================================
void loop() 
{
  while (Serial.available())
  {
    memset(replyBuffer, 0, sizeof(replyBuffer));
    int l = Serial.readBytesUntil('\n', replyBuffer, MAX_REPLY_BUFF);

    if (strncasecmp(replyBuffer, "HELP", 4)==0)
    {
      displayHelp();
      break;
    }

    if (strncasecmp(replyBuffer, "RESET", 5)==0)
    {
      Serial.print("===> ");
      setBaud9600();
      Serial.print("===> ");
      resetModem();
      return;
    }
    if (strncasecmp(replyBuffer, "GETAPI", 6)==0)
    {
      Serial.println("===> GETAPI");
      getRestApiData();
      break;
    }
    if (strncasecmp(replyBuffer, "POSTAPI", 7)==0)
    {
      Serial.println("===> POSTAPI");
      postRestApiData();
      break;
    }
    if (strncasecmp(replyBuffer, "SMS", 3)==0)
    {
      Serial.println("===> sendSMS");
      sendSMS();
      resetModem();
      break;
    }

    while (l>=MAX_REPLY_BUFF) 
    {
      Serial.print(F("---> ")); 
      modemSS.print(replyBuffer);
      Serial.print(F("---> ")); 
      Serial.println(replyBuffer);
      memset(replyBuffer, 0, MAX_REPLY_BUFF);
      l = Serial.readBytesUntil('\n', replyBuffer, MAX_REPLY_BUFF);
    }
    l = removerTrailingCntrChar(replyBuffer);
    if (l>0) 
    {
      rc = splitAndSendAtCmnd(replyBuffer); 
      //rc = sendAtCmnd(replyBuffer, "OK", "ERROR", 10000);
      //rc = sendAtCmnd(replyBuffer, 10000);
      showReturnCode(rc);
    }
    else 
    {
      displayHelp();
      rc = sendAtCmnd("AT", "OK", "", 5000);
      showReturnCode(rc);
    }

  }

  handleInFromSim();

} //  loop()



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

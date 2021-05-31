/*
***************************************************************************  
**  Program  : ATtester (modem tester)
*/
#define _FW_VERSION "v1.1 (31-05-2021)"
/*
**  Copyright (c) 2021 Willem Aandewiel
**
**  TERMS OF USE: MIT License. See bottom of file.                                                            
***************************************************************************      
*/

#include "main.h"


//========================================================================
void setBaud9600() 
{
  uint32_t  waitTimer;

  Serial.println(F("Set Baudrate modem.."));

  Serial.println(F("test 115200 .."));
  modemSS.begin(115200);
  //-- if the next command times-out it was the wrong baudrate
  rc = sendAtCmnd("AT+IPR?", "OK", "", 1000, true); // test baud rate
  if (rc == 1)
  {
    Serial.println(F("its 115200 ..!"));
    //-- change buadrate to 9600bps
    rc = sendAtCmnd("AT+IPR=9600", 1000, true);
  }
  
  Serial.println(F("test 9600 .."));
  modemSS.begin(9600);
  //-- if the next command times-out we have a problem..
  rc = sendAtCmnd("AT+IPR?", "OK", "", 1000, true); // test baud rate
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

  rc = sendAtCmnd("AT+CFUN=1,1", "RDY", "", 10000, true);  // reset board?

  handleInFromSim(1000) ;

  Serial.println("\r\nSET PIN ..");
  rc = sendAtCmnd("AT+CPIN?", "READY", "ERROR", 10000, true);  // Set PIN code
  if (rc != 1)
  {
    rc = sendAtCmnd("AT+CPIN=\"0000\"", "READY", "ERROR", 10000, true);  // Set PIN code
  }
  
  handleInFromSim(1000u);

  Serial.println(F("\r\nContinue..\r\n"));
  
} //  resetModem()


//========================================================================
//-- returns "1" on "trueString" condition
//--        "-1" on "falseString" condition
//--         "0" on time-Out
int8_t  handleInFromSim(const char *trueString, const char *falseString, int32_t maxWait, bool showRC) 
{
  uint32_t waitFor = millis()+maxWait;

  while((int)(millis()-waitFor) < 0)
  {
    while (modemSS.available())
    {
      char c=modemSS.read();
      if (c == '\r')  addChar2Ring(c); 
      if (c >= ' ')  
      {
        Serial.print(c); 
        addChar2Ring(c); 
        if (lastReplyLen < MAX_REPLY_BUFF) lastReply[lastReplyLen++] = c;
        skipNewLine = false;
      }
      if (c == '\n') { skipNewLine = true; Serial.println(); }
      if (strlen(trueString) > 0)
      {
        if (isStringInBuff(trueString)) 
        {
          modemSS.flush();
          if (showRC) showReturnCode(1);
          else        Serial.println();
          //Serial.flush();
          lastReply[lastReplyLen-strlen(trueString)] = 0;
          return 1;
        }
      }
      if (strlen(falseString) > 0)
      {
        if (isStringInBuff(falseString)) 
        {
          modemSS.flush();
          if (showRC) showReturnCode(-1);
          else        Serial.println();
          lastReply[lastReplyLen-strlen(falseString)] = 0;
          return -1;
        }
      }

    }
  }

  lastReply[lastReplyLen] = 0;
  if (showRC) showReturnCode(0);
  else        Serial.println();
  
  return 0;
  
} //  handleInFromSim(char*, char*, int, bool)


//========================================================================
//-- returns "1" on "trueString" condition
//--        "-1" on "falseString" condition
//--         "0" on time-Out
int8_t  handleInFromSim(const char *trueString, const char *falseString, int32_t maxWait) 
{
  return handleInFromSim(trueString, falseString, maxWait, false);
  
} //  handleInFromSim(char*, char*, int)


//========================================================================
int8_t handleInFromSim(int32_t maxWait) 
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
        if (lastReplyLen < MAX_REPLY_BUFF) lastReply[lastReplyLen++] = c;
        skipNewLine = false;
      }
      if (c == '\n') { skipNewLine = true; Serial.println(); }
    }
  }
  lastReply[lastReplyLen] = 0;
  
} //  handleInFromSim(int)


//========================================================================
int8_t handleInFromSim() 
{
  return handleInFromSim(50);
  
} //  handleInFromSim()


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
void setup() 
{
  Serial.begin(38400);
  while(!Serial) { delay(100); }
  Serial.println(F("\r\nAnd than it begins ...\r\n"));
  Serial.print(F("Firmware ")); Serial.println(_FW_VERSION);
  
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_DTR, OUTPUT);

  digitalWrite(MODEM_DTR, HIGH);
  delay(100);
  digitalWrite(MODEM_DTR, LOW);

  Serial.println(F("\r\nInitializing....(May take 5 seconds)"));
  Serial.flush();

  //testRingBuffer();
  
  setBaud9600();

  initModem();

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
    }
    else 
    {
      displayHelp();
      rc = sendAtCmnd("AT", "OK", "", 5000, true);
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

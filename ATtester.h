/*
***************************************************************************  
**  Program  : ATtester.h (modem tester header file)
**  Copyright (c) 2021 Willem Aandewiel 
**
**  TERMS OF USE: MIT License. See bottom of file.                                                            
***************************************************************************      
*/

#define CTRL(x) (#x[0]-'a'+1)

#define _GETURL_      "http://httpbin.org/get"
#define _POSTURL_     "http://httpbin.org/post"

//-- minimal the longest AT command+params
#define MAX_REPLY_BUFF  100
#define MAX_TRUE_BUFF  100

//-- this is a large buffer for replies
char    replyBuffer[MAX_REPLY_BUFF+1];
char    lastReply[MAX_REPLY_BUFF+1];
char    trueBuffer[MAX_TRUE_BUFF+1];

int8_t  rc;    //-- global replyCode for sendAtCmnd()
bool    skipNewLine = false;
int8_t  sendAtCmnd(const char*, const char*, const char*, uint32_t); 
int8_t  sendAtCmnd(const char*, uint32_t) ;
void    showReturnCode(int8_t);
void    resetModem();


// AltSoftSerial always uses these pins:
//
// Board          Transmit  Receive   PWM Unusable
// -----          --------  -------   ------------
// Arduino Uno        9         8         10
// Arduino Leonardo   5        13       (none)
// Arduino Mega      46        48       44, 45

#define MODEM_TX   4 // Microcontroller RX
#define MODEM_RX   3 // Microcontroller TX
#define MODEM_RST  7
#define MODEM_DTR  8  // HIGH = enter SleepMode, LOW = disable Sleep Mode

uint32_t  startupTimer, waitTimer;
bool      isComment   = false;
bool      isCommand   = false;
bool      isTrueCond  = false;

//-- on UNO only on pin 9(TX) & 8(RX)
// #include <AltSoftSerial.h>
// AltSoftSerial modemSS;

#include <SoftwareSerial.h>
SoftwareSerial modemSS = SoftwareSerial(MODEM_TX, MODEM_RX);

bool    lastStatus;


#define MAX_RING_SIZE 15
char    ringBuffer[MAX_RING_SIZE];
uint8_t ringLastIn, ringNextPointer = 0;


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

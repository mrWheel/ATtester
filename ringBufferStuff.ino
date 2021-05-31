/*
***************************************************************************  
**  Program  : ringBufferStuff (part of ATtester.ino)
**  Copyright (c) 2021 Willem Aandewiel 
**
**  TERMS OF USE: MIT License. See bottom of file.                                                            
***************************************************************************      
*/


//========================================================================
void addChar2Ring(char c) 
{
  uint8_t tmpPointer = 0;

  //-- put char in actual ringNextPointer Position
  ringBuffer[ringNextPointer] = toupper(c);
  //-- save ringNextPointer
  ringLastIn  = ringNextPointer;
  //-- calculate new ringNextPointer
  ringNextPointer = ((ringNextPointer+1)%MAX_RING_SIZE);
  ringBuffer[ringNextPointer] = '*';
  
} //  addChar2Ring()


//========================================================================
bool isStringInBuff(const char *testString) 
{
  int8_t  sLen = strlen(testString);
  int8_t  bPos = ringLastIn-sLen;  

  if (bPos<0) bPos = MAX_RING_SIZE+bPos;
  if (sLen >= MAX_RING_SIZE) return false;

  //--                ringLastIn---|
  //--              0  1  2  3  4  V  6  7  8 
  //-- ringBuffer: [a][b][c][d][O][K][g][h][i][0]
  //--                       0  1  2  3
  //-- testString           [x][O][K][0]
  for (int t=0; t<sLen;t++)
  {
    if (toupper(ringBuffer[(bPos+t)%MAX_RING_SIZE]) != toupper(testString[t]))
    {
      return false;
    }
  }
  return true;
  
} //  isStringInBuff()


//========================================================================
bool tmpShowBuffer() 
{
  Serial.print(F("        "));
  for(int i=0; i<MAX_RING_SIZE; i++)
  {
    if (i==ringLastIn)
          Serial.print('V');
    else  Serial.print("-");
  }
  Serial.println();
  Serial.print(F("p[")); Serial.print(ringNextPointer); Serial.print(F("] -> "));
  for(int i=0; i<MAX_RING_SIZE; i++)
  {
    if (ringBuffer[i] < ' ')
          Serial.print('!');
    else  Serial.print(ringBuffer[i]);
  }
  Serial.println();

} //  tmpShowBuffer()


//========================================================================
bool testRingBuffer() 
{
  Serial.println(F("\r\ntestRingBuffer functions..\r\n"));
  
  addChar2Ring('a');
  tmpShowBuffer();
  Serial.print(F("test for [OK]    >>"));
  if (isStringInBuff("OK"))     Serial.println(F(" found OK!"));
  else                          Serial.println();
  addChar2Ring('b');
  addChar2Ring('c');
  addChar2Ring('d');
  addChar2Ring('e');
  addChar2Ring('f');
  addChar2Ring('g');
  addChar2Ring('h');
  addChar2Ring('i');
  addChar2Ring('j');
  addChar2Ring('k');
  addChar2Ring('l');
  isStringInBuff("OK");
  addChar2Ring('O');
  isStringInBuff("OK");
  addChar2Ring('K');
  tmpShowBuffer();
  Serial.print(F("test for [ok]    >>"));
  if (isStringInBuff("ok"))     Serial.println(F(" found OK!"));
  else                          Serial.println();
  addChar2Ring('\r');
  tmpShowBuffer();
  Serial.print(F("test for [ok]    >>"));
  if (isStringInBuff("ok"))     Serial.println(F(" found OK!"));
  else                          Serial.println();
  addChar2Ring('\r');
  tmpShowBuffer();
  Serial.print(F("test for [ok]    >>"));
  if (isStringInBuff("ok"))     Serial.println(F(" found OK!"));
  else                          Serial.println();

  addChar2Ring('t');
  addChar2Ring('e');
  addChar2Ring('s');
  addChar2Ring('t');
  addChar2Ring(' ');
  addChar2Ring('E');
  addChar2Ring('R');
  addChar2Ring('R');
  addChar2Ring('O');
  addChar2Ring('R');
  tmpShowBuffer();
  Serial.print(F("test for [ERROR] >>"));
  if (isStringInBuff("ERROR"))  Serial.println(F(" found ERROR!"));
  else                          Serial.println();
  Serial.print(F("test for [OK]    >>"));
  if (isStringInBuff("OK"))     Serial.println(F(" found OK!"));
  else                          Serial.println();
  addChar2Ring('\r');
  tmpShowBuffer();
  Serial.print(F("test for [ERROR] >>"));
  if (isStringInBuff("ERROR"))  Serial.println(F(" found ERROR!"));
  else                          Serial.println();
  Serial.print(F("test for [OK]    >>"));
  if (isStringInBuff("OK"))     Serial.println(F(" found OK!"));
  else                          Serial.println();

  Serial.println(F("Continue normal operation ..\r\n"));
  
} //  testRingBuffer()



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

# AT Tester #

This program is a simple "challenge-response" sketch.

It is setup to communicate to a modem over a SoftwareSerial line at 9600bps.

The sketch is extensively tested with a SIM7000 modem.

There are two example functions to send restAPI requests to a remote server.
The functions `restAPIget` and `restAPIpost` are located in the tab's with the same name.
You can execute the functions by entering `getapi` or `postapi` in the command prompt
(the input line in the Arduino Serial Monitor).

You have to tailer these functions for your specific needs.

## getAPI ##
<pre>
===> GETAPI
getRestAPI ..
---> AT+CSQ
<--- AT+CSQ
+CSQ: 24,99
OK
---> AT+SAPBR=1,1
<--- AT+SAPBR=1,1
ERROR
---> AT+HTTPTERM
<--- AT+HTTPTERM
ERROR
---> AT+HTTPINIT
<--- AT+HTTPINIT
OK (true state)
---> AT+HTTPPARA="URL","http://httpbin.org/get"
<--- AT+HTTPPARA="URL","http://httpbin.org/get"
OK (true state)
---> AT+HTTPPARA="CID",1
<--- AT+HTTPPARA="CID",1
OK (true state)
---> AT+HTTPACTION=0
<--- AT+HTTPACTION=0
OK
+HTTPACTION: 0,200,255 (true state)
---> AT+HTTPREAD=0,500
<--- AT+HTTPREAD=0,500
+HTTPREAD: 255
{
  "args": {},
  "headers": {
    "Accept": "*/*",
    "Host": "httpbin.org",
    "User-Agent": "SIMCOM_MODULE",
    "X-Amzn-Trace-Id": "Root=1-60b3633d-4a9d49f801e25839122b33bb"
  },
  "origin": "18.197.48.88",
  "url": "http://httpbin.org/get"
}
OK (true state)
---> AT+HTTPTERM
<--- AT+HTTPTERM
OK
==>> Done..
</pre>

## postAPI ##
<pre>
===> POSTAPI
postRestAPI ..
---> AT+CSQ
<--- AT+CSQ
+CSQ: 24,99
OK
---> AT+HTTPINIT
<--- AT+HTTPINIT
OK (true state)
---> AT+HTTPPARA="URL","http://httpbin.org/post"
<--- AT+HTTPPARA="URL","http://httpbin.org/post"
OK (true state)
---> AT+HTTPPARA="CID",1
<--- AT+HTTPPARA="CID",1
OK (true state)
---> AT+HTTPPARA="CONTENT","application/json"
<--- AT+HTTPPARA="CONTENT","application/json"
OK (true state)
AT+HTTPDATA=17,10000
DOWNLOAD
(true state)
---> [{"simple":"json"}]
OK
---> AT+HTTPACTION=1
<--- AT+HTTPACTION=1
OK
+HTTPACTION: 1,200,432 (true state)
---> AT+HTTPREAD=0,500
<--- AT+HTTPREAD=0,500
+HTTPREAD: 432

{
  "args": {},
  "data": "{\"simple\":\"json\"}",
  "files": {},
  "form": {},
  "headers": {
    "Accept": "*/*",
    "Content-Length": "17",
    "Content-Type": "application/json",
    "Host": "httpbin.org",
    "User-Agent": "SIMCOM_MODULE",
    "X-Amzn-Trace-Id": "Root=1-60b3627f-40a256736a4e20064daaeecf"
  },
  "json": {
   "simle" "jso"
  }
  "rigin: "18197.8.88"
  "ul": "htt://htpbin.og/pos"
}
OK (true state)
---> AT+HTTPTERM
<--- AT+HTTPTERM
OK
==>> Done..
</pre>

The following functions handle the "challenge-response" and can be used for your
specific needs:

## sendAtCmnd(&lt;ATcommand&gt;) ##
This function sends the string `ATcommand` to the modem and waits for a maximum of
1000ms for a response.
If the modem sends back the string "OK" the function exits with a returncode "1".
If the modem sends back the string "ERROR" the function exits with a returncode "-1".
If the modem does not send a response the function times-out (returncode "0").

## sendAtCmnd(&lt;ATcommand&gt;, &lt;waitTime&gt;) ##
This function sends the string `ATcommand` to the modem and waits for a maximum of
`waitTime`ms for a response.
If the modem sends back the string "OK" the function exits with a returncode "1".
If the modem sends back the string "ERROR" the function exits with a returncode "-1".
If the modem does not send a response the function times-out (returncode "0").

## sendAtCmnd(&lt;ATcommand&gt;, &lt;OKstring&gt;, &lt;ERRORstring&gt;, &lt;waitTime&gt;) ##
This function sends the string `ATcommand` to the modem and waits for a maximum of
`waitTime`ms for a response.
If the modem sends back the string `OKstring` the function exits with a returncode "1".
If the modem sends back the string `ERRORstring` the function exits with a returncode "-1".
If the modem does not send a response the function times-out (returncode "0").

## handleInFromSim() ##
This function checks for chars send by the modem and prints them.

## handleInFromSim(&lt;waitTime&gt;) ##
This function waits `waitTime`ms for chars send by the modem and prints them.

## handleInFromSim(&lt;OKstring&gt;, &lt;ERRORstring&gt;, &lt;waitTime&gt;) ##
This function checks `waitTime`ms for chars send by the modem and prints them.
if the modem sends the `OKstring` the function exits with returncode "1".
if the modem sends the `ERRORstring` the function exits with returncode "-1".
If neither is received before `waitTime`ms the function exits with returncode "0".


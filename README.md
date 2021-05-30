*AT Tester*

This program is a simple "challenge-response" sketch.

It is setup to communicate to a modem over a SoftwareSerial line at 9600bps.

There are two example functions to send restAPI requests to a remote server.
The functions `restAPIget` and `restAPIpost` are located in the tab's with the same name.
You can execute the functions by entering `getapi` or `postapi` in the command prompt
(the input line in the Arduino Serial Monitor).

You have to tailer these functions for your specific needs.

The following functions handle the "challenge-response" and can be used for your
specific needs:

**sendAtCmnd(&lt;ATcommand&gt;)**
This function sends the string `ATcommand` to the modem and waits for a maximum of
1000ms for a response.
If the modem sends back the string "OK" the function exits with a returncode "1".
If the modem sends back the string "ERROR" the function exits with a returncode "-1".
If the modem does not send a response the function times-out (returncode "0").

**sendAtCmnd(&lt;ATcommand&gt;, &lt;waitTime&gt;)**
This function sends the string `ATcommand` to the modem and waits for a maximum of
`waitTime`ms for a response.
If the modem sends back the string "OK" the function exits with a returncode "1".
If the modem sends back the string "ERROR" the function exits with a returncode "-1".
If the modem does not send a response the function times-out (returncode "0").

**sendAtCmnd(&lt;ATcommand&gt;, &lt;OKstring&gt;, &lt;ERRORstring&gt;, &lt;waitTime&gt;)**
This function sends the string `ATcommand` to the modem and waits for a maximum of
`waitTime`ms for a response.
If the modem sends back the string `OKstring` the function exits with a returncode "1".
If the modem sends back the string `ERRORstring` the function exits with a returncode "-1".
If the modem does not send a response the function times-out (returncode "0").

**handleInFromSim()**
This function checks for chars send by the modem and prints them.

**handleInFromSim(&lt;waitTime&gt;)**
This function waits `waitTime`ms for chars send by the modem and prints them.

**handleInFromSim(&lt;OKstring&gt;, &lt;ERRORstring&gt;, &lt;waitTime&gt;)**
This function checks `waitTime`ms for chars send by the modem and prints them.
if the modem sends the `OKstring` the function exits with returncode "1".
if the modem sends the `ERRORstring` the function exits with returncode "-1".
If neither is received before `waitTime`ms the function exits with returncode "0".


# A Simple ESP01 Emulator on PC
It talks to UNO via serial port.\
Direct input from console to UNO.
It responds to the following AT commands:
- AT+CIPMUX=1
- AT+CIPSERVER=1,4444
- AT+CIPSEND=0,\
Commands and responses will be printed at console.

## Issues
Commands from UNO sometimes corrupted with "line break". Don't think it is a problem in arduino codes. Anyway the corrupted command can be handled manually.




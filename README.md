# TCP SERVER on ARDUINO UNO with ESP01 using AT COMMANDS
It is a loop back demonstration. It echos whatever received from a TCP client.

## Procedures
Setup baud rate and wifi connectivity of ESP01. (115200)\
Connect ESP01 to serial (hardware port: pin#0 and pin#1) of UNO and power up.\
Wait until LED turns on.\
On a PC in the same local network, run netcat:

$ nc 192.168.esp01.ip 4444

Type a line and hit enter.\
The server will send back what have been typed.

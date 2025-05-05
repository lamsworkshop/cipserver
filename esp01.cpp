#include <avr/pgmspace.h>
#include <stdio.h>
#include "grep.h"
#include "message.h"

void server_ready_cb(void);

namespace
{

const char GOT_IP[] PROGMEM = "GOT IP\r\n";
const char MUX[] PROGMEM = "AT+CIPMUX=1\r\n";
const char OK[] PROGMEM = "OK\r\n";
const char SERVER[] PROGMEM = "AT+CIPSERVER=1,4444\r\n";
const char IPD[] PROGMEM = "+IPD,0,";
const char SEND[] PROGMEM = "AT+CIPSEND=0,";
const char PROMPT[] PROGMEM = "> ";

struct __attribute__((packed)) {  // no padding between ZERO and data
  char ZERO;      // a constant zero(0) to deferentiate command from message
  char data[22];  // command.data is always c_str
} command;

Message message;
char *uno2esp;            // points to data to be sent to ESP01. can be command.data, message.data or null
const char* last_command; // keep track of ESP01 activity

Grep scan(GOT_IP);        // grep input from ESP01: "GOT IP" "OK" "+IPD,0," "> "
                          // when reading IPD data, set it to ZERO (scan.set(0)) and DON'T call scan.scan(c)

bool integer(char c, int &n) {  // string to integer. return true when c in not 0~9
  unsigned int i = c - '0';     // update "n" according to "c"
  if (i > 9) return true;
  n = n * 10 + i;
  return false;
}

} // anonymous namespace

int ESP2UNO(char c) {   // handling character received from ESP01
  static int n, m;      // used in reading +IPD data
  if (scan.is(0)) {     // input is +IPD data
    if (n > m) {        // push data to message.data
      message.add(c);
      if (++m == n) { scan.set(IPD); return message.flush(); }  // exit with message or null
    } else {            // reading data size
      if (integer(c, m)) {
        if (c == ':') { n = m; m = 0; } else scan.set(IPD);
      }
    }
  } else {              // input is not +IPD data
    if (scan.scan(c)) {
      if (scan.is(IPD)) scan.set(message.length=n=m=0);
      if (scan.is(OK)) {  // check last_command to see the cause
        if (last_command == SERVER) server_ready_cb();
        if (last_command == MUX) strcpy_P(uno2esp=command.data, last_command=SERVER);
        else scan.set(IPD);
      }
      if (scan.is(GOT_IP)) { strcpy_P(uno2esp=command.data, last_command=MUX); scan.set(OK); }
      if (scan.is(PROMPT)) { uno2esp = message.data; scan.set(OK); }
    }
  }
  return 0; // indicate no message from UNO
}

char *UNO2ESP(void) { char *c= uno2esp; uno2esp = 0; return c; }  // return command or CIPSEND data

void cipsend(int len) {   // initiate a CIPSEND sequence
  if (!len) return;       // only when data is present
  strcpy_P(uno2esp=command.data, last_command=SEND);
  sprintf(&command.data[13], "%d\r\n", len);
  scan.set(PROMPT);
}

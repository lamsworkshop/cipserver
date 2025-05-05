#include <string.h>
#include <iostream>

using namespace std;

const char OK[] = "OK\r\n";
const char PROMPT[] = "> ";
int data_to_send;

bool integer(char c, int &n) {  // string to integer
  unsigned int i = c - '0';
  if (i > 9) return true;
  n = n * 10 + i;
  return false;
}

const char *handler(char *str, int &len) {
  if (data_to_send) {
    while (len--) {
      cout << *str++;
      if (!--data_to_send) { cout << endl; len = 4; return OK; }
    }
  } else {
    str[len] = 0;
cerr << str << endl;
    if (!strcmp("AT+CIPMUX=1\r\n", str)) { len = 4; return OK; }
    if (!strcmp("AT+CIPSERVER=1,4444\r\n", str)) { len = 4; return OK; }
    str[12] = 0;
    if (!strcmp("AT+CIPSEND=0", str)) {
      int i = 13;
      while (!integer(str[i++], data_to_send));
      len = 2; return PROMPT;
    }
  }
  return 0;
}

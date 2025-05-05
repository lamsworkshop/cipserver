#include "message.h"

// $ nc 192.168.50.203 4444
char *loopback(char *d, unsigned char &size) {  // loop back data to ESP01
  if (size == MAX) size = 0;                    // buffer overflow, don't send
  return d;                                     // ESP01 can't handle +IPD/CIPSEND simutaneously
}

void Message::add(char c) {
  data[length++] = c;
  if (length >= MAX) flush();
}

int Message::flush(void) {
  char *d = loopback(data, length);
  for (int i = 0; i < length; i++) data[i] = d[i];
  return length;
}
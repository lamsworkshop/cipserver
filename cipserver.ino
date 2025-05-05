#include "message.h"

#define LED 13    // 4 unocar, 13 builtin

int ESP2UNO(char);
char *UNO2ESP(void);
void cipsend(int);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  char *c;
  if (Serial.available()) cipsend(ESP2UNO(Serial.read()));
  if ((c = UNO2ESP())) {                        // see if serial data to ESP01
    if (LENGTH(c)) Serial.write(c, LENGTH(c));  // write CIPSEND data
    else Serial.print(c);                       // write command
  }
}

void server_ready_cb(void) { digitalWrite(LED, HIGH); }
// turn on LED when CIPSERVER is ready

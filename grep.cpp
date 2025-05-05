#include <avr/pgmspace.h>
#include "grep.h"

Grep::Grep(const char *p) { set(p); }

void Grep::set(const char *p) { pattern = match = p; }

bool Grep::is(const char *p) { return p == pattern; }

bool Grep::scan(char c) {
  if (!pgm_read_byte(match)) match = pattern;
  while (match != pattern) {
    if (c == pgm_read_byte(match)) return !pgm_read_byte(++match);
    shift2match();
  }
  if (c == pgm_read_byte(match)) match++;
  return !pgm_read_byte(match);
}

void Grep::shift2match(void) {
  const char *p = pattern;
  while (++p != match) if (matchPattern(p)) break;
  match -= p - pattern;
}

bool Grep::matchPattern(const char *p) {
  for (int i=0; i < match - p; i++)
    if (pgm_read_byte(&pattern[i]) != pgm_read_byte(&p[i]))
      return false;
  return true;
}
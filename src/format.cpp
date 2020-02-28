#include "format.h"

#include <string>

using std::string;

// DONE:
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  long hh, mm, ss;
  std::string time;
  hh = seconds / 3600;
  seconds -= hh * 3600;
  mm = seconds / 60;
  seconds -= mm * 60;
  ss = seconds;
  time =
      std::to_string(hh) + ":" + std::to_string(mm) + ":" + std::to_string(ss);

  return time;
}
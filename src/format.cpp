#include "format.h"

#include <string>

using std::string;

// Done: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int hour;
  int min;
  int sec;
  hour = seconds / 3600;
  seconds -= hour * 3600;
  min = seconds / 60;
  seconds -= min * 60;
  sec = seconds / 1;
  string h, m, s;
  string output;
  h = std::to_string(hour);
  if (h.size() < 2) h = "0" + h;
  m = std::to_string(min);
  if (m.size() < 2) m = "0" + m;
  s = std::to_string(sec);
  if (s.size() < 2) s = "0" + s;
  output = h + ":" + m + ":" + s;
  return output;
}
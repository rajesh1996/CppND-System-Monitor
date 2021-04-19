#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  long HH = seconds/3600;
  long min_sec = seconds%3600;
  long MM = min_sec/60;
  long SS = min_sec%60;
  string elapsed_time =         std::to_string(HH)+':'+std::to_string(MM)+':'+std::to_string(SS);
  return elapsed_time;
}
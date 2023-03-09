#include "format.h"

#include <string>

using std::string;

string Format::ElapsedTime(long seconds) {
  auto h{seconds / 3600};
  auto m{(seconds - h * 3600) / 60};
  auto s{seconds - h * 3600 - m * 60};
  if (s < 10)
    return std::to_string(h) + ":" + std::to_string(m) + ":0" +
           std::to_string(s);
  else
    return std::to_string(h) + ":" + std::to_string(m) + ":" +
           std::to_string(s);
}
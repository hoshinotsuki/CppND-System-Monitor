#include "format.h"
#include <sstream>
#include <iomanip>
#include <string>

using std::string;

string Format::ElapsedTime(long seconds) {
  auto h{seconds / 3600};
  auto m{(seconds - h * 3600) / 60};
  auto s{seconds - h * 3600 - m * 60};
  std::ostringstream stream;
  stream << std::setw(2) << std::setfill('0') << h << ":" << std::setw(2)
         << std::setfill('0') << m << ":" << std::setw(2) << std::setfill('0')
         << s;
  return stream.str();
}
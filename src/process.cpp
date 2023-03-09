#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return _pid; }
string Process::User() { return LinuxParser::User(Pid()); }
float Process::CpuUtilization() { return LinuxParser::CpuUtilization(Pid()); }
string Process::Ram() { return LinuxParser::Ram(Pid()); }
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }
string Process::Command() { return LinuxParser::Command(Pid()); }
bool Process::operator<(const Process& other) const {
  return LinuxParser::CpuUtilization(_pid)< LinuxParser::CpuUtilization(other._pid);
}

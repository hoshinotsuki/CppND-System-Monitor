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
// TODO: 3
float Process::CpuUtilization() {
  int _pid = Pid();
  float _cpu;
  return _cpu;
}
string Process::Ram() { return LinuxParser::Ram(Pid()); }
// TODO: 5
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }
// TODO: 6. Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }
bool Process::operator<(const Process& other) const {
  return _pid < other._pid;
}

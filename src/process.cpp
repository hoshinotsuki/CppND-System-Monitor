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

string Process::User() {
  int _pid = Pid();
  string _user = LinuxParser::User(_pid);
  return _user;
}

// TODO: 3
float Process::CpuUtilization() {
  int _pid = Pid();
  float _cpu;
  return _cpu;
}

// TODO: 4
string Process::Ram() {
  int _pid = Pid();
  string _ram;
  return _ram;
}

// TODO: 5
long int Process::UpTime() {
  int _pid = Pid();
  long int _uptime;
  return _uptime;
}

// TODO: 6. Return the command that generated this process
string Process::Command() {
  int _pid = Pid();
  string _cmd;
  return _cmd;
}

bool Process::operator<(const Process& other) const {
  return _pid < other._pid;
}

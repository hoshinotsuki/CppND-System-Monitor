#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  Process(int p) : _pid(p) {}
  int Pid();
  std::string User();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  std::string Command();
  bool operator<(Process const& a) const;

 private:
  int _pid;
  std::string _user;
  float _cpu;
  std::string _ram;
  long int _uptime;
  std::string _cmd;
};
#endif
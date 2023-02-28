#ifndef PROCESS_H
#define PROCESS_H

#include <string>
using namespace std;
class Process {
 public:
  Process(int p, string u, float c, string r, long int t, string cmd)
      : _pid(p), _user(u), _cpu(c), _ram(r), _uptime(t), _cmd(cmd) {}
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
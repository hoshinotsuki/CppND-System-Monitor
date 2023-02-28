#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: 1. Return this process's ID
int Process::Pid() {  
    return _pid; }

// TODO: 2. Return the user (name) that generated this process
string Process::User() { return string(); } 

// TODO: 3. Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }


// TODO: 4. Return this process's memory utilization
string Process::Ram() { return string(); }



// TODO: 5. Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }


// TODO: 6. Return the command that generated this process
string Process::Command() { return string(); }
 
bool Process::operator<(const Process& other) const { return _pid< other._pid; }
  
#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <numeric>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// System
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  int value;
  int MemTotal, MemFree;
  std::ifstream fstream(kProcDirectory + kMeminfoFilename);
  if (fstream.is_open()) {
    while (std::getline(fstream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          MemTotal = value;
        }
        if (key == "MemFree") {
          MemFree = value;
        }
      }
    }
  }
  return (MemTotal - MemFree) * 1.0 / MemTotal;
}

long LinuxParser::UpTime() {
  string line;
  long up_sys, idle;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> up_sys >> idle;
    return up_sys;
  }
}

vector<string> LinuxParser::CpuUtilization() {
  string line, key, value;
  vector<string> _cpuStates{};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "cpu") {
          for (int n = 1; n <= 10; n++) {
            linestream >> value;
            _cpuStates.push_back(value);
          }
        };
      }
    }
  }
  return _cpuStates;
}

int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
}

int LinuxParser::RunningProcesses() {
  string line, key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
}

// Processes
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

string LinuxParser::User(int pid) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
}

float LinuxParser::CpuUtilization(int pid) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      std::vector<string> values;
      while (linestream >> value) {
        values.push_back(value);
      }
      long int utime = std::stol(values[13]) /
                       sysconf(_SC_CLK_TCK);  // CPU time spent in user code
      long int stime = std::stol(values[14]) /
                       sysconf(_SC_CLK_TCK);  // CPU time spent in kernel code
      long int cutime = std::stol(values[15]) /
                        sysconf(_SC_CLK_TCK);  // Waited-for children's CPU time
                                               // spent in user code
      long int cstime = std::stol(values[16]) /
                        sysconf(_SC_CLK_TCK);  // Waited-for children's CPU time
                                               // spent in kernel code
      long int starttime =
          std::stol(values[21]) /
          sysconf(_SC_CLK_TCK);  //  Time when the process started

      auto total_time = utime + stime + cutime + cstime;
      auto process_time = LinuxParser::UpTime() - starttime;
      float cpu_usage = 100.0 * total_time / process_time;
      return cpu_usage;
    }
  }
}

string LinuxParser::Ram(int pid) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          float mg = std::stof(value) / 1000;
          return std::to_string(mg).substr(0, 6);
        }
      }
    }
  }
}

long int LinuxParser::UpTime(int pid) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      std::vector<string> values;
      while (linestream >> value) {
        values.push_back(value);
      }
      long int seconds =
          LinuxParser::UpTime() - std::stol(values[21]) / sysconf(_SC_CLK_TCK);
      return seconds;
    }
  }
}

string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  }
}

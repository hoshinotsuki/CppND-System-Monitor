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

template <typename T>
T findValueByKey(std::string const &keyFilter, std::string const &filename) {
  std::string line, key;
  T value;
  std::ifstream stream(LinuxParser::kProcDirectory + filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream iss(line);
      while (iss >> key >> value) {
        if (key == keyFilter) {
          return value;
        }
      }
    }
  }
  return value;
};

template <typename T>
T getValueOfFile(std::string const &filename) {
  std::string line;
  T value;
  std::ifstream stream(LinuxParser::kProcDirectory + filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream iss(line);
    iss >> value;
  }
  return value;
};

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

// Return the system kernel from "/proc/version"
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

// Return the system memory utilization from "/proc/memInfo"
float LinuxParser::MemoryUtilization() {
  string memTotal = "MemTotal:";
  string memFree = "MemFree:";
  float Total = findValueByKey<float>(memTotal, kMeminfoFilename);
  float Free = findValueByKey<float>(memFree, kMeminfoFilename);
  return (Total - Free) * 1.0 / Total;
}

// Return the system uptime from "/proc/uptime"
long LinuxParser::UpTime() {
  string line;
  long up_sys, idle;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> up_sys >> idle;
  }
  return up_sys;
}

// Read and return the 10 values of the Cpu for the system from "/proc/stat"
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

// Read and return the TotalProcesses from "/proc/stat"
int LinuxParser::TotalProcesses() {
  std::string key = "processes";
  int value = findValueByKey<int>(
      key, LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  return value;
}

// Read and return the RunningProcesses from "/proc/stat"
int LinuxParser::RunningProcesses() {
  std::string key = "procs_running";
  int value = findValueByKey<int>(
      key, LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  return value;
}

// Processes
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
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

// Return uid for process[pid] from "/proc/pid/stat"
string LinuxParser::Uid(int pid) {
  std::string key = "Uid:";
  std::string value = findValueByKey<std::string>(
      key, LinuxParser::kProcDirectory + std::to_string(pid) +
               LinuxParser::kStatFilename);
  return value;
}

// Return username for process[pid] from "/etc/passwd"
string LinuxParser::User(int pid) {
  std::string line, user, x, uid;
  std::ifstream filestream(LinuxParser::kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line, ':')) {
      std::istringstream iss(line);
      iss >> user >> x >> uid;
      if (uid == LinuxParser::Uid(pid)) return user;
    }
  }
  return " ";
}
 
float LinuxParser::CpuUtilization(int pid) {
  string line, key, value;
  long int utime,  // CPU time spent in user code
      stime,       // CPU time spent in kernel code
      cutime,      // Waited-for children's CPU time spent in user code
      cstime,      // Waited-for children's CPU time spent in kernel code
      starttime,   //  Time when the process started
      total_time,  // utime + stime + cutime + cstime
      process_time; // system uptime - process start_time
  float cpu_usage;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      std::vector<string> values;
      while (linestream >> value) {
        values.push_back(value);
      }
      utime = std::stol(values[13]) / sysconf(_SC_CLK_TCK);
      stime = std::stol(values[14]) / sysconf(_SC_CLK_TCK);
      cutime = std::stol(values[15]) / sysconf(_SC_CLK_TCK);
      cstime = std::stol(values[16]) / sysconf(_SC_CLK_TCK);
      starttime = std::stol(values[21]) / sysconf(_SC_CLK_TCK);
      total_time = utime + stime + cutime + cstime;
      process_time = LinuxParser::UpTime() - starttime;
      cpu_usage = 1.0 * total_time / process_time;
      return cpu_usage;
    }
  }
  return 0.0;
}

// read and return VmSize in "/proc/pid/status"
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
  return "";
}

// return process[id] uptime from /proc/pid/stat
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
  return 0;
}

// Read and return the process command line
string LinuxParser::Command(int pid) {
  return std::string(
      getValueOfFile<std::string>(std::to_string(pid) + kCmdlineFilename));
}
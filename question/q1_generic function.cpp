// linux_parser.cpp line 127
// this code using template doesn't work, it always return empty value
int LinuxParser::TotalProcesses() {
  std::string key = filterProcesses;
  int value = findValueByKey<int>(key, kProcDirectory + kStatFilename);
  return value;
}

// this code works
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == filterProcesses) {
          return value;
        }
      }
    }
  }
  return 0;
}


// linux_parser.cpp line 146 
// this code using template doesn't work, it always return empty value
int LinuxParser::RunningProcesses() {
  std::string key = filterRunningProcesses;
  int value = findValueByKey<int>(key, kProcDirectory + kStatFilename);
  return value;
}

// this code works
int LinuxParser::RunningProcesses() {
  string line, key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == filterRunningProcesses) {
          return value;
        }
      }
    }
  }
  return value;
}


// linux_parser.cpp line 239
// this code using template doesn't work, it always return empty value
string LinuxParser::Ram(int pid) {
  std::string key = "VmRSS:";
  std::string value = findValueByKey<std::string>(key,kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  return value;
} 

// this code works
string LinuxParser::Ram(int pid) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmRSS:") {
          float mg = std::stof(value) / 1000;
          return std::to_string(mg).substr(0, 6);
        }
      }
    }
  }
  return "";
}
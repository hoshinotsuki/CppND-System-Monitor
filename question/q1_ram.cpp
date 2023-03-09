// linux_parser.cpp line 220

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
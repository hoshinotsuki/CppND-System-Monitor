/* 
 linux_parser.cpp line 258

I can't access the whole command line, i can only display the part of the command line.
*/

// Read and return the process command line
string LinuxParser::Command(int pid) {
  return std::string(
      getValueOfFile<std::string>(std::to_string(pid) + kCmdlineFilename));
}
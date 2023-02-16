#ifndef PROCESSOR_H
#define PROCESSOR_H
#include "linux_parser.h"
class Processor {
 public:
  float Utilization();
  float cpu_usage{0.0};
  int now_total{0},last_total{0}, last_idel{0};
  std::vector<std::string> _cpuStates; 
};
#endif
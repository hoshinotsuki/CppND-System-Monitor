#include "processor.h"

#include <numeric>
#include <string>
#include <vector>

//Return the aggregate CPU utilization
float Processor::Utilization() {
  int delta_total{0}, delta_idel{0};

  while (1) {
    _cpuStates = LinuxParser::CpuUtilization();

    std::vector<int> _cpuvalues{};
    for (auto v : _cpuStates) {
      _cpuvalues.push_back(std::stoi(v));
    }
    now_total = std::accumulate(_cpuvalues.begin(), _cpuvalues.end(), 0);
    delta_total = now_total - last_total;
    delta_idel =
        std::stoi(_cpuStates[LinuxParser::CPUStates::kIdle_]) - last_idel;
    cpu_usage = 1.0 * (delta_total - delta_idel) / delta_total;
    last_total = now_total;
    last_idel = std::stoi(_cpuStates[LinuxParser::CPUStates::kIdle_]);

    return cpu_usage;
  }
}
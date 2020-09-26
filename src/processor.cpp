#include "processor.h"

#include <assert.h>

#include <string>
#include <vector>

#include "linux_parser.h"

using std::stol;
using std::string;
using std::vector;

/*
    [ref]
    https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
    1. Idle = idle + iowait
    2. NonIdle = user + nice + system + irq + softirq + steal
*/
long Processor::CalculateIdle(vector<long>& utilization) const {
  return utilization[mode::idle] + utilization[mode::iowait];
}
long Processor::CalculateNonIdle(vector<long>& utilization) const {
  return utilization[mode::user] + utilization[mode::nice] +
         utilization[mode::system] + utilization[mode::irq] +
         utilization[mode::softirq] + utilization[steal];
}
// Return the aggregate CPU utilizations
float Processor::Utilization() {
  vector<string> cpu_util_str = LinuxParser::CpuUtilization();
  vector<long> cpu_utilization;
  for (auto it = cpu_util_str.begin(); it != cpu_util_str.end(); it++) {
    cpu_utilization.push_back(stol(*it));
  }

  long idle = CalculateIdle(cpu_utilization);
  long nonidle = CalculateNonIdle(cpu_utilization);
  long pre_idle = CalculateIdle(prev_utilization);
  long pre_nonidle = CalculateNonIdle(prev_utilization);

  long total = idle + nonidle;
  long pre_total = pre_idle + pre_nonidle;

  float totald = total - pre_total;
  float idled = idle - pre_idle;
  return (totald - idled) / totald;
}
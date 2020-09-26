#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>

using std::vector;

class Processor {
 public:
  Processor() : prev_utilization(10, 0){};
  float Utilization();
  long CalculateIdle(vector<long>& utilization) const;
  long CalculateNonIdle(vector<long>& utilization) const;

 private:
  enum mode {
    user = 0,
    nice,
    system,
    idle,
    iowait,
    irq,
    softirq,
    steal,
    guest,
    guest_nice
  };
  vector<long> prev_utilization;
};

#endif
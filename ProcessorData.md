## Processor Data in Linux OS

### 1. Processor Utilization(CPU)

- location: `/proc/stat`
- detail: `cpu [user] [nice] [system] [idle] [iowait] [irq] [softirq] [steal] [guest] [guest_nice]`
- meaning:
  - user: normal processes executing in user mode
  - nice: niced processes executing in user mode
  - system: processes executing in kernel mode
  - idle: twiddling thumbs
  - iowait: waiting for I/O to complete
  - irq: servicing interrupt
  - softirq: servicing softirq
  - steal: involuntary wait
  - guest: running a normal guest
  - guest_nice: running a niced guest
- [ref: Stack Overflow](https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux):
- measurement interval:
  - The process data is measured since boot.
  - To calculate the current utilization of the process, we need to compute the rate of utilization change

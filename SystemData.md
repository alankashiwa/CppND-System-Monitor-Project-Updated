## System Data in Linux OS

### 1. Operating System (OS)

- location: `/etc/os-release`
- detail: `PRETTY_NAME=xxx`

### 2. Kernel

- location: `/proc/version`
- detail: `Linux version xxx`

### 3. Memory Utilization

- location: `/proc/meminfo`
- detail:
  - `MemTotal: xxx`
  - `MemFree: xxx`
  - `MemAvailable: xxx`
  - `Buffers: xxx`
- calculation: [Hisham H. Muhammad@Stack Overflow](https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290)

### 4. Total Processes

- location: `/proc/meminfo`
- detail: `processes xxx`

### 5. Running Processes

- location: `/proc/meminfo`
- detail: `procs running xxx`

### 6. Up Time

- location: `/proc/uptime`
- detail: `(the uptime of the system) (the amount of time spent in the idle processes)`

## Processor Data in Linux OS

Linux stores data about individual processes in files within subdirectories of the `/proc` directory. Each subdirectory is named for that particular process's identifier number.

### 1. PID

- location: the subdirectory names

### 2. User idenrifier (UID): Process Owner

- location: `/proc/[PID]/status`
- detail: `Uid: xxx`

### 3. User Name

- location: `/etc/passwd`

### 4. Processor Utilization

- location: `/proc/[PID]/stat`
- ref: [man page](https://man7.org/linux/man-pages/man5/proc.5.html), [Stack Overflow](https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599)

### 5. Memory Utilization

- location: `/proc/[pid]/status`
- detail: `VmSize: xxx`

### 6. Up Time

- location: `/proc/[pid]/stat`

### 7. Command

- location: `/proc/[pid]/cmdline`

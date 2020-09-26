#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream filestream(kProcDirectory + kVersionFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string key, value;
  float total_memory, free_memory;
  string line;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::stringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") {
        total_memory = std::stof(value);
      } else if (key == "MemFree:") {
        free_memory = std::stof(value);
      }
    };
  }
  return (total_memory - free_memory) / total_memory;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string up_time;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::stringstream linestream(line);
    linestream >> up_time;
  }
  return stol(up_time);
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key, value;
  vector<string> cpu_utilization;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream,
                 line);  // Get First Line: Aggregated CPU Utilization
    std::stringstream linestream(line);
    linestream >> key;
    while (linestream >> value) {
      cpu_utilization.push_back(value);
    }
  }

  return cpu_utilization;
}

/*
  [Ref]
  https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
*/
float LinuxParser::CpuUtilization(int pid) {
  string pid_dir = "/" + to_string(pid);
  string line;
  string value;
  enum ParameterPosition {
    utime_no = 14,
    stime_no = 15,
    cutime_no = 16,
    cstime_no = 17,
    starttime_no = 22
  };
  long uptime, utime, stime, cutime, cstime, starttime;
  uptime = UpTime();

  std::ifstream filestream(kProcDirectory + pid_dir + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::stringstream linestream(line);
    int count = 0;
    while (linestream >> value) {
      count += 1;
      switch (count) {
        case ParameterPosition::utime_no:
          utime = stol(value) / sysconf(_SC_CLK_TCK);
          break;
        case ParameterPosition::stime_no:
          stime = stol(value) / sysconf(_SC_CLK_TCK);
          break;
        case ParameterPosition::cutime_no:
          cutime = stol(value) / sysconf(_SC_CLK_TCK);
          break;
        case ParameterPosition::cstime_no:
          cstime = stol(value) / sysconf(_SC_CLK_TCK);
          break;
        case ParameterPosition::starttime_no:
          starttime = stol(value) / sysconf(_SC_CLK_TCK);
          break;
      }
    }
  }
  long total_time = utime + stime + cutime + cstime;
  long seconds = uptime - starttime;
  return (float)total_time / seconds;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  while (std::getline(filestream, line)) {
    std::stringstream linestream(line);
    linestream >> key;
    if (key == "processes") {
      linestream >> value;
      return std::stoi(value);
    }
  }
  return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  while (std::getline(filestream, line)) {
    std::stringstream linestream(line);
    linestream >> key;
    if (key == "procs_running") {
      linestream >> value;
      return std::stoi(value);
    }
  }
  return 0;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string pid_dir = "/" + to_string(pid);
  string line;
  std::ifstream filestream(kProcDirectory + pid_dir + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string pid_dir = "/" + to_string(pid);
  string line;
  string key, value;
  std::ifstream filestream(kProcDirectory + pid_dir + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::stringstream linestream(line);
      linestream >> key;
      if (key == "VmSize:") {
        linestream >> value;
        long mb = stol(value) / 1000;
        return to_string(mb);
      }
    }
  }
  return "0";
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string pid_dir = "/" + to_string(pid);
  string line;
  string key, value;
  std::ifstream filestream(kProcDirectory + pid_dir + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::stringstream linestream(line);
      linestream >> key;
      if (key == "Uid:") {
        linestream >> value;
        return value;
      }
    }
  }
  return "";
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string line;
  string username, password, userid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::stringstream linestream(line);
      std::getline(linestream, username, ':');
      std::getline(linestream, password, ':');
      std::getline(linestream, userid, ':');
      if (userid == uid) {
        return username;
      }
    }
  }
  return "";
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string pid_dir = "/" + to_string(pid);
  string line;
  string value;
  const int starttime_no = 22;
  std::ifstream filestream(kProcDirectory + pid_dir + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::stringstream linestream(line);
    int count = 0;
    while (linestream >> value) {
      count += 1;
      if (count == starttime_no) {
        long clockticks = stol(value);
        return clockticks / sysconf(_SC_CLK_TCK);
      }
    }
  }
  return 0;
}
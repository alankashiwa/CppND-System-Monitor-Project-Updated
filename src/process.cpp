#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() const {
  return LinuxParser::CpuUtilization(pid);
}

// Return the command that generated this process
string Process::Command() {
  const int max_command_size = 40;
  string dotdot = "...";
  string command = LinuxParser::Command(pid);
  // Limit output size to max_command_size + dotdot size
  if (command.size() > (max_command_size + dotdot.size())) {
    return command.substr(0, max_command_size) + dotdot;
  } else {
    return command;
  }
}

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return (CpuUtilization() > a.CpuUtilization());
}
#include "process.h"

#include <unistd.h>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, float cpu) : pid_{pid}, cpu_{cpu} {}

// DONE: Return this process's ID
int Process::Pid() { return pid_; }
string Process::User() { return user_; }
string Process::Command() { return command_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_; }

// DONE: Return this process's memory utilization
string Process::Ram() {
  if (ramStr_.length() > 0 &&
      std::all_of(ramStr_.begin(), ramStr_.end(), isdigit)) {
    std::stringstream str;
    auto local_ram = std::stof(ramStr_);
    local_ram *= 0.001;
    str << std::fixed << std::setprecision(2) << local_ram;
    return str.str();
  }
  return "0";
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { return cpu_ > a.cpu_; }

void Process::user(std::string user) { user_ = user; }
void Process::command(std::string command) { command_ = command; }
void Process::uptime(long uptime) { uptime_ = uptime; }
void Process::Ram(string ram) { ramStr_ = ram; }

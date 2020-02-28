#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

#define MAXPQSIZE 10
using std::cout;
using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {}

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

bool is_number(const std::string& s) {
  return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) {
                         return !std::isdigit(c);
                       }) == s.end();
}

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();
  processes_ = {};
  std::priority_queue<Process> pqProcess;
  for (auto id : pids) {
    string uid = LinuxParser::Uid(id);
    float cpu_usage = 0.01 * static_cast<float>(LinuxParser::ActiveJiffies(id));
    if (pqProcess.size() < 11) {
      pqProcess.push(Process(id, cpu_usage));
    } else {
      auto topItem = pqProcess.top();
      auto topCpu = topItem.CpuUtilization();
      if (topCpu < cpu_usage) {
        pqProcess.pop();
        pqProcess.push(Process(id, cpu_usage));
      }
    }
  }
  while (!pqProcess.empty()) {
    //    user, command, ram, uid
    auto localProcess = pqProcess.top();
    localProcess.command(LinuxParser::Command(localProcess.Pid()));
    string uid = LinuxParser::Uid(localProcess.Pid());
    localProcess.user(LinuxParser::User(uid));
    localProcess.Ram(LinuxParser::Ram(localProcess.Pid()));
    localProcess.uptime(LinuxParser::UpTime(localProcess.Pid()));
    processes_.push_back(localProcess);
    pqProcess.pop();
  }
  std::reverse(processes_.begin(), processes_.end());
  return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() {
  //  constexpr string kernel = LinuxParser::Kernel();
  return LinuxParser::Kernel();
}

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() {
  //   long int time = LinuxParser::UpTime();
  return LinuxParser::UpTime();
}

#include "processor.h"
#include <chrono>
#include <numeric>
#include <string>
#include <thread>
#include <vector>
#include "linux_parser.h"

using std::string;
using std::vector;
// DONE: Return the aggregate CPU utilization
// Calculate the active cpu usage in the last millisecond
float Processor::Utilization() {
  auto cpu_data = LinuxParser::CpuUtilization();
  auto total_usage_prev = LinuxParser::Jiffies(cpu_data);
  if (total_usage_prev <= 0) return 0;
  auto total_active_prev = LinuxParser::ActiveJiffies(cpu_data);
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  cpu_data = LinuxParser::CpuUtilization();
  auto total_usage = LinuxParser::Jiffies(cpu_data);
  if (total_usage <= 0) return 0;
  auto total_active = LinuxParser::ActiveJiffies(cpu_data);
  total_active = total_active - total_active_prev;
  total_usage = total_usage - total_usage_prev;
  if (total_usage <= 0) return 0;
  return (total_active / static_cast<float>(total_usage));
}
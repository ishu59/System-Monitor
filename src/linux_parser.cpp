#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>
#include <filesystem>
#include <numeric>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;
namespace fs = std::filesystem;

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
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// DONE: for BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  for (auto& entry : fs::directory_iterator(kProcDirectory)) {
    if (entry.is_directory()) {
      string dirName = fs::path(entry.path()).filename();
      if (std::all_of(dirName.begin(), dirName.end(), isdigit)) {
        pids.push_back(std::stoi(dirName));
      }
    }
  }
  return pids;
}

// DONE:
float LinuxParser::MemoryUtilization() {
  long memTotal = 0, memFree = 0;
  float memUtil = 0.0;
  std::string line;
  std::string key, val;
  // "/proc/meminfo"
  std::ifstream myStream(kProcDirectory + kMeminfoFilename);
  if (myStream.is_open()) {
    while (std::getline(myStream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> val) {
        if (key == "MemTotal:")
          memTotal = std::stol(val);
        else if (key == "MemFree:")
          memFree = std::stol(val);
        else
          continue;
      }
      if (memFree > 0 && memTotal > 0) break;
    }
    memUtil = memFree / static_cast<float>(memTotal);
    return memUtil;
  }
  return memUtil;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  double uptime;
  double notReq;
  long int timeInt = 0;
  std::string line;
  std::string time;
  std::ifstream mysteam(kProcDirectory + kUptimeFilename);
  if (mysteam.is_open()) {
    getline(mysteam, line);
    std::istringstream lineStream(line);
    while (lineStream >> uptime >> notReq) {
      timeInt = static_cast<long int>(uptime);
    }
  }
  return timeInt;
}

// NotReq: Read and return the number of jiffies for the system
// DO not read the same file again for same info
long LinuxParser::Jiffies() { return 0; }

// NotReq: Read and return the number of active jiffies for the system
// DO not read the same file again for same info
long LinuxParser::ActiveJiffies() { return 0; }

// NotReq: Read and return the number of idle jiffies for the system
// DO not read the same file again for same info
long LinuxParser::IdleJiffies() { return 0; }

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies(vector<string>& cpu_usage) {
  vector<long> cpu_stats;
  if (cpu_usage.size() < 11) return 0;
  for (auto it = std::begin(cpu_usage) + 1; it != end(cpu_usage) - 2; ++it) {
    cpu_stats.push_back(std::stol((*it)));
  }
  auto total = std::accumulate(std::begin(cpu_stats), end(cpu_stats), 0);
  return total;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies(vector<string>& cpu_usage) {
  if (cpu_usage.size() < 11) return 0;
  vector<long> cpu_stats;
  for (auto it = std::begin(cpu_usage) + 1; it != end(cpu_usage) - 2; ++it) {
    cpu_stats.push_back(std::stol((*it)));
  }
  auto totalActive = cpu_stats[0] + cpu_stats[1] + cpu_stats[2] + cpu_stats[4] +
                     cpu_stats[5] + cpu_stats[6];
  return totalActive;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies(vector<string>& cpu_usage) {
  if (cpu_usage.size() < 11) return 0;
  vector<long> cpu_stats;
  for (auto it = std::begin(cpu_usage) + 1; it != end(cpu_usage) - 2; ++it) {
    cpu_stats.push_back(std::stol((*it)));
  }
  auto totalActive = cpu_stats[3] + cpu_stats[4];
  return totalActive;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, data;
  vector<string> cpuUsage;
  std::ifstream mystream(kProcDirectory + kStatFilename);
  if (mystream.is_open()) {
    while (std::getline(mystream, line)) {
      std::istringstream linestream(line);
      while (linestream >> data) cpuUsage.push_back(data);
      return cpuUsage;
    }
  }
  return cpuUsage;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key, val;
  int proctot = 0;
  std::ifstream mystream(kProcDirectory + kStatFilename);
  if (mystream.is_open()) {
    while (std::getline(mystream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> val) {
        if (key == "processes") {
          proctot = std::stoi(val);
          return proctot;
        }
      }
    }
  }
  return proctot;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, val;
  int procRun = 0;
  std::ifstream mystream(kProcDirectory + kStatFilename);
  if (mystream.is_open()) {
    while (std::getline(mystream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> val) {
        if (key == "procs_running") {
          procRun = std::stoi(val);
          return procRun;
        }
      }
    }
  }
  return procRun;
}

bool isNumericStr(string& str) {
  if (str.length() < 1) return false;
  return std::all_of(str.begin(), str.end(), isdigit);
}
// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string pid_path =
      kProcDirectory + "/" + to_string(pid) + "/" + kCmdlineFilename;
  string line, val;
  std::ifstream mystream(pid_path);
  if (mystream.is_open()) {
    while (std::getline(mystream, line)) {
      return line;
    }
  }
  return string();
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string pid_staus_path = kProcDirectory + to_string(pid) + kStatusFilename;
  string line, key, val;
  std::ifstream mystream(pid_staus_path);
  if (mystream.is_open()) {
    while (std::getline(mystream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> val) {
        if (key == "VmSize:") {
          if (isNumericStr(val)) return val;
        }
      }
    }
  }
  return string();
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string pid_path =
      kProcDirectory + "/" + to_string(pid) + "/" + kStatusFilename;
  std::ifstream mystream(pid_path);
  string line, key, val;
  if (mystream.is_open()) {
    while (std::getline(mystream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> val) {
        if (key == "Uid:") return val;
      }
    }
  }
  return string();
}

// DONE: see LinuxParser::allUser, Read and return the user associated with a
// process
string LinuxParser::User(string uid) {
  string line, key, val, notreq;
  string user;
  std::ifstream mystream(kPasswordPath);
  if (mystream.is_open()) {
    while (std::getline(mystream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> val >> notreq >> key) {
        if (key == uid) {
          return val;
        }
      }
    }
  }
  return string();
}
// DONE: Read and fill hashmap the user ID associated with a all process
// void LinuxParser::allUser(std::unordered_map<string, string>& userDict) {
//  string line, key, val, notreq;
//  string user;
//
//  std::ifstream mystream(kPasswordPath);
//  if (mystream.is_open()) {
//    while (std::getline(mystream, line)) {
//      std::replace(line.begin(), line.end(), ':', ' ');
//      std::istringstream linestream(line);
//      while (linestream >> val >> notreq >> key) {
//        if (std::all_of(key.begin(), key.end(), isdigit)) {
//          userDict[key] = val;
//        }
//      }
//    }
//  }
//}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line, word;
  string upTimeStr;
  vector<string> lineVec;
  long uptime;
  std::ifstream mystream(kProcDirectory + to_string(pid) + kStatFilename);
  if (mystream.is_open()) {
    while (std::getline(mystream, line)) {
      std::istringstream linestream(line);
      while (linestream >> word) lineVec.push_back(word);
      break;
    }
  }
  if (lineVec.size() > 22) {
    upTimeStr = lineVec[21];
    if (upTimeStr.length() > 0 &&
        std::all_of(upTimeStr.begin(), upTimeStr.end(), isdigit)) {
      uptime = std::stol(upTimeStr);
      uptime /= sysconf(_SC_CLK_TCK);
      return uptime;
    }
  }
  return 0;
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line, word;
  vector<string> lineVec;
  float utime, stime, cutime, cstime, starttime, sysUptime, cpuUsage;
  std::ifstream mystream(kProcDirectory + to_string(pid) + kStatFilename);
  if (mystream.is_open()) {
    while (std::getline(mystream, line)) {
      std::istringstream linestream(line);
      while (linestream >> word) lineVec.push_back(word);
      break;
    }
  }
  if (lineVec.size() > 22) {
    if (isNumericStr(lineVec[13]) && isNumericStr(lineVec[14]) &&
        isNumericStr(lineVec[15]) && isNumericStr(lineVec[16]) &&
        isNumericStr(lineVec[21])) {
      starttime = std::stof(lineVec[21]);
      utime = std::stof(lineVec[13]);
      stime = std::stof(lineVec[14]);
      cutime = std::stof(lineVec[15]);
      cstime = std::stof(lineVec[16]);
      sysUptime = LinuxParser::UpTime();
      auto total_time = utime + stime + cutime + cstime;
      auto seconds = sysUptime - (starttime / sysconf(_SC_CLK_TCK));
      //      std::cout << "total_time : " << total_time << " seconds: " <<
      //      seconds << "\n";
      cpuUsage = 100 * ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
      return cpuUsage;
    }
  }
  return 0;
}

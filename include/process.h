#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // DONE: See src/process.cpp
  std::string User();                      // DONE: See src/process.cpp
  std::string Command();                   // DONE: See src/process.cpp
  float CpuUtilization();                  // DONE: See src/process.cpp
  std::string Ram();                       // DONE: See src/process.cpp
  long int UpTime();                       // DONE: See src/process.cpp
  bool operator<(Process const& a) const;  // DONE: See src/process.cpp
  explicit Process(int pid, float ram);
  float RamFloat() const;
  void user(std::string user);
  void command(std::string command);
  void Ram(std::string ram);
  void uptime(long uptime);

 private:
  int pid_;
  std::string user_ = "";
  std::string command_ = "";
  std::string uid_ = "";
  float cpu_;
  std::string ramStr_;
  long uptime_;
};

#endif
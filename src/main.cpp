#include <iostream>

#include "linux_parser.h"
#include "ncurses_display.h"
#include "system.h"
int main() {
  System system;
  NCursesDisplay::Display(system);

  //  System system;
  //  auto item = LinuxParser::Uid(11294);
  //  auto item2 = LinuxParser::User(item);
  //  auto item3 = LinuxParser::Command(11294);
  //  std::cout << "UID: " << item << " User: "<< item2 << " command: "<< item3
  //  << "\n";
}
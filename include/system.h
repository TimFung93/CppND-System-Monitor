#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "linux_parser.h"

class System {
 public:
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          
  long UpTime();                     
  int TotalProcesses();               
  int RunningProcesses();             
  std::string Kernel();               
  std::string OperatingSystem();      

 private:
  Processor cpu_ = {};
  std::vector<Process> processes_;
  std::string os_ = LinuxParser::OperatingSystem();
  std::string kernal_ =  LinuxParser::Kernel();
};

#endif
#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/

// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat
class Process {
 public:
  int Pid();                               
  std::string User();                    
  std::string Command();                   
  float CpuUtilization();               
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  Process(int pid, std::string ram, std::string username, long uptime, std::string command, float utilization) 
    : pid_(pid), ram_(ram), username_(username), uptime_(uptime), command_(command), utilization_(utilization) {}

  // TODO: Declare any necessary private members
 private:
    int pid_;
    std::string ram_;
    std::string username_;
    long uptime_;
    std::string command_;
    float utilization_;
};

#endif
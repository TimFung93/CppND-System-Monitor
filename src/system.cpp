#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() 
{  
    vector<Process> processVector; 
    vector<int> pids = LinuxParser::Pids();
    for(auto & pid : pids)
    {
        processes_.clear();
        float utilization = LinuxParser::ProcessorUtilization(pid); 
        std::string ram = LinuxParser::Ram(pid);
        std::string username = LinuxParser::User(pid); 
        long uptime = LinuxParser::UpTime(pid); 
        std::string command = LinuxParser::Command(pid);

        processVector.push_back(Process(pid, ram, username,uptime, command, utilization));            
  
    }
    std::sort(processVector.begin(), processVector.end(), [] (Process& a, Process& b) {return a<b;});
    processes_ = processVector;
    return processes_;
}


std::string System::Kernel() { return kernal_; }

float System::MemoryUtilization() { return  LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return os_; }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid_; }

float Process::CpuUtilization() { return utilization_; }

string Process::Command() { return command_; }

string Process::Ram() { return ram_; }

string Process::User() { return username_; }

long int Process::UpTime() { return uptime_; }

bool Process::operator<(Process const& process) const {  
    return (process.utilization_) < (this->utilization_);
}
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> version;
  }
  return version;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() 
{
   std::string token;
   float memTotal;
   float memFree;
   std::ifstream stream(kProcDirectory + kMeminfoFilename);

   while (stream >> token) {
     if (token == "MemFree:") stream >> memFree;
     if (token == "MemTotal:") stream >> memTotal;
     // ignore rest of the line
     stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
   }

  return memFree / memTotal;  
}

long LinuxParser::UpTime() { 
   long upTimeInSeconds = 0;
   string line;
   std::ifstream stream(kProcDirectory + kUptimeFilename);

   if (stream.is_open()) {
     std::getline(stream, line);
     std::istringstream linestream(line);
     linestream >> upTimeInSeconds;
     return upTimeInSeconds;
   } 

   return upTimeInSeconds;
 }

std::vector<std::string> split(const std::string &text, char delim)
{
  std::vector<std::string> elems;
  std::string token;
  std::stringstream stream(text);

  while(getline(stream, token, delim))
  {
    elems.push_back(token);
  }

  return elems;
}

std::vector<float> LinuxParser::Jiffies() {
  std::string line;
  float activeJiffies;
  float idleJiffies;
  unsigned long user, nice, system, irq, softirq, steal, guest, guest_nice;
  unsigned long idle, iowait;

  std::vector<float> jiffies;
  
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);

    std::vector<std::string> info = split(line,' ');
    user = std::stoi(info[2]);
    nice = std::stoi(info[3]);
    system = std::stoi(info[4]);
    irq = std::stoi(info[7]);
    softirq = std::stoi(info[8]);
    steal = std::stoi(info[9]);
    guest = std::stoi(info[10]);
    guest_nice = std::stoi(info[11]);

    idle =  std::stoi(info[5]);
    iowait = std::stoi(info[6]);
  }

  activeJiffies = float(user + nice + system + irq + softirq + steal + guest + guest_nice);
  idleJiffies = float(idle + iowait);

  jiffies.emplace_back(activeJiffies);
  jiffies.emplace_back(idleJiffies);

  return jiffies;
}

int LinuxParser::TotalProcesses() {
  std::string token;
  int processes = 0;
  
  std::ifstream stream(kProcDirectory + kStatFilename);

   while (stream >> token) {
     if (token == "processes") {
       stream >> processes;
     }

      stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
   }

  return processes;
 }

int LinuxParser::RunningProcesses() {
    std::string token;
    int procs_running;
  
  std::ifstream stream(kProcDirectory + kStatFilename);

   while (stream >> token) {
     if (token == "procs_running") {
       stream >> procs_running;
     }

      stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
   }

  return procs_running;

 }

string LinuxParser::Command(int pid) { 
  std::string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);

  if (stream.is_open())
  {
      std::getline(stream, line);
      std::istringstream linestream(line);
  }
  return line;
}

string LinuxParser::Ram(int pid) 
{
  std::string token;
  std::string ram;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);

  while (stream >> token) {
    // VmSize is the virtual memory size not the exact physical memory being used
    // https://review.udacity.com/#!/reviews/2708636
    if (token == "VmData:") {
      stream >> ram;
    }

    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  if (ram == "")
  {
    return "0";
  }
  return ram.substr(0, ram.size() - 3);
}

string LinuxParser::Uid(int pid) 
{
  std::string token;
  std::string uid;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);

  while (stream >> token) {
    if (token == "Uid:") {
      stream >> uid;
    }

    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return uid;

}

string LinuxParser::User(int pid) 
{ 
  std::string uid = Uid(pid);
  std::string token;
  std::string username;

  std::ifstream stream(kPasswordPath);
  // check the user name since they are all redis
  while (stream >> token) {
    vector<std::string> userInfo = split(token, ':');
    if(userInfo[2] == uid)
    {
       username = userInfo[0];
       return username;
    } 
  }

  return username;
}

long LinuxParser::UpTime(int pid) { 
    long systemUptime = UpTime();
    long startTime;
    std::string line;
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);

    if (stream.is_open())
    {
      std::getline(stream, line);
      std::istringstream linestream(line);

      std::vector<std::string> statVector = split(line, ' ');

      startTime = std::stol(statVector[21]);
    }

    return systemUptime - startTime / sysconf(_SC_CLK_TCK);
 }

 float LinuxParser::ProcessorUtilization(int pid) {
   int uTime;
   int sTime;
   int cuTime;
   int csTime;
   int startTime;
   int hertz = sysconf(_SC_CLK_TCK);

    // get pid stat
  std::ifstream ss(kProcDirectory + to_string(pid) + kStatFilename);

  if (ss.is_open())
  {
    std::string line;
    std::getline(ss, line);
    std::istringstream linestream(line);

    std::vector<std::string> statVector = split(line, ' ');
    uTime = std::stol(statVector[13]);
    sTime = std::stol(statVector[14]);
    cuTime = std::stol(statVector[15]);
    csTime = std::stol(statVector[16]);
    startTime = std::stol(statVector[21]);

    ss.close();
  }

  float totalTime = float(uTime + sTime + cuTime+ csTime);
  float seconds = float(UpTime() - (startTime / hertz));

  return (totalTime / hertz) / seconds;
 }
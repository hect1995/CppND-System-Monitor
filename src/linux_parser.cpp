#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

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
  string os, version, kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string key, value;
  string line;
  string total_mem, free_mem, available_mem, buffers;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  short counter = 0;
  if (stream.is_open()) {
    while (std::getline(stream, line) && counter < 4){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key=="MemTotal:"){
        total_mem = value;
        counter ++;
      }else if (key=="MemFree:"){
        free_mem = value;
        counter ++;
      }else if (key == "MemAvailable:")
      {
        available_mem = value;
        counter ++;
      }else if (key == "Buffers"){
        buffers = value;
        counter ++;
      }
    }
  }
  return (stof(total_mem)-stof(free_mem))/stof(total_mem);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime, idle;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> uptime >> idle;
  }
  return stol(uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }


// TODO: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() {
  string line;
  string key;
  string user,nice,system,idle,iowait,irq,softirq,steal;
  std::ifstream stream(kProcDirectory + kStatFilename.substr(1));
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
  }
  return vector<long> {stol(user),stol(nice),stol(system),stol(idle),stol(iowait),stol(irq),
  stol(softirq),stol(steal)};
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream stream(kProcDirectory + kStatFilename.substr(1));
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes")
      {
        return stoi(value);
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream stream(kProcDirectory + kStatFilename.substr(1));
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running")
      {
        return stoi(value);
      }
    }
  }
  return 0;
}


// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  string key;
  string value;
  int counter = 0;
  std::ifstream stream(kProcDirectory + to_string(pid)+ kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value;
  int counter = 0;
  std::ifstream stream(kProcDirectory + to_string(pid)+ kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:")
      {
        return to_string(stoi(value)/1000); // value is in kB
      }
    }
  }
  return "0"; // It is returned as MB return string(); }
}
// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream stream(kProcDirectory + to_string(pid)+ kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
}
// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string user = LinuxParser::Uid(pid);
  string line,key,x,value;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> x >> value;
      if (value == user) {
        return key;
      }
    
    }
  }

  return string();  
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  string value;
  float starttime;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    int counter = 1;
    while( counter <23)
    {
      linestream >> value;
      if (counter==22){
        starttime=stof(value);
      }
      counter ++;
    }
  }
  return starttime;
}

std::vector<float> LinuxParser::CpuUtilization(int pid){
  string line;
  string value;
  float utime, stime, cutime,cstime,starttime;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    int counter = 1;
    while( counter <23)
    {
      linestream >> value;
      if (counter==14){utime=stof(value);}
      else if (counter==15){stime=stof(value);}
      else if (counter==16){cutime=stof(value);}
      else if (counter==17){cstime=stof(value);}
      else if (counter==22){starttime=stof(value);}
      counter ++;
    }
  }
  return {utime, stime, cutime,cstime,starttime};
}
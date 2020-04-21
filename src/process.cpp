#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int process_id){
    m_id = process_id;
}

// TODO: Return this process's ID
int Process::Pid() { return m_id; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    std::vector<float> data = LinuxParser::CpuUtilization(m_id);
    float total_time = data[LinuxParser::CPUProcess::kUtime_] + data[LinuxParser::CPUProcess::kStime_] + 
        data[LinuxParser::CPUProcess::kCutime_] + data[LinuxParser::CPUProcess::kCstime_];
    float seconds = LinuxParser::UpTime() - (data[LinuxParser::CPUProcess::kStarttime_] / sysconf(_SC_CLK_TCK));
    return (total_time / sysconf(_SC_CLK_TCK)) / seconds;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(m_id); }

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(m_id);
}

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(m_id); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(m_id); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process & a){ 
    return !(CpuUtilization() < a.CpuUtilization()); 
}
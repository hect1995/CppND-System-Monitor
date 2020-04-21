#include <string>

#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::vector<long> data = LinuxParser::CpuUtilization();
    // {user,nice,system,idle,iowait,irq,softirq,steal}
    long prev_idle = m_idle + m_iowait;
    long idle = data[LinuxParser::CPUStates::kIdle_] + data[LinuxParser::CPUStates::kIOwait_];

    long prev_non_idle = m_user + m_nice + m_system + m_irq + m_softirq + m_steal;
    long non_idle = data[LinuxParser::CPUStates::kUser_] + data[LinuxParser::CPUStates::kNice_]+ data[LinuxParser::CPUStates::kSystem_]+
    data[LinuxParser::CPUStates::kIRQ_] + data[LinuxParser::CPUStates::kSoftIRQ_] + data[LinuxParser::CPUStates::kSteal_];

    long prev_total = prev_idle + prev_non_idle;
    long total = idle + non_idle;

    // differentiate: actual value minus the previous one
    long totald = total - prev_total;
    long idled = idle - prev_idle;
    setMembers(data);
    return (totald - idled)/totald;    
}

void setMembers(std::vector<long> updated_members)
{
    
    
}
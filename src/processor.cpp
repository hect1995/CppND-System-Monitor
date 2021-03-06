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
    float totald = total - prev_total;
    float idled = idle - prev_idle;
    setMembers(data);
    return (totald - idled)/totald;    
}

void Processor::setMembers(std::vector<long> updated_members)
{
    m_user = updated_members[LinuxParser::CPUStates::kUser_];
    m_nice = updated_members[LinuxParser::CPUStates::kNice_];
    m_system = updated_members[LinuxParser::CPUStates::kSystem_];
    m_idle = updated_members[LinuxParser::CPUStates::kIdle_];
    m_iowait = updated_members[LinuxParser::CPUStates::kIOwait_];
    m_irq = updated_members[LinuxParser::CPUStates::kIRQ_];
    m_softirq = updated_members[LinuxParser::CPUStates::kSoftIRQ_];
    m_steal = updated_members[LinuxParser::CPUStates::kSteal_];
    
}
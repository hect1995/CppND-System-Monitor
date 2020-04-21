#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  void setMembers(std::vector<long>);
  // TODO: Declare any necessary private members
 private:
    long m_user{0};
    long m_nice{0};
    long m_system{0};
    long m_idle{0};
    long m_iowait{0};
    long m_irq{0};
    long m_softirq{0};
    long m_steal{0};
};

#endif
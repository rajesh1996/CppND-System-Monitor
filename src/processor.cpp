#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  std::vector<std::string> jiffies = LinuxParser::CpuUtilization();
  for (size_t i{0}; i<jiffies.size();i++){
    if (i!=LinuxParser::kGuest_ and i!=LinuxParser::kGuestNice_){
    	total_jiffies += std::stof(jiffies[i]);
    }
    if (i==LinuxParser::kIdle_ or i==LinuxParser::kIOwait_){
    	idle_jiffies += std::stof(jiffies[i]);
    }
  }
  used_jiffies = total_jiffies - idle_jiffies;
  total_usage = (used_jiffies-prev_used_jiffies)/(total_jiffies-prev_total_jiffies);
  prev_used_jiffies = used_jiffies;
  prev_total_jiffies = total_jiffies;
  return total_usage;
}
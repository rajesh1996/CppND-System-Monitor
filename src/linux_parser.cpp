#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

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
    linestream >> os >> version >> kernel;
  }
  return kernel;
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
  string key,value,line,k;
  float Total,Free;
  std::ifstream stream(kProcDirectory  + kMeminfoFilename);
  if(stream.is_open()) {
    while(std::getline(stream,line)) {
      std::replace(line.begin(),line.end(),':',' ');
      std::istringstream linestream(line);
    while(linestream>>key>>value>>k) {
      if(key=="MemTotal") {Total = std::stof(value);}
      if(key=="MemFree") {Free = std::stof(value);}
    }
    }
    return (Total-Free)/Total;
  }
  else
    return 0.0;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {  
  long up_Time;
  string line;
  std::ifstream stream(kProcDirectory+kUptimeFilename);
  if(stream.is_open()) {
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream>>up_Time;
  return up_Time;
  }
  else
    return 0;
  }

// TODO: Read and return the number of jiffies for the system
// long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
string value,line;
  long total_time;
vector<string> time_list;
std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
if(stream.is_open()) {
  std::getline(stream, line);
  std::istringstream linestream(line);
  while(linestream>>value) {
    time_list.push_back(value);
  }
}
  total_time = std::stol(time_list[13])+std::stol(time_list[14])+std::stol(time_list[15])+std::stol(time_list[16]);
  return total_time;
}
  


// TODO: Read and return the number of active jiffies for the system
// long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
// long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
 string value,line,key;
std::vector<string> cpuJiffies;
std::ifstream stream(kProcDirectory +kStatFilename);
if(stream.is_open()) {
std::getline(stream,line);
std::istringstream linestream(line);
while(linestream>>key) {
if(key!="cpu") {
cpuJiffies.push_back(key);
}
}
}
 return cpuJiffies;
}
  

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int val;
  string key, value, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") { 
          val = std::stoi(value);
          return val;
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string key, value, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") { 
              return std::stoi(value);
        }
      }
    }
  }
    return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
string cmd_val, line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      std::replace(line.begin(), line.end(), ' ', '*');
      while (linestream >> cmd_val) {
          std::replace(cmd_val.begin(), cmd_val.end(), '*', ' ');
          return cmd_val;
        }
    }
  }
    return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string key, value, k, line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value >> k) {
        if (key == "VmSize:") { 
          return std::to_string(std::stoi(value)/1000);
               }
        	}
      	}
    }
  return string();
}


// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    string key, value,line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") { 
          return value;
               }
      }
    }
    }
  return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string user, pass, key, line;
  string uid_ = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
        std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
    	while (linestream >> user >> pass >> key) {
               if (key==uid_){
                return user;
               }
        }
    }
  }
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string value, line;
  long int start_time, uptime;
  vector<string> time_list;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
			time_list.push_back(value);
        }
  start_time = std::stol(time_list[21])/sysconf(_SC_CLK_TCK);
  uptime =  LinuxParser::UpTime() - start_time;
  return uptime;
  }
  return 0;
}

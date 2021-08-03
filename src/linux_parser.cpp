#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

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

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memoryUti;
  float MemTotal;
  float MemFree;
  // float MemAvailable;
  // float Buffers;
  string line;
  string key;
  string value;
  string u;
  std::stringstream ss;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      // std::replace(line.begin(), line.end(), ':', ' ');
      while (linestream >> key >> value >> u) {
        if (key == "MemTotal:") {
          MemTotal = stof(value);
        } else if (key == "MemFree:") {
          MemFree = stof(value);
        }
        // else if(key == "MemAvailable") {
        //   MemAvailable=stof(value);
        // }
        // else if(key == "Buffers") {
        //   Buffers = stof(value);
        // }
      }
    }
  }
  memoryUti = (float)(MemTotal - MemFree) / MemTotal;

  return memoryUti;
}

// Done: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime;
  string systemup;
  string line;
  long uptimeInt{0};
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> systemup;
    uptimeInt = stol(uptime);
  }
  return uptimeInt;
}

// // TODO: Read and return the number of jiffies for the system
// long LinuxParser::Jiffies() { return 0; }

// // TODO: Read and return the number of active jiffies for a PID
// // REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// // TODO: Read and return the number of active jiffies for the system
// long LinuxParser::ActiveJiffies() { return 0; }

// // TODO: Read and return the number of idle jiffies for the system
// long LinuxParser::IdleJiffies() { return 0; }

// Done: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key;
  string value;
  vector<string> cpuValue;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "cpu") {
          while (linestream >> value) {
            cpuValue.emplace_back(value);
          }
          return cpuValue;
        }
      }
    }
  }
  return {};
}

// Done: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string totalProcString;
  int totalProcInt;
  std::stringstream ss;
  string line;
  string key;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> totalProcString) {
        if (key == "processes") {
          ss << totalProcString;
          ss >> totalProcInt;
          return totalProcInt;
        }
      }
    }
  }
  return 0;
}

// Done: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string totalProcString;
  int totalProcInt;
  std::stringstream ss;
  string line;
  string key;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> totalProcString) {
        if (key == "procs_running") {
          ss << totalProcString;
          ss >> totalProcInt;
          return totalProcInt;
        }
      }
    }
  }
  return 0;
}

// Done: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    //advised by reviewer: if the command is too long, use substr
    if (line.length() > 40) line = line.substr(0,40) + "...";
    return line;
  }
  return string();
}

// Done: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        // I was recommended by a reviewer that replacing VmSize bto VMRSS in
        // order to show the exact physical memory
        if (key == "VmRSS:") {
          if (std::all_of(value.begin(), value.end(), isdigit) && value != "") {
            value = to_string(stoi(value) / 1000);
            return value;
          }
        }
      }
    }
  }
  return string();
}

// Read and return the cpu used by a process
vector<string> LinuxParser::CPU(int pid) {
  string value;
  string line;
  vector<string> valueVector{};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      valueVector.emplace_back(value);
    }
    return valueVector;
  }
  return valueVector;
}

// Done: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
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

// Done: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string line;
  string value;
  string name;
  string x;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> name >> x >> value) {
        if (value == uid) {
          return name;
        }
      }
    }
  }
  return string();
}

// Done: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string value;
  string line;
  long int uptime = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    int i = 0;
    while (linestream >> value) {
      i++;
      if (i == 22) {
        // advised by a reviewer:
        // in order to get the unit of time it has been running since start you
        // need to subtract it from the UpTime() of the system
        uptime = UpTime() - stol(value) / sysconf(_SC_CLK_TCK);
        return uptime;
      }
    }
  }
  return 0;
}

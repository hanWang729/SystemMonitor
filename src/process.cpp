#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int p) {
  pid_ = p;
  username_ = User();
  cpu_ = CpuUtilization();
  string r = Ram();
  if (r != "") ram_ = stoi(r);
  uptime_ = UpTime();
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  vector<string> vs = LinuxParser::CPU(pid_);
  vector<int> vsi = {13, 14, 15, 16, 21};
  vector<int> v{};
  for (int i : vsi) {
    if (std::all_of(vs[i].begin(), vs[i].end(), isdigit) && vs[i] != "")
      v.emplace_back(stoi(vs[i]));
  }
  int uptime = LinuxParser::UpTime();
  int utime = v[0];
  int stime = v[1];
  int cutime = v[2];
  int cstime = v[3];
  int starttime = v[4];
  int total_time = utime + stime;
  total_time = total_time + cutime + cstime;
  float seconds = (float)uptime - (starttime / sysconf(_SC_CLK_TCK));
  float cpu_usage = (float)((total_time / sysconf(_SC_CLK_TCK)) / seconds);
  cpu_ = cpu_usage;
  return cpu_usage;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return a.cpu_ < cpu_ ? true : false;
}
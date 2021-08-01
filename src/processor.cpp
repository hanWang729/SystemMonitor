#include "processor.h"
#include "linux_parser.h"
using LinuxParser::CPUStates;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<string> cpuVectorString = LinuxParser::CpuUtilization();
  vector<int> cpuVector;
  for (std::string a : cpuVectorString) {
    cpuVector.emplace_back(stoi(a));
  }
  int PrevIdle = currentCPUVector[CPUStates::kIdle_] +
                 currentCPUVector[CPUStates::kIOwait_];
  int Idle = cpuVector[CPUStates::kIdle_] + cpuVector[CPUStates::kIOwait_];

  int a = CPUStates::kGuest_;
  std::cout<<a;
  int PrevNonIdle = currentCPUVector[CPUStates::kUser_] +
                    currentCPUVector[CPUStates::kNice_] +
                    currentCPUVector[CPUStates::kSystem_] +
                    currentCPUVector[CPUStates::kIRQ_] +
                    currentCPUVector[CPUStates::kSystem_] +
                    currentCPUVector[CPUStates::kSteal_];
  int NonIdle =
      cpuVector[CPUStates::kUser_] + cpuVector[CPUStates::kNice_] +
      cpuVector[CPUStates::kSystem_] + cpuVector[CPUStates::kIRQ_] +
      cpuVector[CPUStates::kSystem_] + cpuVector[CPUStates::kSteal_];

  int PrevTotal = PrevIdle + PrevNonIdle;
  int Total = Idle + NonIdle;

  int totald = Total - PrevTotal;
  int idled = Idle - PrevIdle;

  float CPU_Percentage = (float)(totald - idled) / totald;
  currentCPUVector = cpuVector;
  return CPU_Percentage;
}
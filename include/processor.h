#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>
#include <vector>
using std::string;
using std::vector;
class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
 vector<int> currentCPUVector {0,0,0,0,0,0,0,0,0,0};
};

#endif
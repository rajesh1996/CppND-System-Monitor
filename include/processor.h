#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <iostream>
#include "linux_parser.h"
#include <string>
#include <vector>
class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float total_jiffies{0.0},idle_jiffies{0.0},used_jiffies{0.0},prev_used_jiffies{0.0},prev_total_jiffies{0.0},total_usage{0.0};
};

#endif
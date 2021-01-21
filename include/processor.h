#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"
#include "unistd.h"

class Processor {
 public:
  float Utilization(); 
  void RefreshRate(int n) 
  {
    usleep(n);
  };
};

#endif
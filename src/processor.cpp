#include "processor.h"
#include "linux_parser.h"
#include "unistd.h"

float Processor::Utilization() { 
    std::vector<float> jiffies = LinuxParser::Jiffies();
    
    float prevTotalJiffies = 0;
    float preActiveJiffies = 0;

    RefreshRate(1000); //refresh at 100 ms
    float diffActiveJiffies = jiffies[0] - preActiveJiffies;
    float diffTotalJiffies = (jiffies[0] + jiffies[1]) - prevTotalJiffies;
    
    return diffActiveJiffies / diffTotalJiffies;
}
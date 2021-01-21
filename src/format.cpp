#include <sstream>
#include <string>
#include <iomanip>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) {
    std::ostringstream oss;
    int hr = seconds / 3600;
    long remainder = seconds % 3600;
    int min = remainder / 60;
    int sec = remainder % 3600;

    oss << std::setw(2) << std::setfill('0') << hr << ":" 
     << std::setw(2) << std::setfill('0') << min << ":"
     << std::setw(2) << std::setfill('0') << sec;
    
    return oss.str();
}
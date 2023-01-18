#include <string>

#include "format.h"

using std::string;
 
string Format::ElapsedTime(long seconds) 
{ 
    int h {seconds/3600};
    int m {(seconds - h*3600)/60 };
    int s {seconds - h*3600 - m *60};
    return std::to_string(h)+":"+ std::to_string(m)+":"+ std::to_string(s); }
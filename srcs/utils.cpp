#include <ctime>
#include <cstdio>
#include <string>
#include <sstream>

char* getTimeToStr()
{
  time_t rawtime;
  struct tm* timeinfo;
  static const char mon_name[][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };
  static char result[22];

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  sprintf(result, "%d/%s/%d %.2d:%.2d:%.2d",
    timeinfo->tm_mday,
    mon_name[timeinfo->tm_mon],
    1900 + timeinfo->tm_year,
    timeinfo->tm_hour,
    timeinfo->tm_min, timeinfo->tm_sec);
  return result;
}

template<typename T>
std::string i_to_string(T value)
{
  std::ostringstream ss;
  ss << value ;
  return std::string(ss.str());
}

std::string to_string(int value) {return i_to_string<int>(value);}
std::string to_string(long value) {return i_to_string<int>(value);}
std::string to_string(unsigned int value) {return i_to_string<int>(value);}
std::string to_string(unsigned long value) {return i_to_string<int>(value);}

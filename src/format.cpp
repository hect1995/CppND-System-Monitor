#include <string>

#include "format.h"

using std::string;

static const int HOUR = 3600;
static const int MIN = 60;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds_pass) {
    int hour=seconds_pass/HOUR;
    std::string hour_str = (hour<10) ? "0" + std::to_string(hour) : std::to_string(hour);
    int second=seconds_pass % HOUR;
    int minute=second/MIN;
    std::string minute_str = (minute<10) ? "0" + std::to_string(minute) : std::to_string(minute);
    second %= MIN;
    std::string second_str = (second<10) ? "0" + std::to_string(second) : std::to_string(second);
    return hour_str + "::" + minute_str + "::" + second_str;
}
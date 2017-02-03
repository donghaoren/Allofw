#include "allofw/allofw.h"

#ifdef _WIN32
#include <chrono>
#else
#include <sys/time.h>
#endif


ALLOFW_NS_BEGIN

void allofwInit() {
}

void allofwExit() {
}

#ifdef _WIN32
double get_time_seconds() {
	double time_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return (double)time_microseconds / 1.0e6;
}
#else
double get_time_seconds() {
    timeval t;
    gettimeofday(&t, 0);
    double s = t.tv_sec;
    s += t.tv_usec / 1000000.0;
    return s;
}
#endif

ALLOFW_NS_END

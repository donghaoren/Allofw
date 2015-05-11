#include "allofw/allofw.h"
#include <sys/time.h>


ALLOFW_NS_BEGIN

void allofwInit() {
}

void allofwExit() {
}

double get_time_seconds() {
    timeval t;
    gettimeofday(&t, 0);
    double s = t.tv_sec;
    s += t.tv_usec / 1000000.0;
    return s;
}

ALLOFW_NS_END

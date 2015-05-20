// timer.h

#ifndef TIMER_H
#define TIMER_H

// time in microseconds (usec)

#ifdef WIN32

#include <windows.h>
#include <winbase.h>
#include <time.h>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

class Timer {
public:

  LARGE_INTEGER freq, init_time, cur_time;

  Timer()
  {
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&init_time);
  }

  operator size_t() { return get(); }

  size_t get()
  {
    if(QueryPerformanceFrequency(&freq) && QueryPerformanceCounter(&cur_time))
      {
	return (size_t)((cur_time.QuadPart - init_time.QuadPart) * 1000000 / freq.QuadPart);
      }
    return 0;
  }
};

#else

#include <sys/time.h>

class Timer {

  timeval init_time, cur_time;

  public:

  Timer() { gettimeofday(&init_time, 0); }
  
  operator size_t() { return get(); }

  size_t get()
  {
    gettimeofday(&cur_time, 0);
    return (size_t)(cur_time.tv_sec - init_time.tv_sec)*1000000 
      + cur_time.tv_usec;
  }
};

#endif // WIN32

class StopTimer : public Timer {

  size_t t0, t1;

public:

  size_t dt;
  size_t count;
  size_t min, max;
  unsigned long long total;
  double mean, var;

  StopTimer() : 
    t0(0), t1(0), dt(0), count(0), 
    min(0), max(0), total(0), mean(0), var(0)
  { }

  void reset()
  {
    t0 = t1 = dt = 0;
    count = min = max = 0;
    total = 0;
    mean = var = 0;
  }

  void start() { t0 = get(); }

  void stop()
  { 
    t1 = get();

    if(t0 > t1) { dt = 0; return; }

    dt = t1 - t0;
  
    if(min == 0 || dt < min) min = dt;
    if(dt > max) max = dt;

    total += dt;

    count++;
    
     // update variance first
    if(count > 1)
      {
        // s^2 = s^2 (1 - 1 / (n-1)) + ((x - X@n-1)^2) / n
        float m = dt - mean;
	var = var * (1.0 - 1.0 / (count - 1)) + m * m / count;
      }

    // X = X@n-1 (1 - 1/n) + x / n
    mean = mean * (1.0 - 1.0 / count) + dt / count;
  }

  size_t value()
  {
    if(t0 > t1) return 0;
    return t1 - t0;
  }
};

#ifdef OSTREAM
inline std::ostream &operator<<(std::ostream &out, const StopTimer &t)
{
  out << " min=" << t.min << " max=" << t.max 
      << " mean=" << t.mean 
      << " var=" << sqrt(t.var)
      << " count=" << t.count;
  return out;
}
#endif

inline float hz(size_t t1, size_t t2) 
{
  size_t t = t2 - t1;
  if(t == 0) t = 1;
  return (float)1000000.0/t;
}

extern Timer _timer;

#define TIMER(funct) \
{ size_t t0 = _timer, t1; funct; t1 = _timer; cout << "t=" << t1-t0 << endl; }

#endif // TIMER_H

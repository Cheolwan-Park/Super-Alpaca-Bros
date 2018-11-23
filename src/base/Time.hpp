#ifndef BaseTime_hpp
#define BaseTime_hpp

#include "types.hpp"
#include "Array.hpp"
#include "Allocator.hpp"
#include <functional>
#include <chrono>

namespace Base {
class Timer;
class StopWatch;

class Time {
 public:
  typedef std::chrono::system_clock Clock;
  typedef std::chrono::duration<uint32_t, std::milli> milliseconds;
  typedef std::chrono::duration<float32, std::milli> fmilliseconds;

 private:
  struct Data {
    FixedArray<Timer *, 256> timers;
    Clock::time_point now_time;          // elapsed time from application starting
    milliseconds deltatime;             // delta time per frame
    milliseconds ideal_deltatime;       // aimed deltatime per frame
  };

  Time();

 public:

  Time(const Time &other) = delete;

  Time &operator=(const Time &other) = delete;

  ~Time();

  void update();

  int32_t addTimer(float64 time, std::function<int32_t()> &func);

  void removeTimer(int32_t index);

  void deleteAllTimer();

  void setIdealDeltatime(milliseconds ideal_time);

  float32 getDeltatime() const;

  float32 getElapsedtime() const;

  uint32_t getIDeltatime() const;

  uint32_t getIElapsedtime() const;

 public:
  static Time &Get();

 private:
  Data m_data;
};

class Timer {
 public:
  typedef std::function<int32_t()> Function;

 public:
  Timer() = delete;

  Timer(const Timer &other) = delete;

  Timer &operator=(const Timer &other) = delete;

  explicit Timer(float32 time, const Function &func);

  ~Timer();

  void update(float32 deltatime);

  void setIndex(int32_t index);

 private:
  Function m_func;
  float32 m_time;
  float32 m_elapsed;
  int32_t m_index;
};

class StopWatch {
 public:
  explicit StopWatch();

  StopWatch(const StopWatch &other);

  ~StopWatch();

  StopWatch &operator=(const StopWatch &other);

  void update();

  // get
  float32 getElapsedTime() const;

  float32 getSpeed() const;

  // set
  void reset();           // make elapsed time 0

  void setSpeed(float32 value);

  void pause();           // make speed 0

  void resume();          // make speed 1

 private:
  float32 m_elapsedtime;
  float32 m_speed;
};
}

#endif

#include "Time.hpp"
#include "Array.hpp"
#include "String.hpp"
#include <chrono>
#include <thread>

// Time class

namespace Base {
// Time class

Time::Time()
    : m_data() {
  ;
}

Time::~Time() {
  ;
}

void Time::update() {
  using std::chrono::duration_cast;
  using std::this_thread::sleep_for;

  Clock::time_point last_time = m_data.now_time;
  m_data.now_time = Clock::now();
  m_data.deltatime = duration_cast<milliseconds>(m_data.now_time - last_time);
  if (m_data.deltatime < m_data.ideal_deltatime) {
    sleep_for(m_data.ideal_deltatime - m_data.deltatime);
    m_data.now_time = Clock::now();
    m_data.deltatime = duration_cast<milliseconds>(m_data.now_time - last_time);
  } else {
    m_data.deltatime = m_data.ideal_deltatime;
  }

  for (size_t i = 0; i < m_data.timers.getMaxSize(); ++i) {
    if (nullptr != m_data.timers[i]) {
      m_data.timers[i]->update(getDeltatime());
    }
  }
}

int32 Time::addTimer(float64 time, std::function<int32()> &func) {
  for (int32 i = 0; i < m_data.timers.getMaxSize(); ++i) {
    if (nullptr == m_data.timers[i]) {
      Timer *new_timer = new Timer(time, func);
      m_data.timers[i] = new_timer;
      m_data.timers[i]->setIndex(i);
      return RET_SUCC;
    }
  }
  return RET_FAILED;
}

void Time::removeTimer(int32 index) {
  if (nullptr != m_data.timers[index]) {
    delete m_data.timers[index];
    m_data.timers[index] = nullptr;
  }
}

void Time::deleteAllTimer() {
  for (size_t i = 0; i < m_data.timers.getMaxSize(); ++i) {
    if (nullptr != m_data.timers[i]) {
      delete m_data.timers[i];
      m_data.timers[i] = nullptr;
    }
  }
}

void Time::setIdealDeltatime(const milliseconds ideal_time) {
  m_data.ideal_deltatime = ideal_time;
}

float32 Time::getDeltatime() const {
  return m_data.deltatime.count()/1000.0f;
}

float32 Time::getElapsedtime() const {
  return m_data.now_time.time_since_epoch().count()/1000.0f;
}

Uint32 Time::getIDeltatime() const {
  return m_data.deltatime.count();
}

Uint32 Time::getIElapsedtime() const {
  using std::chrono::duration_cast;
  return duration_cast<milliseconds>(m_data.now_time.time_since_epoch()).count();
}

// static
Time &Time::Get() {
  static Time instance;
  return instance;
}



// Timer class

Timer::Timer(float32 time, const Function &func)
    : m_func(func), m_time(time), m_elapsed(0.0f), m_index(0) {
  ;
}

Timer::~Timer() {
  ;
}

void Timer::update(float32 deltatime) {
  m_elapsed += deltatime;
  if (m_elapsed >= m_time) {
    m_func();
    Time &t = Time::Get();
    t.removeTimer(m_index);
  }
}

void Timer::setIndex(int32 index) {
  m_index = index;
}

// StopWatch class
StopWatch::StopWatch()
    : m_elapsedtime(0.0), m_speed(1.0) {
  ;
}

StopWatch::StopWatch(const StopWatch &other)
    : m_elapsedtime(other.m_elapsedtime), m_speed(other.m_speed) {
  ;
}

StopWatch::~StopWatch() {
  ;
}

StopWatch &StopWatch::operator=(const StopWatch &other) {
  assert(this != &other);
  this->m_elapsedtime = other.m_elapsedtime;
  this->m_speed = other.m_speed;
  return (*this);
}

void StopWatch::update() {
  Time &t = Time::Get();
  m_elapsedtime += t.getDeltatime() * m_speed;
}

float32 StopWatch::getElapsedTime() const {
  return m_elapsedtime;
}

float32 StopWatch::getSpeed() const {
  return m_speed;
}

void StopWatch::setSpeed(float32 value) {
  m_speed = value;
}

void StopWatch::reset() {
  m_elapsedtime = 0.0;
}

void StopWatch::pause() {
  m_speed = 0.0;
}

void StopWatch::resume() {
  m_speed = 1.0;
}
}

















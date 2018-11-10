#include "Schedule.hpp"
#include "Time.hpp"

namespace Base {

// Schedule class

Schedule::Schedule()
    : m_function(nullptr), m_flags() {
  ;
}

Schedule::Schedule(OBJECT_FUNCTION function)
    : m_function(function), m_flags() {
  ;
}

Schedule::Schedule(const Schedule &other)
    : m_function(other.m_function), m_flags() {
  ;
}

Schedule::~Schedule() {
  // TODO : remove from scheduler code
}

Schedule &Schedule::operator=(const Schedule &other) {
  assert(this != &other);
  this->m_function = other.m_function;
  return (*this);
}

void Schedule::update(GameObject *object) {
  if (!isPaused()) {
    assert(object);
    (object->*m_function)();
  }
}

int32 Schedule::isDeleted() const {
  return m_flags.getFlag(0);
}

int32 Schedule::isPaused() const {
  return m_flags.getFlag(1);
}

void Schedule::del() {
  m_flags.setFlag(0, true);
}

void Schedule::pause() {
  m_flags.setFlag(1, true);
}

void Schedule::resume() {
  m_flags.setFlag(1, false);
}


// ScheduleOnce class

ScheduleOnce::ScheduleOnce()
    : Schedule(), m_call_time(0) {
  ;
}

ScheduleOnce::ScheduleOnce(OBJECT_FUNCTION function, Uint32 wait_time)
    : Schedule(function), m_call_time(Time::Get().getIElapsedtime() + wait_time) {
  ;
}

ScheduleOnce::ScheduleOnce(const ScheduleOnce &other)
    : Schedule(other), m_call_time(other.m_call_time) {
  ;
}

ScheduleOnce::~ScheduleOnce() {
  ;
}

ScheduleOnce &ScheduleOnce::operator=(const ScheduleOnce &other) {
  assert(this != &other);
  Schedule::operator=(other);
  this->m_call_time = other.m_call_time;
  return (*this);
}

void ScheduleOnce::update(GameObject *object) {
  if (!isPaused()) {
    Time &t = Time::Get();
    if (m_call_time <= t.getIElapsedtime()) {
      Schedule::update(object);
      delete this;
    }
  }
}

Uint32 ScheduleOnce::getRemainTime() const {
  return (m_call_time - Time::Get().getIElapsedtime());
}

// IntervalSchedule class

ScheduleInterval::ScheduleInterval()
    : Schedule(), m_interval(0), m_next_time(0) {
  ;
}

ScheduleInterval::ScheduleInterval(OBJECT_FUNCTION function,
                                   Uint32 interval, Uint32 wait_time)
    : Schedule(function), m_interval(interval),
      m_next_time(Time::Get().getIElapsedtime() + wait_time) {
  ;
}

ScheduleInterval::ScheduleInterval(const ScheduleInterval &other)
    : Schedule(other), m_interval(other.m_interval),
      m_next_time(other.m_next_time) {
  ;
}

ScheduleInterval::~ScheduleInterval() {
  ;
}

ScheduleInterval &ScheduleInterval::operator=(const ScheduleInterval &other) {
  assert(this != &other);
  Schedule::operator=(other);
  this->m_interval = other.m_interval;
  this->m_next_time = other.m_next_time;
  return (*this);
}

void ScheduleInterval::update(GameObject *object) {
  if (!isPaused()) {
    Time &t = Time::Get();
    if (m_next_time <= t.getIElapsedtime()) {
      m_next_time = t.getIElapsedtime() + m_interval;
      Schedule::update(object);
    }
  }
}

Uint32 ScheduleInterval::getInterval() const {
  return m_interval;
}

Uint32 ScheduleInterval::getRemainTime() const {
  return (m_next_time - Time::Get().getIElapsedtime());
}

// Scheduler::ExistSchedule
Scheduler::AnySchedule::AnySchedule()
    : schedule() {
  ;
}

Scheduler::AnySchedule::~AnySchedule() {
  ;
}


// Scheduler class

Scheduler::Scheduler()
    : m_schedules(), m_schedule_count(0) {
  ;
}

Scheduler::Scheduler(const Scheduler &other)
    : m_schedules(other.m_schedules), m_schedule_count(other.m_schedule_count) {
  ;
}

Scheduler::~Scheduler() {

}

Scheduler &Scheduler::operator=(const Scheduler &other) {
  assert(this != &other);
  this->m_schedules = other.m_schedules;
  this->m_schedule_count = other.m_schedule_count;
  return (*this);
}

void Scheduler::update(GameObject *object) {
  class Schedule *schedule = nullptr;
  for (size_t i = 0; i < m_schedules.getMaxSize(); ++i) {
    if (m_schedules[i].IsExist) {
      schedule = &(m_schedules[i].schedule);
      if (schedule->isDeleted()) {
        memset((void *) schedule, 0, sizeof(AnySchedule));
        --m_schedule_count;
      } else {
        schedule->update(object);
      }
    }
  }
}

class Schedule *Scheduler::schedule(OBJECT_FUNCTION function) {
  assert(function);
  assert(m_schedule_count <= m_schedules.getMaxSize());

  class Schedule *schedule = &(getNewSchedulePos()->schedule);

  schedule = new(schedule) class Schedule(function);
  ++m_schedule_count;

  return schedule;
}

class ScheduleOnce *Scheduler::scheduleOnce(OBJECT_FUNCTION function, Uint32 wait_time) {
  assert(function);
  assert(m_schedule_count <= m_schedules.getMaxSize());

  class ScheduleOnce *schedule = &(getNewSchedulePos()->schedule_once);

  schedule = new(schedule) class ScheduleOnce(function, wait_time);
  ++m_schedule_count;

  return schedule;
}

class ScheduleInterval *Scheduler::scheduleInterval(OBJECT_FUNCTION function,
                                                    Uint32 interval, Uint32 wait_time) {
  assert(function);
  assert(m_schedule_count <= m_schedules.getMaxSize());

  class ScheduleInterval *schedule = &(getNewSchedulePos()->schedule_interval);

  schedule = new(schedule) class ScheduleInterval(function, interval, wait_time);
  ++m_schedule_count;

  return schedule;
}

int32 Scheduler::getScheduleCount() const {
  return m_schedule_count;
}

Scheduler::AnySchedule *Scheduler::getNewSchedulePos() {
  for (size_t i = 0; i < m_schedules.getMaxSize(); ++i) {
    if (!m_schedules[i].IsExist) {
      return (&m_schedules[i]);
    }
  }
  return nullptr;
}
}



























#ifndef BaseSchedule_hpp
#define BaseSchedule_hpp

#include "types.hpp"
#include "BitFlag.hpp"
#include "Array.hpp"
#include "String.hpp"

namespace Base {
class GameObject;
class Scheduler;

typedef void (GameObject::*OBJECT_FUNCTION)();

class Schedule {
 public:
  Schedule();

  explicit Schedule(OBJECT_FUNCTION function);

  Schedule(const Schedule &other);            // not copy flag

  virtual ~Schedule();

  Schedule &operator=(const Schedule &other); // not copy flag

  virtual void update(GameObject *object);

  // get
  int32_t isDeleted() const;

  int32_t isPaused() const;

  // set
  void del();

  void pause();

  void resume();

 private:
  OBJECT_FUNCTION m_function;
  BitFlag m_flags;
  /*
   * 0 : is Deleted
   * 1 : is Paused
   */
};

class ScheduleOnce : public Schedule {
 public:
  ScheduleOnce();

  explicit ScheduleOnce(OBJECT_FUNCTION function, uint32_t wait_time);

  ScheduleOnce(const ScheduleOnce &other);            // call same function same timing

  ~ScheduleOnce() override;

  ScheduleOnce &operator=(const ScheduleOnce &other); // call same function same timing

  void update(GameObject *object) override;

  // get
  uint32_t getRemainTime() const;

 private:
  uint32_t m_call_time;
};

class ScheduleInterval : public Schedule {
 public:
  ScheduleInterval();

  explicit ScheduleInterval(OBJECT_FUNCTION function, uint32_t interval, uint32_t wait_time);

  ScheduleInterval(const ScheduleInterval &other);                // reset next time

  ~ScheduleInterval() override;

  ScheduleInterval &operator=(const ScheduleInterval &other);     // reset next time

  void update(GameObject *object) override;

  // get
  uint32_t getInterval() const;

  uint32_t getRemainTime() const;

 private:
  uint32_t m_interval;
  uint32_t m_next_time;
};

class Scheduler {
 public:
  static constexpr uint32_t DEFAULT_SCHEDULE_ARRAY_SIZE = 8;

 private:
  union AnySchedule {
    AnySchedule();
    ~AnySchedule();
    Schedule schedule;
    ScheduleOnce schedule_once;
    ScheduleInterval schedule_interval;
    uint64_t IsExist;
  };

 public:
  explicit Scheduler();

  Scheduler(const Scheduler &other);

  ~Scheduler();

  Scheduler &operator=(const Scheduler &other);

  void update(GameObject *object);

  class Schedule *schedule(OBJECT_FUNCTION function);

  class ScheduleOnce *scheduleOnce(OBJECT_FUNCTION function, uint32_t wait_time);

  class ScheduleInterval *scheduleInterval(OBJECT_FUNCTION function,
                                           uint32_t interval, uint32_t wait_time);

  // get
  int32_t getScheduleCount() const;

 private:
  AnySchedule *getNewSchedulePos();

 private:
  FixedArray<AnySchedule, 8> m_schedules;
  int32_t m_schedule_count;
};
}

#endif





















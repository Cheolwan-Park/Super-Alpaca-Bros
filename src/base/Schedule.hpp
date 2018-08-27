#ifndef BaseSchedule_hpp
#define BaseSchedule_hpp

#include "types.hpp"
#include "BitFlag.hpp"
#include "Array.hpp"
#include "String.hpp"

namespace Base
{
    class GameObject;
    class Scheduler;
    
    typedef void (GameObject::*OBJECT_FUNCTION)();
    
    class Schedule
    {
    public:
        Schedule();
        
        explicit Schedule(OBJECT_FUNCTION function);
        
        Schedule(const Schedule &other);            // not copy flag
        
        virtual ~Schedule();
        
        Schedule &operator=(const Schedule &other); // not copy flag
        
        virtual void Update(GameObject *object);
        
        // get
        int32 IsDeleted()const;
        
        int32 IsPaused()const;
        
        // set
        void Delete();
        
        void Pause();
        
        void Resume();
        
    private:
        OBJECT_FUNCTION m_function;
        /*
         * 0 : is Deleted
         * 1 : is Paused
         */
        BitFlag m_flags;
    };
    
    class ScheduleOnce : public Schedule
    {
    public:
        ScheduleOnce();
        
        explicit ScheduleOnce(OBJECT_FUNCTION function, Uint32 wait_time);
        
        ScheduleOnce(const ScheduleOnce &other);            // call same function same timing
        
        virtual ~ScheduleOnce();
        
        ScheduleOnce &operator=(const ScheduleOnce &other); // call same function same timing
        
        virtual void Update(GameObject *object);
        
        // get
        Uint32 GetRemainTime()const;
        
    private:
        Uint32 m_calltime;
    };
    
    class ScheduleInterval : public Schedule
    {
    public:
        ScheduleInterval();

        explicit ScheduleInterval(OBJECT_FUNCTION function, Uint32 interval, Uint32 wait_time);
        
        ScheduleInterval(const ScheduleInterval &other);                // reset next time
        
        virtual ~ScheduleInterval();
        
        ScheduleInterval &operator=(const ScheduleInterval &other);     // reset next time
        
        virtual void Update(GameObject *object);
        
        // get
        Uint32 GetInterval()const;
        
        Uint32 GetRemainTime()const;
        
    private:
        Uint32 m_interval;
        Uint32 m_next_time;
    };
    
    class Scheduler
    {
    public:
        static constexpr Uint32 Default_ScheduleArraySize = 8;
        
    private:
        union AnySchedule
        {
            AnySchedule();
            ~AnySchedule();
            Schedule            schedule;
            ScheduleOnce        schedule_once;
            ScheduleInterval    schedule_interval;
            Uint64              IsExist;
        };
        
        
    public:
        explicit Scheduler();
        
        Scheduler(const Scheduler &other);
        
        ~Scheduler();
        
        Scheduler &operator=(const Scheduler &other);
        
        void Update(GameObject *object);
        
        class Schedule *Schedule(OBJECT_FUNCTION function);
        
        class ScheduleOnce *ScheduleOnce(OBJECT_FUNCTION function, Uint32 wait_time);
        
        class ScheduleInterval *ScheduleInterval(OBJECT_FUNCTION function,
                                                 Uint32 interval, Uint32 wait_time);
        
        // get
        int32 GetScheduleCount()const;
        
    private:
        AnySchedule *GetNewSchedulePos();
        
    private:
        FixedArray<AnySchedule, 8> m_schedules;
        int32 m_schedulecount;
    };
}


#endif





















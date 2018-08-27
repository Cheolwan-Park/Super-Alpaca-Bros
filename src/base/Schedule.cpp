#include "Schedule.hpp"
#include "Time.hpp"

namespace Base {
    
    // Schedule class
    
    Schedule::Schedule()
    :m_function(nullptr), m_flags()
    {
        ;
    }
    
    Schedule::Schedule(OBJECT_FUNCTION function)
    :m_function(function), m_flags()
    {
        ;
    }
    
    Schedule::Schedule(const Schedule &other)
    :m_function(other.m_function), m_flags()
    {
        ;
    }
    
    Schedule::~Schedule()
    {
        // TODO : remove from scheduler code
    }
    
    Schedule &Schedule::operator=(const Schedule &other)
    {
        assert(this != &other);
        this->m_function = other.m_function;
        return (*this);
    }
    
    void Schedule::Update(GameObject *object)
    {
        if(false == IsPaused())
        {
            assert(nullptr != object);
            (object->*m_function)();
        }
    }
    
    int32 Schedule::IsDeleted()const
    {
        return m_flags.GetFlag(0);
    }
    
    int32 Schedule::IsPaused()const
    {
        return m_flags.GetFlag(1);
    }
    
    void Schedule::Delete()
    {
        m_flags.SetFlag(0, true);
    }
    
    void Schedule::Pause()
    {
        m_flags.SetFlag(1, true);
    }
    
    void Schedule::Resume()
    {
        m_flags.SetFlag(1, false);
    }
    
    
    // ScheduleOnce class
    
    ScheduleOnce::ScheduleOnce()
    :Schedule(), m_calltime(0.0f)
    {
        ;
    }
    
    ScheduleOnce::ScheduleOnce(OBJECT_FUNCTION function, Uint32 wait_time)
    :Schedule(function), m_calltime(Time::Get().GetIElapsedtime() + wait_time)
    {
        ;
    }
    
    ScheduleOnce::ScheduleOnce(const ScheduleOnce &other)
    :Schedule(other), m_calltime(other.m_calltime)
    {
        ;
    }
    
    ScheduleOnce::~ScheduleOnce()
    {
        ;
    }
    
    ScheduleOnce &ScheduleOnce::operator=(const ScheduleOnce &other)
    {
        assert(this != &other);
        Schedule::operator=(other);
        this->m_calltime = other.m_calltime;
        return (*this);
    }
    
    void ScheduleOnce::Update(GameObject *object)
    {
        if(false == IsPaused())
        {
            Time &t = Time::Get();
            if(m_calltime <= t.GetIElapsedtime())
            {
                Schedule::Update(object);
                delete this;
            }
        }
    }
    
    Uint32 ScheduleOnce::GetRemainTime()const
    {
        return (m_calltime - Time::Get().GetIElapsedtime());
    }
    
    // IntervalSchedule class
    
    ScheduleInterval::ScheduleInterval()
    :Schedule(), m_interval(0), m_next_time(0)
    {
        ;
    }
    
    ScheduleInterval::ScheduleInterval(OBJECT_FUNCTION function,
                                       Uint32 interval, Uint32 wait_time)
    :Schedule(function), m_interval(interval),
    m_next_time(Time::Get().GetIElapsedtime() + wait_time)
    {
        ;
    }
    
    ScheduleInterval::ScheduleInterval(const ScheduleInterval &other)
    :Schedule(other), m_interval(other.m_interval),
    m_next_time(other.m_next_time)
    {
        ;
    }
    
    ScheduleInterval::~ScheduleInterval()
    {
        ;
    }
    
    ScheduleInterval &ScheduleInterval::operator=(const ScheduleInterval &other)
    {
        assert(this != &other);
        Schedule::operator=(other);
        this->m_interval = other.m_interval;
        this->m_next_time = other.m_next_time;
        return (*this);
    }
    
    void ScheduleInterval::Update(GameObject *object)
    {
        if(false == IsPaused())
        {
            Time &t = Time::Get();
            if(m_next_time <= t.GetIElapsedtime())
            {
                m_next_time = t.GetIElapsedtime() + m_interval;
                Schedule::Update(object);
            }
        }
    }
    
    Uint32 ScheduleInterval::GetInterval()const
    {
        return m_interval;
    }
    
    Uint32 ScheduleInterval::GetRemainTime()const
    {
        return (m_next_time - Time::Get().GetIElapsedtime());
    }
    
    
    // Scheduler::ExistSchedule
    Scheduler::AnySchedule::AnySchedule()
    :schedule()
    {
        ;
    }
    
    Scheduler::AnySchedule::~AnySchedule()
    {
        ;
    }
    
    
    // Scheduler class
    
    Scheduler::Scheduler()
    :m_schedules(), m_schedulecount(0)
    {
        ;
    }
    
    Scheduler::Scheduler(const Scheduler &other)
    : m_schedules(other.m_schedules), m_schedulecount(other.m_schedulecount)
    {
        ;
    }
    
    Scheduler::~Scheduler()
    {
        
    }
    
    Scheduler &Scheduler::operator=(const Scheduler &other)
    {
        assert(this != &other);
        this->m_schedules       = other.m_schedules;
        this->m_schedulecount   = other.m_schedulecount;
        return (*this);
    }
    
    void Scheduler::Update(GameObject *object)
    {
        class Schedule *schedule = nullptr;
        for(size_t i=0; i<m_schedules.GetMaxSize(); ++i)
        {
            if(true == m_schedules[i].IsExist)
            {
                schedule = &(m_schedules[i].schedule);
                if(true == schedule->IsDeleted())
                {
                    memset((void*)schedule, 0, sizeof(AnySchedule));
                    --m_schedulecount;
                }
                else
                {
                    schedule->Update(object);
                }
            }
        }
    }
    
    class Schedule *Scheduler::Schedule(OBJECT_FUNCTION function)
    {
        assert(nullptr != function);
        assert(m_schedulecount <= m_schedules.GetMaxSize());
        
        class Schedule *schedule = &(GetNewSchedulePos()->schedule);
        
        schedule = new (schedule) class Schedule(function);
        ++m_schedulecount;
        
        return schedule;
    }
    
    class ScheduleOnce *Scheduler::ScheduleOnce(OBJECT_FUNCTION function, Uint32 wait_time)
    {
        assert(nullptr != function);
        assert(m_schedulecount <= m_schedules.GetMaxSize());
        
        class ScheduleOnce *schedule = &(GetNewSchedulePos()->schedule_once);
        
        schedule = new (schedule) class ScheduleOnce(function, wait_time);
        ++m_schedulecount;
        
        return schedule;
    }
    
    class ScheduleInterval *Scheduler::ScheduleInterval(OBJECT_FUNCTION function,
                                                        Uint32 interval, Uint32 wait_time)
    {
        assert(nullptr != function);
        assert(m_schedulecount <= m_schedules.GetMaxSize());
        
        class ScheduleInterval *schedule = &(GetNewSchedulePos()->schedule_interval);
        
        schedule = new (schedule) class ScheduleInterval(function, interval, wait_time);
        ++m_schedulecount;
        
        return schedule;
    }
    
    int32 Scheduler::GetScheduleCount()const
    {
        return m_schedulecount;
    }
    
    Scheduler::AnySchedule *Scheduler::GetNewSchedulePos()
    {
        for(size_t i=0; i<m_schedules.GetMaxSize(); ++i)
        {
            if(false == m_schedules[i].IsExist)
            {
                return (&m_schedules[i]);
            }
        }
        return nullptr;
    }
}



























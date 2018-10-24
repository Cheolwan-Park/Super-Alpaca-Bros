#include "Time.hpp"
#include "Array.hpp"
#include "String.hpp"
#include <chrono>
#include <thread>

// Time class

namespace Base
{
    // Time class
    
    Time::Time()
    :m_data()
    {
        ;
    }
    
    Time::~Time()
    {
        ;
    }
    
    void Time::Update()
    {
        using std::chrono::duration_cast;
        using std::this_thread::sleep_for;
        
        Clock::time_point last_time = m_data.nowtime;
        m_data.nowtime = Clock::now();
        m_data.deltatime = duration_cast<milliseconds>(m_data.nowtime - last_time);
        if(m_data.deltatime < m_data.ideal_deltatime)
        {
            sleep_for(m_data.ideal_deltatime - m_data.deltatime);
            m_data.nowtime = Clock::now();
            m_data.deltatime = duration_cast<milliseconds>(m_data.nowtime - last_time);
        }
        else
        {
            m_data.deltatime = m_data.ideal_deltatime;
        }
        
        for(size_t i=0; i<m_data.timers.GetMaxSize(); ++i)
        {
            if(nullptr != m_data.timers[i])
            {
                m_data.timers[i]->Update(GetDeltatime());
            }
        }
    }
    
    int32 Time::AddTimer(float64 time, std::function<int32()> &func)
    {
        for(int32 i=0; i<m_data.timers.GetMaxSize(); ++i)
        {
            if(nullptr == m_data.timers[i])
            {
                Timer *newtimer = new Timer(time, func);
                m_data.timers[i] = newtimer;
                m_data.timers[i]->SetIndex(i);
                return RET_SUCC;
            }
        }
        return RET_FAILED;
    }
    
    void Time::RemoveTimer(int32 index)
    {
        if(nullptr != m_data.timers[index])
        {
            delete m_data.timers[index];
            m_data.timers[index] = nullptr;
        }
    }
    
    void Time::DeleteAllTimer()
    {
        for(size_t i=0; i<m_data.timers.GetMaxSize(); ++i)
        {
            if(nullptr != m_data.timers[i])
            {
                delete m_data.timers[i];
                m_data.timers[i] = nullptr;
            }
        }
    }
    
    void Time::SetIdealDeltatime(const milliseconds ideal_time)
    {
        m_data.ideal_deltatime = ideal_time;
    }
    
    float64 Time::GetDeltatime()const
    {
        return (m_data.deltatime.count()/1000.0);
    }
    
    float64 Time::GetElapsedtime()const
    {
        return (m_data.nowtime.time_since_epoch().count()/1000.0);
    }
    
    Uint32 Time::GetIDeltatime()const
    {
        return m_data.deltatime.count();
    }
    
    Uint32 Time::GetIElapsedtime()const
    {
        using std::chrono::duration_cast;
        return duration_cast<milliseconds>(m_data.nowtime.time_since_epoch()).count();
    }
    
    // static
    Time &Time::Get()
    {
        static Time instance;
        return instance;
    }
    
    
    
    // Timer class
    
    Timer::Timer(float64 time, const Function &func)
    :m_func(func), m_time(time), m_elapsed(0.0f), m_index(0)
    {
        ;
    }
    
    Timer::~Timer()
    {
        ;
    }
    
    void Timer::Update(float64 deltatime)
    {
        m_elapsed += deltatime;
        if(m_elapsed >= m_time)
        {
            m_func();
            Time &t = Time::Get();
            t.RemoveTimer(m_index);
        }
    }
    
    void Timer::SetIndex(int32 index)
    {
        m_index = index;
    }
    
    
    // StopWatch class
    StopWatch::StopWatch()
    :m_elapsedtime(0.0), m_speed(1.0)
    {
        ;
    }
    
    StopWatch::StopWatch(const StopWatch &other)
    :m_elapsedtime(other.m_elapsedtime), m_speed(other.m_speed)
    {
        ;
    }
    
    StopWatch::~StopWatch()
    {
        ;
    }
    
    StopWatch &StopWatch::operator=(const StopWatch &other)
    {
        assert(this != &other);
        this->m_elapsedtime = other.m_elapsedtime;
        this->m_speed = other.m_speed;
        return (*this);
    }
    
    void StopWatch::Update()
    {
        Time &t = Time::Get();
        m_elapsedtime += t.GetDeltatime()*m_speed;
    }
    
    float64 StopWatch::GetElapsedTime()const
    {
        return m_elapsedtime;
    }
    
     float64 StopWatch::GetSpeed()const
    {
        return m_speed;
    }
    
    void StopWatch::SetSpeed(float64 value)
    {
        m_speed = value;
    }
    
    void StopWatch::Reset()
    {
        m_elapsedtime = 0.0;
    }
    
    void StopWatch::Pause()
    {
        m_speed = 0.0;
    }
    
    void StopWatch::Resume()
    {
        m_speed = 1.0;
    }
}

















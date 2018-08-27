#ifndef BaseTime_hpp
#define BaseTime_hpp

#include "types.hpp"
#include "Array.hpp"
#include "Allocator.hpp"
#include <functional>
#include <chrono>

namespace Base
{
    class Timer;
    class StopWatch;
    
    class Time
    {
    public:
        typedef std::chrono::system_clock Clock;
        typedef std::chrono::duration<Uint32, std::milli> milliseconds;
        typedef std::chrono::duration<float64, std::milli> fmilliseconds;
        
    private:
        struct Data
        {
            FixedArray<Timer*, 256> timers;
            Clock::time_point nowtime;          // elapsed time from application starting
            milliseconds deltatime;             // delta time per frame
            milliseconds ideal_deltatime;       // aimed deltatime per frame
        };
        
        Time();
        
        Time(const Time &other) = delete;
        
        Time &operator=(const Time &other) = delete;
        
    public:
        ~Time();
        
        void Update();
        
        int32 AddTimer(float64 time, std::function<int32()> &func);
        
        void RemoveTimer(int32 index);
        
        void DeleteAllTimer();
        
        void SetIdealDeltatime(const milliseconds ideal_time);
        
        float64 GetDeltatime()const;
        
        float64 GetElapsedtime()const;
        
        Uint32 GetIDeltatime()const;
        
        Uint32 GetIElapsedtime()const;
        
    public:
        static Time &Get();
        
    private:
        Data m_data;
    };
    
    class Timer
    {
    public:
        typedef std::function<int32()> Function;
        
        Timer() = delete;
        Timer(const Timer &other) = delete;
        Timer &operator=(const Timer &other) = delete;
        
    public:
        explicit Timer(float64 time, const Function &func);
        
        ~Timer();
        
        void Update(float64 deltatime);
        
        void SetIndex(int32 index);
        
    private:
        Function m_func;
        float64 m_time;
        float64 m_elapsed;
        int32 m_index;
    };
    
    class StopWatch
    {
    public:
        explicit StopWatch();
        
        StopWatch(const StopWatch &other);
        
        ~StopWatch();
        
        StopWatch &operator=(const StopWatch &other);
        
        void Update();
        
        // get
        float64 GetElapsedTime()const;
        
        float64 GetSpeed()const;
        
        // set
        void Reset();           // make elapsed time 0
        
        void SetSpeed(float64 value);
        
        void Pause();           // make speed 0
        
        void Resume();          // make speed 1
        
    private:
        float64 m_elapsedtime;
        float64 m_speed;
    };
}

#endif

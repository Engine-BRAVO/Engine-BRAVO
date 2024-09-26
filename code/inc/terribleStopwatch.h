#ifndef TERRIBLESTOPWATCH_H
#define TERRIBLESTOPWATCH_H

#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>

#include <ctime>

namespace sc = boost::statechart;

struct IElapsedTime
{
    virtual double ElapsedTime() const = 0;
};

struct EvStartStop : sc::event<EvStartStop>
{
};
struct EvReset : sc::event<EvReset>
{
};

struct ActiveTS;
struct StopWatchTS : sc::state_machine<StopWatchTS, ActiveTS>
{
    double ElapsedTime() const
    {
        return state_cast<const IElapsedTime &>().ElapsedTime();
    }
};

struct StoppedTS;

struct ActiveTS : sc::simple_state<ActiveTS, StopWatchTS, StoppedTS>
{
public:
    typedef sc::transition<EvReset, ActiveTS> reactions;

    ActiveTS() : elapsedTime_(0.0) {}
    double ElapsedTime() const { return elapsedTime_; }
    double &ElapsedTime() { return elapsedTime_; }

private:
    double elapsedTime_;
};

struct RunningTS : IElapsedTime, sc::simple_state<RunningTS, ActiveTS>
{
public:
    typedef sc::transition<EvStartStop, StoppedTS> reactions;

    RunningTS() : startTime_(std::time(0)) {}
    ~RunningTS()
    {
        context<ActiveTS>().ElapsedTime() = ElapsedTime();
    }

    virtual double ElapsedTime() const
    {
        return context<ActiveTS>().ElapsedTime() +
               std::difftime(std::time(0), startTime_);
    }

private:
    std::time_t startTime_;
};

struct StoppedTS : IElapsedTime, sc::simple_state<StoppedTS, ActiveTS>
{
    typedef sc::transition<EvStartStop, RunningTS> reactions;
    virtual double ElapsedTime() const
    {
        return context<ActiveTS>().ElapsedTime();
    }
};

class terribleStopwatch
{
public:
    void run();
};

#endif // TERRIBLESTOPWATCH_H
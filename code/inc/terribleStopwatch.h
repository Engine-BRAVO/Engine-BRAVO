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

struct Active;
struct StopWatch : sc::state_machine<StopWatch, Active>
{
    double ElapsedTime() const
    {
        return state_cast<const IElapsedTime &>().ElapsedTime();
    }
};

struct Stopped;

// The simple_state class template accepts up to four parameters:
// - The third parameter specifies the inner initial state, if
//   there is one. Here, only Active has inner states, which is
//   why it needs to pass its inner initial state Stopped to its
//   base
// - The fourth parameter specifies whether and what kind of
//   history is kept

// Active is the outermost state and therefore needs to pass the
// state machine class it belongs to
struct Active : sc::simple_state<
                    Active, StopWatch, Stopped>
{
public:
    typedef sc::transition<EvReset, Active> reactions;

    Active() : elapsedTime_(0.0) {}
    double ElapsedTime() const { return elapsedTime_; }
    double &ElapsedTime() { return elapsedTime_; }

private:
    double elapsedTime_;
};

// Stopped and Running both specify Active as their Context,
// which makes them nested inside Active
struct Running : IElapsedTime, sc::simple_state<Running, Active>
{
public:
    typedef sc::transition<EvStartStop, Stopped> reactions;

    Running() : startTime_(std::time(0)) {}
    ~Running()
    {
        context<Active>().ElapsedTime() = ElapsedTime();
    }

    virtual double ElapsedTime() const
    {
        return context<Active>().ElapsedTime() +
               std::difftime(std::time(0), startTime_);
    }

private:
    std::time_t startTime_;
};

struct Stopped : IElapsedTime, sc::simple_state<Stopped, Active>
{
    typedef sc::transition<EvStartStop, Running> reactions;
    virtual double ElapsedTime() const
    {
        return context<Active>().ElapsedTime();
    }
};

// Because the context of a state must be a complete type (i.e.
// not forward declared), a machine must be defined from
// "outside to inside". That is, we always start with the state
// machine, followed by outermost states, followed by the direct
// inner states of outermost states and so on. We can do so in a
// breadth-first or depth-first way or employ a mixture of the
// two.

class terribleStopwatch
{
public:
    void run();
};

#endif // TERRIBLESTOPWATCH_H
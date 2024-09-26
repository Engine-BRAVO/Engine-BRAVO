#ifndef STOPWATCH_H
#define STOPWATCH_H

/**
 * @file stopwatch.cpp
 * @brief Implementation of a stopwatch using Boost Statechart library.
 *
 * This file contains the implementation of a simple stopwatch state machine
 * using the Boost Statechart library. The stopwatch has four states: NotActive,
 * Active, Stop, and Running. It transitions between these states based on
 * events such as MakeActive, StartStop, and Reset.
 *
 * The state machine is defined as follows:
 * - Stopwatch: The state machine class.
 * - NotActive: The initial state of the stopwatch.
 * - Active: The state when the stopwatch is active.
 * - Stop: The state when the stopwatch is stopped.
 * - Running: The state when the stopwatch is running.
 *
 * Events:
 * - MakeActive: Event to transition from NotActive to Active.
 * - StartStop: Event to toggle between Stop and Running states.
 * - Reset: Event to reset the stopwatch to the Stop state.
 *
 * Namespaces:
 * - sc: Alias for boost::statechart namespace.
 * - mpl: Alias for boost::mpl namespace.
 *
 * The main function initiates the state machine and simulates events to
 * demonstrate state transitions.
 *
 * @dependencies
 * - Boost Statechart library
 * - Boost MPL library
 * - iostream for console output
 * - chrono and thread for timing and delays
 */

#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/in_state_reaction.hpp>
#include <boost/mpl/list.hpp>
#include <iostream>
#include <chrono>
#include <thread>

namespace sc = boost::statechart;
namespace mpl = boost::mpl;

// Forward declarations of states
struct NotActive;
struct Active;
struct Stop;
struct Running;

// Events
struct MakeActive : sc::event<MakeActive>
{
};
struct StartStop : sc::event<StartStop>
{
};
struct Reset : sc::event<Reset>
{
};

// State machine definition
/**
 * @struct Stopwatch
 * @brief State machine definition for the stopwatch.
 *
 * @details
 * Inherits from sc::state_machine<Stopwatch, NotActive>.
 * - Stopwatch: The state machine class.
 * - NotActive: The initial state of the stopwatch.
 */
struct Stopwatch : sc::state_machine<Stopwatch, NotActive>
{
    void printElapsedTime()
    {
        auto end = running ? std::chrono::steady_clock::now() : end_time;
        auto elapsed = accumulated_time + std::chrono::duration_cast<std::chrono::seconds>(end - start_time);
        std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    std::chrono::time_point<std::chrono::steady_clock> end_time;
    std::chrono::seconds accumulated_time{0};
    bool running = false;

    void start()
    {
        start_time = std::chrono::steady_clock::now();
        running = true;
    }

    void stop()
    {
        end_time = std::chrono::steady_clock::now();
        accumulated_time += std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
        running = false;
    }

    void reset()
    {
        start_time = std::chrono::steady_clock::now();
        end_time = start_time;
        accumulated_time = std::chrono::seconds{0};
    }

    // Make state classes friends to allow them to call private methods
    friend struct NotActive;
    friend struct Active;
    friend struct Stop;
    friend struct Running;
};

// NotActive state definition
/**
 * @struct NotActive
 * @brief State when the stopwatch is not active.
 *
 * @details
 * Inherits from sc::simple_state<NotActive, Stopwatch>.
 * - NotActive: The state class.
 * - Stopwatch: The state machine class.
 *
 * Reactions:
 * - sc::transition<MakeActive, Active>: Transition to Active state on MakeActive event.
 * - sc::in_state_reaction<sc::event_base>: Reaction to any event while in this state.
 */
struct NotActive : sc::simple_state<NotActive, Stopwatch>
{
    typedef mpl::list<
        sc::transition<MakeActive, Active>,
        sc::in_state_reaction<sc::event_base>>
        reactions;

    NotActive()
    {
        std::cout << "Entering NotActive state" << std::endl;
    }

    ~NotActive()
    {
        std::cout << "Exiting NotActive state" << std::endl;
    }
};

// Active state definition
/**
 * @struct Active
 * @brief State when the stopwatch is active.
 *
 * @details
 * Inherits from sc::simple_state<Active, Stopwatch, Stop>.
 * - Active: The state class.
 * - Stopwatch: The state machine class.
 * - Stop: The initial substate of Active.
 *
 * Reactions:
 * - sc::transition<StartStop, Running>: Transition to Running state on StartStop event.
 * - sc::transition<Reset, Stop>: Transition to Stop state on Reset event.
 */
struct Active : sc::simple_state<Active, Stopwatch, Stop>
{
    typedef mpl::list<
        sc::transition<StartStop, Running>,
        sc::transition<Reset, Stop>>
        reactions;

    Active()
    {
        std::cout << "Entering Active state" << std::endl;
    }

    ~Active()
    {
        std::cout << "Exiting Active state" << std::endl;
    }
};

// Stop state definition
/**
 * @struct Stop
 * @brief State when the stopwatch is stopped.
 *
 * @details
 * Inherits from sc::simple_state<Stop, Active>.
 * - Stop: The state class.
 * - Active: The parent state class.
 *
 * Reactions:
 * - sc::transition<StartStop, Running>: Transition to Running state on StartStop event.
 */
struct Stop : sc::simple_state<Stop, Active>
{
    typedef sc::transition<StartStop, Running> reactions;

    Stop()
    {
        std::cout << "Entering Stop state" << std::endl;
    }

    ~Stop()
    {
        std::cout << "Exiting Stop state" << std::endl;
    }
};

// Running state definition
/**
 * @struct Running
 * @brief State when the stopwatch is running.
 *
 * @details
 * Inherits from sc::simple_state<Running, Active>.
 * - Running: The state class.
 * - Active: The parent state class.
 *
 * Reactions:
 * - sc::transition<StartStop, Stop>: Transition to Stop state on StartStop event.
 */
struct Running : sc::state<Running, Active>
{
    typedef sc::transition<StartStop, Stop> reactions;

    Running(my_context ctx) : my_base(ctx)
    {
        std::cout << "Entering Running state" << std::endl;
        outermost_context().start();
    }

    ~Running()
    {
        std::cout << "Exiting Running state" << std::endl;
        outermost_context().stop();
    }
};

class stopwatch
{
public:
    void runStopwatch();
};

#endif // STOPWATCH_H
#ifndef TIMER_H
#define TIMER_H

#include <cstdint>
#include <random>
#include <chrono>

class Timer
{
public:
    // In public instead of private so they can be referenced elsewhere?
    using Nanos = std::chrono::nanoseconds;
    using Micros = std::chrono::microseconds;
    using Millis = std::chrono::milliseconds;
    using Seconds = std::chrono::seconds;
    using Minutes = std::chrono::minutes;
    using Hours = std::chrono::hours;

    // Constructor
    Timer() : lastClickTime(std::chrono::steady_clock::now()) {}

    // Restart
    // Updates the lastClickTime
    void restart() {
        lastClickTime = std::chrono::steady_clock::now();
    }

    // click()
    // Updates the lastClickTime and returns timeElapsed since
    template <typename T> 
    uint64_t click() {
        // assert(T in [Nanos, Micros, Millis, Seconds, Minutes, Hours]);
        auto clickTime = std::chrono::steady_clock::now();  // Gets the time as of now
        auto timeElapsed = std::chrono::duration_cast<T>(clickTime - lastClickTime).count();
        lastClickTime = clickTime;  // Updates the last click time to the most recent click time
        return timeElapsed;
    }

    // glance()
    // Returns the timeElapsed since the lastClickTime
    template <typename T>
    uint64_t glance() const {
        auto glanceTime = std::chrono::steady_clock::now();  // Gets time as of now
        auto timeElapsed = std::chrono::duration_cast<T>(glanceTime - lastClickTime).count();
        return timeElapsed;
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> lastClickTime;
};

#endif  // TIMER_H
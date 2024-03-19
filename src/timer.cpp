#include "timer.hpp"

#include <iostream>
#include <memory>

namespace opengles_workspace
{
std::unique_ptr<Timer> Timer::Instance() {
    static std::unique_ptr<Timer> instance = std::make_unique<Timer>(Timer());

    return std::move(instance);
}

Timer::Timer() 
    : time_scale_(0.5f)
{
    Reset();
}

void Timer::Reset() {
    start_time_ = std::chrono::system_clock::now();
}

void Timer::SetTimeScale(float t /* = 1.0f */) {
    time_scale_ = t;
}

float Timer::GetTimeScale() const {
    return time_scale_;
}

float Timer::CalculateDeltaTime() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time_).count() / 1000.0f;
}

bool Timer::IsTimeUp() {
     if(!(CalculateDeltaTime() >= time_scale_))
        return false;
    
    Reset();
    return true;
}

} // namespace opengles_workspace
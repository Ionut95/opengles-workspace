#include "timer.hpp"
#include <iostream>

using namespace std::literals;

namespace opengles_workspace
{
Timer* Timer::s_instance_ = nullptr;
float Timer::frame_rate_ = 1.0f / 30.0f;

Timer* Timer::Instance() {
    if(s_instance_ == nullptr)
        s_instance_ = new Timer();
    
    return s_instance_;
}

bool Timer::IsInitialized() {
    return s_instance_ != nullptr;
}

Timer::Timer() {
    Reset();
    time_scale_ = 1.0f;
    delta_time_ = std::chrono::duration<float>(0.0f);
}

Timer::~Timer() {
    delete s_instance_;
    s_instance_ = nullptr;
}

void Timer::Reset() {
    start_time_ = std::chrono::system_clock::now();
}

float Timer::GetDeltaTime() const {
    return delta_time_.count();
}

void Timer::SetTimeScale(float t /* = 1.0f */) {
    time_scale_ = t;
}

float Timer::GetTimeScale() const {
    return time_scale_;
}

void Timer::Tick() {
    delta_time_ = std::chrono::system_clock::now() - start_time_;
}

} // namespace opengles_workspace
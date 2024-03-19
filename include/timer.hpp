#pragma once

#include <chrono>
#include <memory>

namespace opengles_workspace
{
class Timer {
    public:
        static std::unique_ptr<Timer> Instance();
        
        void Reset();
        void SetTimeScale(float t = 1.0f);
        float GetTimeScale() const;
        float CalculateDeltaTime() const;
        bool IsTimeUp();
        
        ~Timer() = default;
    
    private:
        Timer();

        std::chrono::system_clock::time_point start_time_;
        float time_scale_;
};
} // namespace opengles_workspace
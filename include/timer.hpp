#pragma once

#include <chrono>

namespace opengles_workspace
{
class Timer {
    public:
        static Timer* Instance();
        static void Release();
        static bool IsInitialized();
        
        void Reset();
        float GetDeltaTime() const;
        void SetTimeScale(float t = 1.0f);
        float GetTimeScale() const;
        void Tick();
        
        static float frame_rate_;
    
    private:
        Timer();
        ~Timer();

        static Timer* s_instance_;

        std::chrono::system_clock::time_point start_time_;
        std::chrono::duration<float> delta_time_;
        float time_scale_;
};
} // namespace opengles_workspace
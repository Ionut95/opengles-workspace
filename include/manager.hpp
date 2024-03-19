#pragma once

#include <memory>
#include <cstddef>

#include "bouncing_box_state.hpp"
#include "polled_object.hpp"
#include "renderer.hpp"
#include "timer.hpp"

namespace opengles_workspace
{
class Manager: public PolledObject
{
    public:
        Manager(std::shared_ptr<BouncingBoxState> pBouncingBoxState, std::shared_ptr<GLFWRenderer> pGLFWRenderer);
        ~Manager() = default;

        bool poll() override;
    
    private:
        void CheckBouncingBoxState();
        void UpdateData();
        void CheckTimer() const;

        std::size_t row_to_be_compared_;
        std::size_t column_to_be_compared_;

        std::shared_ptr<BouncingBoxState> p_bouncing_box_state_;
        std::shared_ptr<GLFWRenderer> p_glfw_renderer_;
        std::unique_ptr<Timer> p_timer_;
};
} // opengles_workspace
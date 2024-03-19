#include "manager.hpp"

#include <chrono>
#include <iostream>

namespace opengles_workspace
{
Manager::Manager(std::shared_ptr<BouncingBoxState> pBouncingBoxState, std::shared_ptr<GLFWRenderer> pGLFWRenderer)
    : p_bouncing_box_state_(pBouncingBoxState)
    , p_glfw_renderer_(pGLFWRenderer)
    , p_timer_(Timer::Instance())
    , row_to_be_compared_(pBouncingBoxState->GetObjectRow())
    , column_to_be_compared_(pBouncingBoxState->GetObjectColumn()) {}

bool Manager::poll()
{
    CheckTimer();
    CheckBouncingBoxState();

    return true;
}

void Manager::CheckBouncingBoxState()
{
    if(row_to_be_compared_ != p_bouncing_box_state_->GetObjectRow() || column_to_be_compared_ != p_bouncing_box_state_->GetObjectColumn())
    {
        UpdateData();
        p_glfw_renderer_->render(
            p_bouncing_box_state_->GetObjectRow(),
            p_bouncing_box_state_->GetObjectColumn(),
            p_bouncing_box_state_->GetNrRows(),
            p_bouncing_box_state_->GetNrColumns()
        );
    }
}

void Manager::CheckTimer() const
{
    if(p_timer_->IsTimeUp())
        p_bouncing_box_state_->ChangeState();
}

void Manager::UpdateData()
{
    row_to_be_compared_ = p_bouncing_box_state_->GetObjectRow();
    column_to_be_compared_ = p_bouncing_box_state_->GetObjectColumn();
}
} // opengles_workspace
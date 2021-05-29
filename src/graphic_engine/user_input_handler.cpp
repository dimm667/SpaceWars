#include "user_input_handler.h"

void InputHandler::Callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    for(auto listener : listeners_)
    {
        listener->Notify(key, action);
    }
}

void InputHandler::RegisterListener(IUserInputListener& listener)
{
    listeners_.push_back(&listener);
}

void InputHandler::RemoveListener(IUserInputListener& listener)
{
    auto listener_it = std::find(listeners_.begin(), listeners_.end(), &listener);
    if(listener_it != listeners_.end())
        listeners_.erase(listener_it);
}

#ifndef USER_INPUT_HANDLER
#define USER_INPUT_HANDLER

#include <GLFW/glfw3.h>
#include <list>
#include <algorithm>

class IUserInputListener
{
public:
    virtual void Notify(int key, int action) = 0;
};


class InputHandler
{
public:
    static void Callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void RegisterListener(IUserInputListener& listener);
    static void RemoveListener(IUserInputListener& listener);

private:
    inline static std::list<IUserInputListener*> listeners_{};
};

#endif // USER_INPUT_HANDLER

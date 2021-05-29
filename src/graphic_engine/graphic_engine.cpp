#include "graphic_engine.h"
#include "user_input_handler.h"

GraphicEngine& GraphicEngine::GetInstance()
{
    static GraphicEngine engine;
    return engine;
}

GraphicEngine::~GraphicEngine()
{
    glfwTerminate();
}

void GraphicEngine::Draw(std::initializer_list<IDrawable *> drawable_objects)
{
    mainViewPort_->Activate();
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(auto& object : drawable_objects)
    {
        object->Draw(screenWidth_, screenHeight_);
    }

    glfwSwapBuffers(window_);
    glfwPollEvents();
}

GraphicEngine::GraphicEngine()
{
    glfwInit();

    auto* monitor{glfwGetPrimaryMonitor()};

    const auto* monitor_mode{glfwGetVideoMode(monitor)};

    screenWidth_ = monitor_mode->width;
    screenHeight_ = monitor_mode->height;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    glfwWindowHint(GLFW_RED_BITS, monitor_mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, monitor_mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, monitor_mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, monitor_mode->refreshRate);

    window_ = glfwCreateWindow(screenWidth_, screenHeight_, "SpaceWars", monitor, nullptr);
    if (window_ == nullptr)
    {
        glfwTerminate();
            // return -1;  TODO: make exception
    }
    glfwMakeContextCurrent(window_);

    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    mainViewPort_.reset(new Viewport(0, 0, screenWidth_, screenHeight_));

    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetKeyCallback(window_, &InputHandler::Callback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
}

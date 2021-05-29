#ifndef GRAPHIC_ENGINE_H
#define GRAPHIC_ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include "i_drawable.h"
#include "viewport.h"
#include <memory>
#include <initializer_list>

class GraphicEngine
{
public:
    GraphicEngine(const GraphicEngine& other) = delete;
    GraphicEngine(const GraphicEngine&& other) = delete;

    static GraphicEngine& GetInstance();

    ~GraphicEngine();

    void Draw(std::initializer_list<IDrawable *> drawable_objects);

private:
    GraphicEngine();

    GLuint screenWidth_{};
    GLuint screenHeight_{};
    GLFWwindow* window_{nullptr};
    std::unique_ptr<Viewport> mainViewPort_{};
};

#endif // GRAPHIC_ENGINE_H

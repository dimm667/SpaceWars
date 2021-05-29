#ifndef GRAPHIC_OBJECT_POOL_H
#define GRAPHIC_OBJECT_POOL_H

#include "graphic_engine/i_drawable.h"
#include "graphic_engine/uniform_structures.h"
#include "graphic_engine/uniform_object.h"
#include "graphic_engine/object_model.h"
#include "graphic_engine/shader_program.h"
#include <list>
#include <unordered_map>
#include "game_object.h"

class ObjectsDrawer : public IDrawable
{
public:
    ObjectsDrawer()
    {
        uniformObj_.bindToShader(shader_program_, "TViewProjMatrices");
    }

    void UpdateObjectListReference(std::list<std::shared_ptr<PhysicObjectWithType>>& objects)
    {
        objects_ = &objects;
    }

    void AddModel(const GameObjectType object_type, const std::string& path_to_model)
    {
        objects_models_.emplace(object_type, path_to_model);
    }

    void Draw(const unsigned int screen_width, const unsigned int screen_height) override
    {
        PrepareShader();

        for(auto& object : *objects_)
        {
            DrawObject(objects_models_.at(object->GetType()).GetModel(), object->GetPosition(), object->GetHeading(), screen_width, screen_height);
        }
    }

private:

    void PrepareShader()
    {
        shader_program_.use();
        shader_program_.SetUniform("viewPos", glm::vec3(0.0f, 0.0f, 100.0f));
        shader_program_.SetUniform("directionalLight.direction", glm::vec3(-0.5f, -1.0f, -0.5f));
        shader_program_.SetUniform("directionalLight.ambient",  glm::vec3(0.7f, 0.7f, 0.7f));
        shader_program_.SetUniform("directionalLight.diffuse",  glm::vec3(0.0f, 0.0f, 0.0f));
        shader_program_.SetUniform("directionalLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
    }

    void DrawObject(const VoxelArray& model_, const Point& position, const double heading, const unsigned int screen_width, const unsigned int screen_height)
    {
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screen_width), 0.0f, static_cast<float>(screen_height), -1000.0f, 1000.0f);

        glm::mat4 abs_position = glm::translate(glm::vec3{position.x, position.y, 0.0}) *
                                 glm::rotate(static_cast<float>(heading), glm::vec3{0.0, 0.0, 1.0});
        model_proxy_ = abs_position;
        projViewProxy_ = projection;
        model_.draw();
    }
    ShaderProgram shader_program_{{{"src/graphic_engine/shaders/vertex_instancing.glsl", GL_VERTEX_SHADER},
                                   {"src/graphic_engine/shaders/fragment_vertex.glsl", GL_FRAGMENT_SHADER}}};

    std::list<std::shared_ptr<PhysicObjectWithType>> * objects_{nullptr};
    std::unordered_map<GameObjectType, ObjectModel, GameObjectTypeHasher> objects_models_{};

    UniformObject<TViewProjMatrices> uniformObj_{};
    UniformObject<TViewProjMatrices>::UniformProxy<glm::mat4> model_proxy_{uniformObj_.getProxy<glm::mat4>(offsetof(TViewProjMatrices, model), sizeof(glm::mat4))};
    UniformObject<TViewProjMatrices>::UniformProxy<glm::mat4> projViewProxy_{uniformObj_.getProxy<glm::mat4>(offsetof(TViewProjMatrices, proj_view), sizeof(glm::mat4))};
};

#endif // GRAPHIC_OBJECT_POOL_H

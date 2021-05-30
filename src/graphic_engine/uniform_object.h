#ifndef UNIFORMOBJECT_H
#define UNIFORMOBJECT_H

#include <glad/glad.h>
#include <type_traits>
#include <string>
#include <stdexcept>

#include "shader_program.h"

class UniformObjectMonitor
{
    static const unsigned int maxBindingPoints = GL_MAX_UNIFORM_BUFFER_BINDINGS;
protected:
    UniformObjectMonitor();
    virtual ~UniformObjectMonitor();
    int own_binding_point_;

private:
    static int current_binding_point_;
};

template <class T, typename std::enable_if<
              std::is_trivial<T>::value &&
              std::is_standard_layout<T>::value, int>::type = 0>
class UniformObject : private UniformObjectMonitor
{
public:
    UniformObject()
    {
        glGenBuffers(1, &ubo_);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(T), NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void bindToShader(const ShaderProgram & shader, std::string name)
    {
        unsigned int ub_index = glGetUniformBlockIndex(shader.getId(), name.c_str());
        if(ub_index == GL_INVALID_INDEX)
        {
            throw std::runtime_error(std::string("ERROR::ubo_::BINDING\n") + std::string("Can't find specidied uniform object in the shader programm.\n"));
        }
        else if(glGetError() == GL_INVALID_OPERATION)
        {
            throw std::runtime_error(std::string("ERROR::ubo_::BINDING\n") + std::string("Can't find specidied shader programm.\n"));
        }

        glUniformBlockBinding(shader.getId(), ub_index, own_binding_point_);
        if(glGetError() == GL_INVALID_VALUE)
        {
            glDeleteBuffers(1, &ubo_);
            throw std::runtime_error(std::string("ERROR::ubo_::BINDING\n") + std::string("Can't bind.\n"));
        }

        glBindBufferBase(GL_UNIFORM_BUFFER, own_binding_point_, ubo_);
        if(glGetError() == GL_INVALID_VALUE || glGetError() == GL_INVALID_ENUM)
        {
            glDeleteBuffers(1, &ubo_);
            throw std::runtime_error(std::string("ERROR::ubo_::BINDING\n") + std::string("Can't bind.\n"));
        }
    }

    void updateAll()
    {
        // save previous binded buffer
        GLint bindedBuffer;
        glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &bindedBuffer);

        glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &ref_data_);

        // bind buffer back
        glBindBuffer(GL_UNIFORM_BUFFER, bindedBuffer);
    }

    template<typename Q>
    class UniformProxy
    {
    public:
        UniformProxy(UniformObject<T> & outer, unsigned int offset, unsigned int size) :
            ref_outer(outer), offset_(offset), size_{ size }, value_{}
        {}

        Q & operator=(const Q & other)
        {
            value_ = other;
            ref_outer.template updatePartial<Q>(value_, offset_, size_);
            return value_;
        }

        operator Q()
        {
            return value_;
        }

    private:
        Q value_;
        UniformObject<T> & ref_outer;
        const unsigned int offset_;
        const unsigned int size_;
    };

    template<typename Q>
    UniformProxy<Q> getProxy(const unsigned int offset, const unsigned int size)
    {
        return UniformProxy<Q>(*this, offset, size);
    }

private:

    template<typename Q>
    void updatePartial(const Q & value, const unsigned int offset, const unsigned int size)
    {
        // save previous binded buffer
        GLint bindedBuffer;
        glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &bindedBuffer);

        glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &value);

        // bind buffer back
        glBindBuffer(GL_UNIFORM_BUFFER, bindedBuffer);
    }
    T ref_data_{};
    unsigned int ubo_{};
};

#endif // UNIFORMOBJECT_H

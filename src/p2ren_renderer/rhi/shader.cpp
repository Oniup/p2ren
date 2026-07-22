#include "p2ren_renderer/rhi/shader.h"

#include <SDL3/SDL_iostream.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <glad/gl.h>

#include <array>
#include <string_view>

#include "p2ren_core/utility/error.h"
#include "p2ren_renderer/rhi/texture.h"

#define ERROR_BUFFER_LENGTH 1024

namespace p2ren {

namespace intern {

    constexpr size_t ShaderTypeCount = 3;

    constexpr std::array<uint32_t, ShaderTypeCount> ShaderTypes = {
        GL_VERTEX_SHADER,
        GL_FRAGMENT_SHADER,
        GL_GEOMETRY_SHADER,
    };

    constexpr std::array<std::string_view, ShaderTypeCount> ShaderTypeNames = {
        "Vertex",
        "Fragment",
        "Geometry",
    };

} // namespace intern

Shader::Shader(std::string_view vertex, std::string_view fragment, std::string_view geometry)
{
    P2REN_ASSERT_STRING_VIEW_NULL_TERMINATED(vertex);
    P2REN_ASSERT_STRING_VIEW_NULL_TERMINATED(fragment);

    std::array<std::string_view, intern::ShaderTypeCount> paths   = {vertex, fragment, geometry};
    std::array<uint32_t, intern::ShaderTypeCount>         shaders = {};

    size_t count = 2;
    if (!geometry.empty())
    {
        count = 3;
        P2REN_ASSERT_STRING_VIEW_NULL_TERMINATED(geometry);
    }

    for (size_t i = 0; i < count; i++)
    {
        std::string_view path = paths[i];

        // Load source code into memory
        size_t source_size;
        char*  source = static_cast<char*>(SDL_LoadFile(path.data(), &source_size));
        if (!source)
        {
            for (size_t j = 0; j < i; j++)
                glDeleteShader(shaders[j]);
            P2REN_ERROR("Failed to read {} shader at path {}", intern::ShaderTypeNames[i], path);
            return;
        }

        // Compile shader
        uint32_t shader = glCreateShader(intern::ShaderTypes[i]);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        // Don't need source code anymore
        SDL_free(source);

        // Check for compile errors
        int32_t success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char buffer[ERROR_BUFFER_LENGTH];
            glGetShaderInfoLog(shader, ERROR_BUFFER_LENGTH, nullptr, buffer);

            glDeleteShader(shader);
            for (size_t j = 0; j < i; j++)
                glDeleteShader(shaders[j]);

            P2REN_ERROR("Failed to compile {} shader at path {}:\n{}",
                        intern::ShaderTypeNames[i],
                        path,
                        buffer);
            return;
        }

        // Successfully compiled shader
        shaders[i] = shader;
    }

    // Create program
    uint32_t program = glCreateProgram();
    for (size_t i = 0; i < count; i++)
        glAttachShader(program, shaders[i]);
    glLinkProgram(program);

    // Delete shaders
    for (size_t i = 0; i < count; i++)
        glDeleteShader(shaders[i]);

    // Check for linking errors
    int32_t success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glDeleteProgram(program);

        char error_message[ERROR_BUFFER_LENGTH];
        glGetProgramInfoLog(program, ERROR_BUFFER_LENGTH, nullptr, error_message);

        std::string shader_info;
        for (size_t i = 0; i < count; i++)
        {
            shader_info += fmt::format("\t- {} -> '{}'\n", intern::ShaderTypeNames[i], paths[i]);
        }
        P2REN_ERROR("Failed to link shaders\n{}{}", shader_info, error_message);
        return;
    }

    // Everything successful
    m_ID = program;
}

Shader::~Shader()
{
    if (m_ID != 0)
    {
        glDeleteProgram(m_ID);
        m_ID = 0;
    }
}

Shader::Shader(Shader&& other)
    : m_ID(other.m_ID)
{
    other.m_ID = 0;
}

Shader& Shader::operator=(Shader&& other)
{
    if (this != &other)
    {
        if (m_ID != 0)
            glDeleteProgram(m_ID);

        m_ID       = other.m_ID;
        other.m_ID = 0;
    }
    return *this;
}

bool Shader::IsValid() const
{
    return m_ID != 0;
}

void Shader::Bind() const
{
    glUseProgram(m_ID);
}

void Shader::PushConstant(std::string_view location, int32_t val) const
{
    glUniform1i(GetUniformLocation(location), val);
}

void Shader::PushConstant(std::string_view location, uint32_t val) const
{
    glUniform1ui(GetUniformLocation(location), val);
}

void Shader::PushConstant(std::string_view location, float val) const
{
    glUniform1f(GetUniformLocation(location), val);
}

void Shader::PushConstant(std::string_view location, const glm::vec2& val) const
{
    glUniform2fv(GetUniformLocation(location), 1, &val[0]);
}

void Shader::PushConstant(std::string_view location, const glm::vec3& val) const
{
    glUniform3fv(GetUniformLocation(location), 1, &val[0]);
}

void Shader::PushConstant(std::string_view location, const glm::vec4& val) const
{
    glUniform4fv(GetUniformLocation(location), 1, &val[0]);
}

void Shader::PushConstant(std::string_view location, const glm::mat3& val, bool transpose) const
{
    glUniformMatrix3fv(GetUniformLocation(location), 1, transpose, &val[0][0]);
}

void Shader::PushConstant(std::string_view location, const glm::mat4& val, bool transpose) const
{
    glUniformMatrix4fv(GetUniformLocation(location), 1, transpose, &val[0][0]);
}

void Shader::PushConstant(const Texture* texture, uint32_t active_id) const
{
    glActiveTexture(GL_TEXTURE0 + active_id);
    texture->Bind();
}

uint32_t Shader::GetUniformLocation(std::string_view location) const
{
    P2REN_ASSERT_STRING_VIEW_NULL_TERMINATED(location);
    return glGetUniformLocation(m_ID, location.data());
}

} // namespace p2ren

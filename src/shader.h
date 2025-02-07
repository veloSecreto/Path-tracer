#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>
#include <iostream>


class BaseShader {
    protected:
        int m_ID = -1;
        std::unordered_map<std::string, int> uniformLocs;

    public:
        BaseShader() = default;
        void use();
        void setInt(const std::string& name, int value);
        void setFloat(const std::string& name, float value);
        void setBool(const std::string& name, bool value);
        void setVec2(const std::string& name, const glm::vec2& value);
        void setVec3(const std::string& name, const glm::vec3& value);
        void setMat4x4(const std::string& name, const glm::mat4& value);
        ~BaseShader();
};

class Shader : public BaseShader
{
    public:
        Shader() = default;
        Shader(const std::string& compName);
        Shader(const std::string& virtName, const std::string& fragName);
        Shader(const std::string& virtName, const std::string& fragName, const std::string& geoName);
        void load(const std::string& virtName, const std::string& fragName);
        void load(const std::string& virtName, const std::string& fragName, const std::string& geoName);
        void load(const std::string& compName);
};
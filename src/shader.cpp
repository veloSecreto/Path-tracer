#include "shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>

int checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n";
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n";
        }
    }
    return success;
}

BaseShader::~BaseShader() { glDeleteProgram(m_ID); }
void BaseShader::use() { glUseProgram(m_ID); }


Shader::Shader(const std::string& compName) {
    load(compName);
}

Shader::Shader(const std::string& virtName, const std::string& fragName) {
    load(virtName, fragName);
}

Shader::Shader(const std::string& virtName, const std::string& fragName, const std::string& geoName) {
    load(virtName, fragName, geoName);
}

void Shader::load(const std::string& compName) {
    std::string compPath = "../../res/shaders/" + compName;
    std::ifstream cShaderFile;
    std::stringstream compCode;

    cShaderFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

    try {
        cShaderFile.open(compPath);
        compCode << cShaderFile.rdbuf();
        cShaderFile.close();
    }
    catch (std::ifstream::failure& e) {
        std::cout << "Shader File not successfully read: " << e.what() << std::endl;
    }

    uint32_t compute;
    std::string temp_comp = compCode.str();
    const char* computeCode = temp_comp.c_str();

    compute = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute, 1, &computeCode, NULL);
    glCompileShader(compute);
    checkCompileErrors(compute, "COMPUTE");

    uint32_t temp_ID = glCreateProgram();
    glAttachShader(temp_ID, compute);
    glLinkProgram(temp_ID);
    

    if (checkCompileErrors(temp_ID, "PROGRAM")) {
        if (m_ID != -1) {
            glDeleteProgram(m_ID);
        }

        m_ID = temp_ID;
        uniformLocs.clear();
        std::cout << "Loaded '" << compName.substr(0, compName.find_last_of('.')) << "' shader\n";
    }
    else {
        std::cout << "shader failed to compile " << compName << "\n";
    }

    glDeleteShader(compute);
}

void Shader::load(const std::string& virtName, const std::string& fragName) {
    std::string vertPath = "../../res/shaders/" + virtName;
    std::string fragPath = "../../res/shaders/" + fragName;
    std::ifstream vShaderFile, fShaderFile;
    std::stringstream vertCode, fragCode;

    vShaderFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    fShaderFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

    try {
        vShaderFile.open(vertPath);
        fShaderFile.open(fragPath);
        vertCode << vShaderFile.rdbuf();
        fragCode << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
    }
    catch (std::ifstream::failure& e) {
        std::cout << "Shader File not successfully read: " << e.what() << std::endl;
    }

    uint32_t vertex, fragment;
    std::string temp_vert = vertCode.str(); std::string temp_frag = fragCode.str();
    const char* vertexCode = temp_vert.c_str();
    const char* fragmentCode = temp_frag.c_str();

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1,  &fragmentCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    uint32_t temp_ID = glCreateProgram();
    glAttachShader(temp_ID, vertex);
    glAttachShader(temp_ID, fragment);
    glLinkProgram(temp_ID);
    

    if (checkCompileErrors(temp_ID, "PROGRAM")) {
        if (m_ID != -1) {
            glDeleteProgram(m_ID);
        }

        m_ID = temp_ID;
        uniformLocs.clear();
        std::cout << "Loaded '" << virtName.substr(0, virtName.find_last_of('.')) << "' shader\n";
    }
    else {
        std::cout << "shader failed to compile " << virtName << " and " << fragName << "\n";
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::load(const std::string& virtName, const std::string& fragName, const std::string& geoName) {
    std::string vertPath = "../../res/shaders/" + virtName;
    std::string fragPath = "../../res/shaders/" + fragName;
    std::string geoPath = "../../res/shaders/" + geoName;
    std::ifstream vShaderFile, fShaderFile, gShaderFile;
    std::stringstream vertCode, fragCode, geoCode;

    vShaderFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    fShaderFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    gShaderFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

    try {
        vShaderFile.open(vertPath);
        fShaderFile.open(fragPath);
        gShaderFile.open(geoPath);
        vertCode << vShaderFile.rdbuf();
        fragCode << fShaderFile.rdbuf();
        geoCode << gShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        gShaderFile.close();
    }
    catch (std::ifstream::failure& e) {
        std::cout << "Shader File not successfully read: " << e.what() << std::endl;
    }

    uint32_t vertex, fragment, geometry;
    std::string temp_vert = vertCode.str(), temp_frag = fragCode.str(), temp_geo = geoCode.str();
    const char* vertexCode = temp_vert.c_str();
    const char* fragmentCode = temp_frag.c_str();
    const char* geometryCode = temp_geo.c_str();

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1,  &fragmentCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1,  &geometryCode, NULL);
    glCompileShader(geometry);
    checkCompileErrors(geometry, "GEOMETRY");

    uint32_t temp_ID = glCreateProgram();
    glAttachShader(temp_ID, vertex);
    glAttachShader(temp_ID, fragment);
    glAttachShader(temp_ID, geometry);
    glLinkProgram(temp_ID);
    

    if (checkCompileErrors(temp_ID, "PROGRAM")) {
        if (m_ID != -1) {
            glDeleteProgram(m_ID);
        }

        m_ID = temp_ID;
        uniformLocs.clear();
        std::cout << "Loaded '" << virtName.substr(0, virtName.find_last_of('.')) << "' shader\n";
    }
    else {
        std::cout << "shader failed to compile " << virtName << ", " << fragName << " and " << geoName << "\n";
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteShader(geometry);
}

void BaseShader::setInt(const std::string& name, int value) {
    if (uniformLocs.find(name) == uniformLocs.end()) {
        uniformLocs[name] = glGetUniformLocation(m_ID, name.c_str());
    }
    glUniform1i(uniformLocs[name], value);
}

void BaseShader::setFloat(const std::string& name, float value) {
    if (uniformLocs.find(name) == uniformLocs.end()) {
        uniformLocs[name] = glGetUniformLocation(m_ID, name.c_str());
    }
    glUniform1f(uniformLocs[name], value);
}

void BaseShader::setBool(const std::string& name, bool value) {
    if (uniformLocs.find(name) == uniformLocs.end()) {
        uniformLocs[name] = glGetUniformLocation(m_ID, name.c_str());
    }
    glUniform1i(uniformLocs[name], ( int )value);
}

void BaseShader::setVec2(const std::string& name, const glm::vec2& value) {
    if (uniformLocs.find(name) == uniformLocs.end()) {
        uniformLocs[name] = glGetUniformLocation(m_ID, name.c_str());
    }
    glUniform2fv(uniformLocs[name], 1, &value[0]);
}

void BaseShader::setVec3(const std::string& name, const glm::vec3& value) {
    if (uniformLocs.find(name) == uniformLocs.end()) {
        uniformLocs[name] = glGetUniformLocation(m_ID, name.c_str());
    }
    glUniform3fv(uniformLocs[name], 1, &value[0]);
}

void BaseShader::setMat4x4(const std::string& name, const glm::mat4& value) {
    if (uniformLocs.find(name) == uniformLocs.end()) {
        uniformLocs[name] = glGetUniformLocation(m_ID, name.c_str());
    }

    glUniformMatrix4fv(uniformLocs[name], 1, GL_FALSE, &value[0][0]);
}
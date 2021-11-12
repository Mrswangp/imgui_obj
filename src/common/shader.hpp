#ifndef SHADER_HPP
#define SHADER_HPP
#include <glm/glm.hpp>
enum shader_type {
    BASIC_FLAG,
    CUBE_FLAG,
    LAMP_FLAG
};
GLuint LoadShaders(enum shader_type type);
void setMat4(const GLuint& ProgramID, const std::string& name, const glm::mat4& mat);
void setVec3(const GLuint& ProgramID, const std::string& name, float x, float y, float z);
void setBool(const GLuint& ProgramID,const std::string& name, bool value);
void setInt(const GLuint& ProgramID,const std::string& name, int value);
void setFloat(const GLuint& ProgramID,const std::string& name, float value);
#endif

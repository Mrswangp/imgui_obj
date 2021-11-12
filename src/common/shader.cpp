#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include"frashader.h"
#include"vershader.h"
#include"lamp_frasha.h"
#include"lamp_versha.h"
#include"bas_light_frgsha.h"
#include"bas_light_versha.h"
#include "shader.hpp"
#include <imgui.h>
using namespace std;
GLuint LoadShaders(enum shader_type type){
	using namespace shader;
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	GLint Result = GL_FALSE;
	int InfoLogLength;
	/*char* vetexShader = nullptr;
	char* fragmentShader = nullptr;*/
	if (type == CUBE_FLAG) {
		glShaderSource(VertexShaderID, 1, &(shader::basic_light_ver), NULL);
	}
	else if(type == LAMP_FLAG){
		glShaderSource(VertexShaderID, 1, &(shader::lamp_ver), NULL);
	}
	else {
		glShaderSource(VertexShaderID, 1, &(shader::vetexShader), NULL);
	}
	glCompileShader(VertexShaderID);
	
	//const char* vetexShader = shader::basic_light_ver;
	// Compile Vertex Shader
	//glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}
	if (type == CUBE_FLAG) {
		glShaderSource(FragmentShaderID, 1, &(shader::basic_light_fra), NULL);
	}
	else if (type == LAMP_FLAG) {
		glShaderSource(FragmentShaderID, 1, &(shader::lamp_fra), NULL);
	}
	else {
		glShaderSource(FragmentShaderID, 1, &(shader::fragmentShader), NULL);
	}
	// Compile Fragment Shader
	/*glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);*/
	glCompileShader(FragmentShaderID);
	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	//Release shader
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	return ProgramID;
}
void setMat4(const GLuint& ProgramID, const std::string& name, const glm::mat4& mat)
{
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void setVec3(const GLuint& ProgramID, const std::string& name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(ProgramID, name.c_str()), x, y, z);
}
void setBool(const GLuint& ProgramID,const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), (int)value);
}
void setInt(const GLuint& ProgramID,const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), value);
}
void setFloat(const GLuint& ProgramID,const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(ProgramID, name.c_str()), value);
}

#ifndef VERTEX_SHADER
#define VERTEX_SHADER
#include<iostream>
namespace shader
{
#if 0
	const char* vetexShader = "#version 330 core\n"
		"// Input vertex data, different for all executions of this shader.\n"
		"layout(location = 0) in vec3 vertexPosition_modelspace;\n"
		"layout(location = 1) in vec3 vertexNormal;\n"
		"out vec3 FragPos;\n"
		"out vec3 Normal;\n"
		"//uniform mat4 obj_MVP;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"FragPos = vec3(model*vec4(vertexPosition_modelspace,1.0));\n"
		"Normal = mat3(transpose(inverse(model))) * vertexNormal;\n"
		"// Output position of the vertex, in clip space : MVP * position\n"
		"//gl_Position = obj_MVP * vec4(vertexPosition_modelspace, 1);\n"
		"gl_Position = projection * view * vec4(FragPos,1.0);\n"
		"}\0";
#endif
#if 1
	const char* vetexShader = R"(#version 330 core
		//Input vertex data, different for all executions of this shader.
		layout(location = 0) in vec3 vertexPosition_modelspace;
		layout(location = 1) in vec3 vertexNormal;
		out vec3 FragPos;
		out vec3 Normal;
		//uniform mat4 obj_MVP;
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;
		void main()
		{
		FragPos = vec3(model*vec4(vertexPosition_modelspace,1.0));
		Normal = mat3(transpose(inverse(model))) * vertexNormal;
		// Output position of the vertex, in clip space : MVP * position
		//gl_Position = obj_MVP * vec4(vertexPosition_modelspace, 1);
		gl_Position = projection * view * vec4(FragPos,1.0);
		};)"; 
#endif
}
#endif // !VERTEX_SHADER


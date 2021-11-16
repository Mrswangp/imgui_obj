#ifndef VERTEX_SHADER
#define VERTEX_SHADER
#include<iostream>
namespace shader
{
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
}
#endif // !VERTEX_SHADER


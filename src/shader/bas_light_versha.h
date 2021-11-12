#ifndef BASIC_LIGHTING_VERTEX
#define BASIC_LIGHTING_VERTEX
#include<iostream>
namespace shader {
	const char* basic_light_ver = "#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"out vec3 FragPos;\n"
		"out vec3 Normal;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"FragPos = vec3(model * vec4(aPos, 1.0));\n"
		"Normal = mat3(transpose(inverse(model))) * aNormal;\n"
		"gl_Position = projection * view * vec4(FragPos, 1.0);\n"
		"}\0";
}
#endif // !BASIC_LIGHTING_VERTEX

#ifndef LAMP_VERTEX
#define LAMP_VERTEX
#include<iostream>
namespace shader {
	const char* lamp_ver = "#version 330 core\n"
		"layout(location = 0) in vec3 lamp_Pos;\n"
		"uniform mat4 lamp_MVP;\n"
		"void main()\n"
		"{\n"
		"gl_Position =lamp_MVP * vec4(lamp_Pos, 1.0);\n"
		"}\0";
}
#endif // !LAMP_VERTEX

#ifndef VERTEX_SHADER
#define VERTEX_SHADER
#include<iostream>
namespace shader
{
	const char* vetexShader = "#version 330 core\n"
		"// Input vertex data, different for all executions of this shader.\n"
		"layout(location = 0) in vec3 vertexPosition_modelspace;\n"
		"layout(location = 1) in vec2 vertexUV;\n"
		"// Output data ; will be interpolated for each fragment.\n"
		"out vec2 UV;\n"
		"// Values that stay constant for the whole mesh.\n"
		"uniform mat4 MVP;\n"
		"void main()\n"
		"{\n"
		"// Output position of the vertex, in clip space : MVP * position\n"
		"gl_Position = MVP * vec4(vertexPosition_modelspace, 1);\n"
		"// UV of the vertex. No special space for this one.\n"
		"UV = vertexUV;\n"
		"}\0";
}
#endif // !VERTEX_SHADER


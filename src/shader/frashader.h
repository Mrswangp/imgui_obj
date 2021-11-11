#ifndef FRAGMENT_SHADER
#define FRAGMENT_SHADER
#include<iostream>
namespace shader
{
	const char* fragmentShader = "#version 330 core\n"
		"// Interpolated values from the vertex shaders\n"
		"in vec2 UV;\n"
		"// Ouput data\n"
		"out vec3 color;\n"
		"// Values that stay constant for the whole mesh.\n"
		"uniform sampler2D myTextureSampler;\n"
		"void main()\n" 
	    "{\n"
		"// Output color = color of the texture at the specified UV\n"
		"color = texture(myTextureSampler, UV).rgb;\n"
		"}\n\0";
}
#endif  //!FRAGMENT_SHADER
#ifndef FRAGMENT_SHADER
#define FRAGMENT_SHADER
#include<iostream>
namespace shader
{
	const char* fragmentShader = "#version 330 core\n"
		"// Interpolated values from the vertex shaders\n"
		"//in vec2 UV;\n"
		"// Ouput data\n"
		"out vec4 color;\n"
		"// Values that stay constant for the whole mesh.\n"
		"//uniform sampler2D myTextureSampler;\n"
		"uniform vec4 objectColor;\n"
		"uniform vec4 lightColor;\n"
		"uniform float transparency;\n"
		"void main()\n" 
	    "{\n"
		"// Output color = color of the texture at the specified UV\n"
		"//color = texture(myTextureSampler, UV).rgb;\n"
		"color = vec4(lightColor*objectColor);\n"
		"}\n\0";
}
#endif  //!FRAGMENT_SHADER
#ifndef FRAGMENT_SHADER
#define FRAGMENT_SHADER
#include<iostream>
namespace shader
{
	const char* fragmentShader = "#version 330 core\n"
		"out vec4 color;\n"
		"in vec3 Normal;\n"
		"in vec3 FragPos;\n"
		"// Values that stay constant for the whole mesh.\n"
		"//uniform sampler2D myTextureSampler;\n"
		"uniform vec3 objectColor;\n"
		"uniform vec3 lightColor;\n"
		"uniform vec3 lightPos;\n"
		"uniform vec3 viewPos;\n"
		"uniform int Shininess;"
		"void main()\n" 
	    "{\n"
		"//ambient\n"
		"float ambientStrength=0.1;\n"
		"vec3 ambient = ambientStrength * lightColor;\n"
		"//diffuse\n"
		"vec3 norm = normalize(Normal);\n"
		"vec3 lightDir = normalize(lightPos - FragPos);\n"
		"float diff = max(dot(norm,lightDir),0.0);\n"
		"vec3 diffuse = diff * lightColor;\n"
		"//specular\n"
		"float specularStrength = 0.7;\n"
		"vec3 viewDir = normalize(viewPos-FragPos);\n"
		"vec3 reflectDir = reflect(-lightDir,norm);\n"
		"float spec = pow(max(dot(viewDir,reflectDir),0.0), Shininess);\n"
		"vec3 specular = specularStrength * spec *lightColor;\n"
		"vec3 result = (ambient+diffuse+specular)*objectColor;\n"
		"color =vec4(result,1.0);\n"
		"//color = vec4(lightColor*objectColor);\n"
		"}\n\0";
}
#endif  //!FRAGMENT_SHADER
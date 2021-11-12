#ifndef BASIC_LIGHRTING_FRAGMENT
#define BASIC_LIGHRTING_FRAGMENT
#include<iostream>
namespace shader{
	const char* basic_light_fra = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 Normal;\n"
		"in vec3 FragPos;\n"
		"uniform vec3 lightPos;\n"
		"uniform vec3 viewPos;\n"
		"uniform vec3 lightColor;\n"
		"uniform vec3 objectColor;\n"
		"uniform float shininess;\n"
		"uniform float specularStrength;\n"
		"uniform float ambientStrength;\n"
		"void main()\n"
		"{\n"
		"// ambient\n"
		"vec3 ambient = ambientStrength * lightColor;\n"
		"//diffuse\n"
		"vec3 norm = normalize(Normal);\n"
		"vec3 lightDir = normalize(lightPos - FragPos);\n"
		"float diff = max(dot(norm, lightDir), 0.0);\n"
		"vec3 diffuse = diff * lightColor;\n"
		"// specular\n"
		"vec3 viewDir = normalize(viewPos - FragPos);\n"
		"vec3 reflectDir = reflect(-lightDir, norm);\n"
		"float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);\n"
		"vec3 specular = specularStrength * spec * lightColor;\n"
		"// vec3 result = (ambient + diffuse + specular) * objectColor;\n"
		"vec3 result = (ambient) * objectColor;\n"
		"FragColor = vec4(result, 1.0);\n"
		"}\n\0";
}
#endif //!BASIC_LIGHRTING_FRAGMENT
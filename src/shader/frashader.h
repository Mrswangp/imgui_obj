#ifndef FRAGMENT_SHADER
#define FRAGMENT_SHADER
#include<iostream>
namespace shader
{   
#if 0
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
		"uniform int Shininess;\n"
		"uniform float alpha;\n"
		"void main()\n" 
	    "{\n"
		"//ambient\n"
		"float ambientStrength=0.5;\n"
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
		"vec4 m_color =vec4(result,alpha);\n"
		"if(m_color.a<0.1)\n"
		"discard;\n"
		"color=m_color;\n"
		"//color =texture(result,alpha);\n"
		"//color = vec4(lightColor*objectColor);\n"
		"}\n\0";
#endif
#if 1
	const char* fragmentShader = R"(#version 330 core
		out vec4 color;
		in vec3 Normal;
		in vec3 FragPos;
		// Values that stay constant for the whole mesh.
		//uniform sampler2D myTextureSampler;\n"
		uniform vec3 objectColor;
		uniform vec3 lightColor;
		uniform vec3 lightPos;
		uniform vec3 viewPos;
		uniform int Shininess;
		uniform float alpha;
		void main()
		{
		//ambient
		float ambientStrength=0.5;
		vec3 ambient = ambientStrength * lightColor;
		//diffuse
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(lightPos - FragPos);
		float diff = max(dot(norm,lightDir),0.0);
		vec3 diffuse = diff * lightColor;
		//specular
		float specularStrength = 0.7;
		vec3 viewDir = normalize(viewPos-FragPos);
		vec3 reflectDir = reflect(-lightDir,norm);
		float spec = pow(max(dot(viewDir,reflectDir),0.0), Shininess);
		vec3 specular = specularStrength * spec *lightColor;
		vec3 result = (ambient+diffuse+specular)*objectColor;
		vec4 m_color =vec4(result,alpha);
		if(m_color.a<0.1)
		discard;
		color=m_color;
		//color =texture(result,alpha);
		//color = vec4(lightColor*objectColor);
		};)"; 
#endif
}
#endif  //!FRAGMENT_SHADER
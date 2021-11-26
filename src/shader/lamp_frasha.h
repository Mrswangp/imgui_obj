#ifndef LAMP_FRAGMENT
#define LAMP_FRAGMENT
#include<iostream>
namespace shader {
#if 0
	const char* lamp_fra = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"//set alle 4 vector values to 1.0\n"
		"FragColor = vec4(1.0);\n"
		"}\n\0";
#endif
#if 1
	const char* lamp_fra = R"(#version 330 core
		out vec4 FragColor;
		void main()
		{
		//set alle 4 vector values to 1.0
		FragColor = vec4(1.0);
		})";
#endif
}
#endif // !LAMP_FRAGMENT

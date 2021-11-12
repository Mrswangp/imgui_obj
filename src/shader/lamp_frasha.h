#ifndef LAMP_FRAGMENT
#define LAMP_FRAGMENT
#include<iostream>
namespace shader {
	const char* lamp_fra = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"//set alle 4 vector values to 1.0\n"
		"FragColor = vec4(0);\n"
		"}\n\0";
}
#endif // !LAMP_FRAGMENT

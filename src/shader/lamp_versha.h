#ifndef LAMP_VERTEX
#define LAMP_VERTEX
#include<iostream>
namespace shader {
#if 0
	const char* lamp_ver = R"( #version 330 core\n
         layout(location = 0) in vec3 lamp_Pos;
         uniform mat4 lamp_MVP;
         void main()
         {
          gl_Position =lamp_MVP * vec4(lamp_Pos, 1.0);\n
         }
         )";
#endif
const char* lamp_ver = "#version 330 core\n"
"layout(location = 0) in vec3 lamp_Pos;\n"
"uniform mat4 lamp_MVP;\n"
"void main()\n"
"{\n"
"gl_Position =lamp_MVP * vec4(lamp_Pos, 1.0);\n"
"}\0";
}
#endif // !LAMP_VERTEX

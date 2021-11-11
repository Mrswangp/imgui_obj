//.........三角形例子
// -------------------------------------------
//#include <stdio.h>
//#include <stdlib.h>
//
//#include <GL/glew.h>
//
//#include <GLFW/glfw3.h>
//GLFWwindow* window;
//
//#include <glm/glm.hpp>
//using namespace glm;
//
//int main()
//{
//	// Initialise GLFW
//	if (!glfwInit())
//	{
//		fprintf(stderr, "Failed to initialize GLFW\n");
//		getchar();
//		return -1;
//	}
//
//	glfwWindowHint(GLFW_SAMPLES, 4);
//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	// Open a window and create its OpenGL context
//	window = glfwCreateWindow(1024, 768, "Playground", NULL, NULL);
//	if (window == NULL)
//	{
//		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
//		getchar();
//		glfwTerminate();
//		return -1;
//	}
//	glfwMakeContextCurrent(window);
//
//	// Initialize GLEW
//	glewExperimental = true; // Needed for core profile
//	if (glewInit() != GLEW_OK)
//	{
//		fprintf(stderr, "Failed to initialize GLEW\n");
//		getchar();
//		glfwTerminate();
//		return -1;
//	}
//
//	// Ensure we can capture the escape key being pressed below
//	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
//
//	// Dark blue background
//	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
//	GLuint VertexArrayID;
//	glGenVertexArrays(1, &VertexArrayID);
//	glBindVertexArray(VertexArrayID);
//
//	// Create and compile our GLSL program from the shaders
//	// GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );
//
//
//	static const GLfloat g_vertex_buffer_data[] = {
//		-1.0f, -1.0f, 0.0f,
//		 1.0f, -1.0f, 0.0f,
//		 0.0f,  1.0f, 0.0f,
//	};
//
//	GLuint vertexbuffer;
//	glGenBuffers(1, &vertexbuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//
//	do
//	{
//		glClear(GL_COLOR_BUFFER_BIT);
//
//		// Draw nothing, see you in tutorial 2 !
//		glEnableVertexAttribArray(0);
//		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//		glVertexAttribPointer(
//			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//			3,                  // size
//			GL_FLOAT,           // type
//			GL_FALSE,           // normalized?
//			0,                  // stride
//			(void*)0            // array buffer offset
//		);
//
//		// Draw the triangle !
//		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
//
//		glDisableVertexAttribArray(0);
//		// Swap buffers
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//
//	} // Check if the ESC key was pressed or the window was closed
//	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
//		glfwWindowShouldClose(window) == 0);
//
//	// Close OpenGL window and terminate GLFW
//	glfwTerminate();
//
//	return 0;
//}
//............imgui控制三角形
// ------------------------------------------
//#include "imgui.h"
//#include "imgui_impl_glfw_gl3.h"
//#include <stdio.h>
//#include <GL/glew.h> //  使用gl3w，glad也行，注意要在项目工程中添加gl3w.c（或者glad.c/使用glad）
//#include <GLFW/glfw3.h>
//#include <iostream>
//#include <common/shader.hpp>
//
//void window_size_callback(GLFWwindow* window, int width, int height);
//
//// 设置窗口大小
//const unsigned int Window_width = 1600;
//const unsigned int Window_height = 1200;
//
//int main()
//{
//    // 实例化GLFW窗口
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    //下面这条语句是为了适应苹果系统
//#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//    // 创建一个窗口对象，这个窗口对象存放了所有和窗口相关的数据，而且会被GLFW的其他函数频繁地用到。
//    // 此外增加 if (window == NULL) 判断窗口是否创建成功
//    GLFWwindow* window = glfwCreateWindow(Window_width, Window_height, "ImGui Triangle", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, window_size_callback);
//    glfwSwapInterval(1);
//
//    //初始化gl3w
//    //gl3wInit();
//    glewInit();
//    //创建并绑定ImGui
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO();
//    (void)io;
//    ImGui_ImplGlfwGL3_Init(window, true);
//    ImGui::StyleColorsDark();
//
//    //初始化各种数据
//    bool ImGui = true;
//    bool the_same_color = false;
//    bool draw_trangle_without_render = false;
//    bool draw_trangle = false;
//    bool bonus_draw_line = false;
//    bool bonus_draw_another_trangle = false;
//    unsigned int VBO, VAO, EBO;
//    bool show_demo_window = true;
//    //创建一个VAO，并将它设为当前对象
//    GLuint VertexArrayID;
//    glGenVertexArrays(1, &VertexArrayID);
//    //绑定顶点数组对象
//    glBindVertexArray(VertexArrayID);
//
//    // 加载shader文件，创建并编译GLSL程序
//    GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
//    ImVec4 v1 = ImVec4(-0.25f, -0.25f, 0.0f, 1.00f);
//    ImVec4 v2 = ImVec4(0.25f, -0.25f, 0.0f, 1.00f);
//    ImVec4 v3 = ImVec4(0.0f, 0.25f, 0.0f, 1.00f);
//
//    //定义顶点缓冲，并将顶点缓冲传给OpenGL
//    GLuint vertexbuffer;
//
//    // 渲染循环
//    while (!glfwWindowShouldClose(window))
//    {
//        GLfloat g_vertex_buffer_data[] = {
//            v1.x,
//            v1.y,
//            v1.z,
//            v2.x,
//            v2.y,
//            v2.z,
//            v3.x,
//            v3.y,
//            v3.z,
//        };
//        glGenBuffers(1, &vertexbuffer);
//        // 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
//        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//
//        // 创建ImGui
//        glfwPollEvents();
//        ImGui_ImplGlfwGL3_NewFrame();
//        ImGui::Begin("change vertex", &ImGui, ImGuiWindowFlags_MenuBar);
//        ImGui::SliderFloat("", &v3.y, -1.0f, 1.0f, "v3.y = %.3f");
//
//        ImGui::End();
//
//        if (show_demo_window)
//        {
//            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
//            ImGui::ShowDemoWindow(&show_demo_window);
//        }
//        // 渲染窗口颜色
//        int view_width, view_height;
//        glfwGetFramebufferSize(window, &view_width, &view_height);
//        glViewport(0, 0, view_width, view_height);
//        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//        ImGui::Render();
//        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
//
//        glUseProgram(programID);
//
//        // 1rst attribute buffer : vertices
//        glEnableVertexAttribArray(0);
//
//        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//        //设定顶点属性指针
//        glVertexAttribPointer(
//            0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
//            3,        // size
//            GL_FLOAT, // type
//            GL_FALSE, // normalized?
//            0,        // stride
//            (void*)0 // array buffer offset
//        );
//
//        // 画三角形
//        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
//
//        glDisableVertexAttribArray(0);
//
//        // 双缓冲。前缓冲保存着最终输出的图像，它会在屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制。
//        glfwSwapBuffers(window);
//    }
//
//    // 释放VAO、VBO、EBO资源
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
//
//    // 释放ImGui资源
//    ImGui_ImplGlfwGL3_Shutdown();
//    ImGui::DestroyContext();
//
//    // 清除所有申请的glfw资源
//    glfwTerminate();
//    return 0;
//}
//
//void window_size_callback(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}
//.......绘制彩色立方体
//--------------------------------
//#include "imgui.h"
//#include "imgui_impl_glfw_gl3.h"
//#include <stdio.h>
//#include <GL/glew.h> //  使用gl3w，glad也行，注意要在项目工程中添加gl3w.c（或者glad.c/使用glad）
//#include <GLFW/glfw3.h>
//#include <iostream>
//#include <common/shader.hpp>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//void window_size_callback(GLFWwindow* window, int width, int height);
//
//// 设置窗口大小
//const unsigned int Window_width = 1600;
//const unsigned int Window_height = 1200;
//
//int main()
//{
//    // 实例化GLFW窗口
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    //下面这条语句是为了适应苹果系统
//#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//    // 创建一个窗口对象，这个窗口对象存放了所有和窗口相关的数据，而且会被GLFW的其他函数频繁地用到。
//    // 此外增加 if (window == NULL) 判断窗口是否创建成功
//    GLFWwindow* window = glfwCreateWindow(Window_width, Window_height, "ImGui Triangle", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, window_size_callback);
//    glfwSwapInterval(1);
//
//    //初始化gl3w
//   // gl3wInit();
//       glewInit();
//    //创建并绑定ImGui
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO();
//    (void)io;
//    ImGui_ImplGlfwGL3_Init(window, true);
//    ImGui::StyleColorsDark();
//
//    //初始化各种数据
//    bool ImGui = true;
//    bool the_same_color = false;
//    bool draw_trangle_without_render = false;
//    bool draw_trangle = false;
//    bool bonus_draw_line = false;
//    bool bonus_draw_another_trangle = false;
//    unsigned int VBO, VAO, EBO;
//    bool show_demo_window = true;
//    int isOrthoCamera = 0;
//
//    // 开启深度测试
//    glEnable(GL_DEPTH_TEST);
//    // Accept fragment if it closer to the camera than the former one
//    glDepthFunc(GL_LESS);
//    //创建一个VAO，并将它设为当前对象
//    GLuint VertexArrayID;
//    glGenVertexArrays(1, &VertexArrayID);
//    //绑定顶点数组对象
//    glBindVertexArray(VertexArrayID);
//
//    // 加载shader文件，创建并编译GLSL程序
//    GLuint programID = LoadShaders("TransformVertexShader1.vertexshader", "ColorFragmentShader.fragmentshader");
//
//    ImVec4 v1 = ImVec4(-0.25f, -0.25f, 0.0f, 1.00f);
//    ImVec4 v2 = ImVec4(0.25f, -0.25f, 0.0f, 1.00f);
//    ImVec4 v3 = ImVec4(0.0f, 0.25f, 0.0f, 1.00f);
//    ImVec4 camPos = ImVec4(4.0f, 3.0f, 3.0f, 1.00f);
//
//    float viewField = 90.0f;
//    //定义顶点缓冲，并将顶点缓冲传给OpenGL
//    GLuint vertexbuffer;
//
//    // 渲染循环
//    while (!glfwWindowShouldClose(window))
//    {
//
//        // Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
//        glm::mat4 Projection = isOrthoCamera ? glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 10.0f) : glm::perspective(glm::radians(viewField), 4.0f / 3.0f, 0.1f, 100.0f);
//
//        // Camera matrix
//        glm::mat4 View = glm::lookAt(
//            glm::vec3(camPos.x, camPos.y, camPos.z), // Camera is at (4,3,3), in World Space
//            glm::vec3(0, 0, 0),                      // and looks at the origin
//            glm::vec3(0, 1, 0)                       // Head is up (set to 0,-1,0 to look upside-down)
//        );
//        // Model matrix : an identity matrix (model will be at the origin)
//        glm::mat4 Model = glm::mat4(1.0f);
//        // Our ModelViewProjection : multiplication of our 3 matrices
//        glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
//
//        static const GLfloat g_vertex_buffer_data[] = {
//            -1.0f, -1.0f, -1.0f, // triangle 1 : begin
//            -1.0f, -1.0f, 1.0f,
//            -1.0f, 1.0f, 1.0f, // triangle 1 : end
//            1.0f, 1.0f, -1.0f, // triangle 2 : begin
//            -1.0f, -1.0f, -1.0f,
//            -1.0f, 1.0f, -1.0f, // triangle 2 : end
//            1.0f, -1.0f, 1.0f,
//            -1.0f, -1.0f, -1.0f,
//            1.0f, -1.0f, -1.0f,
//            1.0f, 1.0f, -1.0f,
//            1.0f, -1.0f, -1.0f,
//            -1.0f, -1.0f, -1.0f,
//            -1.0f, -1.0f, -1.0f,
//            -1.0f, 1.0f, 1.0f,
//            -1.0f, 1.0f, -1.0f,
//            1.0f, -1.0f, 1.0f,
//            -1.0f, -1.0f, 1.0f,
//            -1.0f, -1.0f, -1.0f,
//            -1.0f, 1.0f, 1.0f,
//            -1.0f, -1.0f, 1.0f,
//            1.0f, -1.0f, 1.0f,
//            1.0f, 1.0f, 1.0f,
//            1.0f, -1.0f, -1.0f,
//            1.0f, 1.0f, -1.0f,
//            1.0f, -1.0f, -1.0f,
//            1.0f, 1.0f, 1.0f,
//            1.0f, -1.0f, 1.0f,
//            1.0f, 1.0f, 1.0f,
//            1.0f, 1.0f, -1.0f,
//            -1.0f, 1.0f, -1.0f,
//            1.0f, 1.0f, 1.0f,
//            -1.0f, 1.0f, -1.0f,
//            -1.0f, 1.0f, 1.0f,
//            1.0f, 1.0f, 1.0f,
//            -1.0f, 1.0f, 1.0f,
//            1.0f, -1.0f, 1.0f };
//
//        static const GLfloat g_color_buffer_data[] = {
//            0.583f, 0.700f, 0.014f,
//            0.609f, 0.100f, 0.436f,
//            0.327f, 0.400f, 0.844f,
//            0.822f, 0.500f, 0.201f,
//            0.435f, 0.600f, 0.223f,
//            0.310f, 0.700f, 0.185f,
//            0.597f, 0.700f, 0.761f,
//            0.559f, 0.400f, 0.730f,
//            0.359f, 0.500f, 0.152f,
//            0.483f, 0.500f, 0.789f,
//            0.559f, 0.800f, 0.639f,
//            0.195f, 0.500f, 0.859f,
//            0.014f, 0.100f, 0.576f,
//            0.771f, 0.300f, 0.970f,
//            0.406f, 0.600f, 0.116f,
//            0.676f, 0.900f, 0.133f,
//            0.971f, 0.500f, 0.833f,
//            0.140f, 0.600f, 0.489f,
//            0.997f, 0.500f, 0.064f,
//            0.945f, 0.700f, 0.592f,
//            0.543f, 0.000f, 0.978f,
//            0.279f, 0.300f, 0.505f,
//            0.167f, 0.600f, 0.077f,
//            0.347f, 0.800f, 0.137f,
//            0.055f, 0.900f, 0.042f,
//            0.714f, 0.500f, 0.345f,
//            0.783f, 0.200f, 0.734f,
//            0.722f, 0.600f, 0.174f,
//            0.302f, 0.400f, 0.848f,
//            0.225f, 0.500f, 0.040f,
//            0.517f, 0.700f, 0.338f,
//            0.053f, 0.900f, 0.120f,
//            0.393f, 0.600f, 0.362f,
//            0.673f, 0.200f, 0.457f,
//            0.820f, 0.800f, 0.371f,
//            0.982f, 0.000f, 0.879f };
//        glGenBuffers(1, &vertexbuffer);
//        // 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
//        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//
//        GLuint colorbuffer;
//        glGenBuffers(1, &colorbuffer);
//        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
//
//        // 创建ImGui
//        glfwPollEvents();
//        ImGui_ImplGlfwGL3_NewFrame();
//        ImGui::Begin("change vertex", &ImGui, ImGuiWindowFlags_MenuBar);
//        ImGui::RadioButton("perspective camera", &isOrthoCamera, 0);
//        ImGui::SameLine();
//        ImGui::RadioButton("ortho camera", &isOrthoCamera, 1);
//
//        ImGui::SliderFloat("v3.y", &v3.y, -1.0f, 1.0f, "v3.y = %.3f");
//        ImGui::SliderFloat("viewField", &viewField, 0.0f, 90.0f, "viewField = %.3f");
//        ImGui::SliderFloat("x", &camPos.x, -15.0f, 15.0f, "camPos.x = %.3f");
//        ImGui::SliderFloat("y", &camPos.y, -15.0f, 15.0f, "camPos.y = %.3f");
//        ImGui::SliderFloat("z", &camPos.z, -15.0f, 15.0f, "camPos.z = %.3f");
//
//        ImGui::End();
//
//        if (show_demo_window)
//        {
//            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
//            ImGui::ShowDemoWindow(&show_demo_window);
//        }
//        // 渲染窗口颜色
//        int view_width, view_height;
//        glfwGetFramebufferSize(window, &view_width, &view_height);
//        glViewport(0, 0, view_width, view_height);
//        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        ImGui::Render();
//        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
//
//        glUseProgram(programID);
//
//        // Get a handle for our "MVP" uniform
//        GLuint MatrixID = glGetUniformLocation(programID, "MVP");
//
//        // Send our transformation to the currently bound shader,
//        // in the "MVP" uniform
//        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
//
//        // 1rst attribute buffer : vertices
//        glEnableVertexAttribArray(0);
//
//        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//        //设定顶点属性指针
//        glVertexAttribPointer(
//            0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
//            3,        // size
//            GL_FLOAT, // type
//            GL_FALSE, // normalized?
//            0,        // stride
//            (void*)0 // array buffer offset
//        );
//
//        // 2nd attribute buffer : colors
//        glEnableVertexAttribArray(1);
//        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
//        glVertexAttribPointer(
//            1,        // attribute. No particular reason for 1, but must match the layout in the shader.
//            3,        // size
//            GL_FLOAT, // type
//            GL_FALSE, // normalized?
//            0,        // stride
//            (void*)0 // array buffer offset
//        );
//
//        // 画三角形
//        glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 3 indices starting at 0 -> 1 triangle
//
//        glDisableVertexAttribArray(0);
//
//        // 双缓冲。前缓冲保存着最终输出的图像，它会在屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制。
//        glfwSwapBuffers(window);
//    }
//
//    // 释放VAO、VBO、EBO资源
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
//
//    // 释放ImGui资源
//    ImGui_ImplGlfwGL3_Shutdown();
//    ImGui::DestroyContext();
//
//    // 清除所有申请的glfw资源
//    glfwTerminate();
//    return 0;
//}
//
//void window_size_callback(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}
//
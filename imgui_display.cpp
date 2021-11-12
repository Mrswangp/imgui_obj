#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <GL/glew.h> //  使用gl3w，glad也行，注意要在项目工程中添加gl3w.c（或者glad.c/使用glad）
#include <GLFW/glfw3.h>
#include <iostream>
#include "common/shader.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "common/texture.hpp"
#include "common/objloader.hpp"
float deltatime = 0.0f;
float lastframe = 0.0f;
float radius = 3.0f;
 //lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
//glm::vec3 pos = glm::vec3(radius, 0, radius); // Camera is at (4,3,3), in World Space
glm::vec3 pos = glm::vec3(0, 0, radius); // Camera is at (4,3,3), in World Space
glm::vec3 lookAtPos = glm::vec3(0, 0, -1);
glm::vec3 up = glm::vec3(0, 1, 0);

void window_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float cameraspeed = 2.5 * deltatime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        pos += cameraspeed * lookAtPos;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        pos -= cameraspeed * lookAtPos;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        pos -= glm::normalize(glm::cross(lookAtPos, up)) * cameraspeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        pos += glm::normalize(glm::cross(lookAtPos, up)) * cameraspeed;
}
int main()
{
    // 设置窗口大小
    const unsigned int Window_width = 1800;
    const unsigned int Window_height = 1600;
    // 实例化GLFW窗口
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //下面这条语句是为了适应苹果系统
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 创建一个窗口对象，这个窗口对象存放了所有和窗口相关的数据，而且会被GLFW的其他函数频繁地用到。
    // 此外增加 if (window == NULL) 判断窗口是否创建成功
    GLFWwindow* window = glfwCreateWindow(Window_width, Window_height, "ImGui Triangle", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, window_size_callback);
    glfwSwapInterval(1);
    //初始化glew
    glewInit();
    //创建并绑定ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();
    //初始化各种数据
    bool ImGui = true;
    bool show_demo_window = true;
    int isOrthoCamera = 0;
    float light_vertices[] = {
       -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,

       -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f,  0.5f,
       -0.5f, -0.5f,  0.5f,

       -0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,
       -0.5f, -0.5f,  0.5f,
       -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

       -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f, -0.5f,

       -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f, -0.5f,
    };
    // 开启深度测试
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // 加载shader文件，创建并编译GLSL程序
    GLuint objprogramID = LoadShaders(BASIC_FLAG);
    GLuint lightprogramID = LoadShaders(LAMP_FLAG);
    ImVec4 camPos = ImVec4(radius, 0.0f, radius, 1.00f);
    float viewField = 90.0f;
  
  //   Read our .obj file
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texture;
    std::vector<glm::vec3> normals; // Won't be used at the moment.
    bool res =obj::load_obj("resource/test_new.obj", vertices, texture, normals);
    if (!res) {
        printf("load obj file failed!");
        return -1;
    }
    static int number = vertices.size();
    std::cout << "vertex_indices number is: \n" << number << std::endl;
    printf("load obj file successfully!");
   // 定义顶点缓冲，并将顶点缓冲传给OpenGL
    GLuint objVAO,vertexbuffer;
    glGenVertexArrays(1, &objVAO);
    glGenBuffers(1, &vertexbuffer);
    // 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glBindVertexArray(objVAO);
    glVertexAttribPointer(
        0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void*)0 // array buffer offset
    );
    glEnableVertexAttribArray(0);

    GLuint lightbuffer, lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, lightbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(light_vertices), light_vertices, GL_STATIC_DRAW);
    glBindVertexArray(lightVAO);
    glVertexAttribPointer(
        0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void*)0 // array buffer offset
    );
    glEnableVertexAttribArray(0);
    glm::mat4 RotationMatrix45 = glm::rotate((float)3.14f / 4, glm::vec3(0, 1, 0));
    glm::vec4 objectColor = glm::vec4(0, 1, 0,1);
    glm::vec4 lightColor = glm::vec4(1, 1, 1, 1);
    // 渲染循环
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    const float ZOOM = 45.0f;
    while (!glfwWindowShouldClose(window))
    {
        float currentframe = glfwGetTime();
        deltatime = currentframe - lastframe;
        lastframe = currentframe;
        processInput(window);
        static int speed_divisor = 1;
        float currentTime = glfwGetTime() * speed_divisor;
        // Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        glm::mat4 Projection = isOrthoCamera ? glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 10.0f) : glm::perspective(glm::radians(viewField), 4.0f / 3.0f, 0.1f, 100.0f);

      //   创建ImGui
        
        ImGui_ImplGlfwGL3_NewFrame();
        ImGui::Begin("Panel", &ImGui, ImGuiWindowFlags_MenuBar);
        glm::vec3 rotationAxis = glm::vec3(0, 1, 0);
        glm::mat4 RotationMatrix = glm::rotate((float)currentTime, rotationAxis);
        // Camera matrix
        glm::mat4 inial_view = glm::lookAt(pos,pos +lookAtPos , up);
        glm::mat4 View = inial_view * RotationMatrix * RotationMatrix45;
        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 t_MVP = Projection * View * Model;
        ImGui::RadioButton("perspective camera", &isOrthoCamera, 0);
        ImGui::SameLine();
        ImGui::RadioButton("ortho camera", &isOrthoCamera, 1);
        ImGui::SliderFloat("speed", &currentTime, 0.0f, 90.0f, "speed = %.3f");
        if (ImGui::Button("Triple speed")) {
            speed_divisor = 3;
        }
        ImGui::SameLine();
        if (ImGui::Button("Fivefold speed")) {
            speed_divisor = 5;
        }
        ImGui::SameLine();
        if (ImGui::Button("Sevenfold speed")) {
            speed_divisor = 7;
        }
       
        ImGui::SliderFloat("viewField", &viewField, 0.0f, 90.0f, "viewField = %.3f");
        ImGui::SliderFloat("radius", &radius, 0.0f, 20.0f, "radius = %.3f");
        ImGui::ColorEdit4("object color", (float*)&objectColor,ImGuiColorEditFlags_AlphaBar);
 
        glUniform4fv(glGetUniformLocation(objprogramID,"objectColor"),1, &objectColor[0]);
  
        /* ImGui::SliderFloat("x", &camPos.x, -15.0f, 15.0f, "camPos.x = %.3f");
         ImGui::SliderFloat("y", &camPos.y, -15.0f, 15.0f, "camPos.y = %.3f");
         ImGui::SliderFloat("z", &camPos.z, -15.0f, 15.0f, "camPos.z = %.3f");*/
        ImGui::End();

        if (show_demo_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
            ImGui::ShowDemoWindow(&show_demo_window);
        }
        // 渲染窗口颜色
        int view_width, view_height;
        glfwGetFramebufferSize(window, &view_width, &view_height);
        glViewport(0, 0, view_width, view_height);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        glUseProgram(objprogramID);

        // Get a handle for our "MVP" uniform
        GLuint MatrixID = glGetUniformLocation(objprogramID, "MVP");

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &t_MVP[0][0]);
        // 画三角形
       
       // glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBindVertexArray(objVAO);
        glDrawArrays(GL_TRIANGLES, 0, number); // 3 indices starting at 0 -> 1 triangle
       //  <summary>
         //light shaderprogram enable
        // </summary>
      //  glUseProgram(lightprogramID);
   
      //// 
      //  glm::mat4 lamp_proj = glm::perspective(glm::radians(ZOOM), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
      //  glUniformMatrix4fv(glGetUniformLocation(lightprogramID, "lamp_proj"), 1, GL_FALSE, &lamp_proj[0][0]);
      //  glUniformMatrix4fv(glGetUniformLocation(lightprogramID, "lamp_view"), 1, GL_FALSE, &inial_view[0][0]);
      //  glm::mat4 lamp_model = glm::mat4(1.0f);
      // // lamp_model = glm::mat4(1.0f);
      //  lamp_model = glm::translate(lamp_model, lightPos);
      //  lamp_model = glm::scale(lamp_model, glm::vec3(0.1f)); // a smaller cube
      //  glUniformMatrix4fv(glGetUniformLocation(lightprogramID, "lamp_model"), 1, GL_FALSE, &lamp_model[0][0]);
      ////  glBindVertexArray(lightVAO);
      //  //glVertexAttribPointer(
      //  //    0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
      //  //    3,        // size
      //  //    GL_FLOAT, // type
      //  //    GL_FALSE, // normalized?
      //  //    0,        // stride
      //  //    (void*)0 // array buffer offset
      //  //);
      //  ////glDisableVertexAttribArray(0);
      //  //glEnableVertexAttribArray(0);
      //  glBindVertexArray(lightVAO);
      //  glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // 双缓冲。前缓冲保存着最终输出的图像，它会在屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制。
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 释放VAO、VBO、EBO资源
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &lightbuffer);
    glDeleteVertexArrays(1, &objVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteProgram(objprogramID);
    glDeleteProgram(lightprogramID);
    // 释放ImGui资源
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    // 清除所有申请的glfw资源
    glfwTerminate();
    return 0;
}
void window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


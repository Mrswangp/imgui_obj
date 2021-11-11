#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <GL/glew.h> //  ʹ��gl3w��gladҲ�У�ע��Ҫ����Ŀ���������gl3w.c������glad.c/ʹ��glad��
#include <GLFW/glfw3.h>
#include <iostream>
#include "common/shader.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "common/texture.hpp"
#include "common/objloader.hpp"
void window_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
  /*  float cameraspeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraspeed * camerafront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraspeed * camerafront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(camerafront, cameraup)) * cameraspeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(camerafront, cameraup)) * cameraspeed;*/
}
int main()
{   
    // ���ô��ڴ�С
    const unsigned int Window_width = 1600;
    const unsigned int Window_height = 1200;
    // ʵ����GLFW����
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //�������������Ϊ����Ӧƻ��ϵͳ
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // ����һ�����ڶ���������ڶ����������кʹ�����ص����ݣ����һᱻGLFW����������Ƶ�����õ���
    // �������� if (window == NULL) �жϴ����Ƿ񴴽��ɹ�
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
    //��ʼ��glew
    glewInit();
    //��������ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();
    //��ʼ����������
    bool ImGui = true;
    bool show_demo_window = true;
    int isOrthoCamera = 0;
    float radius = 3.0f;
    // ������Ȳ���
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    //����һ��VAO����������Ϊ��ǰ����
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    //�󶨶����������
    glBindVertexArray(VertexArrayID);
    // ����shader�ļ�������������GLSL����
    GLuint programID = LoadShaders();
    ImVec4 v1 = ImVec4(-0.25f, -0.25f, 0.0f, 1.00f);
    ImVec4 v2 = ImVec4(0.25f, -0.25f, 0.0f, 1.00f);
    ImVec4 v3 = ImVec4(0.0f, 0.25f, 0.0f, 1.00f);
    // ImVec4 camPos = ImVec4(4.0f, 3.0f, 3.0f, 1.00f);
    ImVec4 camPos = ImVec4(radius, 0.0f, radius, 1.00f);
    float viewField = 90.0f;
    // Load the texture using any two methods
   // GLuint Texture = loadBMP_custom("uvtemplate.bmp");
    // GLuint Texture = loadDDS("uvtemplate.DDS");
    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
    // Read our .obj file
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texture;
    std::vector<glm::vec3> normals; // Won't be used at the moment.
    bool res = obj::load_obj("resource/test_new.obj", vertices, texture, normals);
    if (!res) {
        printf("load obj file failed!");
        return -1;
    }
    static int number = vertices.size();
    std::cout << "vertex_indices number is: \n" << number << std::endl;
    printf("load obj file successfully!");
    //���嶥�㻺�壬�������㻺�崫��OpenGL
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    // �����ǵĶ������鸴�Ƶ�һ�����㻺���У���OpenGLʹ��
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
   // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    //GLuint uvbuffer;
    //glGenBuffers(1, &uvbuffer);
    //glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    ////glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, texture.size() * sizeof(glm::vec2), &texture[0], GL_STATIC_DRAW);
    glm::mat4 RotationMatrix45 = glm::rotate((float)3.14f / 4, glm::vec3(0, 1, 0));
    // ��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {   
        processInput(window);
        static int speed_divisor = 1;
        float currentTime = glfwGetTime()* speed_divisor;
        // Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        glm::mat4 Projection = isOrthoCamera ? glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 10.0f) : glm::perspective(glm::radians(viewField), 4.0f / 3.0f, 0.1f, 100.0f);
        glm::vec3 pos = glm::vec3(radius, 0, radius); // Camera is at (4,3,3), in World Space
        glm::vec3 lookAtPos = glm::vec3(0, 0, 0);
        glm::vec3 up = glm::vec3(0, 1, 0);
        // ����ImGui
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();
        ImGui::Begin("Panel", &ImGui, ImGuiWindowFlags_MenuBar);
        glm::vec3 rotationAxis = glm::vec3(0, 1, 0);
        glm::mat4 RotationMatrix = glm::rotate((float)currentTime, rotationAxis);
        // Camera matrix
        glm::mat4 View = glm::lookAt(pos, lookAtPos, up);
        View = View * RotationMatrix * RotationMatrix45;
        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 MVP = Projection * View * Model;
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
       /* ImGui::SliderFloat("x", &camPos.x, -15.0f, 15.0f, "camPos.x = %.3f");
        ImGui::SliderFloat("y", &camPos.y, -15.0f, 15.0f, "camPos.y = %.3f");
        ImGui::SliderFloat("z", &camPos.z, -15.0f, 15.0f, "camPos.z = %.3f");*/
        ImGui::End();

        if (show_demo_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
            ImGui::ShowDemoWindow(&show_demo_window);
        }
        // ��Ⱦ������ɫ
        int view_width, view_height;
        glfwGetFramebufferSize(window, &view_width, &view_height);
        glViewport(0, 0, view_width, view_height);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        glUseProgram(programID);

        // Get a handle for our "MVP" uniform
        GLuint MatrixID = glGetUniformLocation(programID, "MVP");

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // Bind our texture in Texture Unit 0
      //  glActiveTexture(GL_TEXTURE0);
       // glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        //�趨��������ָ��
        glVertexAttribPointer(
            0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void*)0 // array buffer offset
        );

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        //glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
            1,        // attribute. No particular reason for 1, but must match the layout in the shader.
            2,        // size : U+V => 2
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void*)0 // array buffer offset
        );
        // ��������
        glDrawArrays(GL_TRIANGLES, 0, number); // 3 indices starting at 0 -> 1 triangle

        glDisableVertexAttribArray(0);

        // ˫���塣ǰ���屣�������������ͼ����������Ļ����ʾ�������еĵ���Ⱦָ����ں󻺳��ϻ��ơ�
        glfwSwapBuffers(window);
    }

    // �ͷ�VAO��VBO��EBO��Դ
    glDeleteBuffers(1, &vertexbuffer);
   // glDeleteBuffers(1, &uvbuffer);
    glDeleteProgram(programID);
   // glDeleteTextures(1, &Texture);
    glDeleteVertexArrays(1, &VertexArrayID);
    // �ͷ�ImGui��Դ
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    // ������������glfw��Դ
    glfwTerminate();
    return 0;
}
void window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
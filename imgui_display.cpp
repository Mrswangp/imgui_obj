#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <GL/glew.h> //  Use gl3w or glad. Note that gl3w. C (or glad. C / use glad) should be added to the project project
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include<librealsense2/rs.hpp>
#include<thread>
#include <iostream>
#include "common/shader.hpp"
#include "common/texture.hpp"
#include "common/objloader.hpp"
float deltatime = 0.0f;
float lastframe = 0.0f;
float yaw = 0.0f;// -90.0f;// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
bool firstMouse = true;
float lastX = 1200 / 2.0;
float lastY = 800 / 2.0;
float fov = 45.0f;
bool mousestate = false;

 //lighting position
glm::vec3 lightPos(1.2, 1, 1.5);

glm::vec3 camerapos = glm::vec3(0,0,3); 
glm::vec3 camerafront = glm::vec3(0, 0, -1);
glm::vec3 cameraup = glm::vec3(0, 1, 0);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void window_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void delete_resource(GLuint vertexbuffer, GLuint lightbuffer, GLuint objVAO, GLuint lightVAO, GLuint objprogramID, GLuint lightprogramID);
void processInput(GLFWwindow* window);
int realsense();
GLFWwindow* init_environment();
int main()
{   
    GLFWwindow* window = init_environment();
    if (!window) {
        return -1;
    }
    //Initialize various data
    bool ImGui = true;
    bool show_demo_window = true;
    int isOrthoCamera = 0;
    int rotateflag = 0;
    float light_vertices[] = {
    #include"light_vertex.inc"
    };

    // Opening depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    // Load the shader file, create and compile the glsl program
    GLuint objprogramID = LoadShaders(BASIC_FLAG);
    GLuint lightprogramID = LoadShaders(LAMP_FLAG);
    float viewField = 90.0f;
  
  //   Read our .obj file
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texture;
    std::vector<glm::vec3> normals; // Won't be used at the moment.
    auto res =obj::load_obj("resource/cube.obj", vertices, texture, normals);
    if (!res) {
        printf("load obj file failed!");
        return -1;
    }
    static int number = vertices.size();
    std::cout << "vertex_indices number is: \n" << number << std::endl;
    printf("load obj file successfully!\n");

    // Define vertex buffer and pass vertex buffer to OpenGL
    GLuint objVAO,vertexbuffer;
    glGenVertexArrays(1, &objVAO);
    glBindVertexArray(objVAO);
    glGenBuffers(1, &vertexbuffer);

    //Copy our vertex array to a vertex buffer for OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(
        0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void*)0 // array buffer offset
    );
    glEnableVertexAttribArray(0);
    // add normal 
    if (normals.size() != 0) {
        GLuint normalbuffer;
        glGenBuffers(1, &normalbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);

        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(1);
    }
    GLuint lightbuffer, lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glGenBuffers(1, &lightbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, lightbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(light_vertices), light_vertices, GL_STATIC_DRAW);
   // std::cout << "sizeof vertices is: " << sizeof(light_vertices)/sizeof(light_vertices[0]) << std::endl;
    glVertexAttribPointer(
        0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        3 * sizeof(float),        // stride
        (void*)0 // array buffer offset
    );
    glEnableVertexAttribArray(0);
    //glm::mat4 RotationMatrix45 = glm::rotate((float)3.14f / 4, glm::vec3(0, 1, 0));
    glm::mat4 RotationMatrix45 = glm::rotate(glm::radians(45.0f), glm::vec3(0, 1, 0));
   /* glm::mat4 RotationMatrix_pitch = glm::rotate(glm::radians(pitch), glm::vec3(0, 1, 0));
    glm::mat4 RotationMatrix_yaw = glm::rotate(glm::radians(yaw), glm::vec3(1, 0, 0));
    glm::mat4 rotationcombine = RotationMatrix_pitch * RotationMatrix_yaw;*/
    glm::vec3 objectColor = glm::vec3(0, 1, 0);
    glm::vec3 lightColor = glm::vec3(1, 1, 1);
    int ShininessValue = 2;
    float transparency =0.5f;

    // Render Loop
    const float ZOOM = 45.0f;
    float stopAngle = 0;
    float rotateFpara = 0;
    const char* text = "Distance: ";
    float dis_to_center = 0;
    //realsense operation
    auto realsense_sample = [&]()
     {   
            try {
                rs2::pipeline pip;
                pip.start();
                while (true)
                {
                    rs2::frameset frames = pip.wait_for_frames();
                    rs2::depth_frame depth = frames.get_depth_frame();
                    auto width = depth.get_width();
                    auto height = depth.get_height();
                    dis_to_center = depth.get_distance(width / 2, height / 2);
                    std::cout << "the camera is facing an object " << dis_to_center << "meters away \r";
                }
                return EXIT_SUCCESS;
            }
            catch (const rs2::error& e) {
                std::cerr << "RealSense error calling" << e.get_failed_function() << "(" << e.get_failed_args() << "):\n" << e.what() << std::endl;
                return EXIT_FAILURE;
            }
            catch (const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
                return EXIT_FAILURE;
            }
     };
    while (!glfwWindowShouldClose(window))
    {   
        
        auto currentframe = glfwGetTime();
        deltatime = currentframe - lastframe;
        lastframe = currentframe;
        processInput(window);
        static int speed_divisor = 1;
        float currentTime = glfwGetTime() * speed_divisor;
        stopAngle = glm::radians(90.0f);
        rotateFpara = rotateflag ? (float)currentTime : stopAngle;
        // Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        glm::mat4 Projection = isOrthoCamera ? glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 10.0f) : glm::perspective(glm::radians(viewField), 4.0f / 3.0f, 0.1f, 100.0f);
       // glm::mat4 Projection = glm::perspective(glm::radians(fov), (float)Window_width / (float)Window_height, 0.1f, 100.0f);
        glm::vec3 rotationAxis = glm::vec3(0, 1, 0);
        /*glm::mat4 RotationMatrix = glm::rotate((float)currentTime, rotationAxis);*/
        glm::mat4 RotationMatrix = glm::rotate(rotateFpara, rotationAxis);
        // Camera matrix
        glm::mat4 inial_view = glm::lookAt(glm::vec3(camerapos.x,camerapos.y,camerapos.z), camerapos + camerafront, cameraup);
      //  glm::mat4 View = rotateflag?(inial_view * RotationMatrix * RotationMatrix45): inial_view;
        glm::mat4 View = inial_view * RotationMatrix * RotationMatrix45;
        //glm::mat4 View = inial_view * RotationMatrix * rotationcombine;
        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 t_MVP = Projection * View * Model;
     
        // create ImGui interface
        ImGui_ImplGlfwGL3_NewFrame();
        ImGui::Begin("Panel", &ImGui, ImGuiWindowFlags_MenuBar);
        ImGui::RadioButton("perspective camera", &isOrthoCamera, 0);
        ImGui::SameLine();
        ImGui::RadioButton("ortho camera", &isOrthoCamera, 1);
        ImGui::RadioButton("start rotate", &rotateflag, 1);
        ImGui::SameLine();
        ImGui::RadioButton("stop rotate", &rotateflag, 0);
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
        //ImGui::SliderFloat("x",&camerapos.x,-15.0f,15.0f,"campos.x=%0.3f");
        ImGui::SliderFloat("Direction of light", &camerapos.y, -15.0f, 15.0f, "campos.y=%0.3f");
       // ImGui::SliderFloat("z", &camerapos.z, -15.0f, 15.0f, "campos.z=%0.3f");
        ImGui::SliderFloat("viewField", &viewField, 0.0f, 90.0f, "viewField = %.3f");
         ImGui::SliderInt("Shininess", &ShininessValue, 2, 256);
       // ImGui::SliderFloat("radius", &radius, 0.0f, 20.0f, "radius = %.3f");
        ImGui::ColorEdit3("object color", (float*)&objectColor);
        ImGui::ColorEdit3("light color", (float*)&lightColor);
        ImGui::SliderFloat("transparency", (float*)&transparency, 0.0f, 1.0f, "transparency = %.3f");
        if (ImGui::Button("test distance with realsense sdk")) {
            std::thread start_realsense(realsense_sample);
            start_realsense.detach();
        }
        ImGui::Text("camera to object distance : %f ", dis_to_center);
        //ImGui::LabelText(text,text);
        //glUniform3fv(glGetUniformLocation(objprogramID, "objectColor"), 1, &objectColor[0]);
        glUniform1f(glGetUniformLocation(objprogramID, "alpha"), transparency);
        glUniform3fv(glGetUniformLocation(objprogramID,"objectColor"),1, &objectColor[0]);
        glUniform3fv(glGetUniformLocation(objprogramID, "lightColor"), 1, &lightColor[0]);
        ImGui::End();

        if (show_demo_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
            ImGui::ShowDemoWindow(&show_demo_window);
        }
        
        // render window color
        int view_width, view_height;
        glfwGetFramebufferSize(window, &view_width, &view_height);
        glViewport(0, 0, view_width, view_height);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        //render light     /*****************-> error:  different render order cause bad results
        glUseProgram(lightprogramID);
        glm::mat4 lamp_model = glm::mat4(1.0f);
        lamp_model = glm::translate(lamp_model, lightPos);
        lamp_model = glm::scale(lamp_model, glm::vec3(0.1f)); // a smaller cube
        glm::mat4 lamp_Projection = isOrthoCamera ? glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 10.0f) : glm::perspective(glm::radians(90.0f), 4.0f / 3.0f, 0.1f, 100.0f);
        glm::mat4 l_MVP = lamp_Projection * glm::lookAt(glm::vec3(0,0,3), glm::vec3(0, 0, 3) +glm::vec3(0,0,-1), glm::vec3(0,1,0)) * lamp_model;
        GLuint lampID = glGetUniformLocation(lightprogramID, "lamp_MVP");
        glUniformMatrix4fv(lampID, 1, GL_FALSE, &l_MVP[0][0]);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //render obj
        glUseProgram(objprogramID);
        GLuint ShininessID = glGetUniformLocation(objprogramID,"Shininess");
        glUniform1i(ShininessID, ShininessValue);

        // Get a handle for our "MVP" uniform
        GLuint lightID= glGetUniformLocation(objprogramID, "lightPos");
        glUniform3fv(lightID, 1, &lightPos[0]);
        GLuint viewposID = glGetUniformLocation(objprogramID, "viewPos");
        glUniform3fv(viewposID, 1, &camerapos[0]);
        GLuint modelID= glGetUniformLocation(objprogramID, "model");
        glUniformMatrix4fv(modelID, 1, GL_FALSE, &Model[0][0]);
        GLuint viewID = glGetUniformLocation(objprogramID, "view");
        glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
        GLuint proID = glGetUniformLocation(objprogramID, "projection");
        glUniformMatrix4fv(proID, 1, GL_FALSE, &Projection[0][0]);
        //GLuint MatrixID = glGetUniformLocation(objprogramID, "obj_MVP");
        //// Send our transformation to the currently bound shader,
        //// in the "MVP" uniform
        //glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &t_MVP[0][0]);
       // glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBindVertexArray(objVAO);
        glDrawArrays(GL_TRIANGLES, 0, number); // 3 indices starting at 0 -> 1 triangle
      
        // Double buffering. The front buffer holds the final output image, which will be displayed on the screen; All rendering instructions are drawn on the post buffer.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete_resource(vertexbuffer, lightbuffer, objVAO, lightVAO, objprogramID, lightprogramID);
    return 0;
}
void delete_resource(GLuint vertexbuffer, GLuint lightbuffer, GLuint objVAO, GLuint lightVAO, GLuint objprogramID, GLuint lightprogramID)
{
    // release VAO¡¢VBO¡¢EBO resource
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &lightbuffer);
    glDeleteVertexArrays(1, &objVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteProgram(objprogramID);
    glDeleteProgram(lightprogramID);

    // release ImGui resource
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    // Clear all requested glfw resources
    glfwTerminate();
}
void window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
   if(mousestate){
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;  // reversed since y-coordinates go from bottom to top
        lastX = xpos;
        lastY = ypos;
        float sensitivity = 0.1f; //change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        yaw += xoffset;
        pitch += yoffset;

        //make sure that when pitch is out of bounds,screen doesn't get flipped
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camerafront = glm::normalize(front);
    }
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f) {
        fov = 1.0f;
    }
    if (fov > 45.0f) {
        fov = 45.0f;
    }
}
GLFWwindow* init_environment()
{   
    // Set window size
    const unsigned int Window_width = 1200;
    const unsigned int Window_height = 800;
    
    // Instantiate glfw window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    //to adapt macos
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a window object that stores all window related data and is frequently used by other functions of glfw¡£
    // In addition, if (window = = null) is added to judge whether the window is created successfully
    GLFWwindow* window = glfwCreateWindow(Window_width, Window_height, "ImGui Triangle", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, window_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    //  glfwSetScrollCallback(window, scroll_callback);
   //   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(1);
    
    //init glew
    glewInit();
   
    //create and bind ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();
    return window;
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float cameraspeed = 2.5 * deltatime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camerapos += cameraspeed * camerafront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camerapos -= cameraspeed * camerafront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camerapos += glm::normalize(glm::cross(camerafront, cameraup)) * cameraspeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camerapos -= glm::normalize(glm::cross(camerafront, cameraup)) * cameraspeed;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        mousestate = true;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
        mousestate = false;
    camerapos.y = 0.0f;

}
int realsense() try
{
    
        rs2::pipeline pip;
        pip.start();
        while (true)
        {
            rs2::frameset frames = pip.wait_for_frames();
            rs2::depth_frame depth = frames.get_depth_frame();
            auto width = depth.get_width();
            auto height = depth.get_height();
            float dis_to_center = depth.get_distance(width / 2, height / 2);
            std::cout << "the camera is facing an object " << dis_to_center << "meters away \r";
        }
        return EXIT_SUCCESS;
}
catch (const rs2::error& e) {
    std::cerr << "RealSense error calling" << e.get_failed_function() << "(" << e.get_failed_args() << "):\n" << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
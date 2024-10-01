#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>
#include <stb_image.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <shader_class.hpp>
#include <camera.hpp>
#include <window.hpp>

#include <model.hpp>

#include <cube_vertices.hpp>

int main() 
{
    // Create window (my function)
    if(CreateWindow() != 0) return -1;  
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    Shader myshader = Shader("./shaders/vertex.glsl", "./shaders/fragment.glsl");
    Shader lightingShader = Shader("./shaders/light_vertex.glsl", "./shaders/light_fragment.glsl");

    // Vertex arrays lighting
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // Vertex buffer
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // set the vertex attribute 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glm::vec3 lightColor = glm::vec3(1.0f);
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    // The model
    Model theModel("./model/backpack/backpack.obj");

    glEnable(GL_DEPTH_TEST);

    // clear color
    glm::vec3 clearColor(0.1f, 0.1f, 0.1f);
    bool moveLight = true;
    float angle = M_PI / 4;
    float radius = 5.0f;
    float speed = 1.5f;

    // Draw loop
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
        // some boilerplate code
        cout_time(); // account for frametime
        processInput(window); // Input

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        // if(settings) ...
        ImGui::NewFrame();
        ImGui::Begin("Debug:");
        ImGui::ColorEdit3("Clear color", (float*)&clearColor);
        ImGui::ColorEdit3("Light color", (float*)&lightColor);
        ImGui::Checkbox("Light is moving", &moveLight);
        ImGui::SliderFloat("light angle (0 - 2 * PI)", &angle, 0, 2 * M_PI, "%.3f");
        ImGui::End();

        if(moveLight)
        {
            angle = std::remainder(angle + (deltaTime * speed), 2 * M_PI);
            if(angle < 0) angle +=  2 * M_PI;
        }
            
        lightPos = glm::vec3(
                            sin(angle) * radius, 
                            sin(angle) * radius, 
                            cos(angle) * radius);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffers
        view = mycamera.GetViewMatrix();

        myshader.use();
        myshader.setMat4("projection", projection);
        myshader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        myshader.setMat4("model", model);

        myshader.setVec3("lightPosition", lightPos);
        myshader.setVec3("lightColor", lightColor);
        myshader.setVec3("viewPos", mycamera.Position);

        theModel.Draw(myshader);

        // update lighting position
        glm::mat4 Lighting_model = glm::translate(glm::mat4(1.0f), lightPos);
        Lighting_model = glm::scale(Lighting_model, glm::vec3(0.2f));
        lightingShader.use();
        lightingShader.setMat4("model", Lighting_model);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        lightingShader.setVec3("lightColor", lightColor);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    // Exit the application
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
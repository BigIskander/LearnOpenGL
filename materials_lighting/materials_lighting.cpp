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

    // Vertex arrays
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex buffer
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Linking shader attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Vertex arrays lighting
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the vertex attribute 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    Shader myshader = Shader("./shaders/vertex.glsl", "./shaders/fragment.glsl");
    myshader.use();
    glm::mat4 model = glm::mat4(1.0f);
    myshader.setMat4("model", model);

    Shader lightingShader = Shader("./shaders/light_vertex.glsl", "./shaders/light_fragment.glsl");

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // material properties
    glm::vec3 ambient(1.0f, 0.5f, 0.31f);
    glm::vec3 diffuse(1.0f, 0.5f, 0.31f);
    glm::vec3 specular(0.5f, 0.5f, 0.5f);

    // light properties
    glm::vec3 lightAmbient(0.2f, 0.2f, 0.2f);
    glm::vec3 lightDiffuse(0.5f, 0.5f, 0.5f);
    glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);

    bool moveLight = true;
    bool partyTime = false;
    int shininess = 32;

    float radius = 2.0f;
    float speed = 1.5f;
    float partySpeed = 0.7f;
    float angle = M_PI / 4;
    float partyAngle = 0.0f;

    // Draw loop
    while(!glfwWindowShouldClose(window))
    {
        // some boilerplate code
        cout_time(); // account for frametime
        processInput(window); // Input

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        // if(settings) ...
        ImGui::NewFrame();
        ImGui::Begin("Debug:");
        // if(ImGui::CollapsingHeader("Settings:"))
        ImGui::SeparatorText("Material settings:");
        ImGui::ColorEdit3("material ambient", (float*)&ambient, ImGuiColorEditFlags_Float);
        ImGui::ColorEdit3("material diffuse", (float*)&diffuse, ImGuiColorEditFlags_Float);
        ImGui::ColorEdit3("material specular", (float*)&specular, ImGuiColorEditFlags_Float);
        ImGui::SliderInt("shininess (2 - 256)", &shininess, 2, 256, "%d");
        ImGui::SeparatorText("Color settings:");
        ImGui::ColorEdit3("light ambient", (float*)&lightAmbient, ImGuiColorEditFlags_Float);
        ImGui::ColorEdit3("light diffuse", (float*)&lightDiffuse, ImGuiColorEditFlags_Float);
        ImGui::ColorEdit3("light specular", (float*)&lightSpecular, ImGuiColorEditFlags_Float);
        ImGui::SeparatorText("Other settings:");
        ImGui::Checkbox("Light is moving", &moveLight);
        ImGui::SliderFloat("light angle (0 - 2 * PI)", &angle, 0, 2 * M_PI, "%.3f");
        ImGui::Checkbox("Party time", &partyTime);
        if(ImGui::Button("Go back!")) {
            // light properties
            partyTime = false;
            lightAmbient = glm::vec3(0.2f);
            lightDiffuse = glm::vec3(0.5f);
            lightSpecular = glm::vec3(1.0f);
        }
        ImGui::SeparatorText("FPS:");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffers
        view = mycamera.GetViewMatrix();

        if(moveLight)
        {
            angle = std::remainder(angle + (deltaTime * speed), 2 * M_PI);
            if(angle < 0) angle +=  2 * M_PI;
        }
            
        lightPos = glm::vec3(
                            sin(angle) * radius, 
                            sin(angle) * radius, 
                            cos(angle) * radius);
            
        myshader.use();
        myshader.setMat4("projection", projection);
        myshader.setMat4("view", view);
        myshader.setVec3("viewPos", mycamera.Position);

        // set material properties
        myshader.setVec3("material.ambient", ambient);
        myshader.setVec3("material.diffuse", diffuse);
        myshader.setVec3("material.specular", specular);
        myshader.setInt("material.shininess", shininess);
    
        glm::vec3 lightColor = glm::vec3(1.0f);
        if(partyTime) {
            // set light properties
            partyAngle = std::remainder(partyAngle + (deltaTime * partySpeed), 2 * M_PI);
            if(partyAngle < 0) partyAngle +=  2 * M_PI;
            
            lightColor.x = (sin(partyAngle * 2.0f) / 2) + 0.5f;
            lightColor.y = (sin(partyAngle * 0.7f) / 2) + 0.5f;
            lightColor.z = (sin(partyAngle * 1.3f) / 2) + 0.5f;        
            lightDiffuse = lightColor   * glm::vec3(0.5f); 
            lightAmbient = lightDiffuse * glm::vec3(0.2f);
        }
         
        myshader.setVec3("light.position", lightPos); // update light position
        myshader.setVec3("light.ambient", lightAmbient);
        myshader.setVec3("light.diffuse", lightDiffuse);
        myshader.setVec3("light.specular", lightSpecular); 
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
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

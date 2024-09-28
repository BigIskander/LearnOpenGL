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
#include <container_positions.hpp>

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

    // create and load texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load an texture image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("container2.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // create and load texture (specular maps)
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load an texture image
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data2 = stbi_load("container2_specular.png", &width, &height, &nrChannels, 0);
    if (data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data2);

    // create and load texture (color specular maps)
    unsigned int texture3;
    glGenTextures(1, &texture3);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture3);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load an texture image
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data3 = stbi_load("lighting_maps_specular_color.png", &width, &height, &nrChannels, 0);
    if (data3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data3);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data3);

    // create and load texture (color specular maps)
    unsigned int texture4;
    glGenTextures(1, &texture4);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture4);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load an texture image
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data4 = stbi_load("matrix.jpg", &width, &height, &nrChannels, 0);
    if (data4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data4);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data4);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)*3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)*6));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Vertex arrays lighting
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the vertex attribute 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    Shader myshader[] = {
        Shader("./shaders/vertex_directional_light.glsl", "./shaders/fragment_directional_light.glsl"),
        Shader("./shaders/vertex_point_light.glsl", "./shaders/fragment_point_light.glsl"),
        Shader("./shaders/vertex_flash_light.glsl", "./shaders/fragment_flash_light.glsl"),
        Shader("./shaders/vertex.glsl", "./shaders/fragment.glsl")
    };
    
    // shaders settings
    glm::mat4 model = glm::mat4(1.0f);
    for(int i = 0; i < 4; i++) {
        myshader[i].use();
        myshader[i].setInt("material.diffuse", 0); // set texture number 0 as materials diffuse
        myshader[i].setInt("material.specular", 1); // set texture number 1 as material specular
        myshader[i].setInt("material.emission", 3); // set texture number 3 as material emission
        
        myshader[i].setMat4("model", model);
    }	

    Shader lightingShader = Shader("./shaders/light_vertex.glsl", "./shaders/light_fragment.glsl");

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // light properties
    glm::vec3 lightAmbient(0.2f, 0.2f, 0.2f);
    glm::vec3 lightDiffuse(0.5f, 0.5f, 0.5f);
    glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);

    bool moveLight = true;
    bool partyTime = false;
    int shininess = 32;
    bool invertSpecular = false;
    bool colorSpecular = false;
    bool useEmissionMaps = false;

    float radius = 2.0f;
    float speed = 1.5f;
    float partySpeed = 0.7f;
    float angle = M_PI / 4;
    float partyAngle = 0.0f;

    int lightingType = 0;

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
        ImGui::Checkbox("Use emission maps", &useEmissionMaps);
        ImGui::Checkbox("Color specular maps", &colorSpecular);
        ImGui::Checkbox("Invert specular maps", &invertSpecular);
        ImGui::SliderInt("shininess (2 - 256)", &shininess, 2, 256, "%d");
        ImGui::SeparatorText("Type of lighting:");
        ImGui::RadioButton("Directional light", &lightingType, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Point light", &lightingType, 1);
        ImGui::SameLine();
        ImGui::RadioButton("Flash light", &lightingType, 2);
        ImGui::SeparatorText("Color settings:");
        ImGui::ColorEdit3("light ambient", (float*)&lightAmbient, ImGuiColorEditFlags_Float);
        ImGui::ColorEdit3("light diffuse", (float*)&lightDiffuse, ImGuiColorEditFlags_Float);
        ImGui::ColorEdit3("light specular", (float*)&lightSpecular, ImGuiColorEditFlags_Float);
        ImGui::SeparatorText("Other settings:");
        ImGui::Checkbox("Light is moving", &moveLight);
        ImGui::SliderFloat("light angle (0 - 2 * PI)", &angle, 0, 2 * M_PI, "%.3f");
        ImGui::Checkbox("Party time", &partyTime);
        if(ImGui::Button("Turn off light!")) {
            // light properties
            partyTime = false;
            lightAmbient = glm::vec3(0.0f);
            lightDiffuse = glm::vec3(0.0f);
            lightSpecular = glm::vec3(0.0f);
        }
        ImGui::SameLine();
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

        // update selected shader properties    
        myshader[lightingType].use();
        myshader[lightingType].setMat4("projection", projection);
        myshader[lightingType].setMat4("view", view);
        myshader[lightingType].setVec3("viewPos", mycamera.Position);

        // set material properties
        myshader[lightingType].setInt("material.shininess", shininess);
    
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

        if(lightingType == 1) // point light 
        {
            myshader[lightingType].setFloat("light.constant",  1.0f);
            myshader[lightingType].setFloat("light.linear",    0.09f);
            myshader[lightingType].setFloat("light.quadratic", 0.032f);
            myshader[lightingType].setVec3("light.position", lightPos); // update light position
        }
        
        if(lightingType == 0) // directional light 
        {
            myshader[lightingType].setVec3("light.direction", -0.2f, -1.0f, -0.3f);
        }

        if(lightingType == 2) // flash light
        {
            myshader[lightingType].setVec3("light.position",  mycamera.Position);
            myshader[lightingType].setVec3("light.direction", mycamera.Front);
            myshader[lightingType].setFloat("light.cutOff",   glm::cos(glm::radians(12.5f)));
            myshader[lightingType].setFloat("light.outerCutOff",   glm::cos(glm::radians(17.5f)));
        }

        myshader[lightingType].setVec3("light.ambient", lightAmbient);
        myshader[lightingType].setVec3("light.diffuse", lightDiffuse);
        myshader[lightingType].setVec3("light.specular", lightSpecular);
        if(colorSpecular) 
        {
            myshader[lightingType].setInt("material.specular", 2); // color specular maps
        } else
        {
            myshader[lightingType].setInt("material.specular", 1); // black and white specular maps
        }
        myshader[lightingType].setBool("invertSpecular", invertSpecular); // invert or not
        myshader[lightingType].setBool("useEmissionMaps", useEmissionMaps); // emission or not

        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
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

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

    Shader myshader = Shader("./shaders/vertex.glsl", "./shaders/fragment.glsl");
    
    // shaders settings
    glm::mat4 model = glm::mat4(1.0f);
    myshader.use();
    myshader.setInt("material.diffuse", 0); // set texture number 0 as materials diffuse
    myshader.setInt("material.specular", 1); // set texture number 1 as material specular
    myshader.setInt("material.emission", 3); // set texture number 3 as material emission
    
    myshader.setMat4("model", model);

    Shader lightingShader = Shader("./shaders/light_vertex.glsl", "./shaders/light_fragment.glsl");

    glEnable(GL_DEPTH_TEST);

    // light properties
    glm::vec3 lightAmbient(0.2f, 0.2f, 0.2f);
    glm::vec3 lightDiffuse(0.5f, 0.5f, 0.5f);
    glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);

    // direct light
    glm::vec3 directLightAmbient(0.2f, 0.2f, 0.2f);
    glm::vec3 directLightDiffuse(0.5f, 0.5f, 0.5f);
    glm::vec3 directLightSpecular(1.0f, 1.0f, 1.0f);
    // spot light
    glm::vec3 spotLightAmbient(0.2f, 0.2f, 0.2f);
    glm::vec3 spotLightDiffuse(0.5f, 0.5f, 0.5f);
    glm::vec3 spotLightSpecular(1.0f, 1.0f, 1.0f);
    // point lights
    glm::vec3 pointLightsAmbient[] = {
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.2f, 0.2f, 0.2f)
    };
    glm::vec3 pointLightsDiffuse[] = {
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.5f, 0.5f, 0.5f)
    };
    glm::vec3 pointLightsSpecular[] = {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    };

    bool moveLight = false;
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

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    }; 

    // some properties
    bool useDirectLight = true;
    bool usePointLights = true;
    bool useFlashLight = true;

    // direct light settings
    bool directLightReverse = false;

    // spot light properties
    float cutOff = 12.5f;
    float outerCutOff = 17.5f;

    // point light settings
    float pointLightsLinear[] = {0.09f, 0.09f, 0.09f, 0.09f};
    float pointLightsQuadratic[] = {0.032f, 0.032f, 0.032f, 0.032f};

    // clear color
    glm::vec3 clearColor(0.1f, 0.1f, 0.1f);

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
        // if(ImGui::CollapsingHeader("Settings:"))
        ImGui::SeparatorText("Material settings:");
        ImGui::Checkbox("Use emission maps", &useEmissionMaps); ImGui::SameLine();
        ImGui::Checkbox("Color specular maps", &colorSpecular); ImGui::SameLine();
        ImGui::Checkbox("Invert specular maps", &invertSpecular);
        ImGui::SliderInt("shininess (2 - 256)", &shininess, 2, 256, "%d");
        ImGui::SeparatorText("Lights:");
        ImGui::ColorEdit3("Clear color", (float*)&clearColor, ImGuiColorEditFlags_Float);
        ImGui::Checkbox("Use direct light", &useDirectLight); ImGui::SameLine();
        ImGui::Checkbox("Use point lights", &usePointLights); ImGui::SameLine();
        ImGui::Checkbox("Use flash light", &useFlashLight);
        if(useDirectLight)
            if(ImGui::CollapsingHeader("Direct light settings:"))
            {
                ImGui::Checkbox("Direct light reverse", &directLightReverse);
                ImGui::SeparatorText("Light color:");
                ImGui::ColorEdit3("Direct light ambient", (float*)&directLightAmbient, ImGuiColorEditFlags_Float);
                ImGui::ColorEdit3("Direct light diffuse", (float*)&directLightDiffuse, ImGuiColorEditFlags_Float);
                ImGui::ColorEdit3("Direct light specular", (float*)&directLightSpecular, ImGuiColorEditFlags_Float);
            }
        if(usePointLights)
            if(ImGui::CollapsingHeader("Point lights settings:"))
            {
                if(ImGui::CollapsingHeader("Point light 0 settings:")) {
                    ImGui::SliderFloat("light 0 linear", &pointLightsLinear[0], 0.0014, 0.7, "%.4f");
                    ImGui::SliderFloat("light 0 quadratic", &pointLightsQuadratic[0], 0.0, 1.8, "%.4f");
                    ImGui::ColorEdit3("Point 0 light ambient", (float*)&pointLightsAmbient[0], ImGuiColorEditFlags_Float);
                    ImGui::ColorEdit3("Point 0 light diffuse", (float*)&pointLightsDiffuse[0], ImGuiColorEditFlags_Float);
                    ImGui::ColorEdit3("Point 0 light specular", (float*)&pointLightsSpecular[0], ImGuiColorEditFlags_Float);
                }
                if(ImGui::CollapsingHeader("Point light 1 settings:")) {
                    ImGui::SliderFloat("light 1 linear", &pointLightsLinear[1], 0.0014, 0.7, "%.4f");
                    ImGui::SliderFloat("light 1 quadratic", &pointLightsQuadratic[1], 0.0, 1.8, "%.4f");
                    ImGui::ColorEdit3("Point 1 light ambient", (float*)&pointLightsAmbient[1], ImGuiColorEditFlags_Float);
                    ImGui::ColorEdit3("Point 1 light diffuse", (float*)&pointLightsDiffuse[1], ImGuiColorEditFlags_Float);
                    ImGui::ColorEdit3("Point 1 light specular", (float*)&pointLightsSpecular[1], ImGuiColorEditFlags_Float);
                }
                if(ImGui::CollapsingHeader("Point light 2 settings:")) {
                    ImGui::SliderFloat("light 2 linear", &pointLightsLinear[2], 0.0014, 0.7, "%.4f");
                    ImGui::SliderFloat("light 2 quadratic", &pointLightsQuadratic[2], 0.0, 1.8, "%.4f");
                    ImGui::ColorEdit3("Point 2 light ambient", (float*)&pointLightsAmbient[2], ImGuiColorEditFlags_Float);
                    ImGui::ColorEdit3("Point 2 light diffuse", (float*)&pointLightsDiffuse[2], ImGuiColorEditFlags_Float);
                    ImGui::ColorEdit3("Point 2 light specular", (float*)&pointLightsSpecular[2], ImGuiColorEditFlags_Float);
                }
                if(ImGui::CollapsingHeader("Point light 3 settings:")) {
                    ImGui::SliderFloat("light 3 linear", &pointLightsLinear[3], 0.0014, 0.7, "%.4f");
                    ImGui::SliderFloat("light 3 quadratic", &pointLightsQuadratic[3], 0.0, 1.8, "%.4f");
                    ImGui::ColorEdit3("Point 3 light ambient", (float*)&pointLightsAmbient[3], ImGuiColorEditFlags_Float);
                    ImGui::ColorEdit3("Point 3 light diffuse", (float*)&pointLightsDiffuse[3], ImGuiColorEditFlags_Float);
                    ImGui::ColorEdit3("Point 3 light specular", (float*)&pointLightsSpecular[3], ImGuiColorEditFlags_Float);
                }
            }
        if(useFlashLight)
            if(ImGui::CollapsingHeader("Flash light settings:"))
            {
                ImGui::SliderFloat("cutoff", &cutOff, 0.0, 180.0, "%.1f");
                ImGui::SliderFloat("outer cutoff", &outerCutOff, 0.0, 180.0, "%.1f");
                ImGui::SeparatorText("Light color:");
                ImGui::ColorEdit3("Spot light ambient", (float*)&spotLightAmbient, ImGuiColorEditFlags_Float);
                ImGui::ColorEdit3("Spot light diffuse", (float*)&spotLightDiffuse, ImGuiColorEditFlags_Float);
                ImGui::ColorEdit3("Spot light specular", (float*)&spotLightSpecular, ImGuiColorEditFlags_Float);
            }
        ImGui::SeparatorText("Other settings:");
        ImGui::Checkbox("Move lights", &moveLight);
        // ImGui::SliderFloat("light angle (0 - 2 * PI)", &angle, 0, 2 * M_PI, "%.3f");
        ImGui::Checkbox("Party time", &partyTime);
        if(ImGui::Button("Turn off light!")) {
            // light properties
            useDirectLight = false;
            usePointLights = false;
            useFlashLight = false;
            partyTime = false;
            moveLight = false;
        }
        ImGui::SameLine();
        if(ImGui::Button("Everything to dafault!")) {
            // material default
            invertSpecular = false;
            colorSpecular = false;
            useEmissionMaps = false;
            shininess = 32;
            // clear color
            clearColor = glm::vec3(0.1f, 0.1f, 0.1f);
            // light properties
            useDirectLight = true;
            usePointLights = true;
            useFlashLight = true;
            partyTime = false;
            moveLight = false;
            // directional default
            directLightReverse = false;
            directLightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
            directLightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
            directLightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
            // point lights default
            pointLightPositions[0] = glm::vec3(0.7f,  0.2f,  2.0f);
            pointLightPositions[1] = glm::vec3(2.3f, -3.3f, -4.0f);
            pointLightPositions[2] = glm::vec3(-4.0f,  2.0f, -12.0f);
            pointLightPositions[3] = glm::vec3(0.0f,  0.0f, -3.0f);
            for(int p=0; p<4; p++) {
                pointLightsLinear[p] = 0.09f;
                pointLightsQuadratic[p] = 0.032f;
                pointLightsAmbient[p] = glm::vec3(0.2f, 0.2f, 0.2f);
                pointLightsDiffuse[p] = glm::vec3(0.5f, 0.5f, 0.5f);
                pointLightsSpecular[p] = glm::vec3(1.0f, 1.0f, 1.0f);
            }
            // flashlight default
            cutOff = 12.5f;
            outerCutOff = 17.5f;
            spotLightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
            spotLightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
            spotLightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
        }
        ImGui::SeparatorText("FPS:");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffers
        view = mycamera.GetViewMatrix();

        if(moveLight)
        {
            usePointLights = true;
            angle = std::remainder(angle + (deltaTime * speed), 2 * M_PI);
            if(angle < 0) angle +=  2 * M_PI;
            pointLightPositions[0] = glm::vec3(sin(angle) * 0.7f,  cos(angle) * 0.2f,  2.0f);
            pointLightPositions[1] = glm::vec3(sin(angle) * 2.3f,  cos(angle) * -3.3f,  -4.0f);
            pointLightPositions[2] = glm::vec3(sin(angle) * -4.0f,  cos(angle) * 2.0f,  -12.0f);
            pointLightPositions[3] = glm::vec3(0.0f, 0.0f,  sin(angle) * -3.0f);
        }
            
        lightPos = glm::vec3(
                            sin(angle) * radius, 
                            sin(angle) * radius, 
                            cos(angle) * radius);

        // update selected shader properties    
        myshader.use();
        myshader.setMat4("projection", projection);
        myshader.setMat4("view", view);
        myshader.setVec3("viewPos", mycamera.Position);

        // set material properties
        myshader.setInt("material.shininess", shininess);
    
        glm::vec3 lightColor = glm::vec3(1.0f);
        if(partyTime) {
            usePointLights = true;
            // set light properties
            partyAngle = std::remainder(partyAngle + (deltaTime * partySpeed), 2 * M_PI);
            if(partyAngle < 0) partyAngle +=  2 * M_PI;
            lightColor.x = (sin(partyAngle * 2.0f) / 2) + 0.5f;
            lightColor.y = (sin(partyAngle * 0.7f) / 2) + 0.5f;
            lightColor.z = (sin(partyAngle * 1.3f) / 2) + 0.5f;        
            lightDiffuse = lightColor   * glm::vec3(0.5f); 
            lightAmbient = lightDiffuse * glm::vec3(0.2f);
            for(int k=0; k<4; k++) 
            {
                pointLightsDiffuse[k] = lightDiffuse;
                pointLightsAmbient[k] = lightAmbient;
            }   
        }

        myshader.setBool("useDirectLight", useDirectLight);
        if(useDirectLight) 
        {
            // directional light settings
            if(directLightReverse)
                myshader.setVec3("dirLight.direction", -0.2f, 1.0f, -0.3f);
            else
                myshader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
            myshader.setVec3("dirLight.ambient", directLightAmbient);
            myshader.setVec3("dirLight.diffuse", directLightDiffuse);
            myshader.setVec3("dirLight.specular", directLightSpecular);
        }         

        myshader.setBool("usePointLights", usePointLights);
        if(usePointLights) 
        {
            // point light settings
            myshader.setVec3("pointLights[0].position", pointLightPositions[0]);
            myshader.setVec3("pointLights[1].position", pointLightPositions[1]);
            myshader.setVec3("pointLights[2].position", pointLightPositions[2]);
            myshader.setVec3("pointLights[3].position", pointLightPositions[3]);
            
            myshader.setFloat("pointLights[0].constant", 1.0f);
            myshader.setFloat("pointLights[1].constant", 1.0f);
            myshader.setFloat("pointLights[2].constant", 1.0f);
            myshader.setFloat("pointLights[3].constant", 1.0f);
            myshader.setFloat("pointLights[0].linear", pointLightsLinear[0]);
            myshader.setFloat("pointLights[1].linear", pointLightsLinear[1]);
            myshader.setFloat("pointLights[2].linear", pointLightsLinear[2]);
            myshader.setFloat("pointLights[3].linear", pointLightsLinear[3]);
            myshader.setFloat("pointLights[0].quadratic", pointLightsQuadratic[0]);
            myshader.setFloat("pointLights[1].quadratic", pointLightsQuadratic[1]);
            myshader.setFloat("pointLights[2].quadratic", pointLightsQuadratic[2]);
            myshader.setFloat("pointLights[3].quadratic", pointLightsQuadratic[3]);

            myshader.setVec3("pointLights[0].ambient", pointLightsAmbient[0]);
            myshader.setVec3("pointLights[1].ambient", pointLightsAmbient[1]);
            myshader.setVec3("pointLights[2].ambient", pointLightsAmbient[2]);
            myshader.setVec3("pointLights[3].ambient", pointLightsAmbient[3]);
            myshader.setVec3("pointLights[0].diffuse", pointLightsDiffuse[0]);
            myshader.setVec3("pointLights[1].diffuse", pointLightsDiffuse[1]);
            myshader.setVec3("pointLights[2].diffuse", pointLightsDiffuse[2]);
            myshader.setVec3("pointLights[3].diffuse", pointLightsDiffuse[3]);
            myshader.setVec3("pointLights[0].specular", pointLightsSpecular[0]);
            myshader.setVec3("pointLights[1].specular", pointLightsSpecular[1]);
            myshader.setVec3("pointLights[2].specular", pointLightsSpecular[2]);
            myshader.setVec3("pointLights[3].specular", pointLightsSpecular[3]);
        }
        
        myshader.setBool("useFlashLight", useFlashLight);
        if(useFlashLight) 
        {
            // spotlight settings
            myshader.setVec3("spotLight.position", mycamera.Position);
            myshader.setVec3("spotLight.direction", mycamera.Front);
            myshader.setFloat("spotLight.cutOff", glm::cos(glm::radians(cutOff)));
            myshader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(outerCutOff)));
            myshader.setVec3("spotLight.ambient", spotLightAmbient);
            myshader.setVec3("spotLight.diffuse", spotLightDiffuse);
            myshader.setVec3("spotLight.specular", spotLightSpecular);
        }

        if(colorSpecular) 
        {
            myshader.setInt("material.specular", 2); // color specular maps
        } else
        {
            myshader.setInt("material.specular", 1); // black and white specular maps
        }
        myshader.setBool("invertSpecular", invertSpecular); // invert or not
        myshader.setBool("useEmissionMaps", useEmissionMaps); // emission or not

        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        if(usePointLights) {
            // update lighting position
            lightingShader.use();
            lightingShader.setMat4("projection", projection);
            lightingShader.setMat4("view", view);
            glBindVertexArray(lightVAO);

            for(unsigned int j = 0; j < 4; j++)
            {
                glm::mat4 modelL = glm::translate(glm::mat4(1.0f), pointLightPositions[j]);
                modelL = glm::scale(modelL, glm::vec3(0.2f));
                lightingShader.setMat4("model", modelL);
                lightingShader.setVec3("lightColor", pointLightsDiffuse[j]);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

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

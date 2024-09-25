#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>
#include <stb_image.hpp>

#include <shader_class.hpp>
#include <camera.hpp>
#include <window.hpp>

#include <cube_vertices.hpp>

int main()
{
    if(CreateWindow() != 0) return -1;  

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
    // send transformation matrices to shaders (main cube)
    // glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // rotate along x axis (world space)
    glm::mat4 model = glm::mat4(1.0f);
    myshader.setMat4("model", model);
    myshader.setMat4("view", view);
    myshader.setMat4("projection", projection);
    myshader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    myshader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
    myshader.setVec3("lightPos", lightPos);
    myshader.setVec3("viewPos", mycamera.Position);

    Shader lightingShader = Shader("./shaders/light_vertex.glsl", "./shaders/light_fragment.glsl");
    lightingShader.use();
    // lighting cube
    glm::mat4 Lighting_model = glm::translate(glm::mat4(1.0f), lightPos);
    Lighting_model = glm::scale(Lighting_model, glm::vec3(0.2f));
    lightingShader.setMat4("model", Lighting_model);
    lightingShader.setMat4("view", view);
    lightingShader.setMat4("projection", projection);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // Draw loop
    while(!glfwWindowShouldClose(window))
    {
        // some boilerplate code
        cout_time(); // account for frametime
        processInput(window); // Input
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffers
        view = mycamera.GetViewMatrix();
        
        myshader.use();
        myshader.setMat4("projection", projection);
        myshader.setMat4("view", view);
        myshader.setVec3("viewPos", mycamera.Position);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightingShader.use();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    // Exit the application
    glfwTerminate();
    return 0;
}

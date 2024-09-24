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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);   
    glEnableVertexAttribArray(0);

    Shader myshader = Shader("./shaders/vertex.glsl", "./shaders/fragment.glsl");
    myshader.use();

    // send transformation matrices to shaders
    myshader.setMat4("model", model);
    myshader.setMat4("view", view);
    myshader.setMat4("projection", projection);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // Draw loop
    while(!glfwWindowShouldClose(window))
    {
        // some boilerplate code
        cout_time(); // account for frametime
        processInput(window); // Input
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffers
        
        // update projection and view matrices
        myshader.setMat4("projection", projection);
        view = mycamera.GetViewMatrix();
        myshader.setMat4("view", view);

        // draw objects here..
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    // Exit the application
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
#ifdef IMGUI_VERSION
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
#endif
#ifdef USE_FRAMEBUFFER
void recreate_framebuffer();
// for framebuffer
bool framebuffer_firstrun = true;
unsigned int framebuffer;
unsigned int textureColorbuffer;
unsigned int rbo;
#endif

GLFWwindow* window = NULL;
int WindowWidth = 800;
int WindowHeight = 600;

glm::mat4 projection;
glm::mat4 view;

// for calculating camra rotation
bool immersion = true;
bool firstMouse = true;
float lastX = 400, lastY = 300;
float pitch = 0.0f, yaw = -90.0f;

// the camera
Camera mycamera = Camera(glm::vec3(0.0f, 0.0f,  3.0f));
float FoV = mycamera.Zoom;

// to keep track of frametime
float currentFrame = 0.0f;
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int CreateWindow() {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    window = glfwCreateWindow(WindowWidth, WindowHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }  

    glViewport(0, 0, WindowWidth, WindowHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // capture the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
#ifdef IMGUI_VERSION
    glfwSetMouseButtonCallback(window, mouse_button_callback);
#endif
    // Going3D
    projection = glm::perspective(glm::radians(FoV), (float)WindowWidth / (float)WindowHeight, 0.1f, 100.0f); // perspective projection camera
    view = mycamera.GetViewMatrix(); // new view

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    WindowWidth = width;
    WindowHeight = height;
    glViewport(0, 0, width, height);
    projection = glm::perspective(glm::radians(FoV), (float)WindowWidth / (float)WindowHeight, 0.1f, 100.0f);
#ifdef USE_FRAMEBUFFER
    recreate_framebuffer();
#endif
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        // glfwSetWindowShouldClose(window, true);
        firstMouse = true;
        immersion = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        immersion = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if(!immersion) return;
    // new move camera
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        mycamera.ProcessKeyboard(FORWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        mycamera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        mycamera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        mycamera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        mycamera.ProcessKeyboard(SPACE, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        mycamera.ProcessKeyboard(CTRL, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
    if(!immersion) return;

    if(firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    mycamera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
    if(!immersion) return;

    mycamera.ProcessMouseScroll((float)yoffset);
    FoV = mycamera.Zoom;

    projection = glm::perspective(glm::radians(FoV), (float)WindowWidth / (float)WindowHeight, 0.1f, 100.0f);
}

#ifdef IMGUI_VERSION
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
    if(!immersion) {
        ImGuiIO& io = ImGui::GetIO();
        if(io.WantCaptureMouse) {
            io.AddMouseButtonEvent(button, action);
        }
    }
}
#endif

#ifdef USE_FRAMEBUFFER
void recreate_framebuffer()
{
    // framebuffer configuration
    // -------------------------
    // unsigned int framebuffer;
    if(framebuffer_firstrun) glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    // unsigned int textureColorbuffer;
    if(framebuffer_firstrun) glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WindowWidth, WindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    // unsigned int rbo;
    if(framebuffer_firstrun) glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WindowWidth, WindowHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if(framebuffer_firstrun) framebuffer_firstrun = false;
}
#endif

void cout_time()
{
    // account for frametime
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}
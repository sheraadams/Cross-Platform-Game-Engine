// library includes
#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
// learnopengl resources includes
#include "shader.h"
#include "camera.h"
#include "model.h"
// shapes
#include "objects.h"
#include "texture.h"
#include "pen_accent.h"
#include "pen_body.h"
#include "pen_clip.h"
#include "pen_point.h"

/* FUNCTIONS */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

//variables to move objects for development
int t = 0;
int r = 0;
std::vector<unsigned int> textures;
float x = -.0f;
float y = -1.45f; // we are hiding the box in the desk for now
float z = -3.2f; // we do this setting y-position to -1.3 and the z position to -3.2
float xx = 0.0f;
float yy = 0.0f;
float zz = 0.0f;

/* VARIABLES */
float speed = 45.0f; // light orbiting speed
float direction = -1.0; // used for light orbiting direction
bool lampIsOrbiting = true;
bool Keys[1024], firstMouse = true, onPerspective = true;
float SCR_WIDTH = 1000, SCR_HEIGHT = 900;
float lastX = (float)SCR_WIDTH / 2.0, lastY = (float)SCR_HEIGHT / 2.0;
float lastFrame = 0.0f, deltaTime = 0.0f;
GLfloat xoffset = 0.0f, yoffset = 0.0f;
/* VARIABLES */
bool usingCamera = true;
// initialize the camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

int main()
{
    /* GLFW INITIALIZE */
    glfwInit();
    // set the version to be used, I am using version 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // create the glfw window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Shera Adams: Final Project", NULL, NULL);
    // output error message if the window fails to create
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // make the window the current context for the openGL application
    glfwMakeContextCurrent(window);
    // tell glfw to capture the mouse movement within the window and scroll movement within the window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // changed to glfw_cursor_normal form glfw_cursor_disabled
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // load openGL pointers and glad pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Query and print OpenGL version
    const GLubyte* version = glGetString(GL_VERSION);
    if (version) {
        std::cout << "OpenGL Version: " << version << std::endl;
    } else {
        std::cerr << "Failed to get OpenGL version" << std::endl;
    }

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark(); // Use the dark theme,
    // Initialize ImGui for GLFW and OpenGL3

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    glEnable(GL_DEPTH_TEST);

    /* LOAD SHADERS FROM FILES */
    Shader lightingShader("specular.vs", "specular.fs");
    Shader skyboxShader("skybox.vs", "skybox.fs");
    Shader pinkShader("glsl.vs", "light_pink.fs");
    Shader purpleShader("glsl.vs", "light_purple.fs");

    /* VERTICES */
    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,   // this is a modufied cube z axis is my x axis and it is modified to be less tall
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,  // left
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f, // right
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // back
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  // right
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  // top
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  //bottom
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    float compassVertices[] = {
            0.49f, -0.5f, .5f,     0.0f,  0.0f, -1.0f,     0.0f, 0.0f, // back rectangle
            0.5f, -0.5f, .5f,      0.0f, -1.0f,  0.0f,     0.0f, 1.0f,
            0.49f, 0.5f, .5f,      0.0f,  0.0f, -1.0f,     1.0f, 0.0f,
            0.5f, 0.5f, .5f,       0.0f,  0.0f, -1.0f,     1.0f, 1.0f,
            0.5f, -0.5f, .5f,      0.0f,  0.0f, -1.0f,     0.0f, 1.0f,

            0.49f, 0.5f, .5f,      0.0f,  0.0f,  1.0f,     1.0f, 0.0f,  // bottom
            0.0f, 0.0f, 1.0f,      0.0f,  0.0f,  1.0f,     0.5f, 0.5f,
            0.49f, -0.5f, .5f,     0.0f,  0.0f,  1.0f,     0.0f, 0.0f,

            0.49f, 0.5f, 0.5f,     0.0f,  0.0f,  1.0f,     1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,      0.0f,  0.0f,  1.0f,     0.5f, 0.5f,
            0.49f, 0.5f, .5f,      -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,

            0.5f, 0.5f, .5f,       -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,     // modified pyramid to shift .1 down (y axis modified after camera transformations)
            0.0f, 0.0f, 1.0f,      -1.0f,  0.0f,  0.0f,    0.5f, 0.5f,
            0.5f, 0.5f, .5f,       -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
            0.5f, -0.5f, .5f,      -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,      1.0f,  0.0f,  0.0f,     0.5f, 0.5f,
            0.5f, -0.5f, .5f,      1.0f,  0.0f,  0.0f,     0.0f, 0.0f,
            0.49f, -0.5f, .5f,     1.0f,  0.0f,  0.0f,     1.0f, 0.0f,
    };


    float boxVertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,  // left
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // back
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  // front
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  // top
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  //bottom
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    float skyboxVertices[] = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };

    // light box positions
    glm::vec3 lightPositions[] = {
            glm::vec3(-3.0f, 0.0f, 3.0f),
            glm::vec3(-3.0f, 0.0f, -3.0f),
    };
    // lighting positions for our scene
    glm::vec3 pointLightPositions[] = {
            glm::vec3(2.0f, 1.0f, 2.0f),
            glm::vec3(1.0f, 2.0f, -3.0f),
            glm::vec3(-3.0f, 1.0f, -2.0f),
            glm::vec3(1.0f, 1.0f, 2.0f),
            glm::vec3(-0.0f, 2.0f, -2.0f),
            glm::vec3(2.0f, 2.0f, 3.0f),
            glm::vec3(0.0f, 3.0f, -2.0f),
            glm::vec3(-2.0f, 1.0f, -2.0f),
            glm::vec3(1.0f, 2.0f, -3.0f),
            glm::vec3(2.0f, 1.0f, 2.0f),
            glm::vec3(3.0f, 1.0f, 0.0f),
            glm::vec3(2.0f, 1.0f, 2.0f),
            glm::vec3(2.0f, 1.0f, -2.0f),
            glm::vec3(-1.0f, 2.0f, 2.0f),
            glm::vec3(2.0f, 1.0f, 2.0f),
            glm::vec3(3.0f, 3.0f, -5.0f),
            glm::vec3(0.0f, 2.0f, 2.0f),
            glm::vec3(2.0f, 2.0f, 3.0f),
    };

    /* LIGHT CUBE UNIFORM MATRICES */
    unsigned int lightCubeVAO, lightCubeVBO;
    // generate vao
    glGenVertexArrays(1, &lightCubeVAO);
    // generate vbo for the light cubes
    glGenBuffers(1, &lightCubeVBO);
    // bind buffer to the vertices and create a buffer the size of the vertices
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    // bind vbo attribute pointers to the vao
    // our vertices have 3 floats per vertex = 3 floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // create a uniform buffer object (ubo)
    unsigned int uniformBlockIndexPink = glGetUniformBlockIndex(pinkShader.ID, "Matrices");
    unsigned int uniformBlockIndexPurple = glGetUniformBlockIndex(purpleShader.ID, "Matrices");
    // link uniform block to shader
    glUniformBlockBinding(pinkShader.ID, uniformBlockIndexPink, 0);
    glUniformBlockBinding(purpleShader.ID, uniformBlockIndexPurple, 0);
    // create buffer
    unsigned int uboMatrices;
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // define the buffer range
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
    // set the projection matrix
    glm::mat4 projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    /* LOAD TEXTURES */
    Textures texture;
    unsigned int redTexture = texture.loadTexture("resources/textures/class/red.jpg");
    unsigned int blueTexture = texture.loadTexture("resources/textures/class/blue.jpg");
    unsigned int lightgreyTexture = texture.loadTexture("resources/textures/class/lightgrey.jpg");
    unsigned int compassTexture = texture.loadTexture("resources/textures/class/protractor2.jpg");
    unsigned int skyboxTexture = texture.loadTexture("resources/textures/class/wall.png");
    unsigned int blackboardTexture = texture.loadTexture("resources/textures/class/blackboard.jpg");
    unsigned int groundTexture = texture.loadTexture("resources/textures/class/AdobeStock_321846439.jpg");
    unsigned int metalTexture = texture.loadTexture("resources/textures/class/metal.jpg");
    unsigned int deskTexture = texture.loadTexture("resources/textures/class/AdobeStock_372442505.jpg");
    unsigned int ballpointTexture = texture.loadTexture("resources/textures/class/1.jpg");
    unsigned int certificateTexture = texture.loadTexture("resources/textures/class/cert.jpg");
    unsigned int posterTexture = texture.loadTexture("resources/textures/class/poster.jpg");

    textures.push_back(blueTexture);
    textures.push_back(redTexture);
    textures.push_back(lightgreyTexture);
    textures.push_back(groundTexture);
    textures.push_back(metalTexture);

    /* CREATE A CUBEMAP SKYBOX */
    vector<std::string> faces
            {
                    "resources/textures/class/right.jpg", // right
                    "resources/textures/class/left.jpg", // left
                    "resources/textures/class/top.jpg", // top
                    "resources/textures/class/bottom.jpg", // bottom
                    "resources/textures/class/front.jpg", // front
                    "resources/textures/class/back.jpg", // back
            };
    unsigned int cubemapTexture = texture.loadCubemap(faces);

    // set the shader ints
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    /* SET THE PROJECTION AS PERSPECTIVE BY DEFAULT*/
    onPerspective = true;

    /* RENDER LOOP */
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow(); // Show demo window! :)

        // Create an ImGui window
        ImGui::SetNextWindowSize(ImVec2(350, 530));
        ImGui::SetNextWindowPos(ImVec2(560, 470));
        ImGui::Begin("Basic Design Controls");

        /* COLORS  */
        //ImVec4 blueColor = ImVec4(0.0f, 0.5f, 1.0f, 1.0f); //
        ImVec4 blueColor = ImVec4(1.0f, 1.f, 1.0f, 1.0f); //
        ImVec4 greenColor = ImVec4(0, 1, 0, 1);

        /* CENTER IMGUI */
        float windowWidth = ImGui::GetWindowContentRegionWidth();
        float buttonWidth = ImGui::CalcTextSize("Print Coordinates").x;
        float xPosition = (windowWidth - buttonWidth) * 0.5f;

        /* Position data */
        ImGui::TextColored(blueColor, "Position Controls:");
        // ImGui::TextColored(blueColor, "left, right, up, down, y, z");
        /* sliders */
        ImGui::SliderFloat("X", &x, -4.0f, 10.0f);
        ImGui::SliderFloat("Y", &y, -4.0f, 10.0f);
        ImGui::SliderFloat("Z", &z, -10.0f, 10.0f);
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "x: %.2f   y: %.2f   z: %.2f", x, y, z);
        ImGui::Separator();
        ImGui::Text("");

        /* scale data */
        ImGui::TextColored(blueColor, "Scale Controls:");
        // ImGui::TextColored(blueColor, "j, l, i, k");
        /* sliders */
        ImGui::SliderFloat("XX", &xx, -4.0f, 10.0f);
        ImGui::SliderFloat("YY", &yy, -4.0f, 10.0f);
        ImGui::SliderFloat("ZZ", &zz, -10.0f, 10.0f);
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "xx: %.2f   yy: %.2f   zz: %.2f", xx, yy, zz);
        ImGui::Separator();
        ImGui::Text("");

        /* Dropdown: perspective */
        int perspectiveChoice = onPerspective ? 0 : 1;
        ImGui::Text("Projection Type:");
        const char* perspectiveChoices[] = { "True (Perspective)", "False (Orthographic)" };
        const int numChoices = sizeof(perspectiveChoices) / sizeof(perspectiveChoices[0]);
        // Create a dropdown to select between "True (Perspective)" and "False (Orthographic)"
        if (ImGui::Combo("##ProjectionCombo", &perspectiveChoice, perspectiveChoices, numChoices)) {
            // Update onPerspective based on the selected choice
            onPerspective = (perspectiveChoice == 0);
        }


        /* Dropdown: lights */
        int lampOrbitChoice = lampIsOrbiting ? 0 : 1;
        ImGui::Text("Light Orbits:");
        const char* lampOrbitChoices[] = { "True (Orbiting)", "False (Not Orbiting)" };
        const int numLampOrbitChoices = sizeof(lampOrbitChoices) / sizeof(lampOrbitChoices[0]);
        // Create a dropdown to select between "True (Orbiting)" and "False (Not Orbiting)"
        if (ImGui::Combo("##LampOrbitCombo", &lampOrbitChoice, lampOrbitChoices, numLampOrbitChoices)) {
            // Update lampIsOrbiting based on the selected choice
            lampIsOrbiting = (lampOrbitChoice == 0);
        }

        /* Dropdown: camera */
        int cameraChoice = usingCamera ? 0 : 1;
        ImGui::Text("Using Camera:");
        const char* cameraChoices[] = { "True (Using Camera)", "False (Not Using Camera)" };
        const int numCameraChoices = sizeof(cameraChoices) / sizeof(cameraChoices[0]);
        // Create a dropdown to select between "True (Using Camera)" and "False (Not Using Camera)"
        if (ImGui::Combo("##CameraCombo", &cameraChoice, cameraChoices, numCameraChoices)) {
            // Update usingCamera based on the selected choice
            usingCamera = (cameraChoice == 0);
        }


        ImGui::Separator();
        ImGui::Text("");

        /* texture slider*/
        ImGui::TextColored(blueColor, "Texture Controls:");
        //ImGui::TextColored(blueColor, "r, t");
        if (ImGui::SliderInt("Texture 1", &t, 0, textures.size() - 1)) {
            // t is  updated via the slider
            if (t < 0) {
                t = textures.size() - 1;
            }
            else if (t >= textures.size()) {
                t = 0;
            }
        }

        /* texture slider 2*/
        if (ImGui::SliderInt("Texture 2", &r, 0, textures.size() - 1)) {
            // r is updated via the slider
            if (r < 0) {
                r = textures.size() - 1;
            }
            else if (r >= textures.size()) {
                r = 0;
            }
        }

        ImGui::Separator();
        ImGui::Text("");

        /* print coordinates to console button */
        // center the button
        ImGui::SetCursorPosX(xPosition);
        if (ImGui::Button("Print Coordinates"))
        {
            std::cout << "x: " << x << "\ny: " << y << "\nz: " << z << std::endl;
            std::cout << "xx: " << xx << "\nyy: " << yy << "\nzz: " << zz << std::endl;
            std::cout << endl;
        }

        ImGui::End(); // End ImGui window
        // process input throughout the loop
        processInput(window);

        // set the background color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // update the light position coordinates
        /****************************************************************/
        const float angularVelocity = glm::radians(45.0f);
        if (lampIsOrbiting)
        {
            glm::vec4 newPosition = glm::rotate(angularVelocity * deltaTime,
                                                glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(pointLightPositions[0], 1.0f);
            pointLightPositions[0].x = newPosition.x;
            pointLightPositions[0].y = newPosition.y;
            pointLightPositions[0].z = newPosition.z;

            glm::vec4 newPosition2 = glm::rotate(angularVelocity * deltaTime,
                                                 glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(pointLightPositions[1], 1.0f);
            pointLightPositions[1].x = newPosition.x;
            pointLightPositions[1].y = newPosition.y;
            pointLightPositions[1].z = newPosition.z;
        }

        /* LIGHTING SETTINGS FOR THE SCENE */
        /****************************************************************/
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 32.0f);
        // directional light
        lightingShader.setVec3("dirLight.direction", 0.2f, 0.0f, 0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        lightingShader.setVec3("pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[0].constant", 1.0f);
        lightingShader.setFloat("pointLights[0].linear", 0.09f);
        lightingShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        lightingShader.setVec3("pointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[1].constant", 1.0f);
        lightingShader.setFloat("pointLights[1].linear", 0.09f);
        lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[2].constant", 1.0f);
        lightingShader.setFloat("pointLights[2].linear", 0.09f);
        lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[3].constant", 1.0f);
        lightingShader.setFloat("pointLights[3].linear", 0.09f);
        lightingShader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09f);
        lightingShader.setFloat("spotLight.quadratic", 0.032f);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        float ambient[] = { 0.5f, 0.5f, 0.5f, 1 };
        float diffuse[] = { 0.8f, 0.8f, 0.8f, 1 };
        float specular[] = { 1.0f, 1.0f, 1.0f, 1 };
        float shininess = 128;
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);

        /* INITIALIZE VARAIBLES */
        glm::mat4 projection, view, model;

        /* SET PROJECTION
        /****************************************************************/
        if (onPerspective)
            projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        if (!onPerspective)
            projection = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, 1.0f, 100.0f);

        /* SET SHADER */
        view = camera.GetViewMatrix();
        lightingShader.use();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("model", model);

        /* RENDER SCENE */
        /* COMPASS */
        float angle = 0.0;
        Objects pyramid;
        pyramid.link(sizeof(compassVertices), compassVertices);
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(240.0f), glm::vec3(0.01f, 0.01f, 0.01f));
        model = glm::translate(model, glm::vec3(-1.85f, -0.2f, -.536f)); //(forward .14, left .2, up..467) AFTER ROTATION
        model = glm::scale(model, glm::vec3(1.1f));
        model = glm::scale(model, glm::vec3(.75f, .75f, .25f));
        lightingShader.setMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, compassTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        /* CUTOUT BOX */
        glm::vec4 color1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spriteColor", color1);
        Objects box;
        box.link(sizeof(boxVertices), boxVertices);
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
        model = glm::translate(model, glm::vec3(0.0f, 1.65f, -3.240f));
        model = glm::scale(model, glm::vec3(9.0f, 6.5f, 9.0f));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, skyboxTexture);
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /* all cube-like shapes are bound to one vao and are instanced to make the desk,
        books, ground, and blackboard */
        Objects book;
        book.link(sizeof(vertices), vertices);
        for (unsigned int i = 0; i <= 12; i++)
        {   /* translate everything down 1 */
            model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.00f));
            glm::mat4 model = glm::mat4(1.0f);
            if (i == 0)
            {   /* DESK */
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.01f, 0.01f, 0.01f));
                model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.00f));
                model = glm::scale(model, glm::vec3(1.8f, 1.2f, 1.5f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, deskTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            if (i == 1)
            {   /* RIGHT STACK RED TOP BOOK */
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.00f));
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.01f, 0.01f, 0.01f));
                model = glm::translate(model, glm::vec3(0.0f, .62f, 0.05f));
                model = glm::scale(model, glm::vec3(.4f, .035f, .6f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, redTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            if (i == 2)
            {/* RIGHT STACK BLUE BOOK */
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.00f));
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.01f, -1.1f, 0.01f));
                model = glm::translate(model, glm::vec3(0.0f, .695f, 0.03f));
                model = glm::scale(model, glm::vec3(.4f, .035f, .4f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, blueTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            if (i == 3)
            {/* LEFT STACK BIG BOOK */
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.00f));
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.01f, 0.01f, 0.01f));
                model = glm::translate(model, glm::vec3(-.5f, .64f, 0.0285f));
                model = glm::scale(model, glm::vec3(.4f, .07f, .5f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, lightgreyTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            if (i == 4)
            { /* BIG BOOK SIDE BINDING */
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.00f));
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.01f, 0.01f, 0.01f));
                model = glm::translate(model, glm::vec3(-.5f, .605f, 0.03f));
                model = glm::scale(model, glm::vec3(.41f, .01f, .51f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, blueTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            if (i == 5)
            {  /* BIG BOOK TOP BINDING */
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.00f));
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.01f, 0.01f, 0.01f));
                model = glm::translate(model, glm::vec3(-.50f, .678f, 0.03f));
                model = glm::scale(model, glm::vec3(.41f, .01f, .51f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, blueTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            if (i == 6)
            {  /* BIG BOOK BOTTOM BINDING */
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.00f));
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.01f, 0.01f, 0.01f));
                model = glm::translate(model, glm::vec3(-.3f, .639f, 0.03f));
                model = glm::scale(model, glm::vec3(.01f, .07f, .51f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, blueTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            if (i == 7)
            {  /* TOP RED LEFT STACKED BOOK */
                angle = 7.0f;
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.00f));
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.00f, -.08f, 0.00f));
                model = glm::translate(model, glm::vec3(-.50f, .7f, 0.05));
                model = glm::scale(model, glm::vec3(.31f, .03f, .41f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, redTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            if (i == 8)
            {  /* RIGHT STACK MIDDLE BOOK GREY */
                angle = -9;
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.00f));
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.00f, 1.00f, 0.0f));
                model = glm::translate(model, glm::vec3(0.0f, .66f, 0.03f));
                model = glm::scale(model, glm::vec3(.4f, .035f, .6f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, lightgreyTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            if (i == 9)
            {  /* GROUND */
                angle = 0.0f;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.001f, 0.001f, 0.0001f));
                model = glm::translate(model, glm::vec3(0.0f, -1.6f, -3.25f));
                model = glm::scale(model, glm::vec3(9.0f, 0.03f, 9.0f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, groundTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            if (i == 10)
            {  /* BLACKBOARD */
                angle = 0.0f;
                model = glm::mat4(1.0f);
                model = glm::rotate(model, glm::radians(angle), glm::vec3(3.01f, 0.01f, -12.01f));
                model = glm::translate(model, glm::vec3(0.0f, 1.5f, -7.75f));
                model = glm::scale(model, glm::vec3(5.5f, 4.0f, .03f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, textures[t]);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            if (i == 11)
            {  /* CERTIFICATE */
                angle = 0.0f;
                model = glm::mat4(1.0f);
                model = glm::rotate(model, glm::radians(angle), glm::vec3(3.01f, 0.01f, -12.01f));
                model = glm::translate(model, glm::vec3(4.468f, 1.5f, -4.0f));
                model = glm::scale(model, glm::vec3(.02f, .9f, .9f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, certificateTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            if (i == 12)
            {  /* POSTER */
                angle = 0.0f;
                model = glm::mat4(1.0f);
                model = glm::rotate(model, glm::radians(angle), glm::vec3(3.01f, 0.01f, -12.01f));
                model = glm::translate(model, glm::vec3(-4.468f, 1.5f, -4.0f));
                model = glm::scale(model, glm::vec3(.02f, 1.7f, 1.7f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, posterTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        // unbind the vao after drawing all objects
        glBindVertexArray(0);

        // bind metal texture and use it on the next three objects
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, metalTexture);

        // set model and translate the next three objects down 2 and back 3
        model = glm::mat4(0.5f);
        model = glm::translate(model, glm::vec3(0.0f, -2.005f, -3.80f));

        // draw the penbody shape
        lightingShader.setMat4("model", model);
        PenBody penBody;
        penBody.Draw();

        // draw the penclip shape
        lightingShader.setMat4("model", model);
        PenClip penClip;
        penClip.Draw();

        // draw the sphere accent shape
        lightingShader.setMat4("model", model);
        PenAccent sphereAccent;
        sphereAccent.Draw();

        // bind the ballpoint texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ballpointTexture);

        // set the model
        lightingShader.setMat4("model", model);
        model = glm::mat4(0.5f);
        // draw the pen point
        PenPoint penPoint;
        penPoint.Draw();

        // moving an object
        Objects book1;
        book1.link(sizeof(vertices), vertices);
        model = glm::translate(model, glm::vec3(x, y, z));
        model = glm::scale(model, glm::vec3(xx + 1, yy + 1, zz + 1));
        lightingShader.setMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[r]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /* RENDER LIGHTS */
        // bind the light uniform buffer object and the lightcube vao
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindVertexArray(lightCubeVAO);
        // set the speed of the light movement
        // iterate through two draw loops binding the light cube vao twice in two different positions
        for (unsigned int i = 0; i < 2; i++)
        {
            if (i == 0)
            { /* PINK LIGHT */
                // initialize the shader and use it to draw pink lights
                pinkShader.use();
                pinkShader.setMat4("projection", projection);
                pinkShader.setMat4("view", view);
                model = glm::mat4(1.0f);
                /* ORBIT THE LIGHTS */
                if (lampIsOrbiting)
                    model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(speed) * direction * 2.0f, glm::vec3(0.0f, 1.0f, 0.f));
                // place the lights into the lightpoistions vector positions
                model = glm::translate(model, lightPositions[i]);
                model = glm::scale(model, glm::vec3(.25f));
                pinkShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else
            { /* PURPLE LIGHT */
                // initialize the shader and use it to draw purple lights
                purpleShader.use();
                purpleShader.setMat4("projection", projection);
                purpleShader.setMat4("view", view);
                model = glm::mat4(1.0f);
                if (lampIsOrbiting)
                    model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(speed) * 2.0f, glm::vec3(0.0f, 1.0f, 0.f));
                // place the lights into the lightpoistions vector positions
                model = glm::translate(model, lightPositions[i]);
                model = glm::scale(model, glm::vec3(.25f));
                purpleShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        /* RENDER SKYBOX */
        glDepthFunc(GL_LEQUAL);
        Objects skybox;
        skybox.skybox(sizeof(skyboxVertices), skyboxVertices);
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        skybox.bindSkybox();
        glDepthFunc(GL_LESS);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    /* DELETE VAOS AND CLEAR MEMORY */
    Objects skybox;
    skybox.clear();
    Objects cube;
    cube.clear();
    Objects box;
    box.clear();
    Objects pyramid;
    pyramid.clear();

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // clear textures
    texture.destroyTexture(blueTexture);
    texture.destroyTexture(skyboxTexture);
    texture.destroyTexture(lightgreyTexture);
    texture.destroyTexture(blackboardTexture);
    texture.destroyTexture(cubemapTexture);
    texture.destroyTexture(compassTexture);
    texture.destroyTexture(groundTexture);
    texture.destroyTexture(metalTexture);
    texture.destroyTexture(ballpointTexture);
    texture.destroyTexture(posterTexture);
    texture.destroyTexture(certificateTexture);

    glfwTerminate();
    return 0;
}

/* PROCESS INPUT */
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if ((glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS))
        onPerspective = false;
    if ((glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS))
        onPerspective = true;
    if ((glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS))
    {
        if (!usingCamera)
            usingCamera = true;
        else
            usingCamera = false;
    }
    if ((glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS))
    {
        if (!lampIsOrbiting)
            lampIsOrbiting = true;
        else
            lampIsOrbiting = false;
    }
    if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS))
        x -= .005;
    if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS))
        x += .005;
    if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS))
        y += .005;
    if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS))
        y -= .005;
    if ((glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS))
        z -= .005;
    if ((glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS))
        z += .005;
    if ((glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS))
        xx += .0005;
    if ((glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS))
        xx -= .0005;
    if ((glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS))
        yy += .0005;
    if ((glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS))
        yy -= .0005;
    if ((glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS))
    {
        std::cout << "x: " << x << "\ny: " << y << "\nz: " << z << std::endl;
        std::cout << "xx: " << xx << "\nyy: " << yy << "\nzz: " << zz << std::endl;
        std::cout << endl;
    }
    if ((glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS))
    {
        if (t < textures.size() - 1)
            t += 1;
        else
            t = 0;
    }
    if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS))
    {
        if (r < textures.size() - 1)
            r += 1;
        else
            r = 0;
    }
}
// mouse button handling
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    switch (button)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
        {
            if (action == GLFW_PRESS)
                cout << "Left mouse button pressed" << endl;
            else
                cout << "Left mouse button released" << endl;
        }
            break;

        case GLFW_MOUSE_BUTTON_MIDDLE:
        {
            if (action == GLFW_PRESS)
                cout << "Middle mouse button pressed" << endl;
            else
                cout << "Middle mouse button released" << endl;
        }
            break;

        case GLFW_MOUSE_BUTTON_RIGHT:
        {
            if (action == GLFW_PRESS)
                cout << "Right mouse button pressed" << endl;
            else
                cout << "Right mouse button released" << endl;
        }
            break;

        default:
            cout << "Unhandled mouse button event" << endl;
            break;
    }
}
/* CALLBACKS */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (usingCamera)
    {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// function to increase or decrease camera speed depending on the scroll direction
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
    // increase camera speed
    if (yoffset > 0)
        camera.MovementSpeed += 1.0f;
        // decrease camera speed
    else
        camera.MovementSpeed -= 1.0f;
}

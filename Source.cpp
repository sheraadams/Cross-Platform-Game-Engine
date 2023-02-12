/*
Copyright (c) 2022 Shera Adams

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

Except as contained in this notice, the name(s) of the above copyright holders
shall not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// Credit to learnopengl.com for Shader and Camera and Sphere
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include "petal.h"
#include "filesystem.h"
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "ufo.h"
#include "flower_stem.h"
#include "flower_leaf.h"
#include "sphere.h"
#include "flower_left.h"
#include "flower_right.h"
#include "flower_upper.h"
#include "flower_lower.h"
int texturePicker;
int t = 0;
int r = 0;
float xValue = -.15004f;
float yValue = 0.8f;
float zValue = -2.2f;
/* FUNCTIONS */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<std::string> faces);
void GetDesktopResolution(float& horizontal, float& vertical);
/* FUNCTIONS */

/* VARIABLES */
vector<unsigned int> textures;
glm::vec3 viewPos;
double viewX, viewZ;
GLuint  VBO, VAO = 0;
bool Keys[1024], firstMouse = true, onPerspective = true;
float SCR_WIDTH = 1000, SCR_HEIGHT = 900, speed = .1f, delta = .1f;
float lastX = (float)SCR_WIDTH / 2.0, lastY = (float)SCR_HEIGHT / 2.0;
float lastFrame = 0.0f, deltaTime = 0.0f;
unsigned int nrColumns = 7, spacing = 3, nrZ = 6, nrRows = 10, planeVAO, pyramidVAO;
GLfloat rotateX = 0.0f, rotateY = 0.0f, xoffset = 0.0f, yoffset = 0.0f;
glm::vec3 point = glm::vec3(0.0f, 0.0f, 0.0f);
/* VARIABLES */

/* CAMERA */
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
/* CAMERA */

void GetDesktopResolution(float& horizontal, float& vertical)
{
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    horizontal = desktop.right;
    vertical = desktop.bottom;
}

int main()
{
    GetDesktopResolution(SCR_WIDTH, SCR_HEIGHT);
    /* GLFW INITIALIZE */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Game", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    /* GLFW INITIALIZE */

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



    /* SHADERS */

    Shader lightingShader("simpleVS.vs", "simpleFS.fs");
    Shader skyboxShader("skybox.vs", "skybox.fs");
    Shader greenShader("glsl.vs", "light_green.fs");
    Shader pinkShader("glsl.vs", "light_pink.fs");
    Shader purpleShader("glsl.vs", "light_purple.fs");

    glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

    /* VERTICES */
    float cubeVertices[] = {
        -0.5f, -2.5f, -0.5f,    0.0f, 0.0f,
        0.5f, -2.5f, -0.5f, 	1.0f, 0.0f,   // top 
        0.5f, -1.5f, -0.5f, 	1.0f, 1.0f,
        0.5f, -1.5f, -0.5f, 	1.0f, 1.0f,
        -0.5f, -1.5f, -0.5f, 	0.0f, 1.0f,
        -0.5f, -2.5f, -0.5f, 	0.0f, 0.0f,

        -0.5f, -2.5f, 0.5f, 	0.0f, 0.0f,
        0.5f, -2.5f, 0.5f, 	    1.0f, 0.0f,
        0.5f, -1.5f, 0.5f, 		1.0f, 1.0f,
        0.5f, -1.5f, 0.5f, 		1.0f, 1.0f, // bottom 
        -0.5f, -1.5f, 0.5f, 	0.0f, 1.0f,
        -0.5f, -2.5f, 0.5f, 	0.0f, 0.0f,

        -0.5f, -1.5f, 0.5f, 	1.0f, 0.0f,
        -0.5f, -1.5f, -0.5f, 	1.0f, 1.0f,
        -0.5f, -2.5f, -0.5f, 	0.0f, 1.0f,
        -0.5f, -2.5f, -0.5f, 	0.0f, 1.0f,  // left
        -0.5f, -2.5f, 0.5f, 	0.0f, 0.0f,
        -0.5f, -1.5f, 0.5f, 	1.0f, 0.0f,

        0.5f, -1.5f, 0.5f, 		1.0f, 0.0f,
        0.5f, -1.5f, -0.5f, 	1.0f, 1.0f,
        0.5f, -2.5f, -0.5f, 	0.0f, 1.0f,
        0.5f, -2.5f, -0.5f, 	0.0f, 1.0f,  // right
        0.5f, -2.5f, 0.5f, 	    0.0f, 0.0f,
        0.5f, -1.5f, 0.5f, 		1.0f, 0.0f,

        -0.5f, -2.5f, -0.5f, 	0.0f, 1.0f,
        0.5f, -2.5f, -0.5f, 	1.0f, 1.0f,
        0.5f, -2.5f, 0.5f, 	    1.0f, 0.0f,
        0.5f, -2.5f, 0.5f, 	    1.0f, 0.0f,
        -0.5f, -2.5f, 0.5f, 	0.0f, 0.0f,  // back 
        -0.5f, -2.5f, -0.5f, 	0.0f, 1.0f,

        -0.5f, -1.5f, -0.5f, 	0.0f, 1.0f,
        0.5f, -1.5f, -0.5f, 	1.0f, 1.0f,
        0.5f, -1.5f, 0.5f, 		1.0f, 0.0f,
        0.5f, -1.5f, 0.5f, 		1.0f, 0.0f,
        -0.5f, -1.5f, 0.5f, 	0.0f, 0.0f, // front
        -0.5f, -1.5f, -0.5f, 	0.0f, 1.0f
    };

    float boxVertices[] = {


        -0.5f, -2.5f, -0.5f,    0.0f, 0.0f,
        0.5f, -2.5f, -0.5f, 	1.0f, 0.0f,   // top 
        0.5f, -1.5f, -0.5f, 	1.0f, 1.0f,
        0.5f, -1.5f, -0.5f, 	1.0f, 1.0f,
        -0.5f, -1.5f, -0.5f, 	0.0f, 1.0f,
        -0.5f, -2.5f, -0.5f, 	0.0f, 0.0f,

        -0.5f, -1.5f, 0.5f, 	1.0f, 0.0f,
        -0.5f, -1.5f, -0.5f, 	1.0f, 1.0f,
        -0.5f, -2.5f, -0.5f, 	0.0f, 1.0f,
        -0.5f, -2.5f, -0.5f, 	0.0f, 1.0f,  // left
        -0.5f, -2.5f, 0.5f, 	0.0f, 0.0f,
        -0.5f, -1.5f, 0.5f, 	1.0f, 0.0f,

        0.5f, -1.5f, 0.5f, 		1.0f, 0.0f,
        0.5f, -1.5f, -0.5f, 	1.0f, 1.0f,
        0.5f, -2.5f, -0.5f, 	0.0f, 1.0f,
        0.5f, -2.5f, -0.5f, 	0.0f, 1.0f,  // right
        0.5f, -2.5f, 0.5f, 	    0.0f, 0.0f,
        0.5f, -1.5f, 0.5f, 		1.0f, 0.0f,

        -0.5f, -2.5f, -0.5f, 	0.0f, 1.0f,
        0.5f, -2.5f, -0.5f, 	1.0f, 1.0f,
        0.5f, -2.5f, 0.5f, 	    1.0f, 0.0f,
        0.5f, -2.5f, 0.5f, 	    1.0f, 0.0f,
        -0.5f, -2.5f, 0.5f, 	0.0f, 0.0f,  // back 
        -0.5f, -2.5f, -0.5f, 	0.0f, 1.0f,

        -0.5f, -1.5f, -0.5f, 	0.0f, 1.0f,
        0.5f, -1.5f, -0.5f, 	1.0f, 1.0f,
        0.5f, -1.5f, 0.5f, 		1.0f, 0.0f,
        0.5f, -1.5f, 0.5f, 		1.0f, 0.0f,
        -0.5f, -1.5f, 0.5f, 	0.0f, 0.0f, // front
        -0.5f, -1.5f, -0.5f, 	0.0f, 1.0f


    };
    float pyrVertices[] = {
        // positions              // texture coords
         -0.5f, -0.5f, .5f,     0.0f, 0.0f,
         0.5f, -0.5f, .5f,      0.0f, 1.0f,
         -0.5f, 0.5f, .5f,      1.0f, 0.0f,
         0.5f, 0.5f, .5f,       1.0f, 1.0f,
         0.5f, -0.5f, .5f,      0.0f, 1.0f,

         -0.5f, 0.5f, .5f,      1.0f, 0.0f,
         0.0f, 0.0f, 1.0f,      0.5f, 0.5f,
         -0.5f, -0.5f, .5f,     0.0f, 0.0f,
         -0.5f, 0.5f, 0.5f,     1.0f, 0.0f,
         0.0f, 0.0f, 1.0f,      0.5f, 0.5f,

         -0.5f, 0.5f, .5f,      0.0f, 0.0f,
         0.5f, 0.5f, .5f,       1.0f, 0.0f,
         0.0f, 0.0f, 1.0f,      0.5f, 0.5f,
         0.5f, 0.5f, .5f,       0.0f, 0.0f,
         0.5f, -0.5f, .5f,      1.0f, 0.0f,
         0.0f, 0.0f, 1.0f,      0.5f, 0.5f,
         0.5f, -0.5f, .5f,      0.0f, 0.0f,
         -0.5f, -0.5f, .5f,     1.0f, 0.0f,
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

    glm::vec3 pointLightPositions[] = {
        glm::vec3(-5.0f, 7.0f, 5.0f),
        glm::vec3(-5.0f, 7.0f, -5.0f),
        glm::vec3(5.0f, 7.0f, 5.0f),
        glm::vec3(5.0f, 7.0f, -5.0f),
        glm::vec3(0.0f, 7.0f, 0.0f),
        glm::vec3(-5.0f, -7.0f, 5.0f),
        glm::vec3(-5.0f, -7.0f, -5.0f),
        glm::vec3(5.0f, -7.0f, 5.0f),
        glm::vec3(5.0f, -7.0f, -5.0f),
        glm::vec3(0.0f, -7.0f, 0.0f),

    };
    glm::vec3 lightPositions[] = {
        glm::vec3(-3.0f, -1.0f, 3.0f),
        glm::vec3(-3.0f, -1.0f, -3.0f),
        glm::vec3(3.0f, 0.0f, 3.0f),
        glm::vec3(3.0f, 0.0f, -3.0f),
        glm::vec3(0.0f, 4.0f, 0.0f),
        glm::vec3(-3.0f, -4.0f, 3.0f),
        glm::vec3(-3.0f, -4.0f, -3.0f),
        glm::vec3(3.0f, -4.0f, 3.0f),
        glm::vec3(3.0f, -4.0f, -3.0f),
        glm::vec3(0.0f, -4.0f, 0.0f),

    };
    /* CUBE */
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    unsigned int boxVAO, boxVBO;
    glGenVertexArrays(1, &boxVAO);
    glGenBuffers(1, &boxVBO);
    glBindVertexArray(boxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), &boxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    unsigned int pyrVAO, pyrVBO;
    glGenVertexArrays(1, &pyrVAO);
    glGenBuffers(1, &pyrVBO);
    glBindVertexArray(pyrVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pyrVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyrVertices), &pyrVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    /*  SKYBOX */
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    /*  SKYBOX */

    /*  LIGHT CUBE */
    unsigned int lightCubeVAO, lightCubeVBO;
    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &lightCubeVBO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(pinkShader.ID, "Matrices");
    unsigned int uniformBlockIndexGreen = glGetUniformBlockIndex(greenShader.ID, "Matrices");
    glUniformBlockBinding(pinkShader.ID, uniformBlockIndexRed, 0);
    glUniformBlockBinding(greenShader.ID, uniformBlockIndexGreen, 0);
    unsigned int uboMatrices;
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
    glm::mat4 projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    /*  LIGHT CUBE */

    /* TEXTURES */
    unsigned int ufoTexture = loadTexture("resources/textures/burst.png");
    unsigned int flowerTexture2 = loadTexture("resources/textures/myheiro.png");
    unsigned int flowerTexture1 = loadTexture("resources/textures/AdobeStock_416624854.jpeg");
    unsigned int flowerTexture = loadTexture("resources/textures/AdobeStock_408749181.jpeg");
    unsigned int skyboxTexture = loadTexture("resources/textures/AdobeStock_172317258.jpeg");
    unsigned int cubeTexture = loadTexture("resources/textures/box.png");
    unsigned int planeTexture = loadTexture("resources/textures/1.png");
    unsigned int planeTexture1 = loadTexture("resources/textures/AdobeStock_252775020.jpeg");
    unsigned int planeTexture2 = loadTexture("resources/textures/viktorsaznov deepspace.jpeg");
    unsigned int planeTexture3 = loadTexture("resources/textures/AdobeStock_481965458.jpeg");
    unsigned int planeTexture4 = loadTexture("resources/textures/background5.jpg");
    unsigned int planeTexture5 = loadTexture("resources/textures/AdobeStock_293211764.jpeg");


    textures.push_back(ufoTexture);
    textures.push_back(flowerTexture);
    textures.push_back(flowerTexture1);
    textures.push_back(flowerTexture2);
    textures.push_back(skyboxTexture);
    textures.push_back(planeTexture);
    textures.push_back(planeTexture);
    textures.push_back(planeTexture1);
    textures.push_back(planeTexture2);
    textures.push_back(planeTexture3);
    textures.push_back(planeTexture4);
    textures.push_back(planeTexture5);

    
    vector<std::string> faces
    {
        "resources/textures/right.jpg", // right 
        "resources/textures/left.jpg", // left 
        "resources/textures/top.jpg", // top  
        "resources/textures/bottom.jpg", // bottom  
        "resources/textures/front.jpg", // front
        "resources/textures/back.jpg", // back
    };
    unsigned int cubemap3Texture = loadCubemap(faces);
    /* TEXTURES */

    /* SET SHADERS */
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    /* SOUND ENGINE */
    //SoundEngine->play2D("LosingControl.mp3", true);

    /* SET THE PROJECTION */
    onPerspective = true;

    /* RENDER LOOP */
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /* SET SHADER */
        glm::mat4 projection, view, model;
        /* SET PROJECTION
        /****************************************************************/
        if (onPerspective)
            projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        if (!onPerspective)
            projection = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, 1.0f, 100.0f);

        view = camera.GetViewMatrix();
        lightingShader.use();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("model", model);

        /* RENDER SCENE */
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        
        /* GENERATE COLORS */
        double  timeValue = glfwGetTime();
        float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
        float blueValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
        float redValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
        if (redValue < .5)
            redValue = .5;
        if (blueValue < .5)
            blueValue = .5;
        if (greenValue < .5)
            greenValue = .5;
        lightingShader.setInt("spriteColor", 3);
        int vertexColorLocation = glGetUniformLocation(lightingShader.ID, "color");
        
        /* CUTOUT BOX */
        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spriteColor", color);     
        glBindVertexArray(boxVAO);
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
        model = glm::translate(model, glm::vec3(0.0f, 16.155, -3.240f));
        model = glm::scale(model, glm::vec3(8.0f, 7.0f , 8.0f));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[t]);
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
       
        glBindVertexArray(cubeVAO);
        for (int i = 0; i <= 1; i++)
        {
            if (i == 0)
            {   /* PLANE */
                model = glm::mat4(1.0f);
                model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
               // model = glm::translate(model, glm::vec3(0.0f, -1.255, -3.25));
                model = glm::translate(model, glm::vec3(.00496f, -1.26f, -3.32f));
                model = glm::scale(model, glm::vec3(8.0f, .04f, 8.0f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, planeTexture);
                lightingShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            if (i == 1)
            { /* FLOWER POT*/
                model = glm::mat4(1.0f);
                model = glm::rotate(model, glm::radians(240.0f), glm::vec3(0.01f, 0.01f, 0.01f));
                model = glm::translate(model, glm::vec3(-2.0f, 1.5f, -1.0f));
                model = glm::scale(model, glm::vec3(.75f, .75f, .65f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, cubeTexture);
                lightingShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        /* PYRAMIDS */
        glBindVertexArray(pyrVAO);
        float angle = 240.0;
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.01f, 0.01f, 0.01f));
        model = glm::scale(model, glm::vec3(1.75f, 1.75f, 2.25f));
        for (unsigned int i = 0; i < 2; i++)
        {
            if (i == 0)
                model = glm::translate(model, glm::vec3(-2.0f, -1.0f, -1.1f));
            if (i == 1)
                model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
            lightingShader.setMat4("model", model);
            // use the already bound cube texture
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // use flowr texture on the following objects
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[r]);
        //color = glm::vec4(redValue, 0.0f, blueValue, 1.0f);
        //lightingShader.setVec3("spriteColor", color);
        model = glm::mat4(1.0f);
        for (int i = 0; i <= 7; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(51.42857f * i), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::translate(model, glm::vec3(-0.00504, -0.124999, -2.04));
            model = glm::scale(model, glm::vec3(.5f, .5f, .5f));
            lightingShader.setMat4("model", model);
            Petal petal;
            petal.Draw();
        }

        model = glm::translate(model, glm::vec3(-.00040014, -2.70001, .00000102147f));
        lightingShader.setMat4("model", model);
        FlowerStem flowerStem;
        flowerStem.Draw();
       
        model = glm::translate(model, glm::vec3(0.0f, .395, .0550006));
        lightingShader.setMat4("model", model);
        FlowerLeaf flowerLeaf;
        flowerLeaf.Draw(); 

        model = glm::translate(model, glm::vec3(.0000400014, -0.15f, -0.200001f));
        lightingShader.setMat4("model", model);
        Sphere sphere;
        sphere.Draw();


        //color = glm::vec4(0.0f, 0.0f, blueValue, 1.0f);
        //lightingShader.setVec3("spriteColor", color);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ufoTexture);
        lightingShader.setMat4("model", model);
        Ufo ufo;
        ufo.Draw();

        /* RENDER LIGHTS */
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindVertexArray(lightCubeVAO);
        float speed = 45.0f;
        float direction = -1.0;
        for (unsigned int i = 0; i < 2; i++)
        {
            if (i == 1)
            { /* PINK LIGHTS */
                pinkShader.use();
                pinkShader.setMat4("projection", projection);
                pinkShader.setMat4("view", view);
                model = glm::mat4(1.0f);
                model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(speed) * direction * 2.0f, glm::vec3(0.0f, 1.0f, 0.f));
                model = glm::translate(model, lightPositions[i]);
                model = glm::scale(model, glm::vec3(.25f));
                pinkShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else
            { /* PURPLE LIGHTS */
                purpleShader.use();
                purpleShader.setMat4("projection", projection);
                purpleShader.setMat4("view", view);
                model = glm::mat4(1.0f);
                model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(speed) * 2.0f, glm::vec3(0.0f, 1.0f, 0.f));
                model = glm::translate(model, lightPositions[i]);
                model = glm::scale(model, glm::vec3(.25f));
                purpleShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        /* RENDER LIGHTS */

        /* RENDER SKYBOX */
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap3Texture);
        glDrawArrays(GL_TRIANGLES, 0, 72);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /* SWAP BUFFERS AND DELETE VAOS FROM MEMORY */
    Petal petal;
    petal.~Petal();
    FlowerLeaf flowerLeaf;
    flowerLeaf.~FlowerLeaf();
    FlowerStem flowerStem;
    flowerStem.~FlowerStem();
    Ufo ufo;
    ufo.~Ufo();
    Sphere sphere;
    sphere.~Sphere();

    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &lightCubeVBO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteVertexArrays(1, &boxVAO);
    glDeleteBuffers(1, &boxVBO);
    glDeleteVertexArrays(1, &pyrVAO);
    glDeleteBuffers(1, &pyrVBO);

    glDeleteTextures(1, &flowerTexture);
    glDeleteTextures(1, &flowerTexture2);
    glDeleteTextures(1, &flowerTexture1);
    glDeleteTextures(1, &planeTexture);
    glDeleteTextures(1, &planeTexture1);
    glDeleteTextures(1, &planeTexture2);
    glDeleteTextures(1, &planeTexture3);
    glDeleteTextures(1, &planeTexture4);
    glDeleteTextures(1, &planeTexture5);
    glDeleteTextures(1, &cubemap3Texture);
    glDeleteTextures(1, &ufoTexture);
    glDeleteTextures(1, &cubeTexture);

    glDeleteShader(lightingShader.ID);
    glDeleteShader(greenShader.ID);
    glDeleteShader(purpleShader.ID);
    glDeleteShader(skyboxShader.ID);
    glDeleteShader(pinkShader.ID);


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
    if ((glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS))
        onPerspective = false;
    if ((glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS))
        onPerspective = true;
    if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS))
        xValue -= .05;
    if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS))
        xValue += .05;
    if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS))
        yValue += .05;
    if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS))
        yValue -= .05;
    if ((glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS))
        zValue -= .05;
    if ((glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS))
        zValue += .05;
    if ((glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS))
        std::cout << "( " << xValue << "f, " << yValue << "f, " << zValue << "f)" << std::endl;
    if ((glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS))
    {
        if (t < textures.size() -1)
        {
            t += 1;
            texturePicker = textures[t];
        }
        else 
            t = 0;

    }
    if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS))
    {
        if (r < textures.size() -1)
        {
            r += 1;
            texturePicker = textures[r];
        }
        else
            r = 0;

    }
    
}

/* CALLBACKS */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

/* LOAD TEXTURE WITH STBI */
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}

/* LOAD CUBEMAP FOR SKYBOX */
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


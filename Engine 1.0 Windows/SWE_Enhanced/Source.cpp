// Shera Adams
// shera.adams@snhu.edu
// January 23,2024
//
// Version: 2.0, This is an enhanced version of my CS330 final project created for Southern New Hampshire University to 
// satisfy the software engineering category enhancement of CS499, Capstone class.
//
// The main purpose of Source.cpp is to create an OpenGL application with a full interactive scene in which the user can
// navigate the point of view using keyboard and mouse controls. It initializes GLFW and GLAD and follows the 
// OpenGL rendering pipeline to create a classroom scene with a cut-out box classroom with posters on the wall, and
// a desk, protractor, and books centered in front of a blackboard. 
//
// Issues: none

// library includes
#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
// Learn OpenGL resources includes
#include "shader.h"
#include "camera.h"
// additional includes
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
// shapes
#include "objects.h"
#include "texture.h"
#include "pen_accent.h"
#include "pen_body.h"
#include "pen_point.h"
#include "geometry.h"
// free type and irrKlang includes
#include <ft2build.h>
#include FT_FREETYPE_H
#include "irrKlang.h"

// create the sound engine and load the irrKlang library
#pragma comment(lib, "irrKlang.lib")
using namespace irrklang;
ISoundEngine* SoundEngine = createIrrKlangDevice();

// define our functions
// input processing functions and callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
// function to make the program fullscreen (windows only)
void GetDesktopResolution(float& horizontal, float& vertical);
// function to render the text to the screen
void RenderText(Shader& shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
void InitText();

// define our object manipulation variables
int t = 0; // initialize a variable to access an index of the textures vector
int r = 0; // initialize a second variable to access an index of the textures vector

// we are creating x, y, and z variables to manipulate a target object by incrementing the variables with a key press
// we will later create a box and store it inside of the desk to hide it from view
// to do this, we do this set the x-position to 0, the y-position to -1.3 and the z position to -1.78
float x = -.0f;
float y = -1.3f;
float z = -1.78f;

// initialize the xx, yy, and zz variables to later manipulate the target object scale
float xx = 0.0f;
float yy = 0.0f;
float zz = 0.0f;

// initialize our vector to store and scroll textures
std::vector<unsigned int> textures;

// set the sound options
float volume = 0.0f; // define volume, set it low
const std::string songName = "music/Aaron Adams original.mp3";  // define the song file

// character struct to hold font properties
struct Character
{
    unsigned int TextureID; // the font texture
    glm::ivec2   Size;      // The size of glyph
    glm::ivec2   Bearing;   // The offset from baseline to left/top corner of the glyph
    unsigned int Advance;   // The horizontal and vertical offset required to position the glyph
};
// define a map to store the character struct containing the texture, size, advance, and bearing of each character
std::map<GLchar, Character> Characters;

// initialize the vao and vbo to render the 2d text quad
GLuint textVAO, textVBO;

// define our text-rendering-related variables
// define the y position to start the final line of text in terms of distance from the bottom of the screen
float yPos = 100.0f; 
float xPos = 50.0f;  // define the x position to start in terms of distance from the left edge of the screen
float fontSize = 0.8f;  // define the font size
float spacing = 55.0f; // define the spacing we want between each line of text
// define the font colors
glm::vec3 textColor = glm::vec3(2.6f, 0.0f, 2.6f);
glm::vec3 textColor2 = glm::vec3(0.0f, 1.15f, .5f);
// define the variables
bool renderText = true;  // boolean for rendering text options

// define our lighting-related variables
float speed = 45.0f; // light orbiting speed
float direction = -1.0f; // used for light orbiting direction
bool lampIsOrbiting = true;  // boolean to set the orbiting lamp

// define our scene-related variables
bool firstMouse = true;  // boolean to indicate if the mouse movement was the first movement in the program
bool onPerspective = true;  // boolean to set the projection
float SCR_WIDTH = 1000.0f;  // define the screen height
float SCR_HEIGHT = 900.0f;  // define the screen width
float lastX = (float)SCR_WIDTH / 2.0f; // initialize the last horizontal mouse position to horizontal center
float lastY = (float)SCR_HEIGHT / 2.0f; // initialize the last vertical mouse position to vertical center
float lastFrame = 0.0f;  // create a variable to store the time stamp of the last frame
float deltaTime = 0.0f;  // create a variable to define the current time
GLfloat xoffset = 0.0f;  // define the horizonal offset for our callbacks
GLfloat yoffset = 0.0f;  // define the vertical offset for our callbacks

// initialize the camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// Function to make the program full screen (Windows only)
void GetDesktopResolution(float& horizontal, float& vertical)
{
    RECT desktop; // define a desktop using the RECT struct to define a rectangle
    const HWND hDesktop = GetDesktopWindow();  // get the handle for the desktop window using HWND function
    GetWindowRect(hDesktop, &desktop); // get the dimensions of the desktop
    horizontal = static_cast<float>(desktop.right); // store the horizontal dimensions of the desktop
    vertical = static_cast<float>(desktop.bottom); // store the vertical dimensions of the desktop
}

int main()
{
    // make the program fullscreen
    GetDesktopResolution(SCR_WIDTH, SCR_HEIGHT);

    // call gflwInit function to initialize the glfw library
    glfwInit();

    // set the version to be used 3.3 to 4.1 could be used here
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // create the glfw window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Shera Adams SWE Enhancement", NULL, NULL);
  
    // output an error message if the window fails to create
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

    // set the cursor to enabled to allow the user to close the window with the mouse,
    // the opposite condition would be GLFW_CURSOR_DISABLED
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // load OpenGL pointers and GLAD pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // call the init text function to initialize freetype, load the font, load the glyphs, and
    // store the texture, size, bearing, and advance properties in the character struct.
    InitText();

    // configure opengl to enable 3d environment
    glEnable(GL_DEPTH_TEST);
    // reduce aliasing artifacts
    glEnable(GL_MULTISAMPLE);

    // enable GL_BLEND and GL_ONE_MINUS_SRC_ALPHA to render the text to the screen
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // load the shaders from the files
    Shader lightingShader("specular.vs", "specular.fs"); // scene shaders
    Shader skyboxShader("skybox.vs", "skybox.fs");  // skybox shaders
    Shader pinkShader("glsl.vs", "light_pink.fs");  // pink light box shaders
    Shader purpleShader("glsl.vs", "light_purple.fs");  // purple light box shaders
    Shader textShader("text_shader.vs", "text_shader.fs"); // text shaders

    // set an orthographic projection matrix for text rendering
    glm::mat4 Text_projection = glm::ortho(0.0f, SCR_WIDTH, 0.0f, SCR_HEIGHT);
    textShader.use();
    glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(Text_projection));

    // get the vertices for the shapes from the geometry class
    Geometry geometry;
    std::vector<GLfloat>vertices = geometry.GetCubeVertices();
    std::vector<GLfloat>compassVertices = geometry.GetCompassVertices();
    std::vector<GLfloat>pyramidVertices = geometry.GetBoxVertices();
    std::vector<GLfloat>boxVertices = geometry.GetBoxVertices();
    std::vector<GLfloat>skyboxVertices = geometry.GetSkyboxVertices();
    std::vector<GLfloat>planeVertices = geometry.GetPlaneVertices();

    // define the light box initial positions on the screen
    glm::vec3 lightPositions[] = {
            glm::vec3(-3.0f, 0.0f, 3.0f),
            glm::vec3(-3.0f, 0.0f, -3.0f),
    };

    // define the lighting positions for our scene
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

    // generate the vao for the text quad
    glGenVertexArrays(1, &textVAO);
    // generate a buffer for the text quad
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    // bind buffer to the vertices and create a buffer the size of the vertices
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    // bind vbo attribute pointers to the vao
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // unbind the vao
    glBindVertexArray(0);

    // create vao and vbo for the light cubes
    unsigned int lightCubeVAO, lightCubeVBO;
    // generate vao
    glGenVertexArrays(1, &lightCubeVAO);
    // generate vbo for the light cubes
    glGenBuffers(1, &lightCubeVBO);
    glBindVertexArray(lightCubeVAO);
    // bind buffer to the vertices and create a buffer the size of the vertices
    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(GLfloat), &skyboxVertices[0], GL_STATIC_DRAW);
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
    // create a buffer
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


    // call the textures class to load textures with the loadTexture function
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

    // add textures to a vector of textures for texture scrolling
    textures.push_back(blackboardTexture);
    textures.push_back(blueTexture);
    textures.push_back(redTexture);
    textures.push_back(lightgreyTexture);
    textures.push_back(metalTexture);

    // create a vector of textures to store the six faces of the skybox
    std::vector<std::string> faces
    {
            "resources/textures/class/right.jpg", // right face
            "resources/textures/class/left.jpg", // left face
            "resources/textures/class/top.jpg", // top face
            "resources/textures/class/bottom.jpg", // bottom face
            "resources/textures/class/front.jpg", // front face
            "resources/textures/class/back.jpg", // back face
    };
    // load the faces of the skybox using the loadCubemap function
    unsigned int cubemapTexture = texture.loadCubemap(faces);

    // set the skybox shader settings and integers
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // set the scene shader settings and integers
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    // set the default to projection for the 3D scene perspective
    onPerspective = true;

    // check for proper sound engine creation
    if (!SoundEngine)
    {  // handle the case that the sound engine creation failed
        std::cout << "Error creating the sound engine." << std::endl;
        return 0;
    }
    // check for the mp3 file
    std::ifstream file(songName);
    if (file.is_open()) {  // if the file is open
        SoundEngine->play2D(songName.c_str(), true);  // play the song
        file.close();  // close the song when finished
    }
    else {  // handle the case that the file is not found and print an error to the console
        std::cout << "Error: " << songName << " not found." << std::endl;
    }

    // rendering loop
    while (!glfwWindowShouldClose(window))
    {
        // get the current frame using glfwGetTime function
        float currentFrame = static_cast<float>(glfwGetTime());
      
        // define deltaTime as the difference between the current and the last frame
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // process input throughout the loop
        processInput(window);

        // set an initial background color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // update the light position coordinates if the lampIsOrbiting boolean is true
        const float angularVelocity = glm::radians(45.0f);
        if (lampIsOrbiting)  // if the lampIsOrbiting is true
        {
            // update the point light positions by rotating them around the x and y axis throughout deltaTime
            
            glm::vec4 newPosition = glm::rotate(angularVelocity * deltaTime,
                glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(pointLightPositions[0], 1.0f);
            pointLightPositions[0].x = newPosition.x;
            pointLightPositions[0].y = newPosition.y;
            pointLightPositions[0].z = newPosition.z;

            glm::vec4 newPosition2 = glm::rotate(angularVelocity * deltaTime,
                glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(pointLightPositions[1], 1.0f);
            pointLightPositions[1].x = newPosition2.x;
            pointLightPositions[1].y = newPosition2.y;
            pointLightPositions[1].z = newPosition2.z;
        }

        /* LIGHTING SETTINGS FOR THE SCENE */
        // set the shader settings including the positions, properties, and colors in terms of red,
        // green, blue, and alpha for the lights
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
       
        // define the values red, green, blue and alpha for the ambient, diffuse and specular arrays
        float ambient[] = { 0.5f, 0.5f, 0.5f, 1 };
        float diffuse[] = { 0.8f, 0.8f, 0.8f, 1 };
        float specular[] = { 1.0f, 1.0f, 1.0f, 1 };

        // set the shininess
        float shininess = 128;

        // define the material properties for ambient, diffuse, specular, and shininess
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);

        // intialize the shader variables
        glm::mat4 projection, view, model;

        // set the projection to orthographic or perspective depending on the boolean value of onPerspective
        if (onPerspective)
            projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        if (!onPerspective)
            projection = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, 1.0f, 100.0f);

        // set the shader settings
        view = camera.GetViewMatrix();  // calculate the view matrix using the position of the camera
        lightingShader.use();  // use the shader
        lightingShader.setMat4("projection", projection);  // set the shader projection matrix
        lightingShader.setMat4("view", view);  // set the view matrix in the shader
        lightingShader.setMat4("model", model);  // set the model matrix in the shader

        /* RENDER SCENE */

        /* COMPASS */
        float angle = 0.0;
        Objects pyramid;
        // create a vao and vbo using the compass vertices
        pyramid.link(compassVertices.size() * sizeof(GLfloat), &compassVertices[0]);
        // initialize the identity matrix
        model = glm::mat4(1.0f);
        // rotate the model
        model = glm::rotate(model, glm::radians(240.0f), glm::vec3(0.01f, 0.01f, 0.01f));
        // move the model
        model = glm::translate(model, glm::vec3(-1.85f, -0.2f, -.536f + .005f)); // shift the object (forward, left, and down...) after rotation
        // scale the model
        model = glm::scale(model, glm::vec3(1.1f));
        model = glm::scale(model, glm::vec3(.75f, .75f, .25f));
        // set the compass as the model
        lightingShader.setMat4("model", model);
        // activate the texture
        glActiveTexture(GL_TEXTURE0);
        // bind the texture
        glBindTexture(GL_TEXTURE_2D, compassTexture);
        // draw the object with triangles
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // unbind the vao
        glBindVertexArray(0);

        /* CUTOUT BOX */
        glm::vec4 color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spriteColor", color);
        Objects box;
        box.link(boxVertices.size() * sizeof(GLfloat), &boxVertices[0]);
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
        model = glm::translate(model, glm::vec3(0.0f, 1.65f, -3.240f));
        model = glm::scale(model, glm::vec3(9.0f, 6.5f, 9.0f));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, skyboxTexture);
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /* all modified cube shapes are bound to one vao and are instanced to make the desk,
        books, ground, and blackboard in the for-loop */
        Objects book;
        book.link(vertices.size() * sizeof(GLfloat), &vertices[0]);
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
            else if (i == 1)
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
            else if (i == 2)
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
            else if (i == 3)
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
            else if (i == 4)
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
            else if (i == 5)
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
            else if (i == 6)
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
            else if (i == 7)
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
            else if (i == 8)
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
            else if (i == 9)
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
            else if (i == 10)
            {  /* BLACKBOARD */
                angle = 0.0f;
                model = glm::mat4(1.0f);
                model = glm::rotate(model, glm::radians(angle), glm::vec3(3.01f, 0.01f, -12.01f));
                model = glm::translate(model, glm::vec3(0.0f, 1.5f, -7.75f));
                model = glm::scale(model, glm::vec3(5.5f, 4.0f, .03f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                // define the texture as the rth index of the textures vector
                // textures are scrolled on the blackboard object using key press t
                glBindTexture(GL_TEXTURE_2D, textures[t]);  // access the r index of the textures vector
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (i == 11)
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
            else
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


        /* Note: In this scene, the plane is hidden, but it is there. I placed a modified a cube over the plane for the floor as it gives a better appearance
        due to the texture scale. We will keep the plane floor as an option for the future. */
        Objects plane;
        model = glm::mat4(.05f);
        // create a plane object using the vertices of a quad
        plane.link(planeVertices.size() * sizeof(GLfloat), &planeVertices[0]);
        angle = 0.0f;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.001f, 0.001f, 0.0001f));
        // keys 'UP' and 'DOWN' move the plane up and down
        model = glm::translate(model, glm::vec3(0.0f, -30.6998f - 0.6f + 1.2f, -3.25f - 61.4493f));
        model = glm::scale(model, glm::vec3(3.6));
        lightingShader.setMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, groundTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // bind metal texture and use it on the next three objects to be efficient with our resource use
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, metalTexture);

        // set model and translate the next three objects down 2 and back 3 at once to be efficient with our resource use
        model = glm::mat4(0.5f);
        model = glm::translate(model, glm::vec3(0.0f, -2.005f, -3.80f));

        // draw the pen body shape
        lightingShader.setMat4("model", model);
        PenBody penBody;
        penBody.Draw();

        // sphere accent
        // bind the accent texture and draw the accent
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, metalTexture);
        // draw the sphere accent shape
        lightingShader.setMat4("model", model);
        PenAccent sphereAccent;
        sphereAccent.Draw();

        // pen point
        // bind the ballpoint texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ballpointTexture);
        // set the model
        lightingShader.setMat4("model", model);
        model = glm::mat4(0.5f);
        // draw the pen point
        PenPoint penPoint;
        penPoint.Draw();

        // pen clip
        // draw the clip with a long, thin cube
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, metalTexture);
        // set the model
        Objects penclip;
        // create a pen clip object using the vertices of a cube
        penclip.link(vertices.size() * sizeof(GLfloat), &vertices[0]);
        model = glm::rotate(model, glm::radians(1.16f), glm::vec3(0.0f, 1.00f, 1.0f));
        model = glm::translate(model, glm::vec3(.27f + .0415f, -0.739999f + -.04f, -2.62001f)); //translate to the right, down, and back
        model = glm::scale(model, glm::vec3(.2f, .012f, .005f));
        lightingShader.setMat4("model", model);
        // draw with 36 triangles
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // unbind the vao after drawing
        glBindVertexArray(0);


        // create a testing target object to translate, scale, and scroll textures on
        Objects testBox;
        book.link(vertices.size() * sizeof(GLfloat), &vertices[0]);
        model = glm::mat4(1.0f);
        // define the objects translation in terms of x, y, and z which can be incremented and decremented with a key press
        // LEFT moves the object left RIGHT moves the object right
        // UP moves the object up, down moves the object down
        // Z moves the object forward on the Z, Y moves the object back on the Z
        model = glm::translate(model, glm::vec3(x, y, z));
        // define the object's size in terms of 1 + xx, yy, and zz.
        // Adding 1 to the variables must be done to be sure that the objects are rendered to the screen.
        // The ojbect's scale is incremented and decremented on the x axis with J & L
        // The ojbect's scale is incremented and decremented on the y axis with I & K
        model = glm::scale(model, glm::vec3(xx + 1.0f, yy + 1.0f, zz + 1.0f));
        lightingShader.setMat4("model", model);
        // define the texture as the rth index of the textures vector
        // textures are scrolled on the blackboard object using key press r
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[r]);  // access the r index of the textures vector
        glDrawArrays(GL_TRIANGLES, 0, 36);


        /* RENDER LIGHTS */
        // bind the light uniform buffer object and the light cube vao
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
                if (lampIsOrbiting)
                    /* ORBIT THE LIGHTS */
                    model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(speed) * direction * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                // place the lights into the light positions vector positions
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
                    model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(speed) * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                // place the lights into the light positions vector positions
                model = glm::translate(model, lightPositions[i]);
                model = glm::scale(model, glm::vec3(.25f));
                purpleShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        // unbind the vao after drawing
        glBindVertexArray(0);

        /* RENDER SKYBOX */
        // set the skybox depth
        glDepthFunc(GL_LEQUAL);
        Objects skybox;
        skybox.skybox(skyboxVertices.size() * sizeof(GLfloat), &skyboxVertices[0]);
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // bind the cube map texture to create the skybox
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        // draw skybox with 72 triangles
        glDrawArrays(GL_TRIANGLES, 0, 72);
        // unbind the vao
        glBindVertexArray(0);
        // return the depth to normal
        glDepthFunc(GL_LESS);

        // allow the user to increment the volume
        SoundEngine->setSoundVolume(.2f + volume);

        // convert x,y,z, xx, yy, and zz to string using stringstream
        // we will do this to render the value of the variable to the screen
        std::stringstream xs; xs << x;
        std::stringstream ys; ys << y;
        std::stringstream zs; zs << z;
        std::stringstream xxs; xxs << xx;
        std::stringstream yys; yys << yy;
        std::stringstream zzs; zzs << zz;
        std::stringstream ts; ts << t;
        std::stringstream rs; rs << r;

        // if the renderText boolean is true, call the render text function to render the 2d quad containing our desired string
        if (renderText) {
            // call the renderText function passing the shader, the string, the x and y coordinates,
            // the font size, and the font colors as parameters.
            // render the first line of text
            RenderText(textShader, "Volume up: 1, Volume Down 2, Text Off: Q", 
                xPos, yPos + 4 * spacing, fontSize, textColor);
           
            // render the first line of text    
            RenderText(textShader, "Move the object with LEFT, RIGHT, UP, DOWN, Y & Z.", 
                xPos, yPos + 3 * spacing, fontSize, textColor);
           
            // render the second line of text  
            RenderText(textShader, "Resize the x-axis with J & L Resize the y-axis: I & K", 
                xPos, yPos + 2 * spacing, fontSize, textColor);
           
            // render the third line of text that includes the string value of the x, y, and z coordinates for object position
            RenderText(textShader, "x Value:   " + xs.str() + "   y Value:   " + ys.str() + "   z Value:   " + zs.str(), 
                xPos, yPos + spacing, fontSize, textColor2);
           
            // render the last line of text that includes the string value of the xx, yy, and zz coordinates for object scale
            RenderText(textShader, "x Value:   " + xxs.str() + "   yy Value:   " + yys.str() + "   zz Value:   " + zzs.str(), 
                xPos, yPos, fontSize, textColor2);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // deallocate resources after the loop
    // delete the sound engine after the loop
    SoundEngine->drop();

    // delete the vao and vbo for the text
    glDeleteVertexArrays(1, &textVAO);
    glDeleteBuffers(1, &textVBO);

    // delete the vao and vbo for the light cube
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &lightCubeVBO);
    glDeleteBuffers(1, &uboMatrices);

    // destroy the textures
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

    // delete the shader programs
    glDeleteProgram(lightingShader.ID);
    glDeleteProgram(skyboxShader.ID);
    glDeleteProgram(purpleShader.ID);
    glDeleteProgram(pinkShader.ID);
    glDeleteProgram(textShader.ID);

    glfwTerminate();
    return 0;
}

/* PROCESS INPUT */
//process input from the keyboard
void processInput(GLFWwindow* window)
{
    // key 'esc' terminates the program
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Navigate the scene with W,A,S,D
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
   
    // 'Q' and 'E' allow up and down camera movement
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

    // switch between orthographic and perspective with keys 'O' and 'P'
    if ((glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS))
        onPerspective = false;
    if ((glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS))
        onPerspective = true;

    // Object translation controls:
    // customize object translation with key presses for development
    // key LEFT moves the object LEFT
    if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS))
        x -= .005f;
    // key RIGHT moves the object RIGHT
    if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS))
        x += .005f;
    // key UP moves the object UP
    if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS))
        y += .005f;
    // key DOWN moves the object DOWN
    if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS))
        y -= .005f;
    // key Y moves the back on the Z axis
    if ((glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS))
        z -= .005f;
    // key Z moves the object forward on the Z axis
    if ((glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS))
        z += .005f;

    // Object scale controls:
    // Key K scales the object larger on the x axis
    if ((glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS))
        xx += .005f;
    // Key L scales the object smaller on the x axis
    if ((glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS))
        xx -= .005f;
    // Key I scales the object larger on the y axis
    if ((glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS))
        yy += .005f;
    // Key K scales the object smaller on the y axis
    if ((glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS))
        yy -= .005f;
   
    // key 'G' prints out the camera movement speed and the x, y, z, xx, yy, and zz coordinates of the target object
    if ((glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS))
    {
        std::cout << "camera speed: " << camera.MovementSpeed << std::endl;
        std::cout << "xvalue: " << x << ", yValue: " << y << " zValue: " << z << std::endl;
    }
   
    // texture scrolling:
    // key t press increments the value of t, allowing up to scroll though the textures vector of textures as it is accessed on the blackboard
    if ((glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS))
    {
        if (t < textures.size() - 1)
            t += 1;
        else
            t = 0;
    }
    // key r press increments the value of r, allowing up to scroll though the textures vector of textures as it is accessed on the test box
    if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS))
    {
        if (r < textures.size() - 1)
            r += 1;
        else
            r = 0;
    }

    // Key Q toggles the rendering of text
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        renderText = !renderText;
    }

    // volume control:
    // key 1 decreases the volume
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        volume -= .01f;
    }
    // key 2 increases the volume
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        volume += .01f;
    }
}

// mouse button handling
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{   // create a switch case that handles the mouse button press
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:  // handle the left mouse button press
    {
        if (action == GLFW_PRESS)  // if left button is pressed, print the message to the console
            std::cout << "Left mouse button pressed" << std::endl;
        else  // if the button is released, print a message to the console
            std::cout << "Left mouse button released" << std::endl;
    }
    break;
    case GLFW_MOUSE_BUTTON_MIDDLE:  // handle the middle mouse button press
    {
        if (action == GLFW_PRESS)  // if middle mouse button is pressed, print the message to the console
            std::cout << "Middle mouse button pressed" << std::endl;
        else  // if the button is released, print a message to the console
            std::cout << "Middle mouse button released" << std::endl;
    }
    break;

    case GLFW_MOUSE_BUTTON_RIGHT:  // handle the right mouse button press
    {
        if (action == GLFW_PRESS)  // if right mouse button is pressed, print the message to the console
            std::cout << "Right mouse button pressed" << std::endl;
        else  // if the button is released, print a message to the console
            std::cout << "Right mouse button released" << std::endl;
    }
    break;
    default:  // if another button is pressed
        std::cout << "Unhandled mouse button event" << std::endl;
        break;
    }
}

// allow the window to be resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// allow the camera to look up and down or left and right with the mouse cursor
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);  // get the x-position
    float ypos = static_cast<float>(yposIn);  // get the y-position

    if (firstMouse)  // if it is the first mouse button press of the program
    {
        lastX = xpos;  // define the x-axis position
        lastY = ypos;  // define the y-axis position
        firstMouse = false;  // set the firstMouse boolean to false now
    }
    float xoffset = xpos - lastX;  // calculate the offset movement between the last frame and the current frame
    float yoffset = lastY - ypos; // y-coordinates are reversed
    lastX = xpos;   // define the new x-axis position
    lastY = ypos;  // define the new y-axis position
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// function to increase or decrease camera speed depending on the scroll direction
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
    // increase camera speed
    if (yoffset > 0)  // if the direction of the scroll is up, increase the camera speed
        camera.MovementSpeed += 1.0f;
 
    // put an upper limit on the camera movement speed to keep the camera controlled
    if (camera.MovementSpeed >= 5.0f)
        camera.MovementSpeed = 5.0f;
 
    // decrease camera speed
    else  // if the direction of the scroll is down, decrease the camera speed
        camera.MovementSpeed -= 1.0f;
 
    // put a lower limit on the camera movement speed to keep the camera controlled
    if (camera.MovementSpeed <= -5.0f)
        camera.MovementSpeed = -5.0f;
}

// create a function to initialize freetype, load the font, load the glyphs, and
// store the texture, size, bearing, and advance properties in the character struct.
// source: https://learnopengl.com/In-Practice/Text-Rendering
void InitText()
{
    FT_Library freeType;
   
    // check that free type library is loaded
    if (FT_Init_FreeType(&freeType))
        // print a message to the console it there is an issue loading the freetype library
        std::cout << "Error. Could not initialize the FreeType Library" << std::endl;

    FT_Face font;
    // check that font is found before proceeding
    if (FT_New_Face(freeType, "fonts/Antonio-Regular.ttf", 0, &font))
        // print an error message to the console if the font is not successfully loaded
        std::cout << "Error. Failed to load FreeType font" << std::endl;

    // load the data from the font
    FT_Set_Pixel_Sizes(font, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // iterate through the characters, storing the character data
    for (GLubyte i = 0; i < 128; i++)
    {
        // Load the character glyph
        if (FT_Load_Char(font, i, FT_LOAD_RENDER))
        {  // check for errors loading the glyphs
            std::cout << "Error. Failed to load FreeType Glyph" << std::endl;
            continue;
        }

        // Generate the texture
        GLuint fontTexture;
        glGenTextures(1, &fontTexture);
        glBindTexture(GL_TEXTURE_2D, fontTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, font->glyph->bitmap.width, font->glyph->bitmap.rows,
            0, GL_RED, GL_UNSIGNED_BYTE, font->glyph->bitmap.buffer);

        // Set texture wrapping on edges
        // set the texture wrapping to clamp to edges, disabling repeated textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // set the minification filter to allow better visuals when scaling down
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // set the magnification filter to allow better visuals when scaling up
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // store the character for use
        Character character = {
                fontTexture,
                glm::ivec2(font->glyph->bitmap.width, font->glyph->bitmap.rows),
                glm::ivec2(font->glyph->bitmap_left, font->glyph->bitmap_top),
                static_cast<unsigned int>(font->glyph->advance.x)
        };
        // insert the characters into the character map
        Characters.insert(std::pair<GLchar, Character>(i, character));
    }
    // unbind the texture when finished
    glBindTexture(GL_TEXTURE_2D, 0);

    // deallocate resources when finished
    FT_Done_Face(font); // deallocate the font
    FT_Done_FreeType(freeType); // shut down the freetype library
}


// create a function to get the data from the appropriate character stuct of the characters map and
//  update the data based on the projected size of the quad and render a string to the 2d quad
// source: https://learnopengl.com/In-Practice/Text-Rendering
void RenderText(Shader& shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // activate the shader
    shader.use();
    // define the shader uniforms
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    // bind the texture
    glActiveTexture(GL_TEXTURE0);
    // bind the VAO
    glBindVertexArray(textVAO);

    // Iterate through all characters
    std::string::const_iterator i;
    for (i = text.begin(); i != text.end(); i++)
    {
        Character character = Characters[*i];
        // calculate the x and y coordinates of the characters as a product of the scale
        GLfloat xPos = x + character.Bearing.x * scale;
        GLfloat yPos = y - (character.Size.y - character.Bearing.y) * scale;
        // scale the characters
        GLfloat width = character.Size.x * scale;
        GLfloat height = character.Size.y * scale;

        // Update the VBO for each character
        GLfloat vertices[6][4] = {
                { xPos, yPos + height, 0.0, 0.0 }, 
                { xPos, yPos, 0.0, 1.0 },
                { xPos + width, yPos, 1.0, 1.0 }, 
                { xPos, yPos + height, 0.0f, 0.0f },
                { xPos + width, yPos, 1.0, 1.0 }, 
                { xPos + width, yPos + height, 1.0, 0.0 } 
        };

        // Render glyph over the quad
        // First, bind the texture
        glBindTexture(GL_TEXTURE_2D, character.TextureID);
        // bind buffer to the vertices and create a buffer the size of the vertices
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render the quad with gl triangles
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (character.Advance >> 6) * scale;
    }
    // unbind the vao and texture when finished rendering
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


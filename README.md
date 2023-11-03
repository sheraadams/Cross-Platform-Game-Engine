# OpenGL Sims Mode
## About the project

This is an example of drawing multiple shapes in openGL. Controls are added to allow visual customization for development so that objects can be translated and scaled with a key input. Textures can be switched with a key press through the use of a textures vector. 

Up, down, left, right moves an object that is defined in terms of (xValue, yValue, and zValue).
T and R change the textures vector index with a key press to allow visual texture scrolling. 

<div style="text-align: center;">
  <p><strong>See Also: <a href="https://github.com/sheraadams/Fractal-Petals-Animation" target="_blank">Fractal Petals Animation</a> and <a href="https://github.com/sheraadams/Space-Shooter-Game" target="_blank">OpenGL Space Shooter Game</a>.</strong></p>
</div>

![Screenshot 2023-01-31 182547](https://user-images.githubusercontent.com/110789514/216044820-3bd3033f-c45e-4bf2-8efe-f7e835b7b861.png)

## Getting Started

To allow movement for our objects, we can define the following variables at the start of our code: 
```cpp

int t = 0;  // create a textures vector index to scroll through textures

int r = 0; // create a secondary textures vector index to scroll through textures on a separate object
 
vector<unsigned int> textures; // create a textures vector to scroll through textures
        
float xValue = -.15004f; // set a default x-axis position to start
        
float yValue = -0.17f; // set a default y-axis position
        
float zValue = -0.2f; // set a default z-axis position
        
float xx = 0; // set // set a default x-axis value for scaling 
        
float yy = 0; // set a default y-axis value for scaling 
        
float zz = 0; // set a default z-axis value for scaling 
```

## Texture Scrolling
After we create our texture variables as: 
```cpp
    unsigned int ufoTexture = loadTexture("resources/textures/burst.png");
    unsigned int pyrTexture = loadTexture("resources/textures/myheiro.png");
    unsigned int skyboxTexture = loadTexture("resources/textures/AdobeStock_481965458.jpeg");
    unsigned int cubeTexture = loadTexture("resources/textures/box.png");
    unsigned int planeTexture = loadTexture("resources/textures/AdobeStock_336144820.png");
```

We can add each texture to the textures vector so that we can scroll through our available textures by pressing the keys 'r' or 't'. 

```cpp
    textures.push_back(ufoTexture);
    textures.push_back(pyrTexture);
    textures.push_back(cubeTexture);
    textures.push_back(planeTexture);
    textures.push_back(skyboxTexture);
```

Finally, to allow us to scroll through all texture resources with a key press, we use the following code to assign the texutre of our object to the indexed texture at position 't' in the textures vector:

```cpp
glBindTexture(GL_TEXTURE_2D, textures[t]);
```

We can also we use the following code to assign the texutre of a new object to the indexed texture at position 'r' in the textures vector. This allows us to scroll through textures on two objects, one with key 't', the other with key 'r'.
```cpp
glBindTexture(GL_TEXTURE_2D, textures[r]);

```

We then add the following controls to the process input: 
```cpp
void processInput(GLFWwindow* window)
{ 
//  the usual inputs here.. 
    if ((glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS))
    {
        if (t < textures.size() - 1)
        {
            t += 1;
        }
        else
            t = 0;

    }
    if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS))
    {
        if (r < textures.size() - 1)
        {
            r += 1;
        }
        else
            r = 0;

    }
}
```

## Object Translation Control
When we define the object's translation, we define it in terms of x, y, and z. When we press our keys left, right, up, down, y, and z, we increment the position. 
```cpp
model = glm::translate(model, glm::vec3(x, y, z));
```
The function that is responsible for this translation is the void processInput(GLFWwindow* window). WE will also add a 'g' key press to get our current coordinates printed to the console should we need exact coordinates.  
```cpp
void processInput(GLFWwindow* window)
{// .... the usual inputs here.. 
    if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS))
        x -= .05;
    if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS))
        x += .05;
    if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS))
        y += .05;
    if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS))
        y -= .05;
    if ((glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS))
        z -= .05;
    if ((glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS))
        z += .05;
    if ((glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS))
    {
        std::cout << "xvalue: " << x << ", yValue: " << y << " zValue: " << z << std::endl;
        std::cout << "camera speed: " << cameraSpeed << std::endl;
    }
}
```
## Size Control
Size can also be incremeneted by assigning a key press to increment xx, yy, and zz. 
```cpp
model = glm::scale(model, glm::vec3(xx + 1, yy + 1, zz + 1));
```
** note you should keep the x, y, and z values not equal to zero to render them as size 0 cannot exist. 

## Sumary of the Key Controls

**Up**: move object up .05f

**Down**: move object down .05f

**Left**: move object left .05f

**Right**: move object right .05f

**Y**: move object back .05f

**Z**: move object forward .05f

**G**: print x, y, and z location of objects to the console window. 

**T**: switch forward to the next texture object group 1

**R**: switch forward to the next texture object group 2

## References

DeVries, J. (n.d.). LearnOpenGL/LICENSE.md at master · JoeyDeVries/LearnOpenGL. GitHub. https://github.com/JoeyDeVries/LearnOpenGL/blob/master/LICENSE.md

Learn OpenGL, extensive tutorial resource for learning Modern OpenGL. (n.d.-d). https://learnopengl.com/


<div style="text-align: center;">
  <p><strong>Proudly crafted with ❤️ by <a href="https://github.com/sheraadams" target="_blank">Shera Adams</a>.</strong></p>
</div>


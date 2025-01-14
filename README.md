
# Metal/ OpenGL Engine

I started this project with the intention to visually recreate this two-dimensional classroom scene in a more realistic manner using three-dimensional graphics in an interactive environment to allow for a more surreal and interesting user experience. To accomplish this, I used shapes including spheres, planes, cubes, pyramids, rectangular prisms, and modified versions of these shapes. You can read about some of the process that I went through developing and improving this engine at [sheraadams.github.io](https://sheraadams.github.io) and watch my video code review linked on that page. 

The final engine with the Dear ImGui over lay is also [on YouTube](https://youtu.be/82KOBQVYH-I).

<div align="center">
  <p>Before:</p>
</div>
    
<div align="center">
  <img src="https://github.com/sheraadams/C-OpenGL-Metal-Work/assets/110789514/7c35eb5e-f9c8-4c99-b3d3-163540df7435" width="800" alt="Wireframe">
</div>

<div align="center">
  <p>After:</p>
</div>
    
<div align="center">
  <img src="https://github.com/sheraadams/C-OpenGL-Metal-Work/assets/110789514/5334d84c-96dc-48b0-9d5e-3c3d7ea70bd9" width="800" alt="Updated GUI">
</div>

<div align="center">
  <p>With Imgui:</p>
</div>
    
<div align="center">
  <img src="https://github.com/sheraadams/C-OpenGL-Metal-Work/assets/110789514/98ed259c-cd4b-4376-8f8f-df1b1a77d3b4" width="800" alt="Updated GUI">
</div>

## The OpenGL Pipeline 
<div align="center">
  <img src="https://github.com/sheraadams/C-OpenGL-Metal-Work/assets/110789514/88a8047d-4105-41e8-8ec3-c06d9e55139f"
 width="900" alt="Screenshot: d4">
</div>

- Initialization
  - Include libraries
  - Define variables for the program
  - Initialize GFLW and GLAD and Dear Imgui libraries
  - Create an OpenGL window and define the window height and width
  - Define the vertices vectors
  - Create and initialize the shaders
  - Create VAO (Vertex Array Object) that stores vertex attributes such as position, color, and texture coordinates for rendering
  - Create VBO (Vertex Buffer Object) that is a buffer that reserves memory to store vertex data
- Rendering Loop: the rendering process is as follows: 
  - Clear the screen to set a background color.
  - Handle user inputs and manage events.
  - Render objects:
    - Set model, view, and projection matrices for the camera.
    - Define shader uniforms for lighting, materials, and other properties.
    - Bind the Vertex Array Object (VAO) and specify the vertex data layout.
    - Use the shader program for rendering.
    - Apply transformations to the model or objects you're rendering.
    - Draw with glDrawArrays() function.
    - Repeat the process for each object.
    
## Defining the vertices
```cpp
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,   // this is a modified cube z axis is my x axis and it is modified to be less tall
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

```
## Creating the VAO and VBO
```cpp
void Objects::link(GLsizeiptr size, GLfloat* vertices)
{
    // generate vao
    glGenVertexArrays(1, &VAO);
    // generate vao
    glGenBuffers(1, &VBO);
    // bind buffer to the vertices and create a buffer the size of the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    // bind vbo attribute pointers to the vao
    // our vertices have 3 floats per position, 3 floats per normal, and 2 per tex coord = 8 floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(VAO);
}

```

## Rendering the objects
```cpp
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
          // ... continue add more objects
        }
        // unbind the vao after drawing all objects
        glBindVertexArray(0);
```

I implemented a Dear ImGui interface for future project development within OpenGL. Dear ImGui offers many tools to configure and manipulate our environment for efficient workflows with OpenGL and other C++ applications and it provides backend access with front end implementations. To implement ImGui, we must import the library headers and set our configuration. We must initialize ImGui in the main function. We create a window and set the layout in the render loop.  

We can implement a simple ImGui window within the rendering loop with the following code: 
```cpp
        // Create an ImGui window
        ImGui::SetNextWindowSize(ImVec2(350, 530)); 
        ImGui::SetNextWindowPos(ImVec2(1560, 470)); 
        ImGui::Begin("Basic Design Controls");
```
We position and scale objects by accessing x, y, z, xx, yy, and zz variables using the following code: 
```cpp
        //...
        
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
```
We can also pick between configurations like orthographic and perspective mode as shown:
```cpp

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
```
With ImGui, we can now resize and manipulate our objects with sliders and see the results in real-time. As we slide the x slider to the right, the object moves to the right. As we slide it to the left, the object moves to the left. As we slide the y slider to the right, the object moves up. When we slide it to the left, the object moves down.

When we slide the xx slider to the right, the object becomes bigger on the horizontal axis. When we slide it to the left, it becomes smaller on the horizontal axis. When we slide the yy slider to the right, the object becomes bigger on the vertical axis. When we slide it to the left, the object becomes smaller on the vertical axis. 

<div align="center">
  <img src="https://github.com/sheraadams/C-OpenGL-Metal-Work/assets/110789514/0763faef-cd9d-421b-866a-49669b68eb7d" width="300"  alt="Screenshot: d4">
</div>


# Data Structures and Algorithms with OpenGL Vectors

When considering the best data structure for texture scrolling for this application, I comared the vector, hash table, and binary search tree data structures in terms of access alone and for our Open GL program, insertion, deletion, and sorting will not likely be needed. In terms of access, the vector and array structures perform very well and have the average run-time space complexity of O(1). Hash table and binary search trees have higher overhead costs and require storage space for the structure itself. We also know that the binary search tree is great for cases when data sets are likely to grow, and fast searches are important and hash tables are best for small data sets in which fast searches are a priority. Since we are primarily concerned with access and will not need to search the textures, the vector is a strong choice. 

For access, vectors have a worst-case runtime space complexity of O(N) where N is the number of elements in the vector and as we can see here, O(n) is generally fair to generally good space complexity and it is linear. Vectors offer advantages in terms of pure runtime and space complexity while the downside of vectors is that they are expensive in terms of memory allocation. To minimize our overall application overhead, we could incorporate the binary search trees for operations like collision detection.


<div align="center">
  <img src="https://github.com/sheraadams/C-OpenGL-Metal-Work/assets/110789514/7c97d37c-0c88-4db7-8f02-59c5115935c7" width="800" alt="Data Structures and Algorithms: d4">
</div>

Image Credit: (Eric Drowell, n.d.) 

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

Finally, to allow us to scroll through all texture resources with a key press, we use the following code to assign the texture of our object to the indexed texture at position 't' in the textures vector:

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
See that our textures are loaded as unsigned integers and are pushed back into a vector for storage and access all at once.  
```cpp
    /* LOAD TEXTURES */
    Textures texture;
    unsigned int redTexture = texture.loadTexture("resources/textures/class/red.png");
    unsigned int blueTexture = texture.loadTexture("resources/textures/class/blue.png");
    unsigned int lightgreyTexture = texture.loadTexture("resources/textures/class/lightgrey.png");
    unsigned int compassTexture = texture.loadTexture("resources/textures/class/protractor2.png");
    unsigned int skyboxTexture = texture.loadTexture("resources/textures/class/wall.png");
    unsigned int blackboardTexture = texture.loadTexture("resources/textures/class/blackboard.png");
    unsigned int groundTexture = texture.loadTexture("resources/textures/class/AdobeStock_321846439.png");
    unsigned int metalTexture = texture.loadTexture("resources/textures/class/metal.png");
    unsigned int deskTexture = texture.loadTexture("resources/textures/class/AdobeStock_372442505.png");
    unsigned int ballpointTexture = texture.loadTexture("resources/textures/class/1.png");
    unsigned int certificateTexture = texture.loadTexture("resources/textures/class/cert.png");
    unsigned int posterTexture = texture.loadTexture("resources/textures/class/poster.png");

    /* simsmode addition */
    textures.push_back(blueTexture);
    textures.push_back(redTexture);
    textures.push_back(lightgreyTexture);
    textures.push_back(groundTexture);
    textures.push_back(metalTexture);
```

We access our textures vector through an index (r or t). We can scroll through these textures with a key press (if desired, see the input function in this [repo](https://github.com/sheraadams/OpenGL-Sims-Mode), or through a slider with ImGui. 
```cpp
        Objects book1;
        book1.link(sizeof(vertices), vertices);
        model = glm::translate(model, glm::vec3(x, y, z));
        model = glm::scale(model, glm::vec3(xx + 1, yy + 1, zz + 1));
        lightingShader.setMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[r]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
```
We can scroll through these textures with a slider using ImGui as below:
```cpp
        /* texture slider 1*/
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
```
Vectors are, many times, the data structure of choice for OpenGL. Vectors easily pass information back and forth from the shader and to functions. They can store information about vertices, coordinates, and textures. They also allow for easy linear transformations. 


## XCode Usage

Open a terminal in the project directory and run the code to remove the old build folder and generate a new one with the Xcode project file.

```bash
mkdir build
cd build
cmake -G Xcode ..

#Usage: when updating the build folder, remove it first then regenerate the folder as above
rm -rf build 
```
Set the working directory in Xcode using Product > Scheme > Edit Scheme > Run Debug > Options > Working Directory > (Check Set Custom) > Project directory/build.

Build and run the project.

## License:

The code is based on various tutorial code from LearnOpenGL.com.

Tutorial code is © Joey DeVries and licensed under the CC BY 4.0 License.

Modifications and additional contributions are © Shera Adams and licensed under the Apache 2.0 License.

Proudly crafted with ❤️ by Shera Adams.
   
<div align="center">
  <p><strong>Proudly crafted with ❤️ by Shera Adams.</strong></p>
</div>

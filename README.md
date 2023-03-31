## About

Example of drawing multiple shapes in openGL. Controls are added to allow visual customization for development. 

Up, down, left, right moves an object that is defined in terms of (xValue, yValue, and zValue).
T and R change the textures vector index with a key press to allow visual texture scrolling. 

## Texture Scrolling
for texture scrolling add: 

glBindTexture(GL_TEXTURE_2D, textures[t]);

and 

glBindTexture(GL_TEXTURE_2D, textures[r]);

## Object Translation Control

model = glm::translate(model, glm::vec3(x, y, z));

## Size Control
        
model = glm::scale(model, glm::vec3(xx + 1, yy + 1, zz + 1));

** note you need to keep the x, y, and z values not equal to zero to render them.

## Key Controls

Up: move object up .05f

Down: move object down .05f

Left: move object left .05f

Right: move object right .05f

Y: move object back .05f

Z: move object forward .05f

G: print x, y, and z location of objects to the console window. 

T: switch forward to the next texture object group 1

R: switch forward to the next texture object group 2

## References

DeVries, J. (n.d.). LearnOpenGL/LICENSE.md at master · JoeyDeVries/LearnOpenGL. GitHub. https://github.com/JoeyDeVries/LearnOpenGL/blob/master/LICENSE.md

Learn OpenGL, extensive tutorial resource for learning Modern OpenGL. (n.d.-d). https://learnopengl.com/

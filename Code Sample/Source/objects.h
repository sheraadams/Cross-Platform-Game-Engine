// Shera Adams
// shera.adams@snhu.edu
// January 23,2024
//
// Version: 2.0, This is an enhanced version of my CS330 final project created for Southern New Hampshire University to 
// satisfy the software engineering category enhancement of CS499, Capstone class.
//
// The main purpose of objects.cpp is to provide a class for linking VAO and VBO objects for the objects in our scene.
// The class has constructor and deconstructor functions for initializing and destroying the objects when we are finished with them.
// The class has functions like link5() and link() that generate VAO and VBOs and store this information for later use
// in the program. link5() takes 5 floats per vertex while link() takes 8 floats per vertex.
// The class also has a skybox function which is similar to the link functions and takes only 3 floats per vertex.
// The bind() functions draw the objects using gl triangles. 
// 
// Issues: none

#pragma once
#include <glad/glad.h>
#include "camera.h"
class Objects
{
public:

    GLuint VAO, VBO;
    Objects();
    ~Objects();
    void link(GLsizeiptr size, GLfloat* vertices);
    void link5(GLsizeiptr size, GLfloat* vertices);
    void skybox(GLsizeiptr size, GLfloat* vertices);
    void bind();
    void bindSkybox();

};
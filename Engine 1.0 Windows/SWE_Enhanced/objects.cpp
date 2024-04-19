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

#include "objects.h"

Objects::Objects()
{
}
// function to link for objects with 3 normal,  2 tex coords
void Objects::link5(GLsizeiptr size, GLfloat* vertices)
{
    // generate vao
    glGenVertexArrays(1, &VAO);
    // generate vbo
    glGenBuffers(1, &VBO);
    // bind buffer to the vertices and create a buffer the size of the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    // bind vbo attribute pointers to the vao
    // our vertices have 3 floats per position,  and 2 per tex coord = 5 floats
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

}
// function to link for objects with 3 normal, 3 position, 2 tex coords
void Objects::link(GLsizeiptr size, GLfloat* vertices)
{
    // generate vao
    glGenVertexArrays(1, &VAO);
    // generate vbo
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
}
// function to link for skybox or objects that use only 3 positions
void Objects::skybox(GLsizeiptr size, GLfloat* vertices)
{
    // generate vao
    glGenVertexArrays(1, &VAO);
    // generate vao
    glGenBuffers(1, &VBO);
    // bind buffer to the vertices and create a buffer the size of the vertices
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    // bind vbo attribute pointers to the vao
    // our vertices have 3 floats per position, 3 floats per normal, and 2 per tex coord = 8 floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(VAO);
}
// function to draw
void Objects::bind()
{
    // draw the object with gl triangles
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // unbind the vao
    glBindVertexArray(0);
}
// function to draw skybox
void Objects::bindSkybox()
{
    // draw the object with gl triangles
    glDrawArrays(GL_TRIANGLES, 0, 72);
    // unbind the vao
    glBindVertexArray(0);
}
// deconstructor clear vao and vbos
Objects::~Objects()
{
    // delete the vao and vbo
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
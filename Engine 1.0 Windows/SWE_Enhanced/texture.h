// Shera Adams
// shera.adams@snhu.edu
// January 23,2024
//
// Version: 2.0, This is an enhanced version of my CS330 final project created for Southern New Hampshire University to 
// satisfy the software engineering category enhancement of CS499, Capstone class.
//
// The main purpose of texture.cpp is to provide a class for all of our program's texture-related functions including
// loadTexture(), flipImageHorizontally(), flipImageVertically(), createRepeatTexture(), loadCubemap(), and destroyTexture().
// The loadCubemap() function is responsible for loading the six faces of the skybox from a vector of textures. 
// 
// Issues: none

#pragma once
#include <glad/glad.h>
#include<vector>
#include<string>
#include <iostream> 
#include <cstdlib>   

class Textures
{
public:
    Textures();
    bool createRepeatTexture(const char* filename, GLuint& textureId);
    unsigned int loadTexture(const char* path);
    void destroyTexture(GLuint textureId);
    unsigned int loadCubemap(std::vector<std::string> faces);
    void flipImageVertically(unsigned char* image, int width, int height, int channels);
    void flipImageHorizontally(unsigned char* image, int width, int height, int channels);
};
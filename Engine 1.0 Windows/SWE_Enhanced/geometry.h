// Shera Adams
// shera.adams@snhu.edu
// January 23,2024
//
// Version: 2.0, This is an enhanced version of my CS330 final project created for Southern New Hampshire University to 
// satisfy the software engineering category enhancement of CS499, Capstone class.
//
// The main purpose of geometry.cpp is to provide a class for defining the vertices for the various shapes that will 
// be used to create objects in our scene. This class will provide functions including GetBoxVertices(), 
// GetPlaneVertices(), GetPyramidVertices(), GetCompassVertices(), GetCubeVertices(), and GetSkyboxVertices(), each
// of which return a vector of floats containing the vertices data of the specified shape. 
//
// Issues: none

#pragma once
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glad/glad.h>
#include<vector>
#include<string>
#include <iostream> 
#include <cstdlib>    
#include "stb_image.h"
#include"texture.h"


class Geometry
{
public:
    std::vector<GLfloat> GetBoxVertices();
    std::vector<GLfloat> GetSkyboxVertices();
    std::vector<GLfloat> GetCubeVertices();
    std::vector<GLfloat> GetPyramidVertices();
    std::vector<GLfloat> GetCompassVertices();
    std::vector<GLfloat> GetPlaneVertices();
};


#endif
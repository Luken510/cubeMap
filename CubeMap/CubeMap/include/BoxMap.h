#ifndef BOXMAP_H
#define BOXMAP_H

#include "gl_core_4_3.hpp"
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include "Bitmap.h"
#include "QuatCamera.h"


class BoxMap
{
private:
	GLuint BoxTexture; // to set up the textures

	GLuint b_vbo; // to bind textures
	 
	GLuint b_vao; // used to render the cube

	GLuint iNumOfIndicies;

	void createBoxMap(const char* front, const char* back, const char* left, const char* right, const char* up, const char* down); // creates the box and its sides

	void createBoxMapSide(GLenum target, const char* file); // creates one side of the Box

	void init(); // sets up 

public:
	BoxMap();

	BoxMap(const char* front, const char* back, const char* left, const char* right, const char* up, const char* down); // 2 types of inits

	void render(); // renders the BoxMap

	GLuint getBoxTexture(); // returns the texture of the Box


};

#endif
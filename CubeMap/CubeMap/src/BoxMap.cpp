#include "BoxMap.h"

BoxMap::BoxMap() {};

BoxMap::BoxMap(const char* front, const char* back, const char* left, const char* right, const char* up, const char* down)
{
	init();
	createBoxMap(front, back, left, right, up, down);
}

void BoxMap::init()
{
	float points[] = {
		
		50.0f, -50.0f, -50.0f, // front
		-50.0f,  50.0f, -50.0f,
		50.0f,  50.0f, -50.0f,
		-50.0f,  50.0f, -50.0f,
		-50.0f, -50.0, -50.0f,
		50.0f, -50.0f, -50.0f,


		-50.0f, -50.0f,  50.0f,
		-50.0f, -50.0f, -50.0f, // l
		-50.0f,  50.0f, -50.0f,

		-50.0f,  50.0f, -50.0f,
		-50.0f,  50.0f,  50.0f,
		-50.0f, -50.0f,  50.0f,

		50.0f, -50.0f, -50.0f,
		50.0f, -50.0f,  50.0f,
		50.0f,  50.0f,  50.0f,// r
		50.0f,  50.0f,  50.0f,
		50.0f,  50.0f, -50.0f,
		50.0f, -50.0f, -50.0f,

		-50.0f, -50.0f,  50.0f,
		-50.0f,  50.0f,  50.0f, // b
		50.0f,  50.0f,  50.0f,
		50.0f,  50.0f,  50.0f,
		50.0f, -50.0f,  50.0f,
		-50.0f, -50.0f,  50.0f,

		50.0f,  50.0f, -50.0f,
		-50.0f,  50.0f, -50.0f,
		-50.0f,  50.0f,  50.0f,
		-50.0f,  50.0f,  50.0f,
		50.0f,  50.0f,  50.0f, // u
		50.0f,  50.0f, -50.0f,

		-50.0f, -50.0f, -50.0f,
		-50.0f, -50.0f,  50.0f,
		50.0f, -50.0f, -50.0f,
		50.0f, -50.0f, -50.0f, // d
		-50.0f, -50.0f,  50.0f,
		50.0f, -50.0f,  50.0f
		
	};


	gl::GenBuffers(1, &b_vbo);
	gl::BindBuffer(gl::ARRAY_BUFFER, b_vbo);
	gl::BufferData(gl::ARRAY_BUFFER, 6 * 18 * sizeof(float), &points, gl::STATIC_DRAW);

	gl::GenVertexArrays(1, &b_vao);
	gl::BindVertexArray(b_vao);
	gl::EnableVertexAttribArray(0);
	gl::BindBuffer(gl::ARRAY_BUFFER, b_vbo);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, FALSE, 0, NULL);

}

GLuint BoxMap::getBoxTexture()
{
	return BoxTexture;
}

void BoxMap::createBoxMap(const char* front, const char* back, const char* left, const char* right, const char* up, const char* down)
{

	// create textures to hold the sides
	gl::GenTextures(1, &BoxTexture);
	gl::BindTexture(gl::TEXTURE_CUBE_MAP, BoxTexture); // bind the texture to boxTexture

	createBoxMapSide(gl::TEXTURE_CUBE_MAP_POSITIVE_X, right); // had to switch to make textures work?
	createBoxMapSide(gl::TEXTURE_CUBE_MAP_NEGATIVE_X, left);
	createBoxMapSide(gl::TEXTURE_CUBE_MAP_POSITIVE_Y, up);
	createBoxMapSide(gl::TEXTURE_CUBE_MAP_NEGATIVE_Y, down);
	createBoxMapSide(gl::TEXTURE_CUBE_MAP_POSITIVE_Z, front);
	createBoxMapSide(gl::TEXTURE_CUBE_MAP_NEGATIVE_Z, back);

	//set the wrapping and filter paramters
	gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
	gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_MIN_FILTER, gl::LINEAR);

	gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_R, gl::CLAMP_TO_EDGE);
	gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
	gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE); // this wraps the box by clamping

	gl::BindTexture(gl::TEXTURE_CUBE_MAP, 0);// binds the texture to '0'

}
void BoxMap::render()
{
	gl::Disable(gl::DEPTH_TEST);
	gl::DepthMask(FALSE);
	gl::ActiveTexture(0);
	gl::BindTexture(gl::TEXTURE_CUBE_MAP, BoxTexture);
	gl::BindVertexArray(b_vao);
	gl::DrawArrays(gl::TRIANGLES, 0, 36);
	gl::BindTexture(gl::TEXTURE_CUBE_MAP, 0);
	gl::DepthMask(TRUE);
	gl::Enable(gl::DEPTH_TEST);

}


void BoxMap::createBoxMapSide(GLenum target, const char* file)
{

	gl::BindTexture(gl::TEXTURE_CUBE_MAP, BoxTexture); // binds the texture to our texture gluint

	Bitmap b_bmp = Bitmap::bitmapFromFile(file); // get the file


	gl::TexImage2D(target, 0, gl::RGB, (GLsizei)b_bmp.width(), (GLsizei)b_bmp.height(), 0, gl::RGB, gl::UNSIGNED_BYTE, b_bmp.pixelBuffer());

}
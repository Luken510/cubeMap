
#include "CubeMapScene.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::cerr;
using std::endl;

#include "defines.h"

using glm::vec3;


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>


namespace imat3111
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	// Default constructor
	/////////////////////////////////////////////////////////////////////////////////////////////
	CubeMapScene::CubeMapScene()
	{
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	//Initialise the scene
	/////////////////////////////////////////////////////////////////////////////////////////////
	void CubeMapScene::initScene(QuatCamera camera)
	{
		//|Compile and link the shader  
		compileAndLinkShader();

		//test subroutines
		// reflect - reflect
		// refract - refract
		// CubeMap - BoxMap

		Reflect = gl::GetSubroutineIndex(prog.getHandle(), gl::FRAGMENT_SHADER, "Reflect");
		Refract = gl::GetSubroutineIndex(prog.getHandle(), gl::FRAGMENT_SHADER, "Refract");
		CubeMap = gl::GetSubroutineIndex(prog.getHandle(), gl::FRAGMENT_SHADER, "BoxMap");


		gl::Enable(gl::DEPTH_TEST);

		//Set up the lighting
		setLightParams(camera);


		//Create the plane to represent the ground
		plane = new VBOPlane(100.0, 100.0, 100, 100);

		//create the boxmap

		b_pBoxMap = new BoxMap("images/TowerHousepano_f.jpg", "images/TowerHousepano_b.jpg", "images/TowerHousepano_l.jpg", "images/TowerHousepano_r.jpg", "images/TowerHousepano_u.jpg", "images/TowerHousepano_d.jpg");

		//A matrix to move the teapot lid upwards
		glm::mat4 lid = glm::mat4(1.0);
		//lid *= glm::translate(vec3(0.0,1.0,0.0));

		//Create the teapot with translated lid
		teapot = new VBOTeapot(16, lid);


	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	//Update not used at present
	/////////////////////////////////////////////////////////////////////////////////////////////
	void CubeMapScene::update(float t)
	{

	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Set up the lighting variables in the shader
	/////////////////////////////////////////////////////////////////////////////////////////////
	void CubeMapScene::setLightParams(QuatCamera camera)
	{

		vec3 worldLight = vec3(10.0f, 10.0f, 10.0f);

		prog.setUniform("Ld", 1.0f, 1.0f, 1.0f); // diffuse light intentsity
		prog.setUniform("La", 0.5f, 0.5f, 0.5f); // ambient light intentsity
		prog.setUniform("n", 30.f); // specular exponent
		prog.setUniform("Al", 0.0001f); // light attenuation
	  //  prog.setUniform("LightPosition", camera.view() * vec4(worldLight,1.0) );
		prog.setUniform("LightPosition", worldLight);
		prog.setUniform(("CameraPos"), camera.position());
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Render the scene
	/////////////////////////////////////////////////////////////////////////////////////////////
	void CubeMapScene::render(QuatCamera camera)
	{
		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

		//dealing with Box first
		//Initialise the model matrix for the Bo

		//Render the Box with fragment subroutine
		gl::UniformSubroutinesuiv(gl::FRAGMENT_SHADER, 1, &CubeMap);
		b_pBoxMap->render();

		// deal with the plane to represent the ground

		//Initialise the model matrix for the plane
		//model = mat4(1.0f);
		//Set the matrices for the plane although it is only the model matrix that changes so could be made more efficient
		//setMatrices(camera);
		//Set the plane's material properties in the shader and render
		//prog.setUniform("Kd", 0.7f, 1.0f, 0.7f);
		//plane->render();

		gl::ActiveTexture(0);
		gl::BindTexture(gl::TEXTURE_CUBE_MAP, b_pBoxMap->getBoxTexture());

		//Now set up the teapot 
		model = mat4(1.0f);
		setMatrices(camera);
		//Set the Teapot material properties in the shader and render
		prog.setUniform("Kd", 0.9f, 0.5f, 0.3f); // diffuse light
		prog.setUniform("Ka", 0.2f, 0.2f, 0.2f); // ambient light
		prog.setUniform("Ks", 0.9f, 0.5f, 0.3f); // specular light

		if (m_Toggle)
		{
			gl::UniformSubroutinesuiv(gl::FRAGMENT_SHADER, 1, &Reflect);
			std::cout << "hello";
		}
		else
		{
			gl::UniformSubroutinesuiv(gl::FRAGMENT_SHADER, 1, &Refract);
		}
		teapot->render();
	}


	/////////////////////////////////////////////////////////////////////////////////////////////
	//Send the MVP matrices to the GPU
	/////////////////////////////////////////////////////////////////////////////////////////////
	void CubeMapScene::setMatrices(QuatCamera camera)
	{
		mat4 mv = camera.view() * model;
		
			//Negate translate of camera view for skybox
			mat4 TempView = camera.view();
			TempView[3][0] = 0.0f;
			TempView[3][1] = 0.0f;
			TempView[3][2] = 0.0f;
			mv = TempView * model;
		

		prog.setUniform("ModelViewMatrix", mv);
		prog.setUniform("NormalMatrix",
			mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
		prog.setUniform("MVP", camera.projection() * mv);
		// the correct matrix to transform the normal is the transpose of the inverse of the M matrix
		mat3 normMat = glm::transpose(glm::inverse(mat3(model)));
		prog.setUniform("M", model);
		//prog.setUniform("NormalMatrix", normMat);
		prog.setUniform("V", camera.view());
		prog.setUniform("P", camera.projection());
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// resize the viewport
	/////////////////////////////////////////////////////////////////////////////////////////////
	void CubeMapScene::resize(QuatCamera camera, int w, int h)
	{
		gl::Viewport(0, 0, w, h);
		width = w;
		height = h;
		camera.setAspectRatio((float)w / h);

	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Compile and link the shader
	/////////////////////////////////////////////////////////////////////////////////////////////
	void CubeMapScene::compileAndLinkShader()
	{

		try {
			prog.compileShader("Shaders/diffuse.vert");
			prog.compileShader("Shaders/diffuse.frag");
			prog.link();
			prog.validate();
			prog.use();
		}
		catch (GLSLProgramException & e) {
			cerr << e.what() << endl;
			exit(EXIT_FAILURE);
		}
	}

	void CubeMapScene::toggleRs()
	{
		m_Toggle = !m_Toggle;
	}
}
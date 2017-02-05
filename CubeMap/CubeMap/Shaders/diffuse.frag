#version 430
//Subroutine to change between shaders at runtime
subroutine vec4 shaderModelType();
subroutine uniform shaderModelType shaderModel;


in vec3 vertPos;		// world position
in vec3 N;				// surface normal
in vec3 lightPos;		// position of the light
in vec3 texCoords;		//Texture co-ords 
in vec3 eyePos;			// position of eyes
in vec3 eyeNormal;		// normal of eyes

uniform samplerCube BoxTexture;		//Box map sampler
uniform mat4 View;					//View matrix

//phong
uniform vec3 Kd;				// Diffuse reflectivity
uniform vec3 Ka;				// Ambient reflectivity
uniform vec3 Ks;				// Specular reflectivity

uniform vec3 Ld;				// Diffuse light intensity
uniform vec3 La;				// Ambient light intensity

uniform float n;				// Specular reflection exponent

uniform vec3 cameraPos;


layout( location = 0 ) out vec4 FragColour; // output

void main() {

  FragColour = shaderModel(); // this is how the subrountines are chosen and set

}

//Set the textures to the Box
subroutine (shaderModelType)
vec4 BoxMap()
{
	return texture(BoxTexture, texCoords);
}


//reflect model,
subroutine (shaderModelType)
vec4 Reflect()
{

	//reflect
	vec3 incident = normalize(vertPos - cameraPos);
	vec3 reflect = reflect (incident, normalize(N));

	return  texture(BoxTexture, reflect);
}

//Refraction shader
subroutine (shaderModelType)
vec4 Refract()
{
	vec3 incident = normalize(vertPos - cameraPos);
	vec3 normal = normalize(N);
	float ratio = 1.0 /1.5;					//Glass
	vec3 refract = refract (incident, normal, ratio);
	return texture(BoxTexture,refract);
}

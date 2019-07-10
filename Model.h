
#ifndef MODEL_H
#define MODEL_H

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN 
    #include <windows.h>
#endif

#if defined(__APPLE__) && defined (__MAC__)
	#include<OpenGL/gl.h>
	#include<OpenGL/glu.h>
#else
	#include<GL/gl.h>
	#include<GL/glu.h>
#endif



#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "Vector3.h"
#include "Texture.h"

using std::ifstream;
using std::istringstream;
using std::string;

struct Material
{
	float ka[4];
	float kd[4];
	float ks[4];
	float ke[4];

	Texture *ambientMap;
	Texture *diffuseMap;
	Texture *specularMap;
	Texture *transparencyMap;
	Texture *emissionMap;
	Texture *bumpMap;
	Texture *shininessMap;

	float alpha;
	float shininess;
	float illumination; 

	std::string name;

	Material ()
	{	
		float ambient[4] = {0.0f, 0.0f, 0.0f, 1.0f};
		float diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		float specular[4] = {0.0f, 0.0f, 0.0f, 1.0f};
		float emission[4] = {0.0f, 0.0f, 0.0f, 1.0f};

		memcpy(ka, ambient, sizeof(float)*4);
		memcpy(kd, diffuse, sizeof(float)*4);
		memcpy(ks, specular, sizeof(float)*4);
		memcpy(ke, emission, sizeof(float)*4);

		ambientMap			=	NULL;
		diffuseMap			=	NULL;
		specularMap			=	NULL;
		transparencyMap		=	NULL;
		emissionMap			=	NULL;
		Texture *bumpMap	=	NULL;
		shininessMap		=	NULL;
		
	}



};


struct Face
{
	
	Vector3 ** Vertices;
	Vector3 ** UVWs;
	Vector3 ** Normals;

	Material *material;

	int numVertices;
	int numUVWs;
	int numNormals;

	Vector3 faceNormal;
	Vector3 faceCenter;

	
	Face()
	{
	
		Vertices = NULL;
		UVWs = NULL;
		numNormals = NULL;
		material = NULL;
	}
	
};


struct GroupObject
{
	vector<Face *> faces;

	std::string objectName;
	std::string groupName;
	

};



class Model
{

public:
	Model (std::string name = "");
	~Model();

	void drawModel(void);
	void drawObject(bool transparency = false);
	void drawFace( Face &face);

	bool loadModel(std::string modelName);
	void loadMaterial (string materialName);
	void deleteObjects();

	float getRadius();
	Vector3 getCenter();
	std::string getPath();

private:
	vector <Material *> materials;
	vector <GroupObject *> groupObjects;

	vector <Vector3 *> vertices;
	vector <Vector3 *> uvw;
	vector <Vector3 *> normals;

	Vector3 boundingPoints[8];

	float radius;
	Vector3 center;
	std::string modelName;
	bool objectLoaded;

	GLuint displayList;

};

#endif
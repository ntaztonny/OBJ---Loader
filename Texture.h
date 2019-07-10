
#ifndef TEXTURE_H
#define TEXTURE_H


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

#include<string>
#include <vector> 
#include <fstream>

using std::string;
using std::vector;
using std:: ifstream;

struct TGAHeader
{
	GLubyte IDLength;
	GLubyte ColorMapType;
	GLubyte ImageType;
	GLubyte ColorMapSpecification[5];
	GLuint  xOrigin;
	GLuint  yOrigin;
	GLuint  width;
	GLuint  height;
	GLuint  PixelDepth;
	GLubyte ImageDescriptor;

};
class Texture
{	
public:
	Texture(string filename, string name ="" );
	~Texture();

	
	unsigned int	width;    
	unsigned int	height;
	unsigned int	bpp;
	string			filename;
	string			name;
	unsigned int	texID;
	unsigned char	*imageData;

	static vector<Texture *> textures;

private:
	bool loadTGA(string filename);
	bool CreateTexture(unsigned char* imageData, GLuint width, GLuint height, GLuint type);



};

#endif

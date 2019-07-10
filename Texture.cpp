

#include "Texture.h"


vector<Texture *> Texture::textures;
  
Texture::Texture(string in_filename, string in_name)
{
	imageData = NULL;
	filename = in_filename;
	loadTGA(in_filename);
	name = in_name;
	
	textures.push_back(this);

}
Texture::~Texture()
{
	for(vector<Texture *>::iterator it = textures.begin(); it != textures.end(); it++ )
		if((*it) == this)
			textures.erase(it);
	if(imageData)
		delete imageData;


}
	
bool Texture::loadTGA(string filename)
{	
	TGAHeader header;

	ifstream file(filename.data(), std::ios_base::binary);

	if(!file.is_open())
		return false;

	if(!file.read((char*)(&header), sizeof(header)))
		return false;

	if(header.ImageType != 2) 
		return false;


	width = header.width;
	height = header.height;
	bpp = header.PixelDepth;

	if (width <= 0 || height <= 0 || (bpp != 24 && bpp != 32) )
		return false;

	GLuint type = GL_RGBA;

	if(bpp == 24)
		type = GL_RGB;


	GLuint bytePerPixel = bpp/8;
	GLuint imageSize = width*height*bytePerPixel;

	imageData = new GLubyte [imageSize];

	if(imageData == NULL)
		return false;

	if (!file.read((char *)imageData, imageSize ))
	{
		delete imageData;
		return false;
	}

	// from BGR to RGB

	for(GLuint i =0; i <(int) imageSize; i += bytePerPixel)
	{
		GLuint temp = imageData[i];
		imageData[i] = imageData[i+2];
		imageData[i+2] = temp;
	}

	CreateTexture( imageData,  width,  height,  type);
	//if everything works!
	return true;
}
	
bool Texture::CreateTexture(unsigned char* imageData, GLuint width, GLuint height, GLuint type)
{
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);

	// if all works return true

	return true;


}


#include "Model.h"

Model::Model (string name)
{
	objectLoaded = false;
	if (name != "")
		loadModel(name);
	displayList = 0;
}

Model::~Model()
{
	deleteObjects();
}

void Model::drawModel()
{
	if (!objectLoaded)
		return;

	if(displayList != 0)
	{
		glCallList(displayList);
		return;
	}

	displayList = glGenLists(1);
	glNewList(displayList, GL_COMPILE_AND_EXECUTE);

	drawObject(false);
	drawObject(true);

	glEndList();


}


void Model::drawObject(bool transparency)
{	
	Material * lastMaterial = NULL;

	for(int i = 0; i < (int) groupObjects.size(); i++ )
	{
		GroupObject * object = groupObjects[i];
		
		for (int f = 0; f < (int)object ->faces.size(); f++)
		{
			Face * face = object ->faces[f];

			Material *material = face ->material;

			if (material != lastMaterial)
			{	
				if(transparency == false && material -> alpha < 1.0f )
					continue;

				  lastMaterial = material;

				  material -> kd[3] = material ->alpha;

				  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material->ka);
				  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material->kd);
				  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material->ks);
			          glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material->ke);
				  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->shininess);

				  if (material ->diffuseMap != NULL)
				  {
					glEnable (GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, material->diffuseMap->texID);

				  
				  }
				  else
					  glDisable(GL_TEXTURE_2D);

				  lastMaterial = material;
			}

			drawFace(*face);
		}
	
	}
}


void Model::drawFace( Face &face)
{
	if (face.numVertices >= 3)
		glBegin(GL_TRIANGLES);
				
	else
		glBegin(GL_POLYGON);

	for(int i =0; i < (int )face.numVertices; i++)
	{
		if(face.numUVWs > i && face.UVWs != NULL )
			glTexCoord2f(face.UVWs[i]->a, face.UVWs[i]->b);
		if(face.numNormals > i && face.Normals != NULL )
			glNormal3f(face.Normals[i]->a, face.Normals[i]->b, face.Normals[i]->c);
		if(face.numVertices > i && face.Vertices != NULL )
			glVertex3f(face.Vertices[i]->a, face.Vertices[i]->b, face.Vertices[i]->c);

	}

	glEnd();
}


bool Model::loadModel(string modelname)
{
	modelName = modelname;

	ifstream istr(modelName.data());
	if(!istr)
		return false;
	
	deleteObjects();
	displayList = 0;

	objectLoaded = false;

	GroupObject* defaultObject = new GroupObject();
	GroupObject* currentObject = defaultObject;

	groupObjects.push_back(defaultObject);

	// getting only the path 
	char path[256];

	strcpy(path, modelName.data());

	for(int i = modelName.length(); (path[i] != '\\' && path[i] != '/'); i--)
		path[i] = 0;
		

	Material *currentMaterial = NULL;

	char line[256];

	while (istr.getline(line, 256))
	{
		istringstream newLine(line, istringstream::in);
		
		string firstWord;
		newLine >> firstWord;

		if (firstWord == "#")
		{
		}
		// materials library
		else if (firstWord == "mtllib")
		{
			//load the material using the material filename
			string materialfilename;
			while (newLine >> materialfilename)
			{
				loadMaterial(materialfilename);
			}


		}
		// materials name
		else if (firstWord == "usemtl")
		{
			string materialName;
			newLine >> materialName;

			for(int i = 0; i != (int)materials.size(); i++)
			{			
				if(materials[i] ->name == materialName)
					currentMaterial = materials[i];

				break;
			}


		}
		// vertices
		else if (firstWord == "v")
		{
			Vector3 *vertex = new Vector3;
			newLine >> vertex->a  >> vertex->b; vertex->c;

			vertices.push_back(vertex);
		}
		// texture coordinates
		else if (firstWord == "vt")
		{
			Vector3 *txtcoord = new Vector3;
			newLine >> txtcoord->a  >> txtcoord->b; txtcoord->c;

			uvw.push_back(txtcoord);
		}
		//texture normals
		else if (firstWord == "vn")
		{
			Vector3 *vertexnormal = new Vector3;
			newLine >> vertexnormal->a  >> vertexnormal->b; vertexnormal->c;

			normals.push_back(vertexnormal);
		}
		// groups
		else if (firstWord == "g")
		{
			string nameOfObject;
			newLine >> nameOfObject;

			if  (nameOfObject == "default")
				currentObject = defaultObject;
			else
			{
				GroupObject *object = new GroupObject();
				object ->groupName = nameOfObject;

				newLine >> object ->groupName;
				currentObject = object;		

				groupObjects.push_back(object);
			}

		} 
		//faces
		else if (firstWord == "f")
		{
			Face *newface = new Face();
			newface->material = currentMaterial;

			currentObject ->faces.push_back(newface); 

			vector<Vector3 *> tempVertices;
			vector<Vector3 *> tempUvws;
			vector<Vector3 *> tempNormals;


			while(!newLine.eof())
			{
				int Vertex (0), Normal(0), Uvw(0);

				char temp;
				bool noUV = false;

				if (vertices.size() > 0)
				{	
					newLine >> Vertex;

					while(newLine.get(temp))
					{
							if(temp == '/' || temp != ' ')
								break;
					}
					
					newLine.get(temp);

					if(temp == '/')
						noUV = true;
					else 
					{
						noUV = false;
						newLine.unget();
					}
						
					tempVertices.push_back(vertices[--Vertex]);
				
				}

				if((int)uvw.size() > 0 && noUV == false)
					newLine >> Uvw;

					while(newLine.get(temp))
					{
							if(temp == '/' || temp != ' ')
								break;
					}
					tempUvws.push_back(uvw[--Uvw]);

					if((int)normals.size() > 0)
					newLine >> Normal;

					while(newLine.get(temp))
					{
							if(temp == '/' || temp != ' ')
								break;
					}
					tempNormals.push_back(normals[--Normal]);
			}


			newface ->numVertices	= (int)tempVertices.size();
			newface ->numNormals	= (int)tempNormals.size();
			newface ->numUVWs		= (int)tempUvws.size();

			newface ->Vertices = new Vector3 *[newface ->numVertices]; 
			newface ->Normals = new Vector3 *[newface ->numNormals]; 
			newface ->UVWs  = new Vector3 *[newface ->numUVWs]; 
			
			for (int v = 0; v < newface ->numVertices; v++)
				newface ->Vertices[v] = tempVertices[v];

			for (int v = 0; v < newface ->numNormals; v++)
				newface ->Normals[v] = tempNormals[v];

			for (int v = 0; v < newface ->numUVWs; v++)
				newface ->UVWs[v] = tempUvws[v];

			for (int v = 0; v < newface ->numVertices; v++)
				newface ->faceCenter += (*tempVertices[v]);

			newface ->faceCenter = (newface ->faceCenter/ (float) newface -> numVertices);
			
			if (newface -> numVertices >= 3 )
			{
				Vector3 vector1 = ((*newface -> Vertices[0]) -(*newface -> Vertices[1])).normalize();

				Vector3 vector2 = ((*newface -> Vertices[0]) -(*newface -> Vertices[2])).normalize();		

				newface ->faceNormal  = vector1 * vector2;
			}

		} 
	}

		// calculate the center of the object

	float xmin, xmax, ymin, ymax, zmax, zmin;

	center = Vector3(0, 0, 0);
	for(int n = 0; n < (int)vertices.size(); n++)
	{	
		if (n == 0)
		{
			xmin = xmax = vertices[n] -> a;
			ymin = ymax = vertices[n] -> b;
			zmin = zmax = vertices[n] -> c;

			continue;
		}
		if (vertices[n]-> a < xmin)
			xmin = vertices[n] -> a;
		if (vertices[n]-> a < xmax)
			xmax = vertices[n] -> a;

		if (vertices[n]-> b < ymin)
			ymin = vertices[n] -> b;
		if (vertices[n]-> b < ymax)
			ymax = vertices[n] -> b;

		if (vertices[n]-> c < zmin)
			zmin = vertices[n] -> c;
		if (vertices[n]-> c < zmax)
			zmax = vertices[n] -> a;
		
		center += (*vertices[n]);
		
	}

	center = center/(float)vertices.size();

	boundingPoints[0] = Vector3(xmax, ymin, zmin);
	boundingPoints[1] = Vector3(xmin, ymax, zmin);
	boundingPoints[2] = Vector3(xmin, ymin, zmax);

	boundingPoints[3] = Vector3(xmin, ymax, zmax);
	boundingPoints[4] = Vector3(xmax, ymax, zmin);
	boundingPoints[5] = Vector3(xmax, ymin, zmax);

	boundingPoints[6] = Vector3(xmin, ymin, zmin);
	boundingPoints[7] = Vector3(xmax, ymax, zmax);

	radius = (Vector3(xmax, ymax, zmax) - Vector3(xmin, ymin, zmin)).length()/2;

	objectLoaded = true;

	// all is done and good to load object
	return true;
}


void Model::loadMaterial (string materialName)
{
	ifstream istr(materialName.data());
	if(!istr)
		return;

	char path[256];

	strcpy(path, modelName.data());

	for(int i = modelName.length(); (path[i] != '\\' && path[i] != '/'); i--)
		path[i] = 0;

	Material * material = NULL;

	char line[256];

	while (istr.getline(line, 256))
	{
		istringstream newline(line, istringstream::in);

		string firstword;
		newline >> firstword;

		if(firstword == "#")
		{
			
		
		}

		if(firstword == "newmtl")
		{	
			materials.push_back(material);
			newline >> material ->name;
		}

		else if(firstword == "Ns")
		{	
			newline >> material ->shininess;
		}
		else if(firstword == "Ni")
		{	
			newline >> material ->illumination;
		}
		
		else if(firstword == "Tr" || firstword == "d")
		{	
			 newline >> material ->alpha;
		}
		else if(firstword == "Tf")
		{	
			Vector3 color;
			newline >> color.a >> color.b >> color.c;
			material ->alpha = (color.a +color.b + color.c)/3.0f;
		}
		else if(firstword == "illum")
		{	
			newline >> material->illumination;
		}
		else if(firstword == "Ka")
		{	
			newline >> material ->ka[0] >> material ->ka[1] >> material ->ka[2];
		}
		else if(firstword == "Kd")
		{	
			newline >> material ->kd[0] >> material ->kd[1] >> material ->kd[2];
		}
		else if(firstword == "Ks")
		{	
			newline >> material ->ks[0] >> material ->ks[1] >> material ->ks[2];
		}
		else if(firstword == "Ke")
		{	
			newline >> material ->ke[0] >> material ->ke[1] >> material ->ke[2];
		}

		else if(firstword == "map_Ka")
		{	
			string texturename;
			newline >> texturename;
			Texture* textureMap = new Texture(path + texturename);
			material ->ambientMap = textureMap;
		}

		else if(firstword == "map_Kd")
		{	
			string texturename;
			newline >> texturename;
			Texture* textureMap = new Texture(path + texturename);
			material ->diffuseMap = textureMap;
		}

		else if(firstword == "map_Ks")
		{	
			string texturename;
			newline >> texturename;
			Texture* textureMap = new Texture(path + texturename);
			material ->specularMap = textureMap;
		}

		else if(firstword == "map_Ke")
		{	
			string texturename;
			newline >> texturename;
			Texture* textureMap = new Texture(path + texturename);
			material ->emissionMap = textureMap;
		}

		else if(firstword == "map_Ns")
		{	
			string texturename;
			newline >> texturename;
			Texture* textureMap = new Texture(path + texturename);
			material ->shininessMap = textureMap;
		}

		else if(firstword == "map_d")
		{	
			string texturename;
			newline >> texturename;
			Texture* textureMap = new Texture(path + texturename);
			material ->transparencyMap = textureMap;
		}

		else if(firstword == "map_Bump")
		{	
			string texturename;
			newline >> texturename;
			Texture* textureMap = new Texture(path + texturename);
			material ->bumpMap = textureMap;
		}
	
	}
}


void Model::deleteObjects()
{
	for(int m = 0;  m < (int)materials.size(); m++)
	{	
		if(materials[m] -> ambientMap != NULL)
			delete materials[m] -> ambientMap;

		if(materials[m] -> diffuseMap != NULL)
			delete materials[m] -> diffuseMap;

		if(materials[m] -> specularMap != NULL)
			delete materials[m] -> specularMap;

		if(materials[m] -> transparencyMap != NULL)
			delete materials[m] -> transparencyMap;

		if(materials[m] -> emissionMap != NULL)
			delete materials[m] -> emissionMap;

		if(materials[m] -> ambientMap != NULL)
			delete materials[m] -> ambientMap;

		if(materials[m] -> bumpMap != NULL)
			delete materials[m] -> bumpMap;

		if(materials[m] -> shininessMap != NULL)
			delete materials[m] -> shininessMap;

		delete materials[m];

	}

	for(int m = 0;  m < (int)vertices.size(); m++)
		delete vertices[m];

	for(int m = 0;  m < (int)normals.size(); m++)
		delete normals [m];

	for(int m = 0;  m < (int)uvw.size(); m++)
		delete uvw [m];

	for(int m = 0;  m < (int)groupObjects.size(); m++)
	{
	
		GroupObject *object = groupObjects[m];
		
		for (int f = 0; f < (int)object -> faces.size(); f++ )
		{
		
			delete object -> faces[f]->Vertices;
			delete object -> faces[f]->Normals;
			delete object -> faces[f]->UVWs;
			 
			delete object -> faces[f];
		}
	
	}

	uvw.clear();
	normals.clear();
	vertices.clear();
	groupObjects.clear();
	materials.clear();  

}

float Model::getRadius()
{
	return radius;

}
Vector3 Model::getCenter()
{
	return center;
}
	
std::string Model::getPath()
{
	return modelName;
}

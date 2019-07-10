# OBJ-3DModel Loader

This repo contains an OBJ decription code that can be used to upload OBJ 3D files into your 3D application. The directory mainly uses OpenGL for drawing vertices, faces, groupObjects and finally the model in your application.

Inorder to use the files, you can download them and include the "Model.h" file in your project.

There are mainly 2 most important functions: The "Model(string filePath)", which is the model class constructor used to upload the model from the OBJ file format and the "drawModel()" function which actually draws the model by drawing its object groups, and faces using the respective vertices, uvws, and normals.

Another important note is the use of materials; i.e "loadMaterial(string materialName)" function


### Comments

Inorder to understand the code, it is important to understand the OBJ file format that can be downloaded here: https://www.cs.utah.edu/~boulos/cs3505/obj_spec.pdf

I have also included a texture class that only can upload tag texture file formates. However, if your material uses different formats like png, jpg etc, there are different texture libraries you can use to read your textures: To mention just a few; the  C based stb library at: https://github.com/ntaztonny/stb Simple OpenGL Image Library (SOIL) accessible at: https://www.lonesock.net/soil.html. You can also find others online


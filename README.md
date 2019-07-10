
This repo contains an OBJ decription code that can be used to upload OBJ 3D files into your 3D application. The directory mainly uses OpenGL for drawing vertices, faces, groupObjects and finally the model in your application.

Inorder to use the files, you can download them and include the "Model.h" file in your project.

There are mainly 2 most important functions: The "Model(string filePath)", which is the model class constructor used to upload the model from the OBJ file format and the "drawModel()" function which actually draws the model by drawing its object groups, and faces using the respective vertices, uvws, and normals.


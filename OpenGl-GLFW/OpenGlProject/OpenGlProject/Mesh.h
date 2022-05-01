#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>       
#include <assimp/postprocess.h> 

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"Texture.h"

class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	VAO VAO;

	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

	void Draw(Shader& shader, Camera& camera, aiMatrix4x4 matrix, aiVector3t<float> translation, aiQuaterniont<float> rotation, aiVector3t<float> scale);
};
#endif
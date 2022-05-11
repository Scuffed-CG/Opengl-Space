#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include<glad/glad.h>
#include "shaderClass.h"
#include "VAO.h"

class Framebuffer
{
public:
	Framebuffer(const unsigned int width, const unsigned int height, const char* vertexFile, const char* fragmentFile);
	void Draw();
	void Bind();
	void Unbind();

	GLuint ID;
private:
	Shader framebufferProgram;
	Shader blurProgram;

	unsigned int framebufferTexture;
	unsigned int postProcessingTexture;
	unsigned int bloomTexture;

	std::vector<Vertex> vertices =
	{
		//  Coords   // texCoords
		Vertex{glm::vec3(1.0f, -1.0f, 0.0f),  glm::vec2(1.0f, 0.0f)},
		Vertex{glm::vec3(-1.0f, -1.0f, 0.0f),  glm::vec2(0.0f, 0.0f)},
		Vertex{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
		Vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
		Vertex{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		Vertex{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)}
	};

	VAO rectVAO;
	VBO rectVBO;
	unsigned int pingpongFBO[2];
	unsigned int pingpongBuffer[2];

};
#endif 

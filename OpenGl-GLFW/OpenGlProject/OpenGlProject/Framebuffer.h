#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include<glad/glad.h>
#include "shaderClass.h"

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

	unsigned int rectVAO;
	unsigned int rectVBO;
	unsigned int pingpongFBO[2];
	unsigned int pingpongBuffer[2];

};
#endif 

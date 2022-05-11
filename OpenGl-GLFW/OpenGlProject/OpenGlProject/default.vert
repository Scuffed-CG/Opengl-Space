#version 330 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out DATA
{
	vec2 texCoord;
    mat4 projection;
	mat4 model;
	vec3 camPos;
} data_out;

uniform mat4 camMatrix;

uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

uniform vec3 lightPos;
uniform vec3 camPos;


void main()
{
	gl_Position = model * translation * rotation * scale * vec4(aPos, 1.0f);
	data_out.texCoord = aTex;
	data_out.projection = camMatrix;
	data_out.model = model * translation * rotation * scale;
	data_out.camPos = camPos;
}
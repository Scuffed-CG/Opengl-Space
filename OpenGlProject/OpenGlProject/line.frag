#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

uniform vec4 color;

void main()
{
	FragColor = color;

	float brightness = dot(FragColor.rgb, vec3(0.2126f, 0.7152f, 0.0722));
	if(brightness > 1.0f){
		BloomColor = vec4(FragColor.rgb, 1.0f);
	}
	else{
		BloomColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}
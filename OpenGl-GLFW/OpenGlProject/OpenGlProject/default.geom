#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 Normal;
out vec3 color;
out vec2 texCoord;
out vec3 crntPos;
out vec3 lightPos;
out vec3 camPos;

in DATA
{
    vec3 Normal;
	vec3 color;
	vec2 texCoord;
    mat4 projection;
    mat4 model;
    vec3 lightPos;
	vec3 camPos;
} data_in[];


void main()
{
    vec3 edge0 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 edge1 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;

    vec2 deltaUV0 = data_in[1].texCoord - data_in[0].texCoord;
    vec2 deltaUV1 = data_in[2].texCoord - data_in[0].texCoord;

    float invDet = 1.0f / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

    vec3 tangent = vec3(invDet * (deltaUV1.y * edge0 - deltaUV0.y * edge1));
    vec3 bitangent = vec3(invDet * (-deltaUV1.x * edge0 + deltaUV0.x * edge1));

    vec3 T = normalize(vec3(data_in[0].model * vec4(tangent, 0.0f)));
    vec3 B = normalize(vec3(data_in[0].model * vec4(bitangent, 0.0f)));
    vec3 N = normalize(vec3(data_in[0].model * vec4(cross(edge1, edge0), 0.0f)));

    mat3 TBN = mat3(T, B, N);
    TBN = transpose(TBN);

     gl_Position = data_in[0].projection * gl_in[0].gl_Position;
    Normal = data_in[0].Normal;
    color = data_in[0].color;
    texCoord = data_in[0].texCoord;

    crntPos = TBN * gl_in[0].gl_Position.xyz;
    lightPos = TBN * data_in[0].lightPos;
    camPos = TBN * data_in[0].camPos;
    EmitVertex();

    gl_Position = data_in[1].projection * gl_in[1].gl_Position;
    Normal = data_in[1].Normal;
    color = data_in[1].color;
    texCoord = data_in[1].texCoord;

    crntPos = TBN * gl_in[1].gl_Position.xyz;
    lightPos = TBN * data_in[1].lightPos;
    camPos = TBN * data_in[1].camPos;
    EmitVertex();

    gl_Position = data_in[2].projection * gl_in[2].gl_Position;
    Normal = data_in[2].Normal;
    color = data_in[2].color;
    texCoord = data_in[2].texCoord;

    crntPos = TBN * gl_in[2].gl_Position.xyz;
    lightPos = TBN * data_in[2].lightPos;
    camPos = TBN * data_in[2].camPos;
    EmitVertex();

    EndPrimitive();
}
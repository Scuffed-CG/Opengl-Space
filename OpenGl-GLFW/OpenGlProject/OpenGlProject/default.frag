#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

in vec3 crntPos;
in vec2 texCoord;
in vec3 camPos;

struct DirLight {
    vec3 direction;
    vec3 color;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct PointLight {    
    vec3 position;
    vec3 color;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

#define NR_POINT_LIGHTS 3
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLight;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normal0;

uniform vec4 lightColor;



vec3 CalcPointLight(PointLight light, vec3 normal, vec3 crntPos, vec3 viewDir)
{	
    vec3 lightDir = normalize(light.position - crntPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir  = normalize(viewDir + lightDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 16.0f);

    float distance    = length(light.position - crntPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   
    
    vec3 ambient  = light.ambient  * vec3(texture(diffuse0, texCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(diffuse0, texCoord));
    vec3 specular  = light.specular * spec * vec3(texture(specular0, texCoord));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular) * light.color;
}

vec3 CalcDirecLight(DirLight light, vec3 normal, vec3 viewDir)
{

    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir  = normalize(viewDir + light.direction);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 16.0f);

    vec3 ambient  = light.ambient  * vec3(texture(diffuse0, texCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(diffuse0, texCoord));
    vec3 specular = vec3(0.0f, 0.0f, 0.0f);

    if(diff != 0.0f)
        vec3 specular = light.specular * spec * vec3(texture(specular0, texCoord));

    return (ambient + diffuse + specular) * light.color;
}

void main()
{
    vec3 norm = normalize(texture(normal0, texCoord).xyz * 2.0f - 1.0f);
    vec3 viewDir = normalize(camPos - crntPos);

    vec3 result = CalcDirecLight(dirLight, norm, viewDir);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, crntPos, viewDir);    

    FragColor = vec4(result, 1.0);

	float brightness = dot(FragColor.rgb, vec3(0.2126f, 0.7152f, 0.0722));
	if(brightness > 1.0f){
		BloomColor = vec4(FragColor.rgb, 1.0f);
	}
	else{
		BloomColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
};
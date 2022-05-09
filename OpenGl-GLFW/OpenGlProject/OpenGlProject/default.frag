#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

in vec3 crntPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;
in vec3 lightPos;
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

#define NR_POINT_LIGHTS 1
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLight;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normal0;

uniform vec4 lightColor;



vec3 CalcPointLight(PointLight light, vec3 normal, vec3 crntPos, vec3 viewDir)
{	
    vec3 lightDir = normalize(light.position - crntPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 halfwayDir  = normalize(viewDir + lightDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 16.0f);

    // attenuation
    float distance    = length(light.position - crntPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   
    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(diffuse0, texCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(diffuse0, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(specular0, texCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular) * light.color;
}

vec3 CalcDirecLight(DirLight light, vec3 normal, vec3 viewDir)
{

    vec3 lightDir = normalize(-light.direction);
    // diffuse shading

    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading

    vec3 halfwayDir  = normalize(viewDir + light.direction);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 16.0f);
    // combine results

    vec3 ambient  = light.ambient  * vec3(texture(diffuse0, texCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(diffuse0, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(specular0, texCoord));
    return (ambient + diffuse + specular) * light.color;
}

vec4 spotLight()
{
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	float ambient = 0.20f;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float specular = 0.0f;
	if (diffuse != 0.0f)
	{
		float specularLight = 0.50f;
		vec3 viewDirection = normalize(camPos - crntPos);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
		specular = specAmount * specularLight;
	};

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
};


void main()
{
    // properties
    vec3 norm = normalize(texture(normal0, texCoord).xyz * 2.0f - 1.0f);
    vec3 viewDir = normalize(camPos - crntPos);

    // phase 1: Directional lighting
    vec3 result = CalcDirecLight(dirLight, norm, viewDir);

    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, crntPos, viewDir);    

    // phase 3: Spot light
    //result += CalcSpotLight(spotLight, norm, crntPos, viewDir);    
    
    FragColor = vec4(result, 1.0);

	float brightness = dot(FragColor.rgb, vec3(0.2126f, 0.7152f, 0.0722));
	if(brightness > 1.0f){
		BloomColor = vec4(FragColor.rgb, 1.0f);
	}
	else{
		BloomColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
};
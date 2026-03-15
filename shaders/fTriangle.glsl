#version 460 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

// material colors of object

struct Material
{
	// ambient and diffuse the same
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	vec3 specular;
	float shininess;
};


// source light
struct Light
{
	
	vec3 position;
	vec3 direction;
	float inCutoff;
	float outCutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform Light light;
//uniform vec3 cameraPos;


out vec4 FragColor;
void main()
{	

	// ambient
	vec3 ambient =  light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
	// diffuse
	vec3 lightDir = normalize(light.position - FragPos);
	float diffuseValue = max(dot(Normal,lightDir),0.0f);
	vec3 diffuse = diffuseValue * vec3(texture(material.texture_diffuse1, TexCoords)) * light.diffuse;
	// specular
	vec3 cameraDir = normalize(light.position - FragPos);
	vec3 reflection = reflect(-lightDir,Normal);
	float specularValue = pow(max(dot(reflection,cameraDir),0.0f), material.shininess);
	vec3 specular = specularValue * vec3(texture(material.texture_specular1, TexCoords)) * light.specular;

	// calculate intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.inCutoff - light.outCutoff;
	float intensity = clamp((theta - light.outCutoff) / epsilon,0.0f,1.0f);
	diffuse *= intensity;
	specular *= intensity;

	// calculate attenuation
	float lightDist = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * lightDist
	+ light.quadratic * lightDist * lightDist);

	vec3 lightResult = ambient + (diffuse + specular) * attenuation;
	FragColor = vec4(lightResult,1.0f);

}
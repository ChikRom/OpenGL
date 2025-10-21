

#version 460 core
out vec4 FragColor;



struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;


	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
struct SpotLight
{

	vec3 direction;
	float inCutOff;
	float outCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
#define NR_POINT_LIGHTS 4
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;
uniform SpotLight spotLight;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;


vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir);
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 viewDir,vec3 fragPos);
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 viewDir,vec3 fragPos);

void main()
{


	vec3 norm = normalize(Normal);
	vec3 ViewDirection = normalize(-FragPos);
	
	vec3 result = CalcDirLight(dirLight,norm,ViewDirection);

	for (int i = 0; i<NR_POINT_LIGHTS;i++)
		result += CalcPointLight(pointLights[i],norm,ViewDirection,FragPos);

	result += CalcSpotLight(spotLight,norm,ViewDirection,FragPos);
	FragColor = vec4(result,1.0);



}


vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal,lightDir),0.0);
	vec3 reflectDir = reflect(-lightDir,normal);

	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);

	vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse,TexCoords).rgb;
	vec3 specular = light.specular * spec * texture(material.specular,TexCoords).rgb;
	return (ambient + diffuse + specular);
}



vec3 CalcPointLight(PointLight light,vec3 normal,vec3 viewDir,vec3 fragPos)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal,lightDir),0.0);
	vec3 reflectDir = reflect(-lightDir,normal);

	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);

	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

		vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse,TexCoords).rgb;
	vec3 specular = light.specular * spec * texture(material.specular,TexCoords).rgb;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}




vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 viewDir,vec3 fragPos)
{

	vec3 lightDirection = normalize(-fragPos);
	float diff = max(dot(normal,lightDirection),0.0);

	vec3 reflectDirection = reflect(-lightDirection,normal);
	float spec = pow(max(dot(viewDir,reflectDirection),0.0),material.shininess);
	
	float dist = length(-fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
	
	float theta = dot(lightDirection,normalize(-light.direction));
	float epsilon = light.inCutOff - light.outCutOff;
	float intensity = clamp((theta - light.outCutOff) / epsilon,0.0,1.0);

	vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse,TexCoords).rgb;
	vec3 specular = light.specular * spec * texture(material.specular,TexCoords).rgb;

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}




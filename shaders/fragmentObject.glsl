

#version 460 core
out vec4 FragColor;
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Material material;
uniform Light light;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

void main()
{
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(light.position-FragPos);
	float diff = max(dot(norm,lightDirection),0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;


	vec3 viewDirection = normalize(-FragPos);
	vec3 reflectDirection = reflect(-lightDirection,norm);
	float spec = pow(max(dot(viewDirection,reflectDirection),0.0),material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;


	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result,1.0);
}

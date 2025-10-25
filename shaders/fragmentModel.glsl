#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir);

uniform DirLight dirLight;

void main()
{    
	vec3 norm = normalize(Normal);
	vec3 ViewDirection = normalize(-FragPos);
	
	vec3 result = CalcDirLight(dirLight,norm,ViewDirection);
    FragColor = vec4(result,1.0);
}



vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal,lightDir),0.0);



	vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;
	vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;
	return (ambient + diffuse);
}
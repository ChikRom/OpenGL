

#version 460 core
out vec4 FragColor;
uniform vec3 ObjectColour;
uniform vec3 LightColour;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;
void main()
{
	float ambientStrength = 0.2f;
	vec3 ambient = LightColour * ambientStrength;

	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(LightPos-FragPos);
	float diff = max(dot(norm,lightDirection),0.0);
	vec3 diffuse = LightColour * diff;

	float specularStrength = 0.6;
	vec3 viewDirection = normalize(-FragPos);
	vec3 reflectDirection = reflect(-lightDirection,norm);
	float spec = pow(max(dot(viewDirection,reflectDirection),0.0),64);
	vec3 specular = specularStrength * spec * LightColour;
	vec3 result = (ambient + diffuse + specular) * ObjectColour;
	FragColor = vec4(result,1.0);
}
